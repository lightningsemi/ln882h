/**
 ****************************************************************************************
 *
 * @file ln_gap_callback.c
 *
 * @brief GAP callback function  source code
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration

#include <string.h>
#include "ln_app_gap.h"

#include "gapm_task.h"               // GAP Manallger Task API
#include "gapc_task.h"               // GAP Controller Task API
#include "gattm_task.h"              // GAP Manallger Task API
#include "gattc_task.h"              // GAP Controller Task API
#include "co_math.h"                 // Common Maths Definition
#include "co_utils.h"

#include "rwip_task.h"              // Task definitions
#include "ke_task.h"                // Kernel Task

#include "ln_gatt_callback.h"

#include "ble_arch/arch.h"
#include "tuya_os_adapter.h"
#include "usr_ble_app.h"


#define TRACE_ENABLE    (1)

#if (TRACE_ENABLE)
#include "utils/debug/log.h"
#endif


extern TY_BT_MSG_CB ty_bt_msg_cb;
extern TY_BT_SCAN_ADV_CB ty_bt_scan_adv_cb;

extern uint8_t g_adv_actv_idx;
// extern uint8_t init_actv_idx;
extern uint8_t g_scan_actv_idx;

extern uint8_t g_adv_is_started;
extern uint8_t g_conidx;
extern uint8_t g_bt_rssi;


static int gapm_activity_created_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapm_activity_created_ind *p_param = (struct gapm_activity_created_ind *)param;
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE, "gapm_activity_created_ind_handler: actv_idx=%d,actv_type=%d \r\n",p_param->actv_idx,p_param->actv_type);
#endif

    if(p_param->actv_type  ==GAPM_ACTV_TYPE_ADV)
        g_adv_actv_idx =p_param->actv_idx;
    // if(p_param->actv_type  ==GAPM_ACTV_TYPE_INIT)
    //     init_actv_idx =p_param->actv_idx;
    if (p_param->actv_type  == GAPM_ACTV_TYPE_SCAN)
    {
        g_scan_actv_idx = p_param->actv_idx;
    }
    return (KE_MSG_CONSUMED);
}


static int gapm_activity_stopped_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapm_activity_stopped_ind *p_param = (struct gapm_activity_stopped_ind *)param;
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapm_activity_stopped_ind_handler   actv_id=%d   actv_type=%d, reason=0x%x\r\n", p_param->actv_idx, p_param->actv_type,p_param->reason);
#endif

    if (p_param->actv_type == GAPM_ACTV_TYPE_ADV && g_adv_actv_idx == p_param->actv_idx)
    {
        // adv have stopped
        g_adv_is_started = 0;
    }

    if (p_param->actv_type == GAPM_ACTV_TYPE_SCAN && g_scan_actv_idx == p_param->actv_idx)
    {
        // scan stop
    }

    return (KE_MSG_CONSUMED);
}

static int gapm_ext_adv_report_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapm_ext_adv_report_ind *p_param = (struct gapm_ext_adv_report_ind *)param;
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE, "---- adv_report: ----\r\n");
    LOG(LOG_LVL_TRACE, "\taddr_type=%d, addr=0x%x:0x%x:0x%x:0x%x:0x%x:0x%x, "
        "rssi=%d, tx_pwr=%d, len:%d\r\n",
        p_param->trans_addr.addr_type,
        p_param->trans_addr.addr.addr[0], p_param->trans_addr.addr.addr[1],
        p_param->trans_addr.addr.addr[2], p_param->trans_addr.addr.addr[3],
        p_param->trans_addr.addr.addr[4], p_param->trans_addr.addr.addr[5],
        p_param->rssi, p_param->tx_pwr, p_param->length);
    LOG(LOG_LVL_TRACE, "---------------------\r\n");

    hexdump(LOG_LVL_TRACE, "[recv data]", (void *)p_param->data, p_param->length);
#endif
    extern void ty_bt_scan_adv_data_parser(struct gapm_ext_adv_report_ind *adv);
    ty_bt_scan_adv_data_parser(p_param);

    return KE_MSG_CONSUMED;
}

static int gapm_scan_request_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapm_scan_request_ind_handler\r\n");
#endif

    return (KE_MSG_CONSUMED);
}

static int gapm_cmp_evt_handler(ke_msg_id_t const msgid,
                                void const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    struct gapm_cmp_evt *p_param = (struct gapm_cmp_evt *)param;
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"app_gapm_cmp_evt_handler: operation=0x%x, status=0x%x\r\n", p_param->operation,p_param->status);
#endif
    ke_msg_sync_lock_release();

    switch(p_param->operation)
    {
    // Reset completed
    case (GAPM_RESET):
    {
        if(p_param->status == GAP_ERR_NO_ERROR)
        {
            struct ln_gapm_set_dev_config_cmd cfg_param;
            memset(&cfg_param,0,sizeof(struct ln_gapm_set_dev_config_cmd));
            // Set Data length parameters
            cfg_param.sugg_max_tx_octets = BLE_MIN_OCTETS; // 251  -> BLE_MIN_OCTETS;
            cfg_param.sugg_max_tx_time   = BLE_MIN_TIME;   // 2120 -> BLE_MIN_TIME;

            // Host privacy enabled by default
            cfg_param.privacy_cfg = 0;
            memset((void *)&cfg_param.irk.key[0], 0x00, KEY_LEN);
            cfg_param.role    = GAP_ROLE_OBSERVER | GAP_ROLE_PERIPHERAL; // GAP_ROLE_ALL;
            // cfg_param.max_mtu = 1200;//2048;
            // cfg_param.max_mps = 1200;//2048;
            ln_app_set_dev_config(&cfg_param);
           
        }
        else
        {
            ASSERT_ERR(0);
        }
    }
    break;

    // Device Configuration updated
    case (GAPM_SET_DEV_CONFIG):
    {
        ASSERT_INFO(p_param->status == GAP_ERR_NO_ERROR, p_param->operation, p_param->status);
        // Go to the create db state
        ke_state_set(TASK_APP, APP_CREATE_DB);
    }
    break;

    case GAPM_START_ACTIVITY:
    {
        LOG(LOG_LVL_TRACE, "GAPM_START_ACTIVITY, status:%d\r\n",
                p_param->status);
        if (p_param->status == 0)
        {
            // adv ready
            g_adv_is_started = 1;
            extern int ty_ada_bt_hw_ready_notify(void);
            ty_ada_bt_hw_ready_notify();
        }
        break;
    }

    default:
        break;
    }

    return (KE_MSG_CONSUMED);
}

static int gapm_dev_version_ind_handler(ke_msg_id_t const msgid,
                                        void const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
#if (TRACE_ENABLE)
    struct gapm_dev_version_ind *p_param = (struct gapm_dev_version_ind *)param;
    if (p_param) {
        LOG(LOG_LVL_TRACE, "hci:%d.%d; lmp:%d.%d; host:%d.%d; manuf_name:%02X\r\n",
                p_param->hci_ver, p_param->hci_subver, p_param->lmp_ver, p_param->lmp_subver,
                p_param->host_ver, p_param->host_subver, p_param->manuf_name);
    }
#endif
    return (KE_MSG_CONSUMED);
}

static int gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapc_get_dev_info_req_ind *p_param = (struct gapc_get_dev_info_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE, "[%s:%d] param->req=0x%x,conidx=0x%x\r\n",
            __func__, __LINE__, p_param->req, conidx);
#endif

    switch(p_param->req)
    {
    case GAPC_DEV_NAME:
    {
        struct gapc_get_dev_info_cfm * cfm = KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,
                                             src_id, dest_id,
                                             gapc_get_dev_info_cfm, APP_DEVICE_NAME_MAX_LEN);
        cfm->req = p_param->req;

        // Send message
        ke_msg_send(cfm);
    }
    break;

    case GAPC_DEV_APPEARANCE:
    {
        // Allocate message
        struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                                            src_id, dest_id,
                                            gapc_get_dev_info_cfm);
        cfm->req = p_param->req;
        // Set the device appearance
        // Send message
        ke_msg_send(cfm);
    }
    break;

    case GAPC_DEV_SLV_PREF_PARAMS:
    {
        // Allocate message
        struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                                            src_id, dest_id,
                                            gapc_get_dev_info_cfm);
        cfm->req = p_param->req;
        // Slave preferred Connection interval Min
        cfm->info.slv_pref_params.con_intv_min = 8;
        // Slave preferred Connection interval Max
        cfm->info.slv_pref_params.con_intv_max = 10;
        // Slave preferred Connection latency
        cfm->info.slv_pref_params.slave_latency  = 0;
        // Slave preferred Link supervision timeout
        cfm->info.slv_pref_params.conn_timeout    = 200;  // 2s (200*10ms)

        // Send message
        ke_msg_send(cfm);
    }
    break;

    default: /* Do Nothing */
        break;
    }

    return (KE_MSG_CONSUMED);
}


