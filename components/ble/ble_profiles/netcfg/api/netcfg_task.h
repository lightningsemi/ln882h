/**
 ****************************************************************************************
 *
 * @file user_task.h
 *
 * @brief Header file - user Service Server Role Task.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */


#ifndef _NETCFG_TASK_H_
#define _NETCFG_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup NETCFGTASK Task
 * @ingroup user
 * @brief netcfg 'Profile' Task.
 *
 * The NETCFG_TASK is responsible for handling the messages coming in and out of the
 * @ref netcfg block of the BLE Host.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "prf_types.h"
#include "rwip_task.h" // Task definitions

/*
 * DEFINES
 ****************************************************************************************
 */


///Maximal number data of netcfg that can be send/receive in the DB
#define  NETCFG_RX_DATA_LEN  128
#define  NETCFG_TX_DATA_LEN  128

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/// Messages for netcfg Server
/*@TRACE*/
enum netcfg_msg_id
{
    /// Start the  Server - at connection used to restore bond data
    NETCFG_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_NETCFG),

	NETCFG_RX_REQ_IND,

    /// netcfg Tx Update Request
    NETCFG_TX_UPD_REQ,
    /// Inform APP if netcfg Tx has been notified or not
    NETCFG_TX_UPD_RSP,
    /// Inform APP that netcfg Tx Notification Configuration has been changed - use to update bond data
    NETCFG_TX_NTF_CFG_IND,
};

/// Features Flag Masks
enum netcfg_features
{
    /// netcfg Tx Characteristic doesn't support notifications
    NETCFG_TX_NTF_NOT_SUP,
    /// netcfg Tx Characteristic support notifications
    NETCFG_TX_NTF_SUP,
};

/*
 * APIs Structures
 ****************************************************************************************
 */

/// Parameters for the database creation
struct netcfg_db_cfg
{
    /// Number of netcfg to add
    uint8_t netcfg_nb;
    /// Features of each user instance
    uint8_t features;
};

struct netcfg_tx_upd_req
{
    uint8_t conidx;	
    uint8_t length;
    uint8_t data[NETCFG_TX_DATA_LEN];
};

struct netcfg_tx_upd_rsp
{
    ///status
    uint8_t status;
};

struct netcfg_tx_ntf_cfg_ind
{
    /// connection index
    uint8_t conidx;
    ///Notification Configuration
    uint8_t ntf_cfg;
};

struct netcfg_rx_ind
{
    /// Connection index
    uint8_t conidx;
	uint8_t length;
    uint8_t data[NETCFG_RX_DATA_LEN];
};


/// @} NETCFGTASK

#endif /* _NETCFG_TASK_H_ */
