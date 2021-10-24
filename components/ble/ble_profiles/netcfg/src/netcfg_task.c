/**
 ****************************************************************************************
 *
 * @file netcfg_task.c
 *
 * @brief netcfg Service Server Role Task Implementation.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup NETCFGTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_NETCFG_SERVER)

#include "gap.h"
#include "gattc_task.h"

#include "netcfg.h"
#include "netcfg_task.h"

#include "prf_utils.h"

#include "co_utils.h"

#if (TRACE_ENABLE)
#include "utils/debug/log.h"
#endif

/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref SEND_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] p_param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int netcfg_tx_req_handler(ke_msg_id_t const msgid,
                                            struct netcfg_tx_upd_req const *p_param,
                                            ke_task_id_t const dest_id,
                                            ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"netcfg_tx_req_handler \r\n");
#endif

    // check state of the task
    if (ke_state_get(dest_id) == NETCFG_IDLE)
    {
        struct netcfg_env_tag *p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);

        // put task in a busy state
        ke_state_set(dest_id, NETCFG_BUSY);						
		netcfg_send_notify_data(p_netcfg_env, p_param);
		ke_state_set(dest_id, NETCFG_IDLE);   
		msg_status = KE_MSG_CONSUMED;	
    }
    return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the attribute info request message.
 *
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] p_param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int gattc_att_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gattc_att_info_req_ind *p_param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct gattc_att_info_cfm *p_cfm;
    uint8_t att_idx = 0;
    // retrieve handle information
    uint8_t status = netcfg_get_att_idx(p_param->handle, &att_idx);

    //Send write response
    p_cfm = KE_MSG_ALLOC(GATTC_ATT_INFO_CFM, src_id, dest_id, gattc_att_info_cfm);
    p_cfm->handle = p_param->handle;
    
#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"user_task gattc_att_info_req_ind_handler status=%d,att_idx=%d\r\n", status,att_idx);
#endif

    if (status == GAP_ERR_NO_ERROR)
    {
        // check if it's a client configuration char
        if (att_idx == NETCFG_IDX_CCCD)
        {
            // CCC attribute length = 2
            p_cfm->length = 2;
        }
        // not expected request
        else
        {
            p_cfm->length = 0;
            status = ATT_ERR_WRITE_NOT_PERMITTED;
        }
    }

    p_cfm->status = status;
    ke_msg_send(p_cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_WRITE_REQ_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] p_param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int gattc_write_req_ind_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const *p_param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_write_cfm *p_cfm;
    uint8_t att_idx = 0;
    uint8_t conidx = KE_IDX_GET(src_id);
    // retrieve handle information
    uint8_t status = netcfg_get_att_idx(p_param->handle, &att_idx);
    
#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"netcfg_task gattc_write_req_ind_handler status=%d,att_idx=%d\r\n", status,att_idx);
#endif

    // If the attribute has been found, status is GAP_ERR_NO_ERROR
    if (status == GAP_ERR_NO_ERROR)
    {
        struct netcfg_env_tag* p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);
        // Extract value before check
        uint16_t ntf_cfg = co_read16p(&p_param->value[0]);

        // Only update configuration if value for stop or notification enable
        if ((att_idx == NETCFG_IDX_CCCD)
                && ((ntf_cfg == PRF_CLI_STOP_NTFIND) || (ntf_cfg == PRF_CLI_START_NTF)))
        {
            // Conserve information in environment
            if (ntf_cfg == PRF_CLI_START_NTF)
            {
                // Ntf cfg bit set to 1
                p_netcfg_env->ntf_cfg[conidx] |= (NETCFG_TX_NTF_SUP );
            }
            else
            {
                // Ntf cfg bit set to 0
                p_netcfg_env->ntf_cfg[conidx] &= ~(NETCFG_TX_NTF_SUP );
            }

            // Inform APP of configuration change
            struct netcfg_tx_ntf_cfg_ind * ind = KE_MSG_ALLOC(NETCFG_TX_NTF_CFG_IND,
                    prf_dst_task_get(&(p_netcfg_env->prf_env), conidx), dest_id,
                    netcfg_tx_ntf_cfg_ind);
            ind->conidx = conidx;
            ind->ntf_cfg = p_netcfg_env->ntf_cfg[conidx];
						
            ke_msg_send(ind);			
        }
		else if (att_idx == NETCFG_IDX_RX_VAL)
		{
			// Allocate the alert value change indication
			struct netcfg_rx_ind *ind = KE_MSG_ALLOC(NETCFG_RX_REQ_IND,
			        prf_dst_task_get(&(p_netcfg_env->prf_env), conidx),
			        dest_id, netcfg_rx_ind);
			
			// Fill in the parameter structure	
			memcpy(ind->data, p_param->value, p_param->length);
			ind->conidx = conidx;
			ind->length = p_param->length;
			
			// Send the message
			ke_msg_send(ind);
		} else {
            status = PRF_APP_ERROR;
        }
    }

    //Send write response
    p_cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, src_id, dest_id, gattc_write_cfm);
    p_cfm->handle = p_param->handle;
    p_cfm->status = status;
    ke_msg_send(p_cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_READ_REQ_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] p_param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int gattc_read_req_ind_handler(ke_msg_id_t const msgid, struct gattc_read_req_ind const *p_param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_read_cfm *p_cfm;
    uint8_t att_idx = 0;
    uint8_t conidx = KE_IDX_GET(src_id);
    // retrieve handle information
    uint8_t status = netcfg_get_att_idx(p_param->handle, &att_idx);
    uint16_t length = 0;
    struct netcfg_env_tag *p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);

#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"netcfg_task gattc_read_req_ind_handler status=%d,att_idx=%d\r\n", status,att_idx);
#endif

    // If the attribute has been found, status is GAP_ERR_NO_ERROR
    if (status == GAP_ERR_NO_ERROR)
    {
        // read notification information
        if (att_idx == NETCFG_IDX_TX_VAL) 
        {
            length = NETCFG_TX_DATA_LEN * sizeof(uint8_t);
        }
        // read notification information
        else if (att_idx == NETCFG_IDX_CCCD) 
        {
            length = sizeof(uint16_t);
        } 
        else 
        {
            status = PRF_APP_ERROR;
        }
    }

    //Send write response
    p_cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, length);
    p_cfm->handle = p_param->handle;
    p_cfm->status = status;
    p_cfm->length = length;

    if (status == GAP_ERR_NO_ERROR)
    {
        // read notification information
        if (att_idx == NETCFG_IDX_TX_VAL) 
        {
            memcpy(p_cfm->value, p_netcfg_env->netcfg_tx, length);
        }
        // retrieve notification config
        else if (att_idx == NETCFG_IDX_CCCD) 
        {
            uint16_t ntf_cfg = (p_netcfg_env->ntf_cfg[conidx] & NETCFG_TX_NTF_SUP) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND;
            co_write16p(p_cfm->value, ntf_cfg);
        } 
        else 
        {
            /* Not Possible */
        }
    }

    ke_msg_send(p_cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles @ref GATTC_CMP_EVT for GATTC_NOTIFY message meaning that Measurement
 * notification has been correctly sent to peer device (but not confirmed by peer device).
 * @param[in] msgid     Id of the message received.
 * @param[in] p_param   Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
__STATIC int gattc_cmp_evt_handler(ke_msg_id_t const msgid, struct gattc_cmp_evt const *p_param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"user_task gattc_cmp_evt_handler operation=%d,status=%d\r\n", p_param->operation,p_param->status);
#endif

    if(p_param->operation == GATTC_NOTIFY)
    {
        struct netcfg_env_tag *p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);
        struct netcfg_tx_upd_rsp *p_evt;
                // Send the message to the application
        p_evt = KE_MSG_ALLOC(NETCFG_TX_UPD_RSP, 
                            prf_dst_task_get(&(p_netcfg_env->prf_env), KE_IDX_GET(src_id)),
                            dest_id, netcfg_tx_upd_rsp);
        
        p_evt->status      = p_param->status;
        
        ke_msg_send(p_evt);
    }

	// go back in to idle mode
    ke_state_set(dest_id, NETCFG_IDLE);
	
    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
KE_MSG_HANDLER_TAB(netcfg)
{
    {NETCFG_TX_UPD_REQ,               (ke_msg_func_t) netcfg_tx_req_handler},
    {GATTC_ATT_INFO_REQ_IND,        (ke_msg_func_t) gattc_att_info_req_ind_handler},
    {GATTC_WRITE_REQ_IND,           (ke_msg_func_t) gattc_write_req_ind_handler},
    {GATTC_READ_REQ_IND,            (ke_msg_func_t) gattc_read_req_ind_handler},
    {GATTC_CMP_EVT,                 (ke_msg_func_t) gattc_cmp_evt_handler},
};

void netcfg_task_init(struct ke_task_desc *p_task_desc)
{
    // Get the address of the environment
    struct netcfg_env_tag *p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);

    p_task_desc->msg_handler_tab = netcfg_msg_handler_tab;
    p_task_desc->msg_cnt         = ARRAY_LEN(netcfg_msg_handler_tab);
    p_task_desc->state           = p_netcfg_env->state;
    p_task_desc->idx_max         = NETCFG_IDX_MAX;
}

#endif /* #if (BLE_NETCFG_SERVER) */

/// @} NETCFGTASK