static int gapc_set_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapc_set_dev_info_req_ind *p_param = (struct gapc_set_dev_info_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapc_set_dev_info_req_ind_handler  param->req=0x%x,conidx=0x%x\r\n",p_param->req,conidx);
#endif

    LOG(LOG_LVL_INFO, "[%s:%d] param->req=0x%x,conidx=0x%x\r\n",
            __func__, __LINE__, p_param->req, conidx);

    // Set Device configuration
    struct gapc_set_dev_info_cfm* cfm = KE_MSG_ALLOC(GAPC_SET_DEV_INFO_CFM, src_id, dest_id,
                                        gapc_set_dev_info_cfm);
    // Reject to change parameters
    cfm->status = 0;
    cfm->req = p_param->req;
    // Send message  //zhongbo
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

static int gapc_connection_req_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapc_connection_req_ind *p_param = (struct gapc_connection_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
    g_conidx = conidx;

#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE," #######    gapc_connection_req_ind_handler conhdl=0x%x,  conidx=0x%x , conn_intv:%d, role=%d    addr: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x  ###########\r\n",p_param->conhdl,conidx,p_param->con_interval,p_param->role,p_param->peer_addr.addr[0],p_param->peer_addr.addr[1],p_param->peer_addr.addr[2],p_param->peer_addr.addr[3],p_param->peer_addr.addr[4],p_param->peer_addr.addr[5]);
#endif

    // Check if the received connection index is valid
    if (conidx != GAP_INVALID_CONIDX)
    {
        {
            // Allocate connection confirmation
            struct gapc_connection_cfm *cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM,
                                            KE_BUILD_ID(TASK_GAPC,conidx), TASK_APP,
                                            gapc_connection_cfm);

            // Send the message
            ke_msg_send(cfm);
        }

        {
            struct gattc_exc_mtu_cmd *p_cmd = KE_MSG_ALLOC(GATTC_EXC_MTU_CMD,
                                            KE_BUILD_ID(TASK_GATTC, conidx), TASK_APP,
                                            gattc_exc_mtu_cmd);
            p_cmd->operation = GATTC_MTU_EXCH;
            /// identify the command
            p_cmd->seq_num = 0;

            // Send the message
            ke_msg_send(p_cmd);
        }

        {
            struct gapc_set_le_pkt_size_cmd *p_cmd = KE_MSG_ALLOC(GAPC_SET_LE_PKT_SIZE_CMD,
                                                KE_BUILD_ID(TASK_GAPC, conidx), TASK_APP,
                                                gapc_set_le_pkt_size_cmd);
            p_cmd->operation = GAPC_SET_LE_PKT_SIZE;
            p_cmd->tx_octets = 251;
            p_cmd->tx_time = 2120;
            // Send the message
            ke_msg_send(p_cmd);
        }

        if(ty_bt_msg_cb != NULL) {
            LOG(LOG_LVL_INFO, "CONN:%d\r\n", __LINE__);
            ty_bt_msg_cb(0, TY_BT_EVENT_CONNECTED, NULL); // TY_BT_EVENT_CONNECTED
        }
    }

    return (KE_MSG_CONSUMED);
}

