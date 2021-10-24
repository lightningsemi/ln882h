/**
 ****************************************************************************************
 *
 * @file ln_app_gatt.h
 *
 * @brief GATT API.
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_APP_GATT_H_
#define _LN_APP_GATT_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @group BLE
 *
 * @brief GATT API.
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
#include "gattc_task.h"
#include "gattm_task.h"
#include "att.h"
#include "gap.h"
#include "rwip_task.h" // Task definitions
#include "ln_compiler.h"
#include "ke_msg.h"
/*
 * DEFINES
 ****************************************************************************************
 */


#define LN_ATT_UUID_128_LEN                        0x0010

//typedef uint16_t ln_ke_msg_id_t;


/// Command complete event data structure
struct ln_gattc_op_cmd
{
    /// GATT request type
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Command complete event data structure
/*@TRACE*/
struct ln_gattc_cmp_evt
{
    /// GATT request type
    uint8_t operation;
    /// Status of the request
    uint8_t status;
    /// operation sequence number - provided when operation is started
    uint16_t seq_num;
};


/// Service Discovery Command Structure
/*@TRACE*/
struct ln_gattc_exc_mtu_cmd
{
    /// GATT request type
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Indicate that the ATT MTU has been updated (negotiated)
/*@TRACE*/
struct ln_gattc_mtu_changed_ind
{
    /// Exchanged MTU value
    uint16_t mtu;
    /// operation sequence number
    uint16_t seq_num;
};

/// Service Discovery Command Structure
/*@TRACE*/
struct ln_gattc_disc_cmd
{
    /// GATT request type
    uint8_t  operation;
    /// UUID length
    uint8_t  uuid_len;
    /// operation sequence number
    uint16_t seq_num;
    /// start handle range
    uint16_t start_hdl;
    /// start handle range
    uint16_t end_hdl;
    /// UUID
    uint8_t  uuid[__ARRAY_EMPTY];
};

struct ln_gattc_disc_svc
{
    uint8_t conidx;
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// UUID length
    uint8_t  uuid_len;
    /// service UUID
    uint8_t  uuid[__ARRAY_EMPTY];
};

/// Discover Service indication Structure
/*@TRACE*/
struct ln_gattc_disc_svc_ind
{
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// UUID length
    uint8_t  uuid_len;
    /// service UUID
    uint8_t  uuid[__ARRAY_EMPTY];
};

/// Discover Service indication Structure
/*@TRACE*/
struct ln_gattc_disc_svc_incl_ind
{
    /// element handle
    uint16_t attr_hdl;
    /// start handle
    uint16_t start_hdl;
    /// end handle
    uint16_t end_hdl;
    /// UUID length
    uint8_t uuid_len;
    /// included service UUID
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Discovery All Characteristic indication Structure
/*@TRACE*/
struct ln_gattc_disc_char_ind
{
    /// database element handle
    uint16_t attr_hdl;
    /// pointer attribute handle to UUID
    uint16_t pointer_hdl;
    /// properties
    uint8_t prop;
    /// UUID length
    uint8_t uuid_len;
    /// characteristic UUID
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Discovery Characteristic Descriptor indication Structure
/*@TRACE*/
struct ln_gattc_disc_char_desc_ind
{
    /// database element handle
    uint16_t attr_hdl;
    /// UUID length
    uint8_t uuid_len;
    /// Descriptor UUID
    uint8_t uuid[__ARRAY_EMPTY];
};


/// Simple Read (GATTC_READ or GATTC_READ_LONG)
/*@TRACE
 gattc_read = gattc_read_simple
 gattc_read_long = gattc_read_simple*/
struct ln_gattc_read_simple
{
    /// attribute handle
    uint16_t handle;
    /// start offset in data payload
    uint16_t offset;
    /// Length of data to read (0 = read all)
    uint16_t length;
};

/// Read by UUID: search UUID and read it's characteristic value (GATTC_READ_BY_UUID)
/// Note: it doesn't perform an automatic read long.
/*@TRACE*/
struct ln_gattc_read_by_uuid
{
    /// Start handle
    uint16_t start_hdl;
    /// End handle
    uint16_t end_hdl;
    /// Size of UUID
    uint8_t uuid_len;
    /// UUID value
    uint8_t uuid[__ARRAY_EMPTY];
};

/// Read Multiple short characteristic (GATTC_READ_MULTIPLE)
/*@TRACE*/
struct ln_gattc_read_multiple
{
    /// attribute handle
    uint16_t handle;
    /// Known Handle length (shall be != 0)
    uint16_t len;
};

/// request union according to read type
/*@TRACE
 @trc_ref gattc_operation
 */
union ln_gattc_read_req
{
    /// Simple Read (GATTC_READ or GATTC_READ_LONG)
    //@trc_union parent.operation == GATTC_READ or parent.operation == GATTC_READ_LONG
    struct ln_gattc_read_simple simple;
    /// Read by UUID (GATTC_READ_BY_UUID)
    //@trc_union parent.operation == GATTC_READ_BY_UUID
    struct ln_gattc_read_by_uuid by_uuid;
    /// Read Multiple short characteristic (GATTC_READ_MULTIPLE)
    //@trc_union parent.operation == GATTC_READ_MULTIPLE
    struct ln_gattc_read_multiple multiple[1];
};

/// Read command (Simple, Long, Multiple, or by UUID)
/*@TRACE*/
struct ln_gattc_read_cmd
{
    /// request type
    uint8_t operation;
    /// number of read (only used for multiple read)
    uint8_t nb;
    /// operation sequence number
    uint16_t seq_num;
    /// request union according to read type
    union ln_gattc_read_req req;
};

/// Attribute value read indication
/*@TRACE*/
struct ln_gattc_read_ind
{
    /// Attribute handle
    uint16_t handle;
    /// Read offset
    uint16_t offset;
    /// Read length
    uint16_t length;
    /// Handle value
    uint8_t value[__ARRAY_EMPTY];
};

/// Write peer attribute value command
/*@TRACE*/
struct ln_gattc_write_cmd
{
    /// Request type
    uint8_t operation;
    /// Perform automatic execution
    /// (if false, an ATT Prepare Write will be used this shall be use for reliable write)
    bool auto_execute;
    /// operation sequence number
    uint16_t seq_num;
    /// Attribute handle
    uint16_t handle;
    /// Write offset
    uint16_t offset;
    /// Write length
    uint16_t length;
    /// Internal write cursor shall be initialized to 0
    uint16_t cursor;
    /// Value to write
    uint8_t value[__ARRAY_EMPTY];
};

/// Write peer attribute value command
/*@TRACE*/
struct ln_gattc_execute_write_cmd
{
    /// Request type
    uint8_t operation;

