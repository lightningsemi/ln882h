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
    /// Search start handle
    uint16_t start_hdl;
    /// Search end handle
    uint16_t end_hdl;
    /// Service UUID
    uint8_t  uuid[LN_ATT_UUID_128_LEN];
};

/// Service Discovery Command Structure
/*@TRACE*/
struct ln_gattc_disc_cmd
{
    /// GATT request type
    uint8_t  operation;
    /// UUID length
    uint8_t  uuid_len;
    /// start handle range
    uint16_t start_hdl;
    /// start handle range
    uint16_t end_hdl;
    //uuid
    uint8_t* uuid;
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
     * point to List of attribute description present in service.
     */
    const struct gattm_att_desc *atts;
};

/// Add service in database request
/*@TRACE*/
struct ln_gattm_add_svc_req
{
    /// service description
    struct ln_gattm_svc_desc svc_desc;
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
    uint8_t* uuid;
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
    /// request union according to read type
    union ln_gattc_read_req req;
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
    /// Attribute handle
    uint16_t handle;
    /// Write offset
    uint16_t offset;
    /// Write length
    uint16_t length;
    //point to value
    uint8_t* value;
};

/// Send an event to peer device
/*@TRACE*/
struct ln_gattc_send_evt_cmd
{
    /// characteristic handle
    uint16_t handle;
    /// length of packet to send
    uint16_t length;
    /// point to value buff
    uint8_t* value;
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
 * @param[in] p_param               Pointer to attribute write paramters, Operation can fill in one of blow
 * /// Write attribute
    GATTC_WRITE,
    /// Write no response
    GATTC_WRITE_NO_RESPONSE,
    /// Write signed
    GATTC_WRITE_SIGNED,
    /// Execute write
    GATTC_EXEC_WRITE,
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
 * @param[in] p_param               Pointer to attribute  paramters struct.
 *
 ****************************************************************************************
 */
void ln_app_gatt_send_ind(int conidx,struct ln_gattc_send_evt_cmd *p_param);

/**
 ****************************************************************************************
 * @brief Send notification
 * @note This can only issue by the Server.
 *
 * @param[in] conidx                Connection index

 * @param[in] p_param               Pointer to attribute  paramters struct 
 ****************************************************************************************
 */
void ln_app_gatt_send_ntf(int conidx, struct ln_gattc_send_evt_cmd *p_param);


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