static int gapc_param_update_req_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapc_param_update_req_ind *p_param = (struct gapc_param_update_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapc_param_update_req_ind_handler  conidx=0x%x,intv_max=0x%x,intv_min=0x%x,latency=0x%x,time_out=0x%x\r\n",conidx,p_param->intv_max,p_param->intv_min,p_param->latency,p_param->time_out);
#endif
    // Check if the received Connection Handle was valid
    if (conidx != GAP_INVALID_CONIDX)
    {
        // Send connection confirmation
        struct gapc_param_update_cfm *cfm = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,
                                            KE_BUILD_ID(TASK_GAPC,conidx), TASK_APP,
                                            gapc_param_update_cfm);
        cfm->accept = true;
        cfm->ce_len_min = 0;
        cfm->ce_len_max = 0xFFFF;

        // Send message
        ke_msg_send(cfm);
    }
    return (KE_MSG_CONSUMED);
}

static int gapc_param_updated_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gapc_param_updated_ind *p_param = (struct gapc_param_updated_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapc_param_updated_ind_handler conidx=0x%x,con_interval=0x%x,con_latency=0x%x,sup_to=0x%x\r\n",conidx,p_param->con_interval,p_param->con_latency,p_param->sup_to);
#endif

    return (KE_MSG_CONSUMED);
}