    /// [True = perform/False cancel] pending write operations
    bool execute;
    /// operation sequence number
    uint16_t seq_num;
};

struct ln_attc_write_req_ind
{
    uint8_t conidx;
    /// Handle of the attribute that has to be written
    uint16_t handle;
    /// offset at which the data has to be written
    uint8_t offset;
    /// Data length to be written
    uint8_t length;
    /// Data to be written in attribute database
    uint8_t  value[__ARRAY_EMPTY];
};

struct ln_gattc_event
{
    uint8_t conidx;
    /// Event Type
    uint8_t type;
    /// Data length
    uint16_t length;
    /// Attribute handle
    uint16_t handle;
    /// Event Value
    uint8_t value[__ARRAY_EMPTY];
};

/// peer device triggers an event (notification)
/*@TRACE*/
struct ln_gattc_event_ind
{
    /// Event Type
    uint8_t type;
    /// Data length
    uint16_t length;
    /// Attribute handle
    uint16_t handle;
    /// Event Value
    uint8_t value[__ARRAY_EMPTY];
};

/// peer device triggers an event that requires a confirmation (indication)
/*@TRACE*/
struct ln_gattc_event_req_ind
{
    /// Event Type
    uint8_t type;
    /// Data length
    uint16_t length;
    /// Attribute handle
    uint16_t handle;
    /// Event Value
    uint8_t value[__ARRAY_EMPTY];
};

/// Confirm reception of event (trigger a confirmation message)
/*@TRACE*/
struct ln_gattc_event_cfm
{
    /// Attribute handle
    uint16_t handle;
};

/// Register to peer device events command
/*@TRACE*/
struct ln_gattc_reg_to_peer_evt_cmd
{
    /// Request type
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
    /// attribute start handle
    uint16_t start_hdl;
    /// attribute end handle
    uint16_t end_hdl;
};

/// Send an event to peer device
/*@TRACE*/
struct ln_gattc_send_evt_cmd
{
    /// Request type (notification / indication)
    uint8_t operation;
    /// operation sequence number
    uint16_t seq_num;
    /// characteristic handle
    uint16_t handle;
    /// length of packet to send
    uint16_t length;
    /// data value
    uint8_t  value[__ARRAY_EMPTY];
};

/// Inform that attribute value is requested by lower layers.
/*@TRACE*/
struct ln_gattc_read_req_ind
{
    /// Handle of the attribute that has to be read
    uint16_t handle;
};

/// Confirm Read Request requested by GATT to profile
/*@TRACE*/
struct ln_gattc_read_cfm
{
    /// Handle of the attribute read
    uint16_t handle;
    /// Data length read
    uint16_t length;
    /// Status of read command execution by upper layers
    uint8_t status;
    /// attribute data value
    uint8_t  value[__ARRAY_EMPTY];
};

/// Inform that a modification of database has been requested by peer device.
/*@TRACE*/
struct ln_gattc_write_req_ind
{
    /// Handle of the attribute that has to be written
    uint16_t handle;
    /// offset at which the data has to be written
    uint16_t offset;
    /// Data length to be written
    uint16_t length;
    /// Data to be written in attribute database
    uint8_t  value[__ARRAY_EMPTY];
};

/// Confirm modification of database from upper layer when requested by peer device.
/*@TRACE*/
struct ln_gattc_write_cfm
{
    /// Handle of the attribute written
    uint16_t handle;
    /// Status of write command execution by upper layers
    uint8_t status;
};

/// Request Attribute info to upper layer - could be trigger during prepare write
/*@TRACE*/
struct ln_gattc_att_info_req_ind
{
    /// Handle of the attribute for which info are requested
    uint16_t handle;
};

/// Attribute info from upper layer confirmation
/*@TRACE*/
struct ln_gattc_att_info_cfm
{
    /// Handle of the attribute
    uint16_t handle;
    /// Current length of the attribute
    uint16_t length;
    /// use to know if it's possible to modify the attribute
    /// can contains authorization or application error code.
    uint8_t  status;
};


/// Service Discovery command
/*@TRACE*/
struct ln_gattc_sdp_svc_disc_cmd
{
    /// GATT Request Type
    /// - GATTC_SDP_DISC_SVC Search specific service
    /// - GATTC_SDP_DISC_SVC_ALL Search for all services
    /// - GATTC_SDP_DISC_CANCEL Cancel Service Discovery Procedure
    uint8_t operation;
    /// Service UUID Length
    uint8_t  uuid_len;
    /// operation sequence number
    uint16_t seq_num;
    /// Search start handle
    uint16_t start_hdl;
    /// Search end handle
    uint16_t end_hdl;
    /// Service UUID
    uint8_t  uuid[LN_ATT_UUID_128_LEN];
};



/// Information about included service
/*@TRACE*/
struct ln_gattc_sdp_include_svc
{
    /// Attribute Type
    /// - GATTC_SDP_INC_SVC: Included Service Information
    uint8_t att_type;
    /// Included service UUID Length
    uint8_t uuid_len;
    /// Included Service UUID
    uint8_t  uuid[LN_ATT_UUID_128_LEN];
    /// Included service Start Handle
    uint16_t start_hdl;
    /// Included service End Handle
    uint16_t end_hdl;
};

/// Information about attribute characteristic
/*@TRACE*/
struct ln_gattc_sdp_att_char
{
    /// Attribute Type
    /// - GATTC_SDP_ATT_CHAR: Characteristic Declaration
    uint8_t att_type;
    /// Value property
    uint8_t prop;
    /// Value Handle
    uint16_t handle;
};

/// Information about attribute
/*@TRACE*/
struct ln_gattc_sdp_att
{
    /// Attribute Type
    /// - GATTC_SDP_ATT_VAL: Attribute Value
    /// - GATTC_SDP_ATT_DESC: Attribute Descriptor
    uint8_t  att_type;
    /// Attribute UUID Length
    uint8_t  uuid_len;
    /// Attribute UUID
    uint8_t  uuid[LN_ATT_UUID_128_LEN];
};

/// Attribute information
/*@TRACE
 @trc_ref gattc_sdp_att_type
 */
union ln_gattc_sdp_att_info
{
    /// Attribute Type
    uint8_t att_type;
    /// Information about attribute characteristic
    //@trc_union att_type == GATTC_SDP_ATT_CHAR
    struct ln_gattc_sdp_att_char att_char;
    /// Information about included service
    //@trc_union att_type == GATTC_SDP_INC_SVC
    struct ln_gattc_sdp_include_svc inc_svc;
    /// Information about attribute
    //@trc_union att_type == GATTC_SDP_ATT_VAL or att_type == GATTC_SDP_ATT_DESC
    struct ln_gattc_sdp_att att;
};


/// Service Discovery indicate that a service has been found.
/*@TRACE
 @trc_arr info $end_hdl - $start_hdl
 */
struct ln_gattc_sdp_svc_ind
{
    /// Service UUID Length
    uint8_t  uuid_len;
    /// Service UUID
    uint8_t  uuid[LN_ATT_UUID_128_LEN];
    /// Service start handle
    uint16_t start_hdl;
    /// Service end handle
    uint16_t end_hdl;
    /// attribute information present in the service
    /// (length = end_hdl - start_hdl)
    union ln_gattc_sdp_att_info info[__ARRAY_EMPTY];
};

/// Indicate that an unknown message has been received
/*@TRACE*/
struct ln_gattc_unknown_msg_ind
{
    /// Unknown message id
    uint16_t unknown_msg_id;
};

/// Command used to enable Robust database caching
/// (Register to service changed, Set client supported features and read database hash)
/*@TRACE*/
struct ln_gattc_robust_db_cache_en_cmd
{
    /// GATT Request Type
    /// - GATTC_ROBUST_DB_CACHE_EN Enable Robust database caching
    uint8_t  operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Command used to read peer database hash
/*@TRACE*/
struct ln_gattc_read_db_hash_cmd
{
    /// GATT Request Type
    /// - GATTC_READ_DB_HASH Read peer database hash
    uint8_t  operation;
    /// operation sequence number
    uint16_t seq_num;
};

/// Provide value of the peer database hash
/*@TRACE*/
struct ln_gattc_db_hash_ind
{
    /// Database Hash
    uint8_t hash[GAP_KEY_LEN];
};

/// Informs that peer device database updated using service changed indication
/*@TRACE
 @trc_arr info $end_hdl - $start_hdl
 */
struct ln_gattc_svc_chg_req_ind
{
    /// Start handle
    uint16_t start_handle;
    /// End Handle
    uint16_t end_handle;
};

/// Provide information about GATT for current connection that can be reuse on another connection
/*@TRACE*/
struct ln_gattc_con_info_ind
{
    // GATT Client Side
    /// Peer GATT Service Start handle
    uint16_t gatt_start_handle;
    /// Peer GATT Service End Handle
    uint16_t gatt_end_handle;
    /// Peer Service Change value handle
    uint16_t svc_chg_handle;

