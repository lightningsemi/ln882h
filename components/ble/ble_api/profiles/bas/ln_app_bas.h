/**
 ****************************************************************************************
 *
 * @file ln_app_bas.h
 *
 * @brief Battery service APIs.
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_APP_BAS_H_
#define _LN_APP_BAS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @group BLE
 *
 * @brief Battery service APIs.
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */



#include "bass_task.h"
#include "basc_task.h"







/*
 * DEFINES
 ****************************************************************************************
 */




/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/// Create new task for battery profile
/*@TRACE*/
struct ln_gapm_profile_add_bas
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
    /// Number of BAS to add
    uint8_t bas_nb;
    /// Features of each BAS instance
    uint8_t features[BASS_NB_BAS_INSTANCES_MAX];
    /// Battery Level Characteristic Presentation Format - Should not change during connection
    struct prf_char_pres_fmt batt_level_pres_format[BASS_NB_BAS_INSTANCES_MAX];
};







/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Add Battery Service Server  

 * @param[in] p_cmd                 Pointer to battery serice add struct
 *
 ****************************************************************************************
 */
void ln_app_bass_add(struct ln_gapm_profile_add_bas *p_cmd);

/**
 ****************************************************************************************
 * @brief Enable battery service server  
 *  
 * @note call this funciton after connection 
 *
 * @param[in] p_cmd                 Pointer to battery serice enable struct
 ****************************************************************************************
 */
void ln_app_bass_enable_prf(struct bass_enable_req * p_req);


/**
 ****************************************************************************************
 * @brief Update battery level request 
 *  
 *
 * @param[in] conidx                Connection index
 *
 * @param[in] p_cmd                 Pointer to battery serice update level update struct
 ****************************************************************************************
 */
void ln_app_bass_send_lvl(uint8_t conidx,struct bass_batt_level_upd_req *p_req);

/**
 ****************************************************************************************
 * @brief Add Battery Service Client
 
 * @param[in] p_cmd                 Pointer to battery profile add struct 
                                    don't fill in bas_nb,features,batt_level_pres_format
 *
 ****************************************************************************************
 */
void ln_app_basc_add(struct ln_gapm_profile_add_bas *p_cmd);

/**
 ****************************************************************************************
 * @brief Enable battery service client 
 *  
 * @note  call this function after connected
 *
 * @param[in] conidx                Connection index
 * @param[in] p_req         Pointer to battery profile enable struct 

 ****************************************************************************************
 */
void ln_app_basc_enable_prf(uint8_t conidx,struct basc_enable_req * p_req);


/**
 ****************************************************************************************
 * @brief Configure notification 
 *  
 *
 * @param[in] conidx                Connection index
 * @param[in] p_req             Pointer to  Configure notification  struct 
 * 
 ****************************************************************************************
 */
void ln_app_basc_send_cfg(uint8_t conidx,struct basc_batt_level_ntf_cfg_req *p_req);


/**
 ****************************************************************************************
 * @brief Read peer device info 
 *  
 *
 * @param[in] conidx                Connection index
 * @param[in] p_req             Pointer to Read peer device info  struct 
 * 
 ****************************************************************************************
 */
void ln_app_basc_rd_char_req(uint8_t conidx, struct basc_read_info_req * p_req);


#endif // _LN_APP_BAS_H_



