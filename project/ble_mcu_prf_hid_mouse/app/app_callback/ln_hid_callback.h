/**
 ****************************************************************************************
 *
 * @file ln_hid_callback.h
 *
 * @brief  Human Interface Device (HID) Profile callabck funtion 
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_HID_CALLBACK_H_
#define _LN_HID_CALLBACK_H_
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

#include "rwprf_config.h"





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

 //HOGPD CALLBACK FUNCTIONS
#if (BLE_HID_DEVICE)
int hogpd_enable_rsp_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);



 int hogpd_report_upd_rsp_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_report_req_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_hid_info_req_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_ctnl_pt_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_audio_state_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_cmp_evt_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_connect_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_disconnect_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);


int hogpd_dfu_img_id_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);



int hogpd_proto_mode_req_ind_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id);





#endif


    


#endif //LN_HID_CALLBACK_H_



