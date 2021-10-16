#include "proj_config.h"
#include "ln882h.h"
#include "hal/hal_flash.h"
#include "hal/hal_cache.h"
#include "hal/hal_misc.h"
#include "utils/debug/log.h"

#include "flash_partition_mgr.h"
#include "ln_nvds.h"
#include "ota_agent.h"

typedef void (*jump_func_t)(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);
extern uint32_t boot_header_info_init(void);

static void set_interrupt_priority(void)
{
    __NVIC_SetPriorityGrouping(4);

    NVIC_SetPriority(SysTick_IRQn,   1);
    NVIC_SetPriority(UART0_IRQn,     4);
    NVIC_SetPriority(UART1_IRQn,     4);
}

static void jump_to_application(uint32_t app_offset)
{
    jump_func_t *jump_func = NULL;
    uint32_t *vec_int_base;

    //Enable QSPI 4bit mode
    hal_flash_quad_mode_enable(1);

    //Init Flash cache
    flash_cache_init(0);

    //Prepare for jump
    vec_int_base = (uint32_t *)(CACHE_FLASH_BASE + app_offset);
    __set_MSP(*vec_int_base);
    jump_func = (jump_func_t *)(vec_int_base + 1);

    log_deinit(); //clear log uart isr
    
    // Jump to user's Reset_Handler
    (*jump_func)(CACHE_FLASH_BASE + app_offset,1,2,3);
}


static void rf_test_bin_run(uint32_t start_addr)
{
    #define  RF_TEST_BIN_FLAG0        (0xA55AAA55)
    #define  RF_TEST_BIN_FLAG1        (0x61962687)
    #define  RF_TEST_BIN_FLAG2        (0xB27DA9F8)
    #define  RF_TEST_BIN_FLAG3        (0xEDCBA369)
    #define  FLAG_OFFSET_AT_VECTOR    (0xF0)
    
    uint32_t rf_tset_bin_flag0 = 0; //Littile-endian
    uint32_t rf_tset_bin_flag1 = 0; //Littile-endian
    uint32_t rf_tset_bin_flag2 = 0; //Littile-endian
    uint32_t rf_tset_bin_flag3 = 0; //Littile-endian
    
    uint32_t flag0_offset = start_addr + FLAG_OFFSET_AT_VECTOR;
    uint32_t flag1_offset = flag0_offset + sizeof(uint32_t);
    uint32_t flag2_offset = flag1_offset + sizeof(uint32_t);
    uint32_t flag3_offset = flag2_offset + sizeof(uint32_t);
    
    hal_flash_read(flag0_offset, sizeof(rf_tset_bin_flag0), (uint8_t *)&rf_tset_bin_flag0);
    hal_flash_read(flag1_offset, sizeof(rf_tset_bin_flag1), (uint8_t *)&rf_tset_bin_flag1);
    hal_flash_read(flag2_offset, sizeof(rf_tset_bin_flag2), (uint8_t *)&rf_tset_bin_flag2);
    hal_flash_read(flag3_offset, sizeof(rf_tset_bin_flag3), (uint8_t *)&rf_tset_bin_flag3);
    
    if ((rf_tset_bin_flag0 == RF_TEST_BIN_FLAG0) && \
        (rf_tset_bin_flag1 == RF_TEST_BIN_FLAG1) && \
        (rf_tset_bin_flag2 == RF_TEST_BIN_FLAG2) && \
        (rf_tset_bin_flag3 == RF_TEST_BIN_FLAG3)) 
    {
        jump_to_application(start_addr);
    }
}

unsigned int  flash_id = 0;
int main (int argc, char* argv[])
{
    partition_info_t nvds_part_info;
    partition_info_t ota_part_info;
    
    SetSysClock();
    set_interrupt_priority();
    __enable_irq();

    log_init();
    hal_flash_init();
    
    flash_id = hal_flash_read_id();
    
    flash_cache_disable();

    if (LN_TRUE != ln_verify_partition_table()) {
        // partition tab error!
    }
    
    if (LN_TRUE != ln_fetch_partition_info(PARTITION_TYPE_NVDS, &nvds_part_info)) {
        // no NVDS partition!    
    }

    if (NVDS_ERR_OK != ln_nvds_init(nvds_part_info.start_addr)) {
        // NVDS init filed.
    }
    
    if (LN_TRUE == ln_fetch_partition_info(PARTITION_TYPE_OTA, &ota_part_info)) {
        rf_test_bin_run(ota_part_info.start_addr);
    }

    ota_port_init();

    if(OTA_ERR_NONE != ota_boot_upgrade_agent(jump_to_application))
    {
        // TODO:process error code
    }
    while(1);
}



void MemManage_Handler (void)
{

}

void BusFault_Handler (void)
{

}

void UsageFault_Handler (void)
{

}

