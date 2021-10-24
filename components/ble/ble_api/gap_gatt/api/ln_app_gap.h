/**
 ****************************************************************************************
 *
 * @file ln_app_gap.h
 *
 * @brief GAP API.
 *
 *Copyright (C) 2021.Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_APP_GAP_H_
#define _LN_APP_GAP_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @group BLE
 *
 * @brief GAP API.
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
#include "gap.h"                     // GAP Definition
#include "gapm_task.h"               // GAP Manager Task API
#include "gapc_task.h"               // GAP Controller Task API

/*
 * DEFINES
 ****************************************************************************************
 */


// BD address length
#define LN_GAP_BD_ADDR_LEN       (6)
/// LE Channel map length
#define LN_GAP_LE_CHNL_MAP_LEN   (0x05)
/// LE Feature Flags Length
#define LN_GAP_LE_FEATS_LEN      (0x08)
/// ADV Data and Scan Response length
#define LN_GAP_ADV_DATA_LEN      (0x1F)
#define LN_GAP_SCAN_RSP_DATA_LEN (0x1F)
/// Random number length
#define LN_GAP_RAND_NB_LEN       (0x08)
/// Key length
#define LN_GAP_KEY_LEN           (16)
/// P256 Key Len
#define LN_GAP_P256_KEY_LEN      (0x20)



/// Length of resolvable random address prand part
#define LN_GAP_ADDR_PRAND_LEN            (3)
/// Length of resolvable random address hash part
#define LN_GAP_ADDR_HASH_LEN             (3)

/// Number of bytes needed for a bit field indicated presence of a given Advertising Flag value
/// in the Advertising or the Scan Response data
/// Advertising Flags is a 8-bit value, hence 256 value are possible
/// -> 256 / 8 bytes = 32 bytes are needed
#define LN_GAP_AD_TYPE_BITFIELD_BYTES                          (32)



/// Device Attribute write permission requirement
enum ln_write_att_perm
{
    /// Disable write access
    WRITE_DISABLE     = 0,
    /// Enable write access - no authentication required
    WRITE_NO_AUTH     = 1,
    /// Write access requires unauthenticated link
    WRITE_UNAUTH      = 2,
    /// Write access requires authenticated link
    WRITE_AUTH        = 3,
    /// Write access requires secure connected link
    WRITE_SEC_CON     = 4
};

/// get device local info command.
enum ln_get_dev_info_cmd
{
    /// Get local device version
    GET_INFO_VERSION,
    /// Get local device BD Address
    GET_INFO_BDADDR,
    /// Get device advertising power level
    GET_INFO_ADV_TX_POWER,
     /// Get minimum and maximum transmit powers supported by the controller
    GET_INFO_TX_POWER,
    /// Get suggested default data length
    GET_INFO_SUGG_DFT_DATA,
    /// Get supported default data length
    GET_INFO_SUPP_DFT_DATA,
    /// Get maximum advertising data length supported by the controller.
    GET_INFO_MAX_ADV_DATA_LEN,
    /// Get White List Size.
    GET_INFO_WLIST_SIZE,
    /// Get resolving address list size.
    GET_INFO_RAL_SIZE,
    /// Get periodic advertiser list size.
    GET_INFO_PAL_SIZE,
    /// Get number of available advertising sets.
    GET_INFO_NB_ADV_SETS,
};

// get peer device info command.
enum ln_get_peer_info
{
    GET_PEER_NAME,
    GET_PEER_VERSION,
    GET_PEER_FEATURES,
    GET_CON_RSSI,
    GET_CON_CHANNEL_MAP,
    GET_PEER_APPEARANCE,
    GET_PEER_SLV_PREF_PARAMS,
    GET_ADDR_RESOL_SUPP,
    GET_LE_PING_TO,
    GET_PHY,
    GET_CHAN_SEL_ALGO,  
};

/// Generic Security key structure
/*@TRACE*/
struct ln_gap_sec_key
{
    /// Key value MSB -> LSB
    uint8_t key[LN_GAP_KEY_LEN];
} ;




///BD Address structure
/*@TRACE*/
struct ln_bd_addr
{
    ///6-byte array address value
    uint8_t  addr[LN_GAP_BD_ADDR_LEN];
} ;


/// Set device configuration command
/*@TRACE*/
struct ln_gapm_set_dev_config_cmd
{
    /// GAPM requested operation:
    ///  - GAPM_SET_DEV_CONFIG: Set device configuration
    uint8_t operation;
    /// Device Role: Central, Peripheral, Observer, Broadcaster or All roles.
    uint8_t role;

    /// -------------- Privacy Config -----------------------
    /// Duration before regenerate device address when privacy is enabled. - in seconds
    uint16_t renew_dur;
    /// Provided own static private random address
    struct ln_bd_addr addr;
    /// Device IRK used for resolvable random BD address generation (LSB first)
    struct ln_gap_sec_key irk;
    /// Privacy configuration bit field (@see enum gapm_priv_cfg for bit signification)
    uint8_t privacy_cfg;

