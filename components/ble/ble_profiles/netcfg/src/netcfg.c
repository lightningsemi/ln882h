/**
 ****************************************************************************************
 *
 * @file user.c
 *
 * @brief user define Server Implementation.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup user
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"

#if (BLE_NETCFG_SERVER)
#include "gattc_task.h"
#include "netcfg.h"
#include "netcfg_task.h"
#include "prf_utils.h"
#include "prf_ble.h"
#include "attm.h"

#include "ke_mem.h"

#if (TRACE_ENABLE)
#include "utils/debug/log.h"
#endif
/*
 * NETCFG ATTRIBUTES DEFINITION
 ****************************************************************************************
 */

/// Full NETCFG Database Description - Used to add attributes into the database
const struct attm_desc netcfg_att_db[NETCFG_IDX_NB] =
{
    // NETCFG Service Declaration
    [NETCFG_IDX_SVC]     =   {ATT_DECL_PRIMARY_SERVICE,  PERM(RD, ENABLE), 0, 0},

    // Rx Characteristic Declaration
    [NETCFG_IDX_RX_CHAR] =   {ATT_DECL_CHARACTERISTIC,   PERM(RD, ENABLE), 0, 0},
    // Rx Characteristic Value
    [NETCFG_IDX_RX_VAL]  =   {NETCFG_CHAR_RX_UUID,         PERM(WRITE_COMMAND, ENABLE)|PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE), NETCFG_RX_DATA_LEN *sizeof(uint8_t)},

    // Tx Characteristic Declaration
    [NETCFG_IDX_TX_CHAR] =   {ATT_DECL_CHARACTERISTIC,   PERM(RD, ENABLE), 0, 0},
    // Tx Characteristic Value - Characteristic Presentation Format Descriptor
    [NETCFG_IDX_TX_VAL]    = {NETCFG_CHAR_TX_UUID,         PERM(NTF, ENABLE),  PERM(RI, ENABLE), NETCFG_TX_DATA_LEN * sizeof(uint8_t)},
     // Client Characteristic Configuration Descriptor
    [NETCFG_IDX_CCCD]    =   {ATT_DESC_CLIENT_CHAR_CFG,  PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE),0, 0},
};


/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialization of the netcfg module.
 * This function performs all the initializations of the Profile module.
 *  - Creation of database (if it's a service)
 *  - Allocation of profile required memory
 *  - Initialization of task descriptor to register application
 *      - Task State array
 *      - Number of tasks
 *      - Default task handler
 *
 * @param[out]    p_env        Collector or Service allocated environment data.
 * @param[in|out] p_start_hdl  Service start handle (0 - dynamically allocated), only applies for services.
 * @param[in]     app_task     Application task number.
 * @param[in]     sec_lvl      Security level (AUTH, EKS and MI field of @see enum attm_value_perm_mask)
 * @param[in]     p_params     Configuration parameters of profile collector or service (32 bits aligned)
 *
 * @return status code to know if profile initialization succeed or not.
 ****************************************************************************************
 */
static uint8_t netcfg_init (struct prf_task_env *p_env, uint16_t *p_start_hdl, uint16_t app_task,
        uint8_t sec_lvl, struct netcfg_db_cfg *p_params)
{
    struct netcfg_env_tag *p_netcfg_env = NULL;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
        // Service content flag
    uint8_t cfg_flag = NETCFG_SVC_CFG_FLAG;

    // Check number of USER instances

    //-------------------- allocate memory required for the profile  ---------------------
    p_netcfg_env = (struct netcfg_env_tag *) ke_malloc(sizeof(struct netcfg_env_tag), KE_MEM_ATT_DB);
    memset(p_netcfg_env, 0 , sizeof(struct netcfg_env_tag));

    // Save number of user
    p_netcfg_env->svc_nb = p_params->netcfg_nb;


    //Create BAS in the DB
    //------------------ create the attribute database for the profile -------------------
    status = attm_svc_create_db(p_start_hdl, NETCFG_SVC_UUID, (uint8_t *)&cfg_flag,
            NETCFG_IDX_NB, NULL, p_env->task, &netcfg_att_db[0],
            (sec_lvl & (PERM_MASK_SVC_DIS | PERM_MASK_SVC_AUTH | PERM_MASK_SVC_EKS)));

    
    //-------------------- Update profile task information  ---------------------
    if (status == ATT_ERR_NO_ERROR)
    {
        // allocate BASS required environment variable
        p_env->env = (prf_env_t *) p_netcfg_env;
        p_netcfg_env->start_hdl = *p_start_hdl;
        p_netcfg_env->prf_env.app_task = app_task
                | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        p_netcfg_env->prf_env.prf_task = p_env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        p_env->id = TASK_ID_NETCFG;
        netcfg_task_init(&(p_env->desc));
        
#if (TRACE_ENABLE)
        LOG(LOG_LVL_INFO,"netcfg_init start_hdl=%d\r\n",p_netcfg_env->start_hdl);
#endif

        // service is ready, go into an Idle state
        ke_state_set(p_env->task, NETCFG_IDLE);
    }
    else if(p_netcfg_env != NULL)
    {
        ke_free(p_netcfg_env);
    }

    return (status);
}
/**
 ****************************************************************************************
 * @brief Destruction of the BASS module - due to a reset for instance.
 * This function clean-up allocated memory (attribute database is destroyed by another
 * procedure)
 *
 * @param[in|out]    p_env        Collector or Service allocated environment data.
 ****************************************************************************************
 */
static void netcfg_destroy(struct prf_task_env *p_env)
{
    struct netcfg_env_tag *p_netcfg_env = (struct netcfg_env_tag *) p_env->env;

    // clear on-going operation
    if (p_netcfg_env->p_operation != NULL)
    {
        ke_free(p_netcfg_env->p_operation);
    }

    // free profile environment variables
    p_env->env = NULL;
    ke_free(p_netcfg_env);
}

/**
 ****************************************************************************************
 * @brief Handles Connection creation
 *
 * @param[in|out]    p_env      Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 ****************************************************************************************
 */
static void netcfg_create(struct prf_task_env* p_env, uint8_t conidx)
{
    struct netcfg_env_tag *p_netcfg_env = (struct netcfg_env_tag *) p_env->env;
    
    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);

    // force notification config to zero when peer device is connected
    p_netcfg_env->ntf_cfg[conidx] = 1;
}