    // GATT Service Side
    /// Client bond data information (@see enum gapc_cli_info)
    uint8_t  cli_info;
    /// Client supported features    (@see enum gapc_cli_feat)
    uint8_t  cli_feat;
};


//  GATTM


/**
 * Attribute Description
 */
/*@TRACE*/
struct ln_gattm_att_desc
{
    /** Attribute UUID (LSB First) */
    uint8_t uuid[LN_ATT_UUID_128_LEN];

    /**
     *  Attribute Permission (@see attm_perm_mask)
     */
    uint16_t perm;


    /**
     * Maximum length of the attribute
     *
     * Note:
     * For Included Services and Characteristic Declarations, this field contains targeted
     * handle.
     *
     * For Characteristic Extended Properties, this field contains 2 byte value
     *
     * Not used Client Characteristic Configuration and Server Characteristic Configuration,
     * this field is not used.
     */
    uint16_t max_len;

    /**
     * Attribute Extended permissions
     *
     * Extended Value permission bit field
     *
     *   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
     * | RI |UUID_LEN |EKS |                       Reserved                            |
     * +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
    *
     * Bit [0-11] : Reserved
     * Bit [12]   : Encryption key Size must be 16 bytes
     * Bit [13-14]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
     * Bit [15]   : Trigger Read Indication (0 = Value present in Database, 1 = Value not present in Database)
     */
    uint16_t ext_perm;
};

/**
 * Service description
 */
/*@TRACE
 @trc_arr atts $nb_att*/
struct ln_gattm_svc_desc
{
    /// Attribute Start Handle (0 = dynamically allocated)
    uint16_t start_hdl;
    /// Task identifier that manages service
    uint16_t task_id;

