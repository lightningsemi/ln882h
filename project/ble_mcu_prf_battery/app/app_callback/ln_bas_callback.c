/**
 ****************************************************************************************
 *
 * @file ln_bas_callback.c
 *
 * @brief  Battery service callabck funtion source code
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
 * @brief  Battery service callabck funtion source code
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
#include "bass_task.h"
#include "basc_task.h"
#include "usr_app.h"
#include "ln_bas_callback.h"
#if (TRACE_ENABLE)
#include "utils/debug/log.h"
#endif
/*
 * DEFINES
 ****************************************************************************************
 */



/*
 * FUNCTION DEFINITIONS
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
                            ke_task_id_t const src_id)


{
	struct bass_enable_rsp *param = (struct bass_enable_rsp *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_TRACE,"bass_enable_rsp_handler,conidx:%d,status:0x%x\r\n",param->conidx,param->status);
#endif	
//struct bass_enable_rsp *data = blib_malloc(sizeof(struct bass_enable_rsp ));
//	data->conidx=param->conidx;
//	usr_queue_msg_send(MSG_BASS_ENABLE,sizeof(struct bass_enable_rsp ),data);
    ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}

int bass_batt_level_upd_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	struct bass_batt_level_upd_rsp *param = (struct bass_batt_level_upd_rsp *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_TRACE,"bass_batt_level_upd_rsp_handler,status:0x%x\r\n",param->status);
#endif
	ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}

int bass_batt_level_ntf_cfg_ind_handler(ke_msg_id_t const msgid,
                                        void const *p_param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
	struct bass_batt_level_ntf_cfg_ind *param = (struct bass_batt_level_ntf_cfg_ind *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_TRACE,"bass_batt_level_ntf_cfg_ind_handler,conidx:%d,ntf_cfg:0x%x\r\n",param->conidx,param->ntf_cfg);
#endif
	return (KE_MSG_CONSUMED);
}
#endif
#if (BLE_BATT_CLIENT)
/*
 * BASC CALLBACK FUNCTIONS
 ****************************************************************************************
 */
 int basc_enable_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	struct basc_enable_rsp *param = (struct basc_enable_rsp *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_TRACE,"basc_enable_rsp_handler,bas_nb:0x%x,status:0x%x\r\n",param->bas_nb,param->status);
#endif
	ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}

int basc_read_info_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	struct basc_read_info_rsp *param = (struct basc_read_info_rsp *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_TRACE,"basc_read_info_rsp_handler,info:0x%x,status:0x%x\r\n",param->info,param->status);
#endif
	ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}


int basc_batt_level_ntf_cfg_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	struct basc_batt_level_ntf_cfg_rsp *param = (struct basc_batt_level_ntf_cfg_rsp *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_TRACE,"basc_batt_level_ntf_cfg_rsp_handler,bas_nb:0x%x,status:0x%x\r\n",param->bas_nb,param->status);
#endif
	ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}

int basc_batt_level_ind_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	struct basc_batt_level_ind *param = (struct basc_batt_level_ind *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_TRACE,"basc_batt_level_ind_handler,bas_nb:0x%x,batt_level:0x%x\r\n",param->bas_nb,param->batt_level);
#endif
	return (KE_MSG_CONSUMED);
}


#endif    