/**
 ****************************************************************************************
 * @brief Handles Disconnection
 *
 * @param[in|out]    p_env      Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 * @param[in]        reason     Detach reason
 ****************************************************************************************
 */
static void netcfg_cleanup(struct prf_task_env *p_env, uint8_t conidx, uint8_t reason)
{
    struct netcfg_env_tag *p_netcfg_env = (struct netcfg_env_tag *) p_env->env;

    ASSERT_ERR(conidx < BLE_CONNECTION_MAX);
    // force notification config to zero when peer device is disconnected
    p_netcfg_env->ntf_cfg[conidx] = 0;
}


/**
 ****************************************************************************************
 * @brief  Trigger tx notification
 *
 * @param p_user_env profile environment
 * @param conidx     peer destination connection index
 * @param svc_idx    Service index
 ****************************************************************************************
 */
void netcfg_send_notify_data(struct netcfg_env_tag *p_netcfg_env,  
                                        struct netcfg_tx_upd_req const *param)
{
    // Allocate the GATT notification message
    struct gattc_send_evt_cmd *p_tx_data = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
            KE_BUILD_ID(TASK_GATTC, 0), prf_src_task_get(&(p_netcfg_env->prf_env),0),
            gattc_send_evt_cmd, sizeof(uint8_t)* (param->length));

#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"netcfg_send_notify_data\r\n");
#endif

    // Fill in the parameter structure
    p_tx_data->operation = GATTC_NOTIFY;
    p_tx_data->handle = netcfg_get_att_handle(NETCFG_IDX_TX_VAL);
    // pack measured value in database
    p_tx_data->length = param->length;
    memcpy(p_tx_data->value, param->data, param->length);

    // send notification to peer device
    ke_msg_send(p_tx_data);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// USER Task interface required by profile manager
const struct prf_task_cbs netcfg_itf =
{
    (prf_init_fnct)netcfg_init,
    netcfg_destroy,
    netcfg_create,
    netcfg_cleanup,
};


/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

const struct prf_task_cbs *netcfg_prf_itf_get(void)
{
    return &netcfg_itf;
}

uint16_t netcfg_get_svc_handle(void)
{
    struct netcfg_env_tag *p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"netcfg_get_svc_handle=%d\r\n",p_netcfg_env->start_hdl);
#endif

    return (p_netcfg_env->start_hdl);
}


uint16_t netcfg_get_att_handle(uint8_t att_idx)
{
    struct netcfg_env_tag *p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);
    uint16_t handle = ATT_INVALID_HDL;

    handle = p_netcfg_env->start_hdl;

    // increment index according to expected index
    if (att_idx < NETCFG_IDX_CCCD)
    {
        handle += att_idx;
    }
    else if ((att_idx == NETCFG_IDX_CCCD) &&
            ((p_netcfg_env->features & 0x01) == NETCFG_TX_NTF_SUP))
    {
        handle += NETCFG_IDX_CCCD;
    }
    else
    {
        handle = ATT_INVALID_HDL;
    }

    return handle;
}

uint8_t netcfg_get_att_idx(uint16_t handle, uint8_t *p_att_idx)
{
    struct netcfg_env_tag* p_netcfg_env = PRF_ENV_GET(NETCFG, netcfg);
    uint16_t hdl_cursor = p_netcfg_env->start_hdl;
    uint8_t status = PRF_APP_ERROR;

    // Browse list of services
    // handle must be greater than current index 
    // check if it's a mandatory index
    if(handle <= (hdl_cursor + NETCFG_IDX_TX_VAL))
    {
        *p_att_idx = handle -hdl_cursor;
        status = GAP_ERR_NO_ERROR;
        
    }
    hdl_cursor += NETCFG_IDX_TX_VAL;

    // check if it's a notify index
    if(((p_netcfg_env->features ) & 0x01) == NETCFG_TX_NTF_SUP)
    {
        hdl_cursor++;
        if(handle == hdl_cursor)
        {
            *p_att_idx = NETCFG_IDX_CCCD;
            status = GAP_ERR_NO_ERROR;
        }
    }
    hdl_cursor++;
    
    return (status);
}


#endif // (BLE_NETCFG_SERVER)

/// @} NETCFG
