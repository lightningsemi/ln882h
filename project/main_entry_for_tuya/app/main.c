#include "proj_config.h"
#include "usr_app.h"

#include "osal/osal.h"
#include "hal/hal_interrupt.h"

#include "utils/debug/CmBacktrace/cm_backtrace.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/runtime/runtime.h"
#include "utils/system_parameter.h"

#include "ln_nvds.h"
#include "ln_kv_api.h"
#include "ln_utils.h"
#include "ln_misc.h"

#include "ota_port.h"
#include "flash_partition_table.h"

#include "wifi.h"
#include "wifi_manager.h"
#include "netif/ethernetif.h"

#include "ty_ada_export_api.h"
#include "ln_ty_sdk_version.h"
#include "utils/power_mgmt/ln_pm.h"


#define REBOOT_MAGIC_WORD (0xdeadbeefa5a55a5a)

static volatile uint32_t s_hw_reboot_cnt __attribute__((section(".no_init_data"))) __ZERO_INIT__;
static volatile uint64_t s_hw_reboot_magic_word __attribute__((section(".no_init_data"))) __ZERO_INIT__;
static chip_reboot_cause_t s_reboot_cause;

uint32_t usr_reboot_cnt_get(void)
{
    return s_hw_reboot_cnt;
}

int usr_reboot_cause_get(void)
{
    return (int)s_reboot_cause;
}

int main (int argc, char* argv[])
{
    LN_UNUSED(argc);
    LN_UNUSED(argv);

    //0. check reboot cause
    s_reboot_cause = ln_chip_get_reboot_cause();

    //1.sys clock,interrupt
    SetSysClock();
    set_interrupt_priority();
    switch_global_interrupt(HAL_ENABLE);
    ln_runtime_measure_init();

    //2. register os heap mem
    OS_DefineHeapRegions();

    if (s_hw_reboot_magic_word != REBOOT_MAGIC_WORD) {
        s_hw_reboot_magic_word = REBOOT_MAGIC_WORD;
        s_hw_reboot_cnt = 0;
    } else {
        s_hw_reboot_cnt++;
    }

    //3.init log
    log_init();
    LOG(LOG_LVL_INFO, "-- tuya project --\r\n");

    cm_backtrace_init("tuya", "HW_V1.0", LN_TY_SDK_VERSION_STRING);

    if (NVDS_ERR_OK != ln_nvds_init(NVDS_SPACE_OFFSET)) {
        LOG(LOG_LVL_ERROR, "NVDS init filed!\r\n");
    }

    if (KV_ERR_NONE != ln_kv_port_init(KV_SPACE_OFFSET, (KV_SPACE_OFFSET + KV_SPACE_SIZE))) {
        LOG(LOG_LVL_ERROR, "KV init filed!\r\n");
    }

    //init system parameter
    sysparam_integrity_check_all();

    ota_port_init();

    //Init lwip stack(Creat lwip tack).
    TUYA_LwIP_Init();

    ty_ada_bt_init();

    ln_pm_sleep_mode_set(ACTIVE);

    /**
     * CLK_G_I2S | CLK_G_WS2811 | CLK_G_DBGH |
     * CLK_G_SDIO | CLK_G_EFUSE | CLK_G_AES
    */
    ln_pm_always_clk_disable_select(CLK_G_I2S | CLK_G_WS2811 |
                                    CLK_G_SDIO | CLK_G_AES);
    /**
     * CLK_G_BLE | CLK_G_ADC | CLK_G_GPIOA | CLK_G_GPIOB |
     * CLK_G_SPI0 | CLK_G_SPI1 | CLK_G_I2C0 |
     * CLK_G_UART1 | CLK_G_UART2 | CLK_G_WDT |
     * CLK_G_TIM_REG | CLK_G_TIM1 | CLK_G_TIM2 |
     * CLK_G_TIM3 | CLK_G_TIM4 | CLK_G_MAC |
     * CLK_G_DMA | CLK_G_RF | CLK_G_ADV_TIMER |
     * CLK_G_TRNG
     * 
     * CLK_G_TIM3:
     *      For WiFi hardware burst test, in burst mode, need to enable the clock.
     * CLK_G_ADC:
     *      For WiFi calibration
     * 
    */
    ln_pm_lightsleep_clk_disable_select(CLK_G_GPIOA | CLK_G_GPIOB |
                                        CLK_G_SPI0 | CLK_G_SPI1 | CLK_G_I2C0 |
                                        CLK_G_UART1 | CLK_G_UART2 | CLK_G_WDT |
                                        CLK_G_TIM_REG | CLK_G_TIM1 | CLK_G_TIM2 | 
                                        CLK_G_TIM4 | CLK_G_MAC | CLK_G_DMA |
                                        CLK_G_RF | CLK_G_ADV_TIMER | CLK_G_TRNG);

    //Creat usr app task.
    creat_usr_app_task();

    OS_ThreadStartScheduler();

    return 0;
}
