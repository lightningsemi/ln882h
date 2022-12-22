#include "rwip_config.h"    // RW SW configuration TASK_APP
#include "co_utils.h"
#include "ke_task.h"        // Kernel Task
#include "gattm_task.h"     // GATT Manager Task API
#include "gattc_task.h"     // GATT Controller Task API

#include "ln_app_gap.h"
#include "ln_gap_gatt_callback.h"

#include "usr_ble_app.h"
#include "osal/osal.h"

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

static int app_msg_callback(ke_msg_id_t const msgid, void const *param,
                            ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    // Retrieve identifier of the task from received message
    ke_task_id_t src_task_id = MSG_T(msgid);

    LOG(LOG_LVL_INFO,"[%s]: src_task_id=%d, msgid=0x%x\r\n", __func__, src_task_id, msgid);
    return (KE_MSG_CONSUMED);
}


KE_MSG_HANDLER_TAB(app)
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,    (ke_msg_func_t)app_msg_callback},
    
    //GAPM
    {GAPM_CMP_EVT,              gapm_cmp_evt_callback},
    {GAPM_DEV_VERSION_IND,      gapm_dev_version_ind_callback},
    {GAPM_DEV_BDADDR_IND,       gapm_dev_bdaddr_ind_callback},
    {GAPM_ACTIVITY_CREATED_IND, gapm_activity_created_ind_callback},
    {GAPM_ACTIVITY_STOPPED_IND, gapm_activity_stopped_ind_callback},
    {GAPM_EXT_ADV_REPORT_IND,   gapm_ext_adv_report_ind_callback},
    {GAPM_SCAN_REQUEST_IND,     gapm_scan_request_ind_callback},
    {GAPM_PROFILE_ADDED_IND,    gapm_profile_added_ind_callback},
    
    //GAPC
    {GAPC_CMP_EVT,              gapc_cmp_evt_callback},
    {GAPC_CONNECTION_REQ_IND,   gapc_connection_req_ind_callback},
    {GAPC_DISCONNECT_IND,       gapc_disconnect_ind_callback},
    {GAPC_CON_RSSI_IND,         gapc_con_rssi_ind_callback},
    {GAPC_PARAM_UPDATED_IND,    gapc_param_updated_ind_callback},
    {GAPC_LE_PKT_SIZE_IND,      gapc_le_pkt_size_ind_callback},
    {GAPC_GET_DEV_INFO_REQ_IND, gapc_get_dev_info_req_ind_callback},
    {GAPC_SET_DEV_INFO_REQ_IND, gapc_set_dev_info_req_ind_callback},
    {GAPC_PARAM_UPDATE_REQ_IND, gapc_param_update_req_ind_callback},    
    
    //GATTM
    {GATTM_ADD_SVC_RSP,         gattm_add_svc_rsp_callback},
    {GATTM_UNKNOWN_MSG_IND,     gattm_unknown_msg_ind_callback},

    //GATTC
    {GATTC_CMP_EVT,             gattc_cmp_evt_callback},
    {GATTC_MTU_CHANGED_IND,     gattc_mtu_changed_ind_callback},
    {GATTC_READ_IND,            gattc_read_ind_callback},
    {GATTC_READ_REQ_IND,        gattc_read_req_ind_callback},
    {GATTC_WRITE_REQ_IND,       gattc_write_req_ind_callback},
};

/// Number of APP Task Instances
#define APP_IDX_MAX                 (1)

ke_state_t app_state[APP_IDX_MAX];

const struct ke_task_desc TASK_DESC_APP = {app_msg_handler_tab, app_state, APP_IDX_MAX, ARRAY_LEN(app_msg_handler_tab)};




