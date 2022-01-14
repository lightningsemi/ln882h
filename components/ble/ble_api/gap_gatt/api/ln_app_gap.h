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
#include "co_bt_defines.h"

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

/// Set device configuration command
/*@TRACE*/
struct ln_gapm_set_dev_config_cmd
{
    /// Device Role: Central, Peripheral, Observer, Broadcaster or All roles.
    uint8_t role;

    /// -------------- Privacy Config -----------------------
    /// Duration before regenerate device address when privacy is enabled. - in seconds
    uint16_t renew_dur;
    /// Provided own static private random address
    struct bd_addr addr;
    /// Device IRK used for resolvable random BD address generation (LSB first)
    struct gap_sec_key irk;
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
    /// Device Name write permission @see gapm_write_att_perm
    uint8_t att_devname_write_perm;
    /// Device Appearance write permission @see gapm_write_att_perm
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
};

/// Set device channel map
/*@TRACE*/
struct ln_gapm_set_channel_map_cmd
{
    /// Channel map
    struct le_chnl_map chmap;
};

/// Resolve Address command
/*@TRACE*/
struct ln_gapm_resolv_addr_cmd
{
    /// Number of provided IRK (sahlle be > 0)
    uint8_t nb_key;
    /// Resolvable random address to solve
    struct bd_addr addr;
    /// point to IRK used for address resolution (MSB -> LSB)
    struct gap_sec_key* irk;
};

/// Generate a random address.
/*@TRACE*/
struct ln_gapm_gen_rand_addr_cmd
{
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
    /// Device IRK used for resolvable random BD address generation (LSB first)
    struct gap_sec_key irk;
};

/// Register a LE Protocol/Service Multiplexer command
/*@TRACE*/
struct ln_gapm_lepsm_register_cmd
{
    /// LE Protocol/Service Multiplexer
    uint16_t le_psm;
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
    /// LE Protocol/Service Multiplexer
    uint16_t le_psm;
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
    struct gap_bdaddr peer_identity;
};

/// Set content of resolving list command
/*@TRACE*/
struct ln_gapm_list_set_ral_cmd
{
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// point to List of entries to be added in the list
    struct gap_ral_dev_info* ral_info;
};

/// Set content of periodic advertiser list command
/*@TRACE*/
struct ln_gapm_list_set_pal_cmd
{
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// point to List of entries to be added in the list
    struct gapm_period_adv_addr_cfg* pal_info;
};

/// Set content of white list
/*@TRACE*/
struct ln_gapm_list_set_wl_cmd
{
    /// Number of entries to be added in the list. 0 means that list content has to be cleared
    uint8_t size;
    /// point to List of entries to be added in the list
    struct gap_bdaddr* wl_info;
};

/// Create an advertising activity command
struct ln_gapm_activity_create_adv_cmd
{
    /// Own address type (@see enum gapm_own_addr)
    uint8_t own_addr_type;
    /// Advertising parameters (optional, shall be present only if operation is GAPM_CREATE_ADV_ACTIVITY)
    /// For prop parameter, @see enum gapm_leg_adv_prop, @see enum gapm_ext_adv_prop and @see enum gapm_per_adv_prop for help
    struct gapm_adv_create_param adv_param;
};

/// Set advertising, scan response or periodic advertising data command
/*@TRACE*/
struct ln_gapm_set_adv_data_cmd
{
    /// Activity identifier
    uint8_t actv_idx;
    /// Data length
    uint16_t length;
    //point to adv data
    uint8_t* data;
};

/// Start a given activity command
/*@TRACE*/
struct ln_gapm_activity_start_cmd
{
    /// Activity identifier
    uint8_t actv_idx;
    /// Activity parameters
    union gapm_u_param u_param;
};

///  a scanning, an initiating, a periodic synchonization activity command (common)
/*@TRACE*/
struct ln_gapm_activity_create_cmd
{
    /// Own address type (@see enum gapm_own_addr)
    uint8_t own_addr_type;
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
    struct gap_sec_key lcsrk;
    /// Local signature counter value
    uint32_t           lsign_counter;
    /// Remote CSRK value
    struct gap_sec_key rcsrk;
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

/// Start Bonding command procedure
/*@TRACE*/
struct ln_gapc_bond_cmd
{
    /// Pairing information
    struct gapc_pairing pairing;
};

/// Confirm requested bond information.
/*@TRACE*/
struct ln_gapc_bond_cfm
{
    /// Bond request type (@see gapc_bond)
    uint8_t request;
    /// Request accepted
    uint8_t accept;

