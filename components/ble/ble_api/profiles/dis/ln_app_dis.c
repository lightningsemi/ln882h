/**
 ****************************************************************************************
 *
 * @file ln_app_dis.c
 *
 * @brief Device Information service APIs source code
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
 * @brief Device Information service APIs source code
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
#include "ln_app_dis.h"
#include "prf.h"
/*
 * DEFINES
 ****************************************************************************************
 */




/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

struct gapm_profile_added_ind  diss_ind = {0};
struct gapm_profile_added_ind  disc_ind = {0};


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */


//Device Information servicer
// DISS API
void ln_app_diss_add(struct ln_gapm_profile_add_diss *p_cmd)
{
    struct diss_db_cfg* db_cfg;
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_profile_task_add_cmd, sizeof(struct diss_db_cfg));
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl    = p_cmd->sec_lvl ;//PERM(SVC_AUTH, UNAUTH);
    req->prf_task_id = TASK_ID_DISS;
    req->app_task    = TASK_APP;
    req->start_hdl   = p_cmd->start_hdl;

    // Set parameters
    db_cfg = (struct diss_db_cfg* ) req->param;
    // Add a BAS instance
    db_cfg->features = p_cmd->features;
    // Send the message
    ln_ke_msg_send(req);
}

void ln_app_diss_set_value_req(uint8_t conidx, struct diss_set_value_req *p_req)    
{
    struct diss_set_value_req *p_cmd = KE_MSG_ALLOC_DYN(DISS_SET_VALUE_REQ,
                                     KE_BUILD_ID(diss_ind.prf_task_nb, conidx),
                                     TASK_APP,
                                     diss_set_value_req, p_req->length);
    // Fill in the parameter structure
    p_cmd->value = p_req->value;
    p_cmd->length = p_req->length;
    memcpy(&p_cmd->data, &p_req->data, p_req->length); 
    // Send the message
    ln_ke_msg_send(p_cmd);
}

void ln_app_diss_value_cfm(uint8_t conidx, struct diss_value_cfm *p_cfm)    
{
    struct diss_value_cfm *p_cmd = KE_MSG_ALLOC_DYN(DISS_VALUE_CFM,
                                 KE_BUILD_ID(diss_ind.prf_task_nb, conidx),
                                 TASK_APP,
                                 diss_value_cfm, p_cfm->length);
    // Fill in the parameter structure
    p_cmd->value = p_cfm->value;
    p_cmd->length = p_cfm->length;
    memcpy(&p_cmd->data, &p_cfm->data, p_cfm->length); 
    // Send the message
    ke_msg_send(p_cmd);
}

//Device Information client 
//DISC API
void ln_app_disc_add(struct ln_gapm_profile_add_diss *p_cmd)
{
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC(GAPM_PROFILE_TASK_ADD_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_profile_task_add_cmd);
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl    = p_cmd->sec_lvl ;//PERM(SVC_AUTH, UNAUTH);
    req->prf_task_id = TASK_ID_DISC;
    req->app_task    = TASK_APP;
    req->start_hdl   = p_cmd->start_hdl;
    // Send the message
    ln_ke_msg_send(req);
}

void ln_app_disc_enable_prf(uint8_t conidx,struct disc_enable_req * p_req )
{
    // Allocate the message
    struct disc_enable_req * req = KE_MSG_ALLOC(DISC_ENABLE_REQ,
                                   KE_BUILD_ID(disc_ind.prf_task_nb, conidx),
                                   TASK_APP,
                                   disc_enable_req);
    // Fill in the parameter structure
    req->con_type = p_req->con_type;
    memcpy(&req->dis,&p_req->dis,sizeof(struct disc_dis_content));
    // Send the message
    ln_ke_msg_send(req);
}

void ln_app_disc_rd_char_req(uint8_t conidx, struct disc_rd_char_cmd * p_req)
{
    // Allocate the message
    struct disc_rd_char_cmd * req = KE_MSG_ALLOC(DISC_RD_CHAR_CMD,
                                                prf_get_task_from_id(KE_BUILD_ID(TASK_ID_DISC, conidx)),
                                                TASK_APP,
                                                disc_rd_char_cmd);

    // Fill in the parameter structure
    req->char_code = p_req->char_code;//DISC_MANUFACTURER_NAME_CHAR;
    // Send the message
    ln_ke_msg_send(req);
}

 