    /// -------------- Security Config -----------------------

    /// Pairing mode authorized (@see enum gapm_pairing_mode)
    uint8_t pairing_mode;

    /// -------------- ATT Database Config -----------------------

    /// GAP service start handle
    uint16_t gap_start_hdl;
    /// GATT service start handle
    uint16_t gatt_start_hdl;

/// Attribute database configuration
    /// Device Name write permission @see ln_write_att_perm
    uint8_t att_devname_write_perm;
    /// Device Appearance write permission @see ln_write_att_perm
    uint8_t att_apperance_write_perm;
    /// Slave Preferred Connection Parameters present ( 1 = yes, 0 = no )
    uint8_t att_slv_pref_conn_param_present;
    /// Service change feature present in GATT attribute database ( 1 = yes, 0 = no )
    uint8_t svc_change_feat_present;

    /// -------------- LE Data Length Extension -----------------------
    ///Suggested value for the Controller's maximum transmitted number of payload octets to be used
    uint16_t sugg_max_tx_octets;
    ///Suggested value for the Controller's maximum packet transmission time to be used
    uint16_t sugg_max_tx_time;

    /// --------------- L2CAP Configuration ---------------------------
    /// Maximal MTU acceptable for device
    uint16_t max_mtu;
    /// Maximal MPS Packet size acceptable for device
    uint16_t max_mps;
    /// Maximum number of LE Credit based connection that can be established
    uint8_t  max_nb_lecb;

    /// --------------- LE Audio Mode Supported -----------------------
    ///
    /// LE Audio Mode Configuration (@see gapm_audio_cfg_flag)
    uint16_t  audio_cfg;

    /// ------------------ LE PHY Management  -------------------------
    /// Preferred LE PHY for data transmission (@see enum gap_phy)
    uint8_t tx_pref_phy;
    /// Preferred LE PHY for data reception (@see enum gap_phy)
    uint8_t rx_pref_phy;

    /// ------------------ Miscellaneous 2 ----------------------------
    /// RF TX Path Compensation value (from -128dB to 128dB, unit is 0.1dB)
    uint16_t tx_path_comp;
    /// RF RX Path Compensation value (from -128dB to 128dB, unit is 0.1dB)
    uint16_t rx_path_comp;
} ;


///Channel map structure
/*@TRACE*/
struct ln_le_chnl_map
{
    ///5-byte channel map array
    uint8_t map[LN_GAP_LE_CHNL_MAP_LEN];
} ;




/// Set device channel map
/*@TRACE*/
struct ln_gapm_set_channel_map_cmd
{
    /// GAPM requested operation:
    ///  - GAPM_SET_CHANNEL_MAP: Set device channel map.
    ///  - GAPM_GET_SUGGESTED_DFLT_LE_DATA_LEN: Get Suggested Default LE Data Length
    ///  - GAPM_GET_MAX_LE_DATA_LEN: Get Maximum LE Data Length
    ///  - GAPM_GET_NB_ADV_SETS: Read number of advertising sets currently supported by the controller
    ///  - GAPM_GET_MAX_LE_ADV_DATA_LEN: Get maximum data length for advertising data

    ///
    ///  - GAPM_GET_ANTENNA_INFO:
    uint8_t operation;
    /// Channel map
    struct ln_le_chnl_map chmap;
} ;



/// Resolve Address command
/*@TRACE*/
struct ln_gapm_resolv_addr_cmd
{
    /// GAPM requested operation:
    ///  - GAPM_RESOLV_ADDR: Resolve device address
    uint8_t operation;
    /// Number of provided IRK (sahlle be > 0)
    uint8_t nb_key;
    /// Resolvable random address to solve
    struct ln_bd_addr addr;
    /// Array of IRK used for address resolution (MSB -> LSB)
    struct ln_gap_sec_key irk[__ARRAY_EMPTY];
};


/// Generate a random address.
/*@TRACE*/
struct ln_gapm_gen_rand_addr_cmd
{
    /// GAPM requested operation:
    ///  - GAPM_GEN_RAND_ADDR: Generate a random address
    uint8_t  operation;
    /// Dummy parameter used to store the prand part of the address
    uint8_t  prand[LN_GAP_ADDR_PRAND_LEN];
    /// Random address type @see gap_rnd_addr_type
    ///  - BD_ADDR_STATIC: Static random address
    ///  - BD_ADDR_NON_RSLV: Private non resolvable address
    ///  - BD_ADDR_RSLV: Private resolvable address
    uint8_t rnd_type;
};




/// Set new IRK
/*@TRACE*/
struct ln_gapm_set_irk_cmd
{
    /// GAPM requested operation:
    ///  - GAPM_SET_IRK: Set device configuration
    uint8_t operation;
    /// Device IRK used for resolvable random BD address generation (LSB first)
    struct ln_gap_sec_key irk;
};



