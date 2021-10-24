/**
 ****************************************************************************************
 *
 * @file ln_app_hid.h
 *
 * @brief Human Interface Device (HID) Profile APIs.
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_APP_HID_H_
#define _LN_APP_HID_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @group BLE
 *
 * @brief Human Interface Device (HID) Profile APIs.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "hogpd.h"
#include "hogpd_task.h"


/*
 * DEFINES
 ****************************************************************************************
 */

#define KEYBOARD_IN_REP_ID          0x01
#define CONSUMEFR_CTRL_IN_REP_ID    0x02
#define AUDIO_IN_FEP_ID             0xF0
#define AUIDO_CONFIG_REP_ID         0xF1
#define AUIDO_STATE_REP_ID          0xF2
#define DIA_REP_ID                  0xF3

#define KEYBOARD_IN_REP_SIZE       8
#define KEYBOARD_IN_REP_COUNT      3

#define CONSUMER_CTRL_IN_REP_SIZE  16
#define CONSUMER_CTRL_IN_REP_COUNT 2

#define AUDIO_IN_REP_SIZE          8
#define AUDIO_IN_REP_COUNT         128*2

#define AUDIO_CONFIG_REP_SIZE      8
#define AUDIO_CONFIG_REP_COUNT     3

#define AUDIO_STATE_REP_SIZE       8
#define AUDIO_STATE_REP_COUNT      1

#define DIA_REP_SIZE               8
#define DIA_REP_COUNT              10





/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

struct ln_gapm_profile_add_hid
{ 
    /// Security Level :
    ///  7    6    5    4    3    2    1    0
    /// +----+----+----+----+----+----+----+----+
    /// |   Reserved   |DIS |  AUTH   |EKS | MI |
    /// +----+----+----+----+----+----+----+----+
    ///
    /// - MI: 1 - Application task is a Multi-Instantiated task, 0 - Mono-Instantiated
    /// Only applies for service - Ignored by collectors:
    /// - EKS: Service needs a 16 bytes encryption key
    /// - AUTH: 0 - Disable, 1 - Enable, 2 - Unauth, 3 - Auth
    /// - DIS: Disable the service
    uint8_t  sec_lvl;
    /// Service start handle
    /// Only applies for services - Ignored by collectors
    /// 0: dynamically allocated in Attribute database
    uint16_t start_hdl;
    struct hogpd_db_cfg db_cfg;
};




/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


  /*
   * hid device functions
   ****************************************************************************************
   */


 /**
 ****************************************************************************************
 * @brief Add HOGPD profile device  

 * @param[in] p_param                   Pointer to HOGPD profile device add struct
 *
 ****************************************************************************************
 */
 void ln_app_hogpd_add(struct ln_gapm_profile_add_hid *p_param);
  
  /**
 ****************************************************************************************
 * @brief Enable HOGPD profile device  
 *  
 * @note  call this function after connected
 *
 * @param[in] conidx                Connection index
 * @param[in] ntf_cfg           Notifiction configuraton, @see enum hogpd_cfg

 ****************************************************************************************
 */
 void ln_app_hogpd_enable(int conidx,uint16_t ntf_cfg);
  
  /**
 ****************************************************************************************
 * @brief Request sending of a report to the host use notifiction 
 *  
 *
 * @param[in] p_param           Pointer to report request struct        
 *
 ****************************************************************************************
 */
 void ln_app_hogpd_report_upd_req(struct hogpd_report_upd_req *p_param);
  
  /**
 ****************************************************************************************
 * @brief Confirm for peer device for Reading or Updating a report value  
 *  
 *
 * @param[in] p_param           Pointer to report confirm struct
 ****************************************************************************************
 */
 void ln_app_hogpd_report_req_cfm(struct hogpd_report_cfm *p_param);
  
 /**
 ****************************************************************************************
 * @brief Confirm  new protocol mode value  
 *  
 *
 * 
 * @param[in] proto_mode        Pointer to protocol mode struct
 ****************************************************************************************
 */
 void ln_app_hogpd_proto_mode_req_cfm(struct hogpd_proto_mode_cfm *p_param);
   






#endif // _LN_APP_HID_H_



