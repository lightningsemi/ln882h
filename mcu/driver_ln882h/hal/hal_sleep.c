
#include <string.h>
#include "ln_types.h"
#include "ln_utils.h"
#include <stdbool.h>
#include "ln882h.h"
#include "hal/hal_syscon.h"

#include "hal/hal_gpio.h"

#include "hal/hal_sleep.h"
#include "utils/debug/log.h"
//#include "osal/osal.h"

#define SLEEP_TIMER_EXTRA_COMPENSATION_USEC                             (220)
#define PRE_SLEEP_LOOP_COUNT            10
#if 0
#define RAM_RETENTION           (1 << 0)
#define RAM_MAC                 (1 << 1)
#define RAM_CPU                 (1 << 2)
#endif

typedef struct
{
    int                   irq_num;
    sleep_condition_cb_t  sleep_condition;
    sleep_processing_cb_t pre_sleep;
    sleep_processing_cb_t post_sleep;
}hal_sleep_action_t ;

typedef struct {
    sleep_mode_enum_t       sleep_mode;
    uint32_t                peripheral_mod;
    hal_sleep_action_t      actions[29];
}hal_sleep_config_t;

typedef struct
{
    bool                    flag;
    hal_sleep_config_t      sleep_config;
    uint32_t                nvic_int_en[2];
    uint64_t                compensation;
    uint32_t                msec;
}hal_sleep_ctrl_t ;

hal_sleep_ctrl_t g_sleep_ctrl __attribute__((section("retention_data")));

static hal_sleep_ctrl_t *hal_sleep_get_handle(void)
{
    return &g_sleep_ctrl;
}

void EXTERNAL_IRQHandler(void)
{
    HAL_SYSCON_EXT_INTR_Clear();
}

void SLEEP_IRQHandler(void)
{
    //do nothing
}

