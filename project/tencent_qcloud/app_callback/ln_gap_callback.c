#include <string.h>
#include "gapm_task.h"               // GAP Manallger Task API
#include "gapc_task.h"               // GAP Controller Task API
#include "co_math.h"                 // Common Maths Definition
#include "co_utils.h"
#include "ke_task.h"                 // Kernel Task


#include "ln_app_gap.h"
#include "ln_gap_gatt_callback.h"

#include "utils/debug/log.h"
#include "ble_qiot_export.h"


//////////////////////////////////////
uint8_t  adv_actv_idx = 0xFF;

int gapm_activity_created_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapm_activity_created_ind *p_param = (struct gapm_activity_created_ind *)param;

    if(p_param->actv_type  ==GAPM_ACTV_TYPE_ADV) {
        adv_actv_idx = p_param->actv_idx;
    }
    return (KE_MSG_CONSUMED);
}

int gapm_activity_stopped_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapm_activity_stopped_ind *p_param = (struct gapm_activity_stopped_ind *)param;

    return (KE_MSG_CONSUMED);
}

int gapm_ext_adv_report_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                    ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapm_ext_adv_report_ind *p_param = (struct gapm_ext_adv_report_ind *)param;

    return KE_MSG_CONSUMED;
}

int gapm_scan_request_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                  ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

int gapm_profile_added_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                   ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    ke_state_t state = ke_state_get(dest_id);

    return (KE_MSG_CONSUMED);
}

int gapm_cmp_evt_callback(ke_msg_id_t const msgid, void const *param, 
                         ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapm_cmp_evt *p_param = (struct gapm_cmp_evt *)param;

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
                cfg_param.sugg_max_tx_octets = BLE_MIN_OCTETS;
                cfg_param.sugg_max_tx_time   = BLE_MIN_TIME;
                // Host privacy enabled by default
                cfg_param.privacy_cfg = 0;
                memset((void *)&cfg_param.irk.key[0], 0x00, KEY_LEN);
                cfg_param.role    = GAP_ROLE_ALL;
                //cfg_param.max_mtu = 1200;//2048;
                //cfg_param.max_mps = 1200;//2048;
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
        }
        break;
        
        default:
        break;
    }
    return (KE_MSG_CONSUMED);
}

int gapm_dev_version_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}

int gapm_dev_bdaddr_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}


int gapc_get_dev_info_req_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_get_dev_info_req_ind *p_param = (struct gapc_get_dev_info_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
    LOG(LOG_LVL_INFO,"[%s] param->req=0x%x,conidx=0x%x\r\n",__func__,p_param->req,conidx);
    switch(p_param->req)
    {
        case GAPC_DEV_NAME:
        {
            struct gapc_get_dev_info_cfm * cfm = KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,src_id, dest_id,
                                                                 gapc_get_dev_info_cfm, APP_DEVICE_NAME_MAX_LEN);
            cfm->req = p_param->req;
            // Send message
            ke_msg_send(cfm);
        }
        break;

        case GAPC_DEV_APPEARANCE:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,src_id, dest_id,gapc_get_dev_info_cfm);
            cfm->req = p_param->req;
            // Set the device appearance
            // Send message
            ke_msg_send(cfm);
        }
        break;

        case GAPC_DEV_SLV_PREF_PARAMS:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,src_id, dest_id,gapc_get_dev_info_cfm);
            cfm->req = p_param->req;
            // Slave preferred Connection interval Min
            cfm->info.slv_pref_params.con_intv_min = 8;
            // Slave preferred Connection interval Max
            cfm->info.slv_pref_params.con_intv_max = 10;
            // Slave preferred Connection latency
            cfm->info.slv_pref_params.slave_latency  = 0;
            // Slave preferred Link supervision timeout
            cfm->info.slv_pref_params.conn_timeout   = 200;  // 2s (500*10ms)
            // Send message
            ke_msg_send(cfm);
        }
        break;

        default: /* Do Nothing */
            break;
    }

    return (KE_MSG_CONSUMED);
}