/// Register a LE Protocol/Service Multiplexer command
/*@TRACE*/
struct ln_gapm_lepsm_register_cmd
{
    /// GAPM requested operation:
    ///  - GAPM_LEPSM_REG: Register a LE Protocol/Service Multiplexer
    uint8_t  operation;
    /// LE Protocol/Service Multiplexer
    uint16_t le_psm;
    /// Application task number
    uint16_t app_task;
    /// Security level
    ///   7   6   5   4   3   2   1   0
    /// +---+---+---+---+---+---+---+---+
    /// |MI |      RFU      |EKS|SEC_LVL|
    /// +---+---+---+---+---+---+---+---+
    /// bit[0-1]: Security level requirement (0=NO_AUTH, 1=UNAUTH, 2=AUTH, 3=SEC_CON)
    /// bit[2]  : Encryption Key Size length must have 16 bytes
    /// bit[7]  : Does the application task is multi-instantiated or not
    uint8_t sec_lvl;
};


struct ln_gapm_lepsm_unregister_cmd
{
    /// GAPM requested operation:
    ///  - GAPM_LEPSM_UNREG: Unregister a LE Protocol/Service Multiplexer
    uint8_t  operation;
    /// LE Protocol/Service Multiplexer
    uint16_t le_psm;
};

/// Address information about a device address
/*@TRACE*/
struct ln_gap_bdaddr
{
    /// BD Address of device
    struct ln_bd_addr addr;
    /// Address type of the device 0=public/1=private random
    uint8_t addr_type;
};


/// Resolving list device information
/*@TRACE*/
struct ln_gap_ral_dev_info
{
    /// Device identity
    struct ln_gap_bdaddr addr;
    /// Privacy Mode
    uint8_t priv_mode;
    /// Peer IRK
    uint8_t peer_irk[LN_GAP_KEY_LEN];
    /// Local IRK
    uint8_t local_irk[LN_GAP_KEY_LEN];
};

/// Read local or peer address
/*@TRACE*/
struct ln_gapm_get_ral_addr_cmd
{
    /// GAPM request operation:
    ///  - GAPM_GET_RAL_LOC_ADDR: Get resolving local address
    ///  - GAPM_GET_RAL_PEER_ADDR: Get resolving peer address
    uint8_t operation;
    /// Peer device identity
    struct ln_gap_bdaddr peer_identity;
};




/// Set content of resolving list command
/*@TRACE*/
struct ln_gapm_list_set_ral_cmd
{
    /// GAPM request operation:
    ///  - GAPM_SET_RAL: Set resolving list content
    uint8_t operation;
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// List of entries to be added in the list
    struct ln_gap_ral_dev_info ral_info[__ARRAY_EMPTY];
};


/// Periodic advertising information
/*@TRACE*/
struct ln_gapm_period_adv_addr_cfg
{
    /// Advertiser address information
    struct ln_gap_bdaddr addr;
    /// Advertising SID
    uint8_t adv_sid;
};



/// Set content of periodic advertiser list command
/*@TRACE*/
struct ln_gapm_list_set_pal_cmd
{
    /// GAPM request operation:
    ///  - GAPM_SET_PAL: Set periodic advertiser list content
    uint8_t operation;
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// List of entries to be added in the list
    struct ln_gapm_period_adv_addr_cfg pal_info[__ARRAY_EMPTY];
};



/// Set content of white list
/*@TRACE*/
struct ln_gapm_list_set_wl_cmd
{
    /// GAPM request operation:
    ///  - GAPM_SET_WHITE_LIST: Set white list content
    uint8_t operation;
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// List of entries to be added in the list
    struct ln_gap_bdaddr wl_info[__ARRAY_EMPTY];
};




/// Set advertising, scan response or periodic advertising data command
/*@TRACE*/
struct ln_gapm_set_adv_data_cmd
{
    /// GAPM request operation:
    ///  - GAPM_SET_ADV_DATA: Set advertising data
    ///  - GAPM_SET_SCAN_RSP_DATA: Set scan response data
    ///  - GAPM_SET_PERIOD_ADV_DATA: Set periodic advertising data
    uint8_t operation;
    /// Activity identifier
    uint8_t actv_idx;
    /// Data length
    uint16_t length;
    /// Data
    uint8_t data[__ARRAY_EMPTY];
};



/// Configuration for advertising on primary channel
/*@TRACE*/
struct ln_gapm_adv_prim_cfg
{
    /// Minimum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t adv_intv_min;
    /// Maximum advertising interval (in unit of 625us). Must be greater than 20ms
    uint32_t adv_intv_max;
    /// Bit field indicating the channel mapping
    uint8_t chnl_map;
    /// Indicate on which PHY primary advertising has to be performed (@see enum gapm_phy_type)
    /// Note that LE 2M PHY is not allowed and that legacy advertising only support LE 1M PHY
    uint8_t phy;
};

/// Configuration for advertising on secondary channel
/*@TRACE*/
struct ln_gapm_adv_second_cfg
{
    /// Maximum number of advertising events the controller can skip before sending the
    /// AUX_ADV_IND packets. 0 means that AUX_ADV_IND PDUs shall be sent prior each
    /// advertising events
    uint8_t max_skip;
    /// Indicate on which PHY secondary advertising has to be performed (@see enum gapm_phy_type)
    uint8_t phy;
    /// Advertising SID
    uint8_t adv_sid;
};