static void pre_sleep_processing_hardware(sleep_mode_enum_t sleep_mode, uint8_t sram_powerdown)
{
    int i = 0;
    hal_sleep_ctrl_t *sleep_ctrl = hal_sleep_get_handle();
    hal_sleep_config_t *sleep_config = &(sleep_ctrl->sleep_config);

    if(sleep_mode >= RETENTION_SLEEP){
        HAL_SYSCON_AWO_sram_pd_en(sram_powerdown);
        HAL_SYSCON_AWO_sram_po_en(0x07);
        HAL_SYSCON_AWO_pmu_ret_en(1);
        HAL_SYSCON_AWO_pmu_unret_en(1);
        HAL_SYSCON_AWO_pdcmp_po(0);
        if(sleep_mode == FROZEN_SLEEP){
            HAL_SYSCON_AWO_cpucore_retreg_po(0);//set 0 to enter frozon mode
            HAL_SYSCON_AWO_idle_reg_set(0x52);
        }else{
            HAL_SYSCON_AWO_cpucore_retreg_po(1);
            HAL_SYSCON_AWO_lp_mode_awo(1);
        }
    }else{//sleep_mode <= DEEP_SLEEP
        HAL_SYSCON_AWO_sram_pd_en(sram_powerdown);
        HAL_SYSCON_AWO_pmu_ret_en(0);
        HAL_SYSCON_AWO_pmu_unret_en(0);

        // if phy needs power down, then save phy reg to cmp
        HAL_SYSCON_CMP_rfreg_len(0x158);
        HAL_SYSCON_CMP_rfreg_base_addr(0x0);
        HAL_SYSCON_CMP_rfreg_action_set(0x1);//bit 0 is save rf register
        while(HAL_SYSCON_CMP_save_done_get() != 1);
    }

    if((sleep_mode >= DEEP_SLEEP) && (sleep_mode <= RETENTION_SLEEP)){
        ll_sleep_mac_tsf_clk_switch(1);//switch to 32k and power saving
        HAL_SYSCON_AWO_clk_sel_set(0); // switch to crystal clock
    }

    //set CM4 sleep mode
    if(sleep_mode == LIGHT_SLEEP){
        HAL_SYSCON_AWO_sw_pwr_ctrl_set(0x02);
        while(i++ < PRE_SLEEP_LOOP_COUNT){
            __NOP();
        }
        while(HAL_SYSCON_AWO_pmu_fsm_get() != 0);
        HAL_SYSCON_CMP_sw_clkg_set(sleep_config->peripheral_mod|(1<<MOD_QSPI)|(1<<MOD_CACHE)|(1<<MOD_DMA)|(1<<MOD_RFREG));
    }else{
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    }
}
static void post_sleep_processing_hardware(sleep_mode_enum_t sleep_mode)
{
    if(sleep_mode <= DEEP_SLEEP){
        if(sleep_mode == LIGHT_SLEEP){
            HAL_SYSCON_AWO_sw_pwr_ctrl_set(0x01);
            while(HAL_SYSCON_AWO_pmu_fsm_get() != 0);
            HAL_SYSCON_CMP_sw_clkg_set(0xFFFFFFFF);
        }else{
            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
            SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
        }
        HAL_SYSCON_AWO_pmu_ret_en(0);
        HAL_SYSCON_AWO_pmu_unret_en(0);

       //restore RF register firstly
        HAL_SYSCON_CMP_rfreg_len(0x158);
        HAL_SYSCON_CMP_rfreg_base_addr(0x0);
        HAL_SYSCON_CMP_rfreg_action_set(0x2);//bit 1 is restore rf register
        while(HAL_SYSCON_CMP_restore_done_get() != 1);
    }

    if((sleep_mode >= DEEP_SLEEP) && (sleep_mode <= RETENTION_SLEEP)){
        ll_sleep_mac_tsf_clk_switch(0);//switch to 40M
        HAL_SYSCON_AWO_clk_sel_set(1); // switch to crystal clock
    }
}
static int hal_sleep_peripheral_module_to_IRQn(hal_peripheral_module_t peripheral_module)
{
    int ret = 0;

    switch(peripheral_module){
        case MOD_QSPI:
            ret = QSPI_IRQn;
            break;
        case MOD_ADDC:
            ret = ADC_IRQn;
            break;
        case MOD_I2S:
            ret = I2S_IRQn;
            break;
        case MOD_GPIO:
            ret = GPIO_IRQn;
            break;
        case MOD_SPIM:
            ret = SPI0_IRQn;
            break;
        case MOD_SPIS:
            ret = SPI2_IRQn;
            break;
        case MOD_I2C0:
            ret = I2C0_IRQn;
            break;
        case MOD_I2C1:
            ret = I2C1_IRQn;
            break;
        case MOD_UART0:
            ret = UART0_IRQn;
            break;
        case MOD_UART1:
            ret = UART1_IRQn;
            break;
        case MOD_SPIM2:
            ret = SPI1_IRQn;
            break;
        case MOD_WDT:
            ret = WDT_IRQn;
            break;
        case MOD_TIMER:
        case MOD_TIMER1:
        case MOD_TIMER2:
        case MOD_TIMER3:
        case MOD_TIMER4:
            ret = TIMER_IRQn;
            break;
        case MOD_SDIO:
            ret = SDIO_FUN1_IRQn;
            break;
        case MOD_MAC:
            ret = MAC_IRQn;
            break;
        case MOD_DMA:
            ret = DMA_IRQn;
            break;
        case MOD_PWM:
            ret = PWM_IRQn;
            break;
        case MOD_TRNG:
            ret = TRNG_IRQn;
            break;
        case MOD_AES:
            ret = AES_IRQn;
            break;
        case MOD_EXT_INT:
            ret = EXTERNAL_IRQn;
            break;
        case MOD_DGBH:
        case MOD_CACHE:
        case MOD_RFREG:
        case MOD_EF:
        default:
            break;
    }
    return ret;
}

static int log2(uint32_t val)
{
    int ret = 0;
    switch(val)
    {
        case 0x1: ret = 0; break;
        case 0x2: ret = 1; break;
        case 0x4: ret = 2; break;
        case 0x8: ret = 3; break;
        case 0x10: ret = 4; break;
        case 0x20: ret = 5; break;
        case 0x40: ret = 6; break;
        case 0x80: ret = 7; break;
        case 0x100: ret = 8; break;
        case 0x200: ret = 9; break;
        case 0x400: ret = 10; break;
        case 0x800: ret = 11; break;
        case 0x1000: ret = 12; break;
        case 0x2000: ret = 13; break;
        case 0x4000: ret = 14; break;
        case 0x8000: ret = 15; break;
        case 0x10000: ret = 16; break;
        case 0x20000: ret = 17; break;
        case 0x40000: ret = 18; break;
        case 0x80000: ret = 19; break;
        case 0x100000: ret = 20; break;
        case 0x200000: ret = 21; break;
        case 0x400000: ret = 22; break;
        case 0x800000: ret = 23; break;
        case 0x1000000: ret = 24; break;
        case 0x2000000: ret = 25; break;
        case 0x4000000: ret = 26; break;
        case 0x8000000: ret = 27; break;
        case 0x10000000: ret = 28; break;
        case 0x20000000: ret = 29; break;
        case 0x40000000: ret = 30; break;
        case 0x80000000: ret = 31; break;
        default: break;
    }
    return ret;
}