    /**
     *    7    6    5    4    3    2    1    0
     * +----+----+----+----+----+----+----+----+
     * |SEC |UUID_LEN |DIS |  AUTH   |EKS | MI |
     * +----+----+----+----+----+----+----+----+
     *
     * Bit [0]  : Task that manage service is multi-instantiated (Connection index is conveyed)
     * Bit [1]  : Encryption key Size must be 16 bytes
     * Bit [2-3]: Service Permission      (0 = NO_AUTH, 1 = UNAUTH, 2 = AUTH, 3 = Secure Connect)
     * Bit [4]  : Disable the service
     * Bit [5-6]: UUID Length             (0 = 16 bits, 1 = 32 bits, 2 = 128 bits, 3 = RFU)
     * Bit [7]  : Secondary Service       (0 = Primary Service, 1 = Secondary Service)
     */
    uint8_t perm;

    /// Number of attributes
    uint8_t nb_att;

    /** Service  UUID */
    uint8_t uuid[LN_ATT_UUID_128_LEN];

    /**
     * List of attribute description present in service.
     */
    struct ln_gattm_att_desc atts[__ARRAY_EMPTY];
};


/// Add service in database request
/*@TRACE*/
struct ln_gattm_add_svc_req
{
    /// service description
    struct ln_gattm_svc_desc svc_desc;
};

/// Add service in database response
/*@TRACE*/
struct ln_gattm_add_svc_rsp
{
    /// Start handle of allocated service in attribute database
    uint16_t start_hdl;
    /// Return status of service allocation in attribute database.
    uint8_t status;
};

/* Service management */
/// Get permission settings of service request
/*@TRACE*/
struct ln_gattm_svc_get_permission_req
{
    /// Service start attribute handle
    uint16_t start_hdl;
};

/// Get permission settings of service response
/*@TRACE*/
struct ln_gattm_svc_get_permission_rsp
{
    /// Service start attribute handle
    uint16_t start_hdl;
    /// service permission
    uint8_t perm;
    /// Return status
    uint8_t status;
};

/// Set permission settings of service request
/*@TRACE*/
struct ln_gattm_svc_set_permission_req
{
    /// Service start attribute handle
    uint16_t start_hdl;
    /// service permission
    uint8_t perm;
};

/// Set permission settings of service response
/*@TRACE*/
struct ln_gattm_svc_set_permission_rsp
{
    /// Service start attribute handle
    uint16_t start_hdl;
    /// Return status
    uint8_t status;
};


/* Attribute management */
/// Get permission settings of attribute request
/*@TRACE*/
struct ln_gattm_att_get_permission_req
{
    /// Handle of the attribute
    uint16_t handle;
};

/// Get permission settings of attribute response
/*@TRACE*/
struct ln_gattm_att_get_permission_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute permission
    uint16_t perm;
    /// Extended Attribute permission
    uint16_t ext_perm;
    /// Return status
    uint8_t status;
};

/// Set permission settings of attribute request
/*@TRACE*/
struct ln_gattm_att_set_permission_req
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute permission
    uint16_t perm;
    /// Extended Attribute permission
    uint16_t ext_perm;
};

/// Set permission settings of attribute response
/*@TRACE*/
struct ln_gattm_att_set_permission_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Return status
    uint8_t status;
};


/// Get attribute value request
/*@TRACE*/
struct ln_gattm_att_get_value_req
{
    /// Handle of the attribute
    uint16_t handle;
};

/// Get attribute value response
/*@TRACE*/
struct ln_gattm_att_get_value_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute value length
    uint16_t length;
    /// Return status
    uint8_t status;
    /// Attribute value
    uint8_t value[__ARRAY_EMPTY];
};

/// Set attribute value request
/*@TRACE*/
struct ln_gattm_att_set_value_req
{
    /// Handle of the attribute
    uint16_t handle;
    /// Attribute value length
    uint16_t length;
    /// Attribute value
    uint8_t value[__ARRAY_EMPTY];
};

/// Set attribute value response
/*@TRACE*/
struct ln_gattm_att_set_value_rsp
{
    /// Handle of the attribute
    uint16_t handle;
    /// Return status
    uint8_t status;
};

/// Indicate that an unknown message has been received
/*@TRACE*/
struct ln_gattm_unknown_msg_ind
{
    /// Unknown message id
    uint16_t unknown_msg_id;
};

/// DEBUG ONLY: Destroy Attribute database request
/*@TRACE*/
struct ln_gattm_destroy_db_req
{
    /// New Gap Start Handle
    uint16_t gap_hdl;
    /// New Gatt Start Handle
    uint16_t gatt_hdl;
};

/// DEBUG ONLY: Destroy Attribute database Response
/*@TRACE*/
struct ln_gattm_destroy_db_rsp
{
    /// Return status
    uint8_t status;
};


/// Service information
/*@TRACE*/
struct ln_gattm_svc_info
{
    /// Service start handle
    uint16_t start_hdl;
    /// Service end handle
    uint16_t end_hdl;
    /// Service task_id
    uint16_t task_id;
    /// Service permission
    uint8_t perm;
};

/// DEBUG ONLY: Retrieve list of services response
/*@TRACE*/
struct ln_gattm_svc_get_list_rsp
{
    /// Return status
    uint8_t status;
    /// Number of services
    uint8_t nb_svc;
    /// Array of information about services
    struct  ln_gattm_svc_info svc[__ARRAY_EMPTY];
};

/// DEBUG ONLY: Retrieve information of attribute request
/*@TRACE*/
struct ln_gattm_att_get_info_req
{
    /// Attribute Handle
    uint16_t handle;
};

/// DEBUG ONLY: Retrieve information of attribute response
/*@TRACE*/
struct  ln_gattm_att_get_info_rsp
{
    /// Return status
    uint8_t status;
    /// UUID Length
    uint8_t uuid_len;
    /// Attribute Handle
    uint16_t handle;
    /// Attribute Permissions
    uint16_t perm;
    /// Extended Attribute permission
    uint16_t ext_perm;
    /// UUID value
    uint8_t uuid[ATT_UUID_128_LEN];
};

/// DEBUG ONLY: Set Service visibility request
/*@TRACE*/
struct ln_gattm_svc_visibility_set_req
{
    /// Service handle
    uint16_t handle;
    /// True to set service visible, false to hide it
    bool     visible;
};

/// DEBUG ONLY: Set Service visibility response
/*@TRACE*/
struct ln_gattm_svc_visibility_set_rsp
{
    /// Return status
    uint8_t  status;
    /// Service handle
    uint16_t handle;
};

/// Compute hash value of the attribute database response
/*@TRACE*/
struct ln_gattm_att_db_hash_comp_rsp
{
    /// Return status
    uint8_t status;
    /// Computed hash value (valid if status is GAP_ERR_NO_ERROR)
    uint8_t hash[GAP_KEY_LEN];
};

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */















/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Exchange MTU request
 * @note This can only issue by the Client.
 *
 * @param[in] conidx                Connection index
 ****************************************************************************************
 */
void ln_app_gatt_exc_mtu(int conidx);


/**
 ****************************************************************************************
 * @brief Service Discovery Procedure
 * @note This can only issue by the Client.
 *
 * @param[in] conidx                Connection index
 * @param[in] p_param               Pointer to service discovery paramters,
 * @param[in] p_sdp
 *
 ****************************************************************************************
 */
void ln_app_gatt_sdp(int conidx, struct ln_gattc_sdp_svc_disc_cmd *p_param);


/**
 ****************************************************************************************
 * @brief Attributes Discovery
 * @note This can only issue by the Client.
 *
 * @param[in] conidx                Connection index
 * @param[in] p_param               Pointer to  discovery paramters,  operation fill in follow
 *  Discover all services
 *   GATTC_DISC_ALL_SVC,
 *   /// Discover services by UUID
 *  GATTC_DISC_BY_UUID_SVC,
 *   /// Discover included services
 *   GATTC_DISC_INCLUDED_SVC,
 *   /// Discover all characteristics
 *    GATTC_DISC_ALL_CHAR,
 *   /// Discover characteristic by UUID
 *   GATTC_DISC_BY_UUID_CHAR,
 *   /// Discover characteristic descriptor
 *   GATTC_DISC_DESC_CHAR,
 *
 ****************************************************************************************
 */
void ln_app_gatt_discovery(int conidx, struct ln_gattc_disc_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Add a new Service
 * @note This can only issue by the Server.
 *
 * @param[in] p_param               Pointer to service data structure
 ****************************************************************************************
 */
void ln_app_gatt_add_svc(struct ln_gattm_add_svc_req *p_param);


/**
 ****************************************************************************************
 * @brief Read Attribute
 * @note This can only issue by the Client.
 *
 * @param[in] conidx                Connection index
 * @param[in] p_param               Pointer to   gattc_read_cmd structure,  Operation can fill in one of blow
 *  /// Read attribute
 *   GATTC_READ,
 *   /// Read long attribute
 *   GATTC_READ_LONG,
 *   /// Read attribute by UUID
 *   GATTC_READ_BY_UUID,
 *   /// Read multiple attribute
 *   GATTC_READ_MULTIPLE,
 *
 *
 ****************************************************************************************
 */
void  ln_app_gatt_read(int conidx,  struct ln_gattc_read_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Write Attribute
 * @note This can only issue by the Client.
 *
 * @param[in] conidx                Connection index
 *
 * @param[in] p_param               Pointer to attribute write paramters, seq_num and cursor don't set. Operation can fill in one of blow
 * /// Write attribute
    GATTC_WRITE,
    /// Write no response
    GATTC_WRITE_NO_RESPONSE,
    /// Write signed
    GATTC_WRITE_SIGNED,
    /// Execute write
    GATTC_EXEC_WRITE,
 *
 ****************************************************************************************
 */
void  ln_app_gatt_write(int conidx, struct ln_gattc_write_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Execute Write command for queue writes
 * @note This can only issue by the Client.
 *
 * @param[in] conidx                Connection index
 * @param[in] execute               True: execute, false: cancel
 *
 *
 ****************************************************************************************
 */
void ln_app_gatt_excute_write(int conidx, bool execute);

/**
 ****************************************************************************************
 * @brief Send indication
 * @note This can only issue by the Server.
 *
 * @param[in] conidx                Connection index
 * @param[in] p_param               Pointer to attribute  paramters struct ,Operation don't fill in .
 *
 ****************************************************************************************
 */
void ln_app_gatt_send_ind(int conidx,struct ln_gattc_send_evt_cmd *p_param );

/**
 ****************************************************************************************
 * @brief Send notification
 * @note This can only issue by the Server.
 *
 * @param[in] conidx                Connection index

 * @param[in] p_param               Pointer to attribute  paramters struct ,Operation don't fill in .
 ****************************************************************************************
 */
void ln_app_gatt_send_ntf(int conidx,struct ln_gattc_send_evt_cmd *p_param );


/**
 ****************************************************************************************
 * @brief Confirm attribute write reqeust
 * @note This can only issue by the Server.
 *
 * @param[in] conidx                Connection index
 *
 * @param[in] p_param               Pointer to   paramters struct
 *
 ****************************************************************************************
 */
void ln_app_gatt_write_req_cfm(uint8_t conidx, struct ln_gattc_write_cfm *p_param);


#endif // _LN_APP_GATT_H_