/// Configuration for periodic advertising
/*@TRACE*/
struct ln_gapm_adv_period_cfg
{
    /// Minimum advertising interval (in unit of 1.25ms). Must be greater than 20ms
    uint16_t    adv_intv_min;
    /// Maximum advertising interval (in unit of 1.25ms). Must be greater than 20ms
    uint16_t    adv_intv_max;
    /// CTE count (number of CTEs to transmit in each periodic advertising interval, range 0x01 to 0x10)
    /// 0 to disable CTE transmission
    uint8_t     cte_count;
    /// CTE type (0: AOA | 1: AOD-1us | 2: AOD-2us) (@see enum gap_cte_type)
    uint8_t     cte_type;
    /// CTE length (in 8us unit)
    uint8_t     cte_len;
    /// Length of switching pattern (number of antenna IDs in the pattern)
    uint8_t     switching_pattern_len;
    /// Antenna IDs
    uint8_t     antenna_id[__ARRAY_EMPTY];
};



/// Advertising parameters for advertising creation
/*@TRACE*/
struct ln_gapm_adv_create_param
{
    /// Advertising type (@see enum gapm_adv_type)
    uint8_t type;
    /// Discovery mode (@see enum gapm_adv_disc_mode)
    uint8_t disc_mode;
    /// Bit field value provided advertising properties (@see enum gapm_adv_prop for bit signification)
    uint16_t prop;
    /// Maximum power level at which the advertising packets have to be transmitted
    /// (between -127 and 126 dBm)
    int8_t max_tx_pwr;
    /// Advertising filtering policy (@see enum adv_filter_policy)
    uint8_t filter_pol;
    /// Peer address configuration (only used in case of directed advertising)
    struct ln_gap_bdaddr peer_addr;
    /// Configuration for primary advertising
    struct ln_gapm_adv_prim_cfg prim_cfg;
    /// Configuration for secondary advertising (valid only if advertising type is
    /// GAPM_ADV_TYPE_EXTENDED or GAPM_ADV_TYPE_PERIODIC)
    struct ln_gapm_adv_second_cfg second_cfg;
    /// Configuration for periodic advertising (valid only if advertising type os
    /// GAPM_ADV_TYPE_PERIODIC)
    struct ln_gapm_adv_period_cfg period_cfg;
};




/// Create an advertising activity command
struct ln_gapm_activity_create_adv_cmd
{
    /// GAPM request operation:
    ///  - GAPM_CREATE_ADV_ACTIVITY: Create advertising activity
    uint8_t operation;
    /// Own address type (@see enum gapm_own_addr)
    uint8_t own_addr_type;
    /// Advertising parameters (optional, shall be present only if operation is GAPM_CREATE_ADV_ACTIVITY)
    /// For prop parameter, @see enum gapm_leg_adv_prop, @see enum gapm_ext_adv_prop and @see enum gapm_per_adv_prop for help
    struct ln_gapm_adv_create_param adv_param;
};



/// Additional advertising parameters
/*@TRACE*/
struct ln_gapm_adv_param
{
    /// Advertising duration (in unit of 10ms). 0 means that advertising continues
    /// until the host disable it
    uint16_t duration;
    /// Maximum number of extended advertising events the controller shall attempt to send prior to
    /// terminating the extending advertising
    /// Valid only if extended advertising
    uint8_t max_adv_evt;
};

/// Scan Window operation parameters
/*@TRACE*/
struct ln_gapm_scan_wd_op_param
{
    /// Scan interval
    uint16_t scan_intv;
    /// Scan window
    uint16_t scan_wd;
};

/// Scanning parameters
/*@TRACE*/
struct ln_gapm_scan_param
{
    /// Type of scanning to be started (@see enum gapm_scan_type)
    uint8_t type;
    /// Properties for the scan procedure (@see enum gapm_scan_prop for bit signification)
    uint8_t prop;
    /// Duplicate packet filtering policy
    uint8_t dup_filt_pol;
    /// Reserved for future use
    uint8_t rsvd;
    /// Scan window opening parameters for LE 1M PHY
    struct ln_gapm_scan_wd_op_param scan_param_1m;
    /// Scan window opening parameters for LE Coded PHY
    struct ln_gapm_scan_wd_op_param scan_param_coded;
    /// Scan duration (in unit of 10ms). 0 means that the controller will scan continuously until
    /// reception of a stop command from the application
    uint16_t duration;
    /// Scan period (in unit of 1.28s). Time interval betweem two consequent starts of a scan duration
    /// by the controller. 0 means that the scan procedure is not periodic
    uint16_t period;
};