/**
 * @brief  Check whether can sleep
 *
 * @param  none
 *
 * @note   If can sleep, return true; else return flase.
 */
static bool hal_sleep_condition_check(hal_sleep_config_t *sleep_config)
{
    bool ret = true;
    hal_sleep_action_t *action;
    uint32_t value, tmp, idx;

    if(!sleep_config){
        return false;
    }
    value = sleep_config->peripheral_mod;
    while(value){
        tmp = value & (value - 1);
        idx = log2(value - tmp);
        action = &(sleep_config->actions[idx]);
        if(action->sleep_condition){
            ret = ret && action->sleep_condition();
        }
        value = tmp;
    }
    return ret;
}

static void hal_pre_sleep_processing_registered_cb(hal_sleep_config_t *sleep_config)
{
    hal_sleep_action_t *action;
    uint32_t value, tmp, idx;

    if(!sleep_config){
        return;
    }
    value = sleep_config->peripheral_mod;
    while(value){
        tmp = value & (value - 1);
        idx = log2(value - tmp);
        action = &(sleep_config->actions[idx]);
        if(sleep_config->sleep_mode >= DEEP_SLEEP){
            NVIC_EnableIRQ((IRQn_Type)action->irq_num);
        }
        if(action->pre_sleep){
            action->pre_sleep();
        }
        value = tmp;
    }
}
static void hal_post_sleep_processing_registered_cb(hal_sleep_config_t *sleep_config)
{
    hal_sleep_action_t *action;
    uint32_t value, tmp, idx;

    if(!sleep_config){
        return;
    }
    value = sleep_config->peripheral_mod;
    while(value){
        tmp = value & (value - 1);
        idx = log2(value - tmp);
        action = &(sleep_config->actions[idx]);
        if(action->post_sleep){
            action->post_sleep();
        }
        value = tmp;
    }
}

void hal_pre_sleep_processing(uint32_t *xExpectedIdleTime)
{
    hal_sleep_ctrl_t *sleep_ctrl = hal_sleep_get_handle();
    sleep_mode_enum_t sleep_mode = hal_sleep_get_mode();
    uint64_t u64val = 0;
    uint8_t sram_powerdown = 0x00;//RAM_RETENTION | RAM_MAC | RAM_CPU
    hal_sleep_config_t *sleep_config = &(sleep_ctrl->sleep_config);

    if(sleep_mode == ACTIVE || sleep_ctrl->flag)
    {
        return;
    }

    if(!hal_sleep_condition_check(sleep_config)){
        return;
    }

    sleep_ctrl->msec = *xExpectedIdleTime;

    sleep_ctrl->flag = true;
    /*
    * In light sleep mode:
    *     All interrupts can wake up the system, including sysTick and peripheral.
    *     Therefore, use systick to calculate system time.
    *
    * In deep/retention/frozen sleep mode:
    *     Use the sleep timer to calculate system time.
    */
    if(sleep_mode >= DEEP_SLEEP){
        /*
        * Save the interrupt enable register of the CPU(CM4), so that the value of the register can be restored after wake-up.
        */
        sleep_ctrl->nvic_int_en[0] = NVIC->ISER[0];
        sleep_ctrl->nvic_int_en[1] = NVIC->ISER[1];

        /*
        * Disable off all interrupts of the CPU(CM4)
        */
        NVIC->ICER[0] = 0xFFFFFFFF;
        NVIC->ICER[1] = 0xFFFFFFFF;

        /*
        * Set sleep timer
        */
        sleep_ctrl->compensation = HAL_SYSCON_CalculateCompensateNs() + (SLEEP_TIMER_EXTRA_COMPENSATION_USEC*1000);
        if(((uint64_t)*xExpectedIdleTime * 1000000000 / OS_HZ) > sleep_ctrl->compensation){
            u64val = ((uint64_t)*xExpectedIdleTime * 1000000000 / OS_HZ) - sleep_ctrl->compensation;
        }else{
            /*
            * If the value is illegal, set the default value
            */
            u64val = 1000000;
        }
        HAL_SYSCON_CPUSleepDurationEnable(u64val);
        NVIC_EnableIRQ(SLEEP_IRQn);
    }

    hal_pre_sleep_processing_registered_cb(sleep_config);
    pre_sleep_processing_hardware(sleep_mode, sram_powerdown);
}
int hal_post_sleep_processing(uint32_t *xExpectedIdleTime)
{
    int ret = -1;
    uint64_t sleep_ns = 0, real0=0;

    hal_sleep_ctrl_t *sleep_ctrl = hal_sleep_get_handle();
    sleep_mode_enum_t sleep_mode = hal_sleep_get_mode();

    if(sleep_mode == ACTIVE || !sleep_ctrl->flag){
        return -1;
    }

#if 0
    /*
    * If flash is turned off in retention and frozen, It must be re-enabled after waking up.
    */
    if(sleep_mode >= RETENTION_SLEEP){
        hal_flash_init();
        //Enable QSPI 4bit mode
        hal_flash_quad_mode_enable(1);
        //Init Flash cache
        flash_cache_init(0);
        log_init();
    }
#endif

    sleep_ctrl->flag = false;
    post_sleep_processing_hardware(sleep_mode);

    /*
    * Restored the interrupt enable register of the CPU(CM4) after wake-up.
    */
    NVIC->ISER[0] = sleep_ctrl->nvic_int_en[0];
    NVIC->ISER[1] = sleep_ctrl->nvic_int_en[1];
    if(sleep_mode >= DEEP_SLEEP){
        /*
        * Calculate actual sleep time.
        */
        real0 = HAL_SYSCON_RealSleepTime();
        sleep_ns = real0 + sleep_ctrl->compensation;
        *xExpectedIdleTime = MIN(sleep_ctrl->msec, (sleep_ns * OS_HZ) / 1000000000);
        ret = 0;
    }

    hal_post_sleep_processing_registered_cb(&(sleep_ctrl->sleep_config));
    return ret;
}

