/**
 ****************************************************************************************
 *
 * @file ln_app_bas.c
 *
 * @brief  Battery service APIs source code
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
 * @brief Battery service APIs source code
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
#include "ln_app_bas.h"
#include "ln_def.h"
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Battery Application Module Environment Structure


struct gapm_profile_added_ind  bass_ind = {0};
struct gapm_profile_added_ind  basc_ind = {0};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

//battery servicer
// BASS API
void ln_app_bass_add(struct ln_gapm_profile_add_bas *p_cmd)
{
    struct bass_db_cfg* db_cfg;

    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_profile_task_add_cmd, sizeof(struct bass_db_cfg));
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl    = p_cmd->sec_lvl ;//PERM(SVC_AUTH, UNAUTH);
    req->prf_task_id = TASK_ID_BASS;
    req->app_task    = TASK_APP;
    req->start_hdl   = p_cmd->start_hdl;

    // Set parameters
    db_cfg = (struct bass_db_cfg* ) req->param;
    // Add a BAS instance
    db_cfg->bas_nb      =p_cmd->bas_nb;
    for(int i=0; i<p_cmd->bas_nb; i++)
    {
        db_cfg->features[i]=p_cmd->features[i];
        memcpy(&db_cfg->batt_level_pres_format[i],&p_cmd->batt_level_pres_format[i],sizeof(struct prf_char_pres_fmt));
    }
    // Send the message
    ln_ke_msg_send(req);
}

void ln_app_bass_enable_prf(struct bass_enable_req * p_req)
{
    // Allocate the message
    struct bass_enable_req * req = KE_MSG_ALLOC(BASS_ENABLE_REQ,
                                   KE_BUILD_ID(bass_ind.prf_task_nb, p_req->conidx),
                                   TASK_APP,
                                   bass_enable_req);

    // Fill in the parameter structure
    req->conidx             = p_req->conidx;
    // NTF initial status - Disabled
    req->ntf_cfg           = p_req->ntf_cfg;
    for(int i=0; i<BASS_NB_BAS_INSTANCES_MAX; i++)
    {
        req->old_batt_lvl[i]=p_req->old_batt_lvl[i];
    }
    // Send the message
    ln_ke_msg_send(req);
}



void ln_app_bass_send_lvl(uint8_t conidx,struct bass_batt_level_upd_req *p_req)
{
    ASSERT_ERR(p_req->batt_level <= BAS_BATTERY_LVL_MAX);
    // Allocate the message
    struct bass_batt_level_upd_req * req = KE_MSG_ALLOC(BASS_BATT_LEVEL_UPD_REQ,
                                           KE_BUILD_ID(bass_ind.prf_task_nb, conidx),
                                           TASK_APP,
                                           bass_batt_level_upd_req);
    // Fill in the parameter structure
    req->bas_instance = p_req->bas_instance;
    req->batt_level   = p_req->batt_level;
    // Send the message
    ln_ke_msg_send(req);
}


//battery client
void ln_app_basc_add(struct ln_gapm_profile_add_bas *p_cmd)
{
    // Allocate the BASS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                            TASK_GAPM, TASK_APP,
                                            gapm_profile_task_add_cmd, 0);
    // Fill message
    req->operation   = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl     = p_cmd->sec_lvl;//PERM(SVC_AUTH, UNAUTH);
    req->prf_task_id = TASK_ID_BASC;
    req->app_task    = TASK_APP;
    req->start_hdl   = p_cmd->start_hdl;

    // Send the message
    ln_ke_msg_send(req);
}

void ln_app_basc_enable_prf(uint8_t conidx,struct basc_enable_req * p_req )
{
    // Allocate the message
    struct basc_enable_req * req = KE_MSG_ALLOC(BASC_ENABLE_REQ,
                                   KE_BUILD_ID(basc_ind.prf_task_nb, conidx),
                                   TASK_APP,
                                   basc_enable_req);


    // Fill in the parameter structure
    req->con_type = p_req->con_type;
    req->bas_nb = p_req->bas_nb;
    for(int i=0; i<p_req->bas_nb; i++)
    {
        memcpy(&req->bas[i],&p_req->bas[i],sizeof(struct bas_content));
    }
    // Send the message
    ln_ke_msg_send(req);
}


void ln_app_basc_send_cfg(uint8_t conidx,struct basc_batt_level_ntf_cfg_req *p_req)
{
    // Allocate the message
    struct basc_batt_level_ntf_cfg_req * req = KE_MSG_ALLOC(BASC_BATT_LEVEL_NTF_CFG_REQ,
            KE_BUILD_ID(basc_ind.prf_task_nb, conidx),
            TASK_APP,
            basc_batt_level_ntf_cfg_req);

    // Fill in the parameter structure, see @enum prf_cli_conf
    req->ntf_cfg =p_req->ntf_cfg;
    req->bas_nb=p_req->bas_nb;
    // Send the message
    ln_ke_msg_send(req);
}

void ln_app_basc_rd_char_req(uint8_t conidx, struct basc_read_info_req * p_req)
{
    // Allocate the message
    struct basc_read_info_req * req = KE_MSG_ALLOC(BASC_READ_INFO_REQ,
                                      KE_BUILD_ID(basc_ind.prf_task_nb, conidx),
                                      TASK_APP,
                                      basc_read_info_req);


    // Fill in the parameter structure
    req->info= p_req->info;
    req->bas_nb = p_req->bas_nb;

    // Send the message
    ln_ke_msg_send(req);
}
  