/// Connection parameters
/*@TRACE*/
struct ln_gapm_conn_param
{
    /// Minimum value for the connection interval (in unit of 1.25ms). Shall be less than or equal to
    /// conn_intv_max value. Allowed range is 7.5ms to 4s.
    uint16_t conn_intv_min;
    /// Maximum value for the connection interval (in unit of 1.25ms). Shall be greater than or equal to
    /// conn_intv_min value. Allowed range is 7.5ms to 4s.
    uint16_t conn_intv_max;
    /// Slave latency. Number of events that can be missed by a connected slave device
    uint16_t conn_latency;
    /// Link supervision timeout (in unit of 10ms). Allowed range is 100ms to 32s
    uint16_t supervision_to;
    /// Recommended minimum duration of connection events (in unit of 625us)
    uint16_t ce_len_min;
    /// Recommended maximum duration of connection events (in unit of 625us)
    uint16_t ce_len_max;
};

/// Initiating parameters
/*@TRACE*/
struct ln_gapm_init_param
{
    /// Initiating type (@see enum gapm_init_type)
    uint8_t type;
    /// Properties for the initiating procedure (@see enum gapm_init_prop for bit signification)
    uint8_t prop;
    /// Timeout for automatic connection establishment (in unit of 10ms). Cancel the procedure if not all
    /// indicated devices have been connected when the timeout occurs. 0 means there is no timeout
    uint16_t conn_to;
    /// Scan window opening parameters for LE 1M PHY
    struct ln_gapm_scan_wd_op_param scan_param_1m;
    /// Scan window opening parameters for LE Coded PHY
    struct ln_gapm_scan_wd_op_param scan_param_coded;
    /// Connection parameters for LE 1M PHY
    struct ln_gapm_conn_param conn_param_1m;
    /// Connection parameters for LE 2M PHY
    struct ln_gapm_conn_param conn_param_2m;
    /// Connection parameters for LE Coded PHY
    struct ln_gapm_conn_param conn_param_coded;
    /// Address of peer device in case white list is not used for connection
    struct ln_gap_bdaddr peer_addr;
};



/// Periodic synchronization parameters
/*@TRACE*/
struct ln_gapm_per_sync_param
{
    /// Number of periodic advertising that can be skipped after a successful receive. Maximum authorized
    /// value is 499
    uint16_t                        skip;
    /// Synchronization timeout for the periodic advertising (in unit of 10ms between 100ms and 163.84s)
    uint16_t                        sync_to;
    /// Periodic synchronization type (@see enum gapm_per_sync_type)
    uint8_t                         type;
    /// Connection index used for periodic sync info reception (only valid for GAPM_PER_SYNC_TYPE_PAST)
    uint8_t                         conidx;
    /// Address of advertiser with which synchronization has to be established (used only if use_pal is false)
    struct ln_gapm_period_adv_addr_cfg adv_addr;
    /// 1 to disable periodic advertising report, 0 to enable them by default
    uint8_t                         report_disable;
    /// Type of Constant Tone Extension device should sync on (@see enum gapm_sync_cte_type).
    uint8_t                         cte_type;
};

/// Operation command structure in order to keep requested operation.
struct ln_gapm_operation_cmd
{
    /// GAP request type
    uint8_t operation;
};



/// Activity parameters
/*@TRACE
 @trc_ref gapm_actv_type
 */
union ln_gapm_u_param
{
    /// Additional advertising parameters (for advertising activity)
    //@trc_union @activity_map[$parent.actv_idx] == GAPM_ACTV_TYPE_ADV
    struct ln_gapm_adv_param adv_add_param;
    /// Scan parameters (for scanning activity)
    //@trc_union @activity_map[$parent.actv_idx] == GAPM_ACTV_TYPE_SCAN
    struct ln_gapm_scan_param scan_param;
    /// Initiating parameters (for initiating activity)
    //@trc_union @activity_map[$parent.actv_idx] == GAPM_ACTV_TYPE_INIT
    struct ln_gapm_init_param init_param;
    /// Periodic synchronization parameters (for periodic synchronization activity)
    //@trc_union @activity_map[$parent.actv_idx] == GAPM_ACTV_TYPE_PER_SYNC
    struct ln_gapm_per_sync_param per_sync_param;
};

/// Start a given activity command
/*@TRACE*/
struct ln_gapm_activity_start_cmd
{
    /// GAPM request operation:
    ///  - GAPM_START_ACTIVITY: Start a given activity
    uint8_t operation;
    /// Activity identifier
    uint8_t actv_idx;
    /// Activity parameters
    union ln_gapm_u_param u_param;
};

/// Connection Parameter used to update connection parameters
struct ln_gapc_conn_param
{
    /// Connection interval minimum
    uint16_t intv_min;
    /// Connection interval maximum
    uint16_t intv_max;
    /// Latency
    uint16_t latency;
    /// Supervision timeout
    uint16_t time_out;
};


