#include "proj_config.h"
#include "osal/osal.h"
#include "wifi.h"
#include "hal/hal_interrupt.h"
#include "ln_utils.h"
#include "ln_nvds.h"
#include "ln_kv_api.h"
#include "ln_at.h"
#include "flash_partition_table.h"
#include "utils/debug/CmBacktrace/cm_backtrace.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/runtime/runtime.h"
#include "usr_app.h"


int main (int argc, char* argv[])
{
    LN_UNUSED(argc);
    LN_UNUSED(argv);

    *(uint32_t *)(0x40100098) = 0x13141517;

    //0. check reboot cause
    ln_chip_get_reboot_cause();

    //1.sys clock,interrupt
    SetSysClock();
    set_interrupt_priority();
    switch_global_interrupt(true);
    ln_runtime_measure_init();

    //2. register os heap mem
    OS_DefineHeapRegions();

    //3.rf preprocess,img cal
    wifi_rf_preprocess();
    wifi_rf_image_cal();

    //4.init log&AT
    log_init();
    if (LN_AT_ERR_NONE != ln_at_init()) {
        LOG(LOG_LVL_ERROR, "ln at init fail.\r\n");
        return -1;
    }

    cm_backtrace_init("ble_mcu_data_trans", "HW_V1.0", "SW_V1.0");
    LOG(LOG_LVL_INFO, "------  ble_mcu_data_trans  ------\r\n");
    extern void rw_init(void);
    rw_init();
    ble_creat_usr_app_task();
    OS_ThreadStartScheduler();

    return 0;
}

// ----------------------------------------------------------------------------
