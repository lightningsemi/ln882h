/**
 ****************************************************************************************
 *
 * @file netcfg.h
 *
 * @brief Header file - network config Service Server Role
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

#ifndef _NETCFG_H_
#define _NETCFG_H_

/**
 ****************************************************************************************
 * @addtogroup netcfg 'Profile' Server
 * @ingroup user
 * @brief netcfg 'Profile' Server
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_NETCFG_SERVER)

#include "netcfg_task.h"
#include "prf_types.h"
#include "prf_ble.h"
#include "att.h"

/*
 * DEFINES
 ****************************************************************************************
 */

///Maximum number of Battery Server task instances
#define NETCFG_IDX_MAX     0x01

#define NETCFG_SVC_CFG_FLAG       (0x3F)

#define NETCFG_SVC_UUID       ATT_UUID_16(0x1910)
#define NETCFG_CHAR_RX_UUID   ATT_UUID_16(0x2b11)
#define NETCFG_CHAR_TX_UUID   ATT_UUID_16(0x2b10)


/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// Possible states of the user task
enum netcfg_state
{
    /// Idle state
    NETCFG_IDLE,
    /// busy state
    NETCFG_BUSY,
    /// Number of defined states.
    NETCFG_STATE_MAX
};

/// network config Service Attributes Indexes
enum
{
    NETCFG_IDX_SVC,

    NETCFG_IDX_RX_CHAR,
    NETCFG_IDX_RX_VAL,
    NETCFG_IDX_TX_CHAR,
    NETCFG_IDX_TX_VAL,
    NETCFG_IDX_CCCD,
    
    NETCFG_IDX_NB,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// netcfg 'Profile' Server environment variable
struct netcfg_env_tag
{
    /// profile environment
    prf_env_t prf_env;
    /// On-going operation
    struct ke_msg *p_operation;
    /// user Services Start Handle
    uint16_t start_hdl;
    //uint8_t netcfg_rx[NETCFG_RX_DATA_LEN];
    uint8_t netcfg_tx[NETCFG_TX_DATA_LEN];
    /// user task state
    ke_state_t state[NETCFG_IDX_MAX];
    /// Notification configuration of peer devices.
    uint8_t ntf_cfg[BLE_CONNECTION_MAX];
    /// Database features
    uint8_t features;
    /// Number of user
    uint8_t svc_nb;
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Retrieve netcfg service profile interface
 *
 * @return netcfg service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs *netcfg_prf_itf_get(void);

/**
 ****************************************************************************************
 * @brief Retrieve Attribute handle from service and attribute index
 *
 * @param[in] svc_idx BAS Service index
 * @param[in] att_idx Attribute index
 *
 * @return BAS attribute handle or INVALID HANDLE if nothing found
 ****************************************************************************************
 */
uint16_t netcfg_get_att_handle(uint8_t att_idx);

/**
 ****************************************************************************************
 * @brief Retrieve Service and attribute index form attribute handle
 *
 * @param[out] handle     Attribute handle
 * @param[out] p_svc_idx  BAS Service index
 * @param[out] p_att_idx  Attribute index
 *
 * @return Success if attribute and service index found, else Application error
 ****************************************************************************************
 */
uint8_t netcfg_get_att_idx(uint16_t handle, uint8_t *p_att_idx);

void netcfg_send_notify_data(struct netcfg_env_tag *p_user_env, struct netcfg_tx_upd_req const *param);


/**
 ****************************************************************************************
 * Initialize task handler
 *
 * @param p_task_desc Task descriptor to fill
 ****************************************************************************************
 */
void netcfg_task_init(struct ke_task_desc *p_task_desc);


#endif /* #if BLE_NETCFG_SERVER) */

/// @} USER

#endif /* _NETCFG_H_ */
