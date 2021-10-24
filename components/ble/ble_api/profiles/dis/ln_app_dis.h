/**
 ****************************************************************************************
 *
 * @file ln_app_dis.h
 *
 * @brief Device Information service APIs.
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_APP_DIS_H_
#define _LN_APP_DIS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @group BLE
 *
 * @brief Device Information service APIs.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "disc_task.h"               // Device Information Profile Functions
#include "diss_task.h"               // Device Information Profile Functions



/*
 * DEFINES
 ****************************************************************************************
 */




/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// Create new task for DIS service
/*@TRACE*/
struct ln_gapm_profile_add_diss
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
    /// Database configuration @see enum diss_features_bf
    uint16_t features;
};


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Add Device Information Service Server  

 * @param[in] p_cmd                 Pointer to Device Information Service add struct
 *
 ****************************************************************************************
 */
 void ln_app_diss_add(struct ln_gapm_profile_add_diss *p_cmd);

/**
 ****************************************************************************************
 * @brief Set the value of an attribute - Request
 *  
 *
 * @param[in] conidx                connect index
 
 * @param[in] p_req             Pointer to Set the value of an attribute struct
 ****************************************************************************************
 */
 void ln_app_diss_set_value_req(uint8_t conidx, struct diss_set_value_req *p_req);  


/**
 ****************************************************************************************
 * @brief Peer device confirm value of requested attribute
 *  
 *
 * @param[in] conidx                Connection index
 *
 * @param[in] p_cfm                 Pointer to device  value of requested attribute confirm struct
 ****************************************************************************************
 */
 void ln_app_diss_value_cfm(uint8_t conidx, struct diss_value_cfm *p_cfm);   

/**
 ****************************************************************************************
 * @brief Add Device Information Service Client
 
 * @param[in] p_cmd                 Pointer to Device Information Service add struct 
                                    don't fill in features
 *
 ****************************************************************************************
 */
 void ln_app_disc_add(struct ln_gapm_profile_add_diss *p_cmd);

/**
 ****************************************************************************************
 * @brief Enable Device Information service client 
 *  
 * @note  call this function after connected
 *
 * @param[in] conidx                Connection index
 * @param[in] p_req         Pointer to battery profile enable struct 

 ****************************************************************************************
 */
 void ln_app_disc_enable_prf(uint8_t conidx,struct disc_enable_req * p_req);


/**
 ****************************************************************************************
 * @brief Generic message to read a Device Information service characteristic value
 *  
 *
 * @param[in] conidx                Connection index
 * @param[in] p_req             Pointer to  read characteristic value  struct 
 * 
 ****************************************************************************************
 */
 void ln_app_disc_rd_char_req(uint8_t conidx, struct disc_rd_char_cmd * p_req);

#endif // _LN_APP_BAS_H_



