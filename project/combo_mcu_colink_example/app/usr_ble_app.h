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
#include "ln_ble_trans_server.h"
#include "ble_app_user_cfg.h"

#define CHAR_VAL_MAX_LEN    1024

enum trans_svc_att_idx
{
    DATA_TRANS_DECL_SVC = 0,
    DATA_TRANS_DECL_CHAR_RX,
    DATA_TRANS_DECL_CHAR_RX_VAL,
    DATA_TRANS_DECL_CHAR_TX,
    DATA_TRANS_DECL_CHAR_TX_VAL,
    DATA_TRANS_DECL_CHAR_TX_CCC,

    DATA_TRANS_IDX_MAX,
};

enum ble_usr_msg_type
{
    BLE_MSG_CONN_IND,
    BLE_MSG_DISCONN_IND,
    BLE_MSG_SCAN_REPORT,
    BLE_MSG_GATT_READ_REQ,
    BLE_MSG_GATT_WRITE_DATA,
    BLE_MSG_GATT_READ_RSP,
    BLE_MSG_GATT_NOTIFY,
    BLE_MSG_GATT_INDICATION,
};


/*
 * DEFINES
 ****************************************************************************************
 */

#define LN_MALLOC_CHECK(cond)                                                        \
     do {                                                                            \
         if (!(cond)) {                                                              \
                LOG(LOG_LVL_ERROR, "malloc error  %s  %d \r\n",__FILE__ ,__LINE__);  \
         }                                                                           \
     } while(0) 


#if (BLE_DATA_TRANS_SERVER)
struct user_svc_desc {
    ln_trans_svr_desc_t desc;
    uint8_t ccc;//character client config
};
#endif


void ble_creat_usr_app_task(void);
struct user_svc_desc *ble_usr_svc_get_handle(void);

#endif // _APP_GAP_H_
