#include "proj_config.h"
#include "osal/osal.h"
#include "hal/hal_interrupt.h"
#include "ln_utils.h"
#include "ln_misc.h"
#include "ln_at.h"
#include "utils/debug/CmBacktrace/cm_backtrace.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/runtime/runtime.h"
#include "ln_kv_api.h"
#include "ln_nvds.h"
#include "flash_partition_table.h"

#include "ln_ble_app_kv.h"
#include "usr_app.h"
#include "ln_ble_app_default_cfg.h"


int main (int argc, char* argv[])
{
    LN_UNUSED(argc);
    LN_UNUSED(argv);

    //0. check reboot cause
    ln_chip_get_reboot_cause();

    //1.sys clock,interrupt
    SetSysClock();
    set_interrupt_priority();
    switch_global_interrupt(true);
    ln_runtime_measure_init();

    //2. register os heap mem
    OS_DefineHeapRegions();

    //3.rf cfg
    extern void ln882h_ble_single_mode_rf_cfg(void);
    ln882h_ble_single_mode_rf_cfg();

    //4.init log&AT
    log_init();
    if (LN_AT_ERR_NONE != ln_at_init()) {
        LOG(LOG_LVL_ERROR, "ln at init fail.\r\n");
        return -1;
    }
    cm_backtrace_init("ble_mcu_prf_battery", "HW_V1.0", "SW_V1.0");
    LOG(LOG_LVL_INFO, "------  ble_mcu_prf_battery  ------\r\n");

    //5.init nvds & kv
    if (NVDS_ERR_OK != ln_nvds_init(NVDS_SPACE_OFFSET)) {
        LOG(LOG_LVL_ERROR, "NVDS init filed!\r\n");
    }

    if (KV_ERR_NONE != ln_kv_port_init(KV_SPACE_OFFSET, (KV_SPACE_OFFSET + KV_SPACE_SIZE))) {
        LOG(LOG_LVL_ERROR, "KV init filed!\r\n");
    }

    //6.Creat usr app task.
    creat_usr_app_task();

    OS_ThreadStartScheduler();

    return 0;
}

// ----------------------------------------------------------------------------