static int gapc_cmp_evt_handler(ke_msg_id_t const msgid,
                                void const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    struct gapc_cmp_evt const *p_param = (struct gapc_cmp_evt const *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapc_cmp_evt_handler   operation=0x%x, status=0x%x   conidx=0x%x\r\n",p_param->operation,p_param->status,conidx);
#endif

    ke_msg_sync_lock_release();
    return (KE_MSG_CONSUMED);
}

static int gapc_disconnect_ind_handler(ke_msg_id_t const msgid,
                                       void const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    struct gapc_disconnect_ind *p_param = (struct gapc_disconnect_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapc_disconnect_ind_handler conidx=0x%x, reason=0x%x\r\n",conidx,p_param->reason);
#endif

    g_conidx = GAP_INVALID_CONIDX;

    ke_msg_sync_lock_release();

    extern void ln_ble_adv_start(void);
    ln_ble_adv_start();

    if(ty_bt_msg_cb!=NULL)
    {
        ty_bt_msg_cb(0, TY_BT_EVENT_DISCONNECTED ,NULL);
    }

    return (KE_MSG_CONSUMED);
}

static int gapc_con_rssi_ind_handler(ke_msg_id_t const msgid,
                                     void const *param,
                                     ke_task_id_t const dest_id,
                                     ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);
    struct gapc_con_rssi_ind *rssi_info = (struct gapc_con_rssi_ind *)param;

    if (rssi_info == NULL)
    {
        return (KE_MSG_CONSUMED);
    }

#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapc_con_rssi_ind_handler conidx=0x%x; rssi:%d\r\n",
            conidx, rssi_info->rssi);
#endif

    extern void ty_ada_get_rssi_cb(int8_t rssi);
    ty_ada_get_rssi_cb(rssi_info->rssi);

    return (KE_MSG_CONSUMED);
}

static int gapc_con_channel_map_ind_handler(ke_msg_id_t const msgid,
        void const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gapc_con_channel_map_ind_handler conidx=0x%x\r\n",conidx);
#endif

    return (KE_MSG_CONSUMED);
}