/// Set specific link data configuration.
/*@TRACE*/
struct ln_gapc_connection_cfm
{
    /// Local CSRK value
    struct ln_gap_sec_key lcsrk;
    /// Local signature counter value
    uint32_t           lsign_counter;
    /// Remote CSRK value
    struct ln_gap_sec_key rcsrk;
    /// Remote signature counter value
    uint32_t           rsign_counter;
    /// Authentication (@see enum gap_auth)
    uint8_t            auth;
    /// Client bond data information (@see enum gapc_cli_info)
    uint8_t            cli_info;
    /// LTK exchanged during pairing.
    bool               ltk_present;
    /// Client supported features    (@see enum gapc_cli_feat)
    uint8_t            cli_feat;
    /// Peer GATT Service Start handle
    uint16_t           gatt_start_handle;
    /// Peer GATT Service End Handle
    uint16_t           gatt_end_handle;
    /// Peer Service Change value handle
    uint16_t           svc_chg_handle;
};


/// Retrieve information command
/*@TRACE*/
struct ln_gapc_get_info_cmd
{
    /// GAP request type:
    /// - GAPC_GET_PEER_NAME: Retrieve name of peer device.
    /// - GAPC_GET_PEER_VERSION: Retrieve peer device version info.
    /// - GAPC_GET_PEER_FEATURES: Retrieve peer device features.
    /// - GAPC_GET_CON_RSSI: Retrieve connection RSSI.
    /// - GAPC_GET_CON_CHANNEL_MAP: Retrieve Connection Channel MAP.
    /// - GAPC_GET_PEER_APPEARANCE: Get Peer device appearance
    /// - GAPC_GET_PEER_SLV_PREF_PARAMS: Get Peer device Slaved Preferred Parameters
    /// - GAPC_GET_ADDR_RESOL_SUPP: Address Resolution Supported
    /// - GAPC_GET_LE_PING_TIMEOUT: Retrieve LE Ping Timeout Value
    uint8_t operation;
};


/// Master confirm or not that parameters proposed by slave are accepted or not
/*@TRACE*/
struct ln_gapc_param_update_cfm
{
    /// True to accept slave connection parameters, False else.
    bool accept;
    /// Minimum Connection Event Duration
    uint16_t ce_len_min;
    /// Maximum Connection Event Duration
    uint16_t ce_len_max;
};



/// Pairing parameters
/*@TRACE*/
struct ln_gapc_pairing
{
    /// IO capabilities (@see gap_io_cap)
    uint8_t iocap;
    /// OOB information (@see gap_oob)
    uint8_t oob;
    /// Authentication (@see gap_auth)
    /// Note in BT 4.1 the Auth Field is extended to include 'Key Notification' and
    /// and 'Secure Connections'.
    uint8_t auth;
    /// Encryption key size (7 to 16)
    uint8_t key_size;
    ///Initiator key distribution (@see gap_kdist)
    uint8_t ikey_dist;
    ///Responder key distribution (@see gap_kdist)
    uint8_t rkey_dist;

    /// Device security requirements (minimum security level). (@see gap_sec_req)
    uint8_t sec_req;
};


/// Start Bonding command procedure
/*@TRACE*/
struct ln_gapc_bond_cmd
{
    /// GAP request type:
    /// - GAPC_BOND:  Start bonding procedure.
    uint8_t operation;
    /// Pairing information
    struct ln_gapc_pairing pairing;
};



/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Reset BLE stack
 *
 *
 ****************************************************************************************
 */
void ln_app_gapm_reset(void);

/**
 ****************************************************************************************
 * @brief Set device configuration
 *
 * @param[in] cfg_param                 Pointer to the device configurtion structure.  don't fill in  operation.

 *
 ****************************************************************************************
 */
void ln_app_set_dev_config(struct ln_gapm_set_dev_config_cmd *cfg_param);

/**
 ****************************************************************************************
 * @brief Get local device informations
 *
 * @param[in] cmd                   Command for information, @see enum ln_get_dev_info_cmd
 *
 ****************************************************************************************
 */

void ln_app_get_dev_info(int cmd);

/**
 ****************************************************************************************
 * @brief Set channel map
 * @note This function is only for central device
 *
 * @param[in] p_chn_map         Pointer to the channel map array. don't fill in  operation.
 *
 *
 ****************************************************************************************
 */
void ln_app_set_chn_map(struct ln_gapm_set_channel_map_cmd *p_chn_map);

/**
 ****************************************************************************************
 * @brief Resolve provided random address by usng array of IRK
 *
 * @param[in] resolv_addr           Pointer to the random address and possible keys,  don't fill in  operation.
 *
 ****************************************************************************************
 */
void ln_app_resolve_addr(struct ln_gapm_resolv_addr_cmd *resolv_addr);

/**
 ****************************************************************************************
 * @brief Generate random address
 *
 * @param[in] p_param       random address type, don't fill in  operation. @see enum  random_addr_type

 ****************************************************************************************
 */
void ln_app_gen_random_addr(struct ln_gapm_gen_rand_addr_cmd *p_param);

/**
 ****************************************************************************************
 * @brief get public key
 *
 *
 ****************************************************************************************
 */
void ln_app_get_pub_key(void);

