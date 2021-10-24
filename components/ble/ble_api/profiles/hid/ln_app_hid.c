/**
 ****************************************************************************************
 *
 * @file ln_app_hid.c
 *
 * @brief Human Interface Device (HID) Profile  APIs source code
 *
 *Copyright (C) 2021.Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup BLE
 *
 * @brief Human Interface Device (HID) Profile APIs source code
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"             // SW configuration
#include <string.h>
#include "ke_msg.h"
#include "gapm_task.h"
#include "ln_def.h"
#include "prf_types.h"               // Profile Common Types Definitions
#include "ke_task.h"                 // Kernel
#include "ln_app_hid.h"
/*
 * DEFINES
 ****************************************************************************************
 */




/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


struct gapm_profile_added_ind  hogpd_ind;



/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

 //HOGPD FUNCTIONS

 /*
  * hid device functions
  ****************************************************************************************
  */
 void ln_app_hogpd_add(struct ln_gapm_profile_add_hid *p_param)
 {
    struct hogpd_db_cfg *db_cfg;
     struct gapm_profile_task_add_cmd *p_cmd = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                         TASK_GAPM, TASK_APP,
                                                         gapm_profile_task_add_cmd, sizeof(struct hogpd_db_cfg));
 
     p_cmd->operation = GAPM_PROFILE_TASK_ADD;
     p_cmd->sec_lvl = p_param->sec_lvl;
     p_cmd->prf_task_id = TASK_ID_HOGPD;
     p_cmd->app_task = TASK_APP;
     p_cmd->start_hdl = p_param->start_hdl;
      db_cfg = (struct hogpd_db_cfg* ) p_cmd->param;
     db_cfg->hids_nb = p_param->db_cfg.hids_nb;
 
     for (int i = 0; i < db_cfg->hids_nb; i++) {
         db_cfg->cfg[i].svc_features = p_param->db_cfg.cfg[i].svc_features;
         db_cfg->cfg[i].report_nb = p_param->db_cfg.cfg[i].report_nb;
         for (int j = 0; j < HOGPD_NB_REPORT_INST_MAX; j++) {
             db_cfg->cfg[i].report_char_cfg[j] = p_param->db_cfg.cfg[i].report_char_cfg[j];
             db_cfg->cfg[i].report_id[j] = p_param->db_cfg.cfg[i].report_id[j];
         }                                                                             
         memcpy(&db_cfg->cfg[i].hid_info, &p_param->db_cfg.cfg[i].hid_info, sizeof(struct hids_hid_info));
         memcpy(&db_cfg->cfg[i].ext_ref, &p_param->db_cfg.cfg[i].ext_ref, sizeof(struct hogpd_ext_ref));  
     }
    // Send the message
    ln_ke_msg_send(p_cmd);
 }
 
 void ln_app_hogpd_enable(int conidx,uint16_t ntf_cfg)
 {
     struct hogpd_enable_req *p_cmd = KE_MSG_ALLOC(HOGPD_ENABLE_REQ,
                                                   KE_BUILD_ID(hogpd_ind.prf_task_nb,conidx),
                                                   TASK_APP,
                                                    hogpd_enable_req);
     p_cmd->conidx = conidx;
     p_cmd->ntf_cfg[conidx] = ntf_cfg;
     // Send the message
     ln_ke_msg_send(p_cmd);
 }
 
 void ln_app_hogpd_report_upd_req(struct hogpd_report_upd_req *p_param)
 {
    
 
     struct hogpd_report_upd_req *p_cmd = KE_MSG_ALLOC_DYN(HOGPD_REPORT_UPD_REQ,
                                                           KE_BUILD_ID(hogpd_ind.prf_task_nb,p_param->conidx),
                                                            TASK_APP,
                                                            hogpd_report_upd_req, p_param->report.length);
     p_cmd->conidx = p_param->conidx;
     memcpy(&p_cmd->report, &p_param->report, (sizeof(struct hogpd_report_info) + p_param->report.length));
     // Send the message
     ln_ke_msg_send(p_cmd);
 }
 
 void ln_app_hogpd_report_req_cfm(struct hogpd_report_cfm *p_param)
 {
     struct hogpd_report_cfm *p_cmd = KE_MSG_ALLOC_DYN(HOGPD_REPORT_CFM,
                                                           KE_BUILD_ID(hogpd_ind.prf_task_nb,p_param->conidx),
                                                           TASK_APP,
                                                             hogpd_report_cfm,p_param->report.length);
     p_cmd->conidx = p_param->conidx;
     p_cmd->operation = p_param->operation;
     p_cmd->status = p_param->status;
     memcpy(&p_cmd->report, &p_param->report, (sizeof(struct hogpd_report_info) + p_param->report.length));
     // Send the message
     ke_msg_send(p_cmd);
 }

 void ln_app_hogpd_proto_mode_req_cfm(struct hogpd_proto_mode_cfm *p_param)
  {
      struct hogpd_proto_mode_cfm *p_cmd = KE_MSG_ALLOC(HOGPD_REPORT_CFM,
                                                        KE_BUILD_ID(hogpd_ind.prf_task_nb,p_param->conidx),
                                                        TASK_APP,
                                                        hogpd_proto_mode_cfm);
    p_cmd->conidx = p_param->conidx;
    p_cmd->status = p_param->status;
    p_cmd->hid_idx = p_param->hid_idx;
    p_cmd->proto_mode = p_param->proto_mode;
    // Send the message
    ke_msg_send(p_cmd);
  }


 
 #if 0
 void ln_app_hogpd_proto_mode_req_cfm(int conidx,)
 {
     struct hogpd_proto_mode_cfm *p_cmd = KE_MSG_ALLOC(HOGPD_PROTO_MODE_CFM,
                                                       KE_BUILD_ID(hogpd_ind.prf_task_nb,conidx),
                                                       TASK_APP,
                                                       hogpd_proto_mode_cfm);
 
     p_cmd->conidx = conidx;
     p_cmd->status = status;
     p_cmd->hid_idx = hid_idx;
     p_cmd->proto_mode = proto_mode;
       // Send the message
     ln_ke_msg_send(p_cmd);
 }
#endif







