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

#ifndef __USR_BLE_APP_H__
#define __USR_BLE_APP_H__


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


void ble_app_init(void);

#endif /* __USR_BLE_APP_H__ */