    /// Bond procedure information data
    union gapc_bond_cfm_data data;
};

/// Parameters of the @ref GAPC_SET_LE_PKT_SIZE_CMD message
/*@TRACE*/
struct ln_gapc_set_le_pkt_size_cmd
{
    ///Preferred maximum number of payload octets that the local Controller should include
    ///in a single Link Layer Data Channel PDU.
    uint16_t tx_octets;
    ///Preferred maximum number of microseconds that the local Controller should use to transmit
    ///a single Link Layer Data Channel PDU
    uint16_t tx_time;
};

/// Parameters of the @ref GAPM_USE_ENC_BLOCK_CMD message
/*@TRACE*/
struct ln_gapm_use_enc_block_cmd
{
    /// Operand 1
    uint8_t operand_1[GAP_KEY_LEN];
    /// Operand 2
    uint8_t operand_2[GAP_KEY_LEN];
};

/// Parameters of the @ref GAPC_KEY_PRESS_NOTIFICATION_CMD message
/*@TRACE*/
struct ln_gapc_key_press_notif_cmd
{
    /// notification type
    uint8_t notification_type;
};

/// Send requested info to peer device
/*@TRACE*/
struct ln_gapc_get_dev_info_cfm
{
    /// Requested information
    /// - GAPC_DEV_NAME: Device Name
    /// - GAPC_DEV_APPEARANCE: Device Appearance Icon
    /// - GAPC_DEV_SLV_PREF_PARAMS: Device Slave preferred parameters
    uint8_t req;

    /// Peer device information data
    union gapc_dev_info_val info;
};

/// Local device accept or reject device info modification
/*@TRACE*/
struct ln_gapc_set_dev_info_cfm
{
    /// Requested information
    /// - GAPC_DEV_NAME: Device Name
    /// - GAPC_DEV_APPEARANCE: Device Appearance Icon
    uint8_t req;

