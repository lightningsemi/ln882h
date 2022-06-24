#include "proj_config.h"
#include "rwip_config.h"
#include "ble_nvds.h"
#include "osal/osal.h"
#include "hal/hal_interrupt.h"
#include "ln_utils.h"
#include "ln_misc.h"
#include "ln_kv_api.h"
#include "flash_partition_table.h"
#include "utils/debug/CmBacktrace/cm_backtrace.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/runtime/runtime.h"
#include "usr_app.h"

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

    //4.init log 
    log_init();
        cm_backtrace_init("ble_mcu_prf_hid_mouse", "HW_V1.0", "SW_V1.0");
    LOG(LOG_LVL_INFO, "------  ble_mcu_prf_hid_mouse  ------\r\n");
    if (KV_ERR_NONE != ln_kv_port_init(KV_SPACE_OFFSET, (KV_SPACE_OFFSET + KV_SPACE_SIZE))) 
    {
        LOG(LOG_LVL_ERROR, "KV init filed!\r\n");
    }

    {
        uint8_t mac[6] ={0x15,0x48,0x55,0x88,0x55,0x88};  //now,hid dowmo ble address can't use random.then device can auto connect restart power up
       // ln_generate_random_mac(mac);
       // mac[5] |= 0xC0; // This address is random generated, so assign 0x11 => Static Random Address

        extern void rw_init(uint8_t mac[6]);
        rw_init(mac);
    }

    ble_creat_usr_app_task();
    OS_ThreadStartScheduler();

    return 0;
}

// ----------------------------------------------------------------------------
