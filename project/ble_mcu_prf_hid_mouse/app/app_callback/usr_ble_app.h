/**
 ****************************************************************************************
 *
 * @file usr_ble_app.h
 *
 * @brief define  API.
 *
 *Copyright (C) 2021.Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_DEF_H_
#define _LN_DEF_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * 
 *
 * @brief define API.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>
#include <stdbool.h>
#include "co_bt_defines.h"
#include "gapc_task.h"
/*
 * DEFINES
 ****************************************************************************************
 */
#define BLE_USR_MSG_QUEUE_SIZE    96

#define LN_MALLOC_CHECK(cond)                                                           \
     do {                                                                            \
         if (!(cond)) {                                                              \
                LOG(LOG_LVL_ERROR, "malloc error  %s  %d \r\n",__FILE__ ,__LINE__);  \
         }                                                                           \
     } while(0) 



#define KV_BLE_ADDRESS                      ((const char *)"0_ble_address")
#define KV_BLE_LOC_IRK                      ((const char *)"1_ble_loc_irk")
#define KV_BLE_PERIPH_BONDED                 ((const char *)"2_ble_periph_bonded")      /// Peripheral Bonded
#define KV_BLE_PEER_BD_ADDRESS              ((const char *)"3_ble_periph_bonde")   /// Peer Device BD Address
#define KV_BLE_LTK                          ((const char *)"4_ble_ltk")   
#define KV_BLE_PEER_IRK                     ((const char *)"5_ble_peer_irk")   /// Peer device Resolving identity key (+identity address)


#define KV_BLE_LEN_LOC_IRK              16
#define KV_BLE_LEN_PERIPH_BONDED        1
#define KV_BLE_LEN_PEER_BD_ADDRESS      7
#define KV_BLE_LEN_LTK                  28
#define KV_BLE_LEN_PEER_IRK             sizeof(struct gapc_irk)



typedef struct
{
    uint16_t  id;
    uint16_t  len;
    void     *msg;
} ble_usr_msg_t;

enum ble_usr_msg_type
{
    BLE_MSG_WRITE_DATA,
    BLE_MSG_CONN_IND,
    BLE_MSG_SVR_DIS,
    BLE_MSG_DIS_CONN_IND,
    BLE_MSG_BASS_ENABLE,
    BLE_MSG_DISC_ENABLE,
	BLE_MSG_HOGPD_ENABLE,
    BLE_MSG_HOGPD_NTF_CFG_IND,
    BLE_MSG_HOGPD_REP_UPD_RSP,
	BLE_MSG_SET_DEV_CONFIG,
	BLE_MSG_SET_IRK,
};

/// States of APP task
enum app_state
{
    /// Initialization state
    APP_INIT,
    /// Database create state
    APP_CREATE_DB,
    /// Ready State
    APP_READY,
    /// Connected state
    APP_CONNECTED,
    /// advertising
    APP_ADVERTISING,

    /// Number of defined states.
    APP_STATE_MAX
};


#endif // _APP_GAP_H_