/**
 ****************************************************************************************
 * @brief Generate a 8 bytes random number
 *

 ****************************************************************************************
 */
void ln_app_gen_random_nb(void);

/**
 ****************************************************************************************
 * @brief Change the current IRK for a renewed one
 * @note This can only be called during no air operation.
 *
 * @param[out] p_param              Pointer to a security key data structure,  don't fill in  operation.
 *

 ****************************************************************************************
 */
void ln_app_set_irk(struct ln_gapm_set_irk_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Register a LE Protocol/Service Multiplexer identified.
 * @note This has to be registered before LE Credit Based Connection established.
 *
 * @param[in] p_param                   Pointer to a LEPSM data structure,
 *
 *
 ****************************************************************************************
 */
void ln_app_register_lepsm(struct ln_gapm_lepsm_register_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Un-register a LE Protocol/Service Multiplexer identified.
 *
 * @param[in] p_param                     Pointer to  data structure, only fill in le_psm,LE Protoca/Service Multiplexer (16 bits),
 *
 ****************************************************************************************
 */

void ln_app_unregister_lepsm(struct ln_gapm_lepsm_unregister_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Get local or peer resolvable private address.
 *
 * @param[in] p_param           Pointer Read local or peer address cmd ptr,.

 *
 ****************************************************************************************
 */
void ln_app_get_ral_addr(struct ln_gapm_get_ral_addr_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Set the resolving list content.
 * @note This will overwrite the current resolving list.
 *
 * @param[in] p_param                   Pointer to resolving list data structure,don't fill in  operation.
 *
 ****************************************************************************************
 */

void ln_app_set_ral_list(struct ln_gapm_list_set_ral_cmd *p_param);
/**
 ****************************************************************************************
 * @brief Set the peroidic advertiser list content.
 * @note This will overwrite the current periodic advertiser list.
 *
 * @param[in] p_param                   Pointer to periodic advertiser list data structure,don't fill in  operation.
 *
 ****************************************************************************************
 */

void ln_app_set_pal_list(struct ln_gapm_list_set_pal_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Set the white list content.
 * @note This will overwrite the current white list.
 *
 * @param[in] p_wlist               Pointer to white list data structure,
 *
 ****************************************************************************************
 */

void ln_app_set_white_list(struct ln_gapm_list_set_wl_cmd *wlist);


/**
 ****************************************************************************************
 * @brief Create an advertising activity
 *
 * @param[in] adv_creat_param               Pointer to activity create structure,don't fill in  operation.
 *
 ****************************************************************************************
 */
void ln_app_advertise_creat(struct ln_gapm_activity_create_adv_cmd * adv_creat_param);

/**
 ****************************************************************************************
 * @brief Set  advertising data.
 *
 * 
 * @param[in]   adv_data            Pointer to  advertising data structure,don't fill in  operation.
 *
 *
 ****************************************************************************************
 */
void ln_app_set_adv_data(struct ln_gapm_set_adv_data_cmd *adv_data);

/**
 ****************************************************************************************
 * @brief Set  scan response data.
 *
 * 
 * @param[in] scan_rsp_data             Pointer to scan response data structure ,don't fill in  operation.
 *
 *
 ****************************************************************************************
 */
void ln_app_set_scan_rsp_data(struct ln_gapm_set_adv_data_cmd *scan_rsp_data);

/**
 ****************************************************************************************
 * @brief Start an advertising activity
 *
 * @param[in] adv_start_param               Pointer to activity start structure,don't fill in  operation.
 *
 ****************************************************************************************
 */
 
void ln_app_advertise_start(struct ln_gapm_activity_start_cmd * adv_start_param);

/**
 ****************************************************************************************
 * @brief Create a scanning activity
 *
 * @param[in] scan_creat_param              Pointer to activity create structure,don't fill in  operation.
 *
 ****************************************************************************************
 */
void ln_app_scan_creat(struct ln_gapm_activity_create_adv_cmd *scan_creat_param);

/**
 ****************************************************************************************
 * @brief Start a scanning activity
 * @param[in] scan_start_param              Pointer to activity start structure,don't fill in  operation.
 *
 ****************************************************************************************
 */
void ln_app_scan_start(struct ln_gapm_activity_start_cmd * scan_start_param);

/**
 ****************************************************************************************
 * @brief Create a initiating activity
 *
 * @param[in] init_creat_param              Pointer to activity create structure,don't fill in  operation.
 *
 ****************************************************************************************
 */
void ln_app_init_creat(struct ln_gapm_activity_create_adv_cmd *init_creat_param);

/**
 ****************************************************************************************
 * @brief Start a initiating activity
 * @param[in] init_start_param              Pointer to activity start structure,don't fill in  operation.
 *
 ****************************************************************************************
 */
void ln_app_init_start(struct ln_gapm_activity_start_cmd * init_start_param);

/**
 ****************************************************************************************
 * @brief Stop an activity or all activities
 *
 * @param[in] actv_idx              Activity index ,If less than 0, stop all activities
 *
 ****************************************************************************************
 */
void ln_app_activity_stop(int actv_idx);

/**
 ****************************************************************************************
 * @brief Delete an activity or all activities
 *
 * @param[in] actv_idx              Activity index , If less than 0, stop all activities
 *
 ****************************************************************************************
 */
void ln_app_activity_delete(int actv_idx);

/**
 ****************************************************************************************
 * @brief Set  PHY cmd
 *
 * @param[in] conidx                    Connection index
 * @param[in] tx_phy                    @see struct gapc_set_phy_cmd
 * @param[in] rx_phy                    @see struct gapc_set_phy_cmd
 * @param[in] phy_opt                   @see struct gapc_set_phy_cmd
 ****************************************************************************************
 */
void ln_app_set_phy(int conidx,uint8_t tx_phy, uint8_t rx_phy, uint8_t phy_opt);

/**
 ****************************************************************************************
 * @brief Disconnect a link
 * @note: Either master or slave can call this function.
 *
 * @param[in] conidx                Connection index.
 * @param[in] reason                Reason of disconnection,  0x13:User on the remote device terminated the connnection
 *
 *
 ****************************************************************************************
 */
void ln_app_disconnect(int conidx,uint32_t reason);

/**
 ****************************************************************************************
 * @brief Update connection paramters
 * @note: Can be used by both master and slave,
 *
 * @param[in] conidx                Connection index
 * @param[in] p_conn_param          Pointer to update parameters structure.
 *
 ****************************************************************************************
 */
void ln_app_update_param(int conidx,struct ln_gapc_conn_param *p_conn_param);

/**
 ****************************************************************************************
 * @brief Confirm connection request
 * @note: This is to response to the event "GAP_EVT_CONN_REQ".  Application can call this API in the call back.
 *
 * @param[in] conidx                Connection index
 * @param[in] p_cfm         Pointer to comfirm  parameters structure.
 *
 ****************************************************************************************
 */
void ln_app_conn_cfm(int conidx, struct ln_gapc_connection_cfm *p_cfm);


/**
 ****************************************************************************************
 * @brief get peer device  information
 *
 * @param[in] conidx                Connection index
 * @param[in] get_info              Pointer to  parameters structure operation see@ln_get_peer_info.
 *
 ****************************************************************************************
 */
void ln_app_get_peer_info(int conidx,struct gapc_get_info_cmd* get_info );

/**
 ****************************************************************************************
 * @brief Confirm updated paramters
 * @note: This is only for master to accept or reject the new parameters proposed by slave,
 *
 * @param[in] conidx                Connection index
 * @param[in] update_cfm              Pointer to  parameters structure.
 *
 ****************************************************************************************
 */
void ln_app_param_update_cfm(int conidx, struct gapc_param_update_cfm *update_cfm);

/**
 ****************************************************************************************
 * @brief Initiate bond
 * @note This can only issue by master.
 *
 * @param[in] conidx                Connection index
 * @param[in] p_bond                Pointer to pairing requirement of initiator.
 *
 ****************************************************************************************
 */
void  ln_app_bond(int conidx,struct ln_gapc_bond_cmd *p_param );

/**
 ****************************************************************************************
 * @brief Confirm bond request
 *
 * @param[in] conidx                Connection index
 * @param[in] p_cfm                 Pointer to bond confirm data structure.
 *
 ****************************************************************************************
 */
void ln_app_bond_cfm(int conidx,struct gapc_bond_cfm *p_param);

/**
 ****************************************************************************************
 * @brief Change current LE packet size
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_pref_pkt_size   Pointer to packet size data structure.
 ****************************************************************************************
*/
void ln_app_param_set_pkt_size(int conidx, struct gapc_set_le_pkt_size_cmd *pkt_size);

/**
 ****************************************************************************************
 * @brief Use the AES-128 block in the controller
 *
 * @param[in] p_param               Pointer to data structure.
 *
 ****************************************************************************************
 */
void ln_app_use_enc(struct gapm_use_enc_block_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Request to inform the remote device when keys have been entered or erased
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_keypress_notify(int conidx, struct gapc_key_press_notif_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Send requested info to peer device
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_get_dev_info_cfm(int conidx, struct gapc_get_dev_info_cfm *p_param);
/**
 ****************************************************************************************
 * @brief  Local device accept or reject device info modification
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_set_dev_info_cfm(int conidx, struct gapc_set_dev_info_cfm *p_param);

/**
 ****************************************************************************************
 * @brief Start Encryption command procedure
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_encrypt(int conidx,struct gapc_encrypt_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Confirm requested Encryption information.
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_encrypt_cfm(int conidx, struct gapc_encrypt_cfm *p_param);

/**
 ****************************************************************************************
 * @brief  Start Security Request command procedure
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_req_security(int conidx, struct gapc_security_cmd *p_param);

/**
 ****************************************************************************************
 * @brief  Update LE Ping timeout value
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_set_ping_tmo(int conidx, struct gapc_set_le_ping_to_cmd *p_param);

#endif // _LN_APP_GAP_H_





