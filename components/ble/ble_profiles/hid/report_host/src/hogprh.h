/**
 ****************************************************************************************
 *
 * @file hogpd.h
 *
 * @brief Header file - Hid Over Gatt profile host .
 *
 * Copyright (C) LIGHTNINGSEMI 2020-2026
 *
 *
 ****************************************************************************************
 */

#ifndef HOGPRH_H_
#define HOGPRH_H_

/**
 ****************************************************************************************
 * @addtogroup Hid Over Gatt profile device
 * @ingroup DIS
 * @brief Hid Over Gatt profile device
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwprf_config.h"
#if (BLE_HID_REPORT_HOST)
#include "prf_types.h"
#include "prf.h"
#include "../api/hogprh_task.h"
/*
 * DEFINES
 ****************************************************************************************
 */

#define HOGPRH_IDX_MAX        (BLE_CONNECTION_MAX)

enum
{
    HOGPRH_FREE,
    /// Idle state
    HOGPRH_IDLE,
    
    HOGPRH_DISCOVERING,
    /// Busy state
    HOGPRH_BUSY,
    
    /// Number of defined states.
    HOGPRH_STATE_MAX,
};

/// Environment variable for each Connections
struct hogprh_cnx_env
{
    ///Current Time Service Characteristics
    struct hogprh_device_content device;
    /// Last char. code requested to read.
    uint8_t last_char_code;
    /// counter used to check service uniqueness
    uint8_t nb_svc;
};

/// Device Information Service Client environment variable
struct hogprh_env_tag
{
    /// profile environment
    prf_env_t prf_env;
    /// Environment variable pointer for each connections
    struct hogprh_cnx_env *p_env[HOGPRH_IDX_MAX];
    /// State of different task instances
    ke_state_t state[HOGPRH_IDX_MAX];
};





#endif//#if (BLE_HID_DEVICE)

#endif ///#ifndef HOGPRH_H_