int gapc_set_dev_info_req_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_set_dev_info_req_ind *p_param = (struct gapc_set_dev_info_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
    LOG(LOG_LVL_INFO,"[%s]  param->req=0x%x,conidx=0x%x\r\n",__func__,p_param->req,conidx);
    // Set Device configuration
    struct gapc_set_dev_info_cfm* cfm = KE_MSG_ALLOC(GAPC_SET_DEV_INFO_CFM, src_id, dest_id,gapc_set_dev_info_cfm);
    // Reject to change parameters
    cfm->status = 0;
    cfm->req = p_param->req;
    // Send message
    ke_msg_send(cfm);
    return (KE_MSG_CONSUMED);
}

int gapc_connection_req_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                    ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_connection_req_ind *p_param = (struct gapc_connection_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);

    extern uint8_t llsync_conid;
    llsync_conid = conidx;

    LOG(LOG_LVL_INFO,"####### gapc_connection_req_ind_handler conhdl=0x%x,  conidx=0x%x , conn_intv:%d, role=%d addr: 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x  ###########\r\n",p_param->conhdl,conidx,p_param->con_interval,p_param->role,p_param->peer_addr.addr[0],p_param->peer_addr.addr[1],p_param->peer_addr.addr[2],p_param->peer_addr.addr[3],p_param->peer_addr.addr[4],p_param->peer_addr.addr[5]);

    if (conidx != GAP_INVALID_CONIDX)
    {
        struct gapc_connection_cfm *cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM, KE_BUILD_ID(TASK_GAPC,conidx), TASK_APP, gapc_connection_cfm);
        ke_msg_send(cfm);
        ble_gap_connect_cb();
    }    
    return (KE_MSG_CONSUMED);
}

int gapc_param_update_req_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_param_update_req_ind *p_param = (struct gapc_param_update_req_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
    LOG(LOG_LVL_INFO,"[%s]  conidx=0x%x, intv_max=0x%x, intv_min=0x%x, latency=0x%x, time_out=0x%x\r\n", __func__,\
                                    conidx,p_param->intv_max,p_param->intv_min,p_param->latency,p_param->time_out);
    // Check if the received Connection Handle was valid
    if (conidx != GAP_INVALID_CONIDX)
    {
        // Send connection confirmation
        struct gapc_param_update_cfm *cfm = KE_MSG_ALLOC(GAPC_PARAM_UPDATE_CFM,KE_BUILD_ID(TASK_GAPC,conidx), TASK_APP,gapc_param_update_cfm);
        cfm->accept = true;
        cfm->ce_len_min = 0;
        cfm->ce_len_max = 0xFFFF;
        // Send message
        ke_msg_send(cfm);
    }
        
    return (KE_MSG_CONSUMED);
}

int gapc_param_updated_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                   ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_param_updated_ind *p_param = (struct gapc_param_updated_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
    LOG(LOG_LVL_INFO,"[%s] conidx=0x%x,con_interval=0x%x, con_latency=0x%x, sup_to=0x%x\r\n",__func__,\
                                     conidx,p_param->con_interval,p_param->con_latency,p_param->sup_to);
    return (KE_MSG_CONSUMED);
}

int gapc_cmp_evt_callback(ke_msg_id_t const msgid, void const *param, 
                         ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_cmp_evt const *p_param = (struct gapc_cmp_evt const *)param;
    uint8_t conidx = KE_IDX_GET(src_id);

    return (KE_MSG_CONSUMED);
}

int gapc_disconnect_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_disconnect_ind *p_param = (struct gapc_disconnect_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);

    return (KE_MSG_CONSUMED);
}

int gapc_con_rssi_ind_callback(ke_msg_id_t const msgid, void const *param, 
                              ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_con_rssi_ind *p_param = (struct gapc_con_rssi_ind *)param;

    return (KE_MSG_CONSUMED);
}

int gapc_le_pkt_size_ind_callback(ke_msg_id_t const msgid, void const *param, 
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gapc_le_pkt_size_ind *p_param = (struct gapc_le_pkt_size_ind *)param;
    uint8_t conidx = KE_IDX_GET(src_id);
    LOG(LOG_LVL_INFO,"[%s] conidx=0x%x,max_rx_octets:%d,max_rx_time:%d,max_tx_octets:%d, max_tx_time :%d\r\n", __func__, \
                            conidx, p_param->max_rx_octets, p_param->max_rx_time, p_param->max_tx_octets, p_param->max_tx_time);
    return (KE_MSG_CONSUMED);
}


