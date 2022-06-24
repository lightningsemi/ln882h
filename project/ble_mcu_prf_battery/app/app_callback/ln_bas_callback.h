/**
 ****************************************************************************************
 *
 * @file ln_bas_callback.h
 *
 * @brief  Battery service callabck funtion 
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_BAS_CALLBACK_H_
#define _LN_BAS_CALLBACK_H_

#include "rwprf_config.h"
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






/*
 * DEFINES
 ****************************************************************************************
 */




/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */






/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


#if (BLE_BATT_SERVER)
/*
 * BASS CALLBACK FUNCTIONS
 ****************************************************************************************
 */
int bass_enable_rsp_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);




int bass_batt_level_upd_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);


int bass_batt_level_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                        void const *p_param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id);

#endif
#if (BLE_BATT_CLIENT)

/*
 * BASC CALLBACK FUNCTIONS
 ****************************************************************************************
 */
int basc_enable_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);


int basc_read_info_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);



int basc_batt_level_ntf_cfg_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);


int basc_batt_level_ind_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id);


#endif    //#if (BLE_BATT_CLIENT)
#endif //LN_BAS_CALLBACK_H_



