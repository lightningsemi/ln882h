#include "proj_config.h"
#include "osal/osal.h"
#include "hal/hal_interrupt.h"
#include "ln_utils.h"
#include "ln_misc.h"
#include "ln_kv_api.h"
#include "ln_nvds.h"
#include "ble_port/arch.h" 
#include "utils/debug/CmBacktrace/cm_backtrace.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/runtime/runtime.h"
#include "flash_partition_table.h"
#include "usr_app.h"

static void ble_rf_config(void)
{
    extern void ln882h_ble_single_mode_rf_cfg(void);
    ln882h_ble_single_mode_rf_cfg();
}

int main (int argc, char* argv[])
{
    LN_UNUSED(argc);
    LN_UNUSED(argv);

    //0. check reboot cause
    ln_chip_get_reboot_cause();

    //1. sys clock & interrupt
    SetSysClock();
    set_interrupt_priority();
    switch_global_interrupt(HAL_ENABLE);
    ln_runtime_measure_init();

    //2. register os heap mem
    OS_DefineHeapRegions();

    //3. ble rf cfg
    ble_rf_config();

    //4. init log
    log_init();

    cm_backtrace_init("ble_cam", "HW_V1.0", "SW_V1.0");
    LOG(LOG_LVL_INFO, "------  ble data trans demo ------\r\n");

    //5. init kv & nvds
    if (KV_ERR_NONE != ln_kv_port_init(KV_SPACE_OFFSET, (KV_SPACE_OFFSET + KV_SPACE_SIZE))) {
        LOG(LOG_LVL_ERROR, "KV init filed!\r\n");
    }
    if (NVDS_ERR_OK != ln_nvds_init(NVDS_SPACE_OFFSET)) {
        LOG(LOG_LVL_ERROR, "NVDS init filed!\r\n");
    }

    //6. load config info
    ln_load_config_info();
    
    //7. init ble stack
    ble_init(ln_get_mac_array(true));

    //8. creat usr app task
    creat_usr_app_task();
    
    OS_ThreadStartScheduler();

    return 0;
}

// ----------------------------------------------------------------------------
