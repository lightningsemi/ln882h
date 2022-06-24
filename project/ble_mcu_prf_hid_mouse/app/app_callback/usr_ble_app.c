#include "rwip_config.h" // RW SW configuration
#include "rwprf_config.h"
#include "ble_arch/arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition

#include "rwip.h"      // RW SW initialization
#include "ke_task.h"   // Kernel Task

#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
#include "rf.h"        // RF initialization
#endif // BLE_EMB_PRESENT || BT_EMB_PRESENT

//LN 
#include "utils/debug/log.h"
#include "osal/osal.h"
// #include "ln_def.h"
#include "ln_misc.h"
#include "ln_app_gap.h"
#include "ble_nvds.h"
#include "flash_partition_table.h"
#include "usr_app.h"
#include "usr_ble_app.h"

void ble_app_init(void)
{
    /// Application Task Descriptor
    extern const struct ke_task_desc TASK_DESC_APP;

    // Create APP task
    ke_task_create(TASK_APP, &TASK_DESC_APP);

    // Initialize Task state
    ke_state_set(TASK_APP, APP_INIT);
    // Reset the stack
    ln_app_gapm_reset();
}