    /// Status code used to know if requested has been accepted or not
    uint8_t status;
};

/// Start Encryption command procedure
/*@TRACE*/
struct ln_gapc_encrypt_cmd
{
    /// Long Term Key information
    struct gapc_ltk ltk;
};

/// Confirm requested Encryption information.
/*@TRACE*/
struct ln_gapc_encrypt_cfm
{
    /// Indicate if a LTK has been found for the peer device
    uint8_t found;
    /// Long Term Key
    struct gap_sec_key ltk;
    /// LTK Key Size
    uint8_t key_size;
};

/// Start Security Request command procedure
/*@TRACE*/
struct ln_gapc_security_cmd
{
    /// Authentication level (@see gap_auth)
    uint8_t auth;
};

/// Parameters of the @ref GAPC_SET_LE_PING_TO_CMD message
/*@TRACE*/
struct ln_gapc_set_le_ping_to_cmd
{
    /// Authenticated payload timeout
    uint16_t timeout;
};


struct ln_gapc_connection_req_info
{
    /// Connection handle
    uint16_t conhdl;
    /// Connection interval
    uint16_t con_interval;
    /// Connection latency
    uint16_t con_latency;
    /// Link supervision timeout
    uint16_t sup_to;
    /// Clock accuracy
    uint8_t clk_accuracy;
    /// Peer address type
    uint8_t peer_addr_type;
    /// Peer BT address
    bd_addr_t peer_addr;
    /// Role of device in connection (0 = Master / 1 = Slave)
    uint8_t role;
    /// Connection index
    uint8_t conidx;
};

struct ln_gapc_disconnect_info
{
    /// Connection handle
    uint16_t conhdl;
    /// Reason of disconnection
    uint8_t reason;
    /// Connection index
    uint8_t conidx;
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
 * @param[in] cfg_param                 Pointer to the device configurtion structure.

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
 * @param[in] p_chn_map         Pointer to the channel map array. 
 *
 *
 ****************************************************************************************
 */
void ln_app_set_chn_map(struct ln_gapm_set_channel_map_cmd *p_chn_map);

/**
 ****************************************************************************************
 * @brief Resolve provided random address by usng array of IRK
 *
 * @param[in] resolv_addr           Pointer to the random address and possible keys.  
 *
 ****************************************************************************************
 */
void ln_app_resolve_addr(struct ln_gapm_resolv_addr_cmd *resolv_addr);

/**
 ****************************************************************************************
 * @brief Generate random address
 *
 * @param[in] p_param       random address type. @see enum  random_addr_type

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
 * @param[out] p_param              Pointer to a security key data structure.
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
 * @param[in] p_param                   Pointer to resolving list data structure.
 *
 ****************************************************************************************
 */

void ln_app_set_ral_list(struct ln_gapm_list_set_ral_cmd *p_param);
/**
 ****************************************************************************************
 * @brief Set the peroidic advertiser list content.
 * @note This will overwrite the current periodic advertiser list.
 *
 * @param[in] p_param                   Pointer to periodic advertiser list data structure.
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
 * @param[in] adv_creat_param               Pointer to activity create structure.
 *
 ****************************************************************************************
 */
void ln_app_advertise_creat(struct ln_gapm_activity_create_adv_cmd * adv_creat_param);

/**
 ****************************************************************************************
 * @brief Set  advertising data.
 *
 * 
 * @param[in]   adv_data            Pointer to  advertising data structure .
 *
 ****************************************************************************************
 */
void ln_app_set_adv_data(struct ln_gapm_set_adv_data_cmd *adv_data);

/**
 ****************************************************************************************
 * @brief Set  scan response data.
 *
 * 
 * @param[in] scan_rsp_data             Pointer to scan response data structure.
 *
 ****************************************************************************************
 */
void ln_app_set_scan_rsp_data(struct ln_gapm_set_adv_data_cmd *scan_rsp_data);

/**
 ****************************************************************************************
 * @brief Start an advertising activity
 *
 * @param[in] adv_start_param               Pointer to activity start structure.
 *
 ****************************************************************************************
 */
 
void ln_app_advertise_start(struct ln_gapm_activity_start_cmd * adv_start_param);

/**
 ****************************************************************************************
 * @brief Create a scanning activity
 *
 * @param[in] scan_creat_param              Pointer to activity create structure.
 *
 ****************************************************************************************
 */
void ln_app_scan_creat(struct ln_gapm_activity_create_cmd *scan_creat_param);

/**
 ****************************************************************************************
 * @brief Start a scanning activity
 * @param[in] scan_start_param              Pointer to activity start structure.
 *
 ****************************************************************************************
 */
void ln_app_scan_start(struct ln_gapm_activity_start_cmd * scan_start_param);

/**
 ****************************************************************************************
 * @brief Create a initiating activity
 *
 * @param[in] init_creat_param              Pointer to activity create structure.
 *
 ****************************************************************************************
 */
void ln_app_init_creat(struct ln_gapm_activity_create_cmd *init_creat_param);

/**
 ****************************************************************************************
 * @brief Start a initiating activity
 * @param[in] init_start_param              Pointer to activity start structure.
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
void ln_app_get_peer_info(int conidx,struct ln_gapc_get_info_cmd* get_info );

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
void ln_app_param_update_cfm(int conidx, struct ln_gapc_param_update_cfm *update_cfm);

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
void ln_app_bond_cfm(int conidx,struct ln_gapc_bond_cfm *p_param);

/**
 ****************************************************************************************
 * @brief Change current LE packet size
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_pref_pkt_size   Pointer to packet size data structure.
 ****************************************************************************************
*/
void ln_app_param_set_pkt_size(int conidx, struct ln_gapc_set_le_pkt_size_cmd *pkt_size);

/**
 ****************************************************************************************
 * @brief Use the AES-128 block in the controller
 *
 * @param[in] p_param               Pointer to data structure.
 *
 ****************************************************************************************
 */
void ln_app_use_enc(struct ln_gapm_use_enc_block_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Request to inform the remote device when keys have been entered or erased
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_keypress_notify(int conidx, struct ln_gapc_key_press_notif_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Send requested info to peer device
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_get_dev_info_cfm(int conidx, struct ln_gapc_get_dev_info_cfm *p_param);
/**
 ****************************************************************************************
 * @brief  Local device accept or reject device info modification
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_set_dev_info_cfm(int conidx, struct ln_gapc_set_dev_info_cfm *p_param);

/**
 ****************************************************************************************
 * @brief Start Encryption command procedure
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_encrypt(int conidx,struct ln_gapc_encrypt_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Confirm requested Encryption information.
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_encrypt_cfm(int conidx, struct ln_gapc_encrypt_cfm *p_param);

/**
 ****************************************************************************************
 * @brief  Start Security Request command procedure
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_req_security(int conidx, struct ln_gapc_security_cmd *p_param);

/**
 ****************************************************************************************
 * @brief  Update LE Ping timeout value
 *
 * @param[in] conidx                Connection index  
 * @param[in] p_param         Pointer to  data structure.
 ****************************************************************************************
*/
void ln_app_set_ping_tmo(int conidx, struct ln_gapc_set_le_ping_to_cmd *p_param);

#endif // _LN_APP_GAP_H_