static int app_msg_handler(ke_msg_id_t const msgid,
                           void const *param,
                           ke_task_id_t const dest_id,
                           ke_task_id_t const src_id)
{
#if (TRACE_ENABLE)
    struct ke_msg *kmsg = ke_param2msg(param);

    // Retrieve identifier of the task from received message
    ke_task_id_t src_task_id = MSG_T(msgid);
    LOG(LOG_LVL_TRACE,"-------- app_msg_default_handler --------\r\n");
    LOG(LOG_LVL_TRACE," src_task_id=0x%x, msgid=0x%x\r\n", src_task_id, msgid);
    LOG(LOG_LVL_TRACE,"did:0x%04x, sid=0x%04x\r\n", dest_id, src_id);
    LOG(LOG_LVL_TRACE,"ke_msg: id=0x%04x, did=0x%04x, sid=0x%04x\r\n",kmsg->id, kmsg->dest_id, kmsg->src_id);
    LOG(LOG_LVL_TRACE,"---------------------------------\r\n");
#endif

    return (KE_MSG_CONSUMED);
}


KE_MSG_HANDLER_TAB(app)
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,    (ke_msg_func_t)app_msg_handler},                // For test

    //GAPM
    {GAPM_CMP_EVT, gapm_cmp_evt_handler},                                       // For adv ready
    {GAPM_DEV_VERSION_IND, gapm_dev_version_ind_handler},
    {GAPM_ACTIVITY_CREATED_IND, gapm_activity_created_ind_handler},             // For actv_idx
    {GAPM_ACTIVITY_STOPPED_IND, gapm_activity_stopped_ind_handler},             // For actv_idx
    {GAPM_EXT_ADV_REPORT_IND,  gapm_ext_adv_report_ind_handler},                // For scan resp
    {GAPM_SCAN_REQUEST_IND, gapm_scan_request_ind_handler},                     // For scan req
    //GAPC
    {GAPC_CMP_EVT, gapc_cmp_evt_handler },
    {GAPC_CONNECTION_REQ_IND, gapc_connection_req_ind_handler},                 // For ble conn
    {GAPC_DISCONNECT_IND, gapc_disconnect_ind_handler},                         // For ble disconn
    {GAPC_CON_RSSI_IND, gapc_con_rssi_ind_handler},                             // For connection rssi
    {GAPC_CON_CHANNEL_MAP_IND, gapc_con_channel_map_ind_handler},
    {GAPC_GET_DEV_INFO_REQ_IND, gapc_get_dev_info_req_ind_handler},             // Must be set
    {GAPC_SET_DEV_INFO_REQ_IND, gapc_set_dev_info_req_ind_handler},             // For dev info config
    {GAPC_PARAM_UPDATE_REQ_IND, gapc_param_update_req_ind_handler},             // For param sync
    {GAPC_PARAM_UPDATED_IND, gapc_param_updated_ind_handler},                   // For param sync
    //GATTM
    {GATTM_ADD_SVC_RSP, gattm_add_svc_rsp_handler },                            // Need
    //GATTC
    {GATTC_CMP_EVT, gattc_cmp_evt_handler },                                    // Need
    {GATTC_MTU_CHANGED_IND, gattc_mtu_changed_ind_handler},                     // Need
    {GATTC_READ_REQ_IND, gattc_read_req_ind_handler},                           // Need
    {GATTC_WRITE_REQ_IND, gattc_write_req_ind_handler},                         // Need
    {GATTC_TRANSACTION_TO_ERROR_IND, gattc_transaction_to_error_ind_handler},
};

/// Number of APP Task Instances
#define APP_IDX_MAX                 (1)

ke_state_t app_state[APP_IDX_MAX];

const struct ke_task_desc TASK_DESC_APP = {app_msg_handler_tab, app_state, APP_IDX_MAX, ARRAY_LEN(app_msg_handler_tab)};
/// @} APP
