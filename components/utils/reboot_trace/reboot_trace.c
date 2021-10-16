#include "ln882h.h"
#include "ln_compiler.h"
#include "ln_types.h"
#include "hal/hal_wdt.h"
#include "utils/reboot_trace/reboot_trace.h"

static uint8_t  no_init_data[128]  __attribute__((section(".no_init_data"),aligned(8))) __ZERO_INIT__;

static int ln_chip_set_reboot_cause(chip_reboot_cause_t cause)
{
    chip_no_init_data_t * pdata = NULL;
    pdata = (chip_no_init_data_t *) no_init_data;

    switch (cause)
    {
        case CHIP_REBOOT_WATCHDOG:
            pdata->reboot_magic = WATCHDOG_BOOT_MAGIC;
            break;
        case CHIP_REBOOT_SOFTWARE:
            pdata->reboot_magic = SOFTWARE_BOOT_MAGIC;
            break;
        default:
            return LN_FALSE;
    }

    return LN_TRUE;
}

// 1.just called once. 2.Call it in the first phase of the main function.
chip_reboot_cause_t ln_chip_get_reboot_cause(void)
{
    chip_reboot_cause_t cause = CHIP_REBOOT_POWER_ON;
    chip_no_init_data_t * pdata = NULL;
    pdata = (chip_no_init_data_t *) no_init_data;

    if (pdata->reboot_magic == WATCHDOG_BOOT_MAGIC) {
        cause = CHIP_REBOOT_WATCHDOG;
    }
    else if(pdata->reboot_magic == SOFTWARE_BOOT_MAGIC) {
        cause = CHIP_REBOOT_SOFTWARE;
    }
    else{
        cause = CHIP_REBOOT_POWER_ON;
    }

    ln_chip_set_reboot_cause(CHIP_REBOOT_WATCHDOG);

    return cause;
}

void ln_chip_reboot(void)
{
//    HAL_SYSCON_CPUResetReqMask(0);
    ln_chip_set_reboot_cause(CHIP_REBOOT_SOFTWARE);
    NVIC_SystemReset();
}

void ln_chip_watchdog_keepalive(void)
{
//    hal_wdt_rstart(REG_WDT_BASE);
}

// void WDT_IRQHandler(void)
// {
//     //nothing to do.
// }

void ln_chip_watchdog_start(watchdog_timeout_level_t level)
{
    wdt_init_t_def config;

    config.wdt_rpl   = WDT_RPL_256_PCLK;
//    config.wdt_rmod  = WDT_RMOD_SET;

    switch (level)
    {
        case WDT_TIMEOUT_LEVEL0:
            config.top = WDT_TOP_VALUE_0;
            break;
        case WDT_TIMEOUT_LEVEL1:
            config.top = WDT_TOP_VALUE_1;
            break;
        case WDT_TIMEOUT_LEVEL2:
            config.top = WDT_TOP_VALUE_2;
            break;
        case WDT_TIMEOUT_LEVEL3:
            config.top = WDT_TOP_VALUE_3;
            break;
        case WDT_TIMEOUT_LEVEL4:
            config.top = WDT_TOP_VALUE_4;
            break;
        case WDT_TIMEOUT_LEVEL5:
            config.top = WDT_TOP_VALUE_5;
            break;
        case WDT_TIMEOUT_LEVEL6:
            config.top = WDT_TOP_VALUE_6;
            break;
        case WDT_TIMEOUT_LEVEL7:
            config.top = WDT_TOP_VALUE_7;
            break;
        case WDT_TIMEOUT_LEVEL8:
            config.top = WDT_TOP_VALUE_8;
            break;

        default:
            config.top = WDT_TOP_VALUE_8;
            break;
    }

   hal_wdt_init(WDT_BASE, &config);
   hal_wdt_en(WDT_BASE, HAL_ENABLE);
}