int hal_sleep_set_mode(sleep_mode_enum_t sleep_mode)
{
    hal_sleep_ctrl_t *sleep_ctrl = hal_sleep_get_handle();

    sleep_ctrl->sleep_config.sleep_mode = sleep_mode;

    return 0;
}

sleep_mode_enum_t hal_sleep_get_mode(void)
{
    hal_sleep_ctrl_t *sleep_ctrl = hal_sleep_get_handle();
    return sleep_ctrl->sleep_config.sleep_mode;
}

int hal_sleep_register(hal_peripheral_module_t peripheral_module, sleep_condition_cb_t sleep_condition, sleep_processing_cb_t pre_sleep, sleep_processing_cb_t post_sleep)
{
    hal_sleep_ctrl_t *sleep_ctrl = hal_sleep_get_handle();
    hal_sleep_config_t *sleep_config = &(sleep_ctrl->sleep_config);
    hal_sleep_action_t *action;

    if(peripheral_module >= MOD_MAX || peripheral_module < MOD_QSPI){
        return -1;
    }

    action = &(sleep_config->actions[peripheral_module]);
    LN_SET_BIT(sleep_config->peripheral_mod, peripheral_module);
    if(MOD_TIMER1 == peripheral_module || MOD_TIMER2 == peripheral_module || MOD_TIMER3 == peripheral_module || MOD_TIMER4 == peripheral_module){
        LN_SET_BIT(sleep_config->peripheral_mod, MOD_TIMER);
    }
    action->irq_num = hal_sleep_peripheral_module_to_IRQn(peripheral_module);
    if(sleep_condition){
        action->sleep_condition = sleep_condition;
    }
    if(pre_sleep){
        action->pre_sleep = pre_sleep;
    }
    if(post_sleep){
        action->post_sleep = post_sleep;
    }
    return 0;
}
int hal_sleep_unregister(hal_peripheral_module_t peripheral_module)
{
    hal_sleep_ctrl_t *sleep_ctrl = hal_sleep_get_handle();
    hal_sleep_config_t *sleep_config = &(sleep_ctrl->sleep_config);
    hal_sleep_action_t *action;

    if(peripheral_module >= MOD_MAX || peripheral_module < MOD_QSPI){
        return -1;
    }

    action = &(sleep_config->actions[peripheral_module]);
    LN_CLR_BIT(sleep_config->peripheral_mod, peripheral_module);
    action->irq_num = 0;
    action->sleep_condition = NULL;
    action->pre_sleep = NULL;
    action->post_sleep = NULL;
    return 0;
}

