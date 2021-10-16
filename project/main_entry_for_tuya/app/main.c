#include "proj_config.h"
#include "osal/osal.h"
#include "wifi.h"
#include "netif/ethernetif.h"
#include "usr_app.h"
#include "wifi_manager.h"
#include "utils/system_parameter.h"
#include "hal/hal_interrupt.h"
#include "utils/debug/CmBacktrace/cm_backtrace.h"
#include "ln_nvds.h"
#include "ln_kv_api.h"
#include "flash_partition_table.h"
#include "utils/reboot_trace/reboot_trace.h"
#include "utils/runtime/runtime.h"

#include "ota_port.h"

int main (int argc, char* argv[])
{
    *(uint32_t *)(0x40100098) = 0x13141517;//Open DBG-Host

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

    cm_backtrace_init("tuya", "HW_V1.0", "SW_V1.0");
    LOG(LOG_LVL_INFO, "------  tuya project  ------\r\n");

    if (NVDS_ERR_OK != ln_nvds_init(NVDS_SPACE_OFFSET)) {
        LOG(LOG_LVL_ERROR, "NVDS init filed!\r\n");
    }

    if (KV_ERR_NONE != ln_kv_port_init(KV_SPACE_OFFSET, (KV_SPACE_OFFSET + KV_SPACE_SIZE))) {
        LOG(LOG_LVL_ERROR, "KV init filed!\r\n");
    }

	//init system parameter
	sysparam_integrity_check_all();

    ota_port_init();

    //Init wifi
    wifi_init();

    //Init lwip stack(Creat lwip tack).
    TUYA_LwIP_Init();

    //Init wifi manager(Creat wifi manager task).
    wifi_manager_init();

    //Creat usr app task.
    creat_usr_app_task();
    app_init_entry();

    OS_ThreadStartScheduler();

    return 0;
}

// ----------------------------------------------------------------------------
