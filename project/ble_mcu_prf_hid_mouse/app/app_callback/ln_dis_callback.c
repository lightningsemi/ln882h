/**
 ****************************************************************************************
 *
 * @file ln_bas_callback.c
 *
 * @brief  Device Information service callabck funtion source code
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
 * @brief  Device Information service callabck funtion source code
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */



#include "rwip_config.h"             // SW configuration
#include "rwprf_config.h"
#include <string.h>
#include "ke_msg.h"
#include "usr_app.h"

#include "ln_dis_callback.h"
#include "ln_app_dis.h"
#if (TRACE_ENABLE)
#include "utils/debug/log.h"
#endif

extern uint8_t disc_read_char_cle;



/*
 * DEFINES
 ****************************************************************************************
 */



/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#if (BLE_DIS_SERVER)
/*
 * DISS CALLBACK FUNCTIONS
 ****************************************************************************************
 */
int diss_set_value_rsp_handler(ke_msg_id_t const msgid,
                            void const *p_param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
	struct diss_set_value_rsp *param = (struct diss_set_value_rsp *)p_param;
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"diss_set_value_rsp_handler,value:0x%x,status:0x%x\r\n",param->value,param->status);
#endif
    ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}

int diss_value_req_ind_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	struct diss_value_req_ind *param = (struct diss_value_req_ind *)p_param;
	uint8_t conidx = KE_IDX_GET(src_id);    
#if (TRACE_ENABLE)
        LOG(LOG_LVL_INFO,"diss_value_req_ind_handler,value:0x%x\r\n",param->value);
#endif
    // Initialize length
    uint8_t len = 0;
    // Pointer to the data
    uint8_t *data = NULL;
    // Check requested value
    switch (param->value)
    {
        case DIS_MANUFACTURER_NAME_CHAR:
        {
            // Set information
            len = LN_DIS_MANUFACTURER_NAME_LEN;
            data = (uint8_t *)LN_DIS_MANUFACTURER_NAME;
        } break;

        case DIS_MODEL_NB_STR_CHAR:
        {
            // Set information
            len = LN_DIS_MODEL_NB_STR_LEN;
            data = (uint8_t *)LN_DIS_MODEL_NB_STR;
        } break;

        case DIS_SYSTEM_ID_CHAR:
        {
            // Set information
            len = LN_DIS_SYSTEM_ID_LEN;
            data = (uint8_t *)LN_DIS_SYSTEM_ID;
        } break;

        case DIS_PNP_ID_CHAR:
        {
            // Set information
            len = LN_DIS_PNP_ID_LEN;
            data = (uint8_t *)LN_DIS_PNP_ID;
        } break;

        case DIS_SERIAL_NB_STR_CHAR:
        {
            // Set information
            len = LN_DIS_SERIAL_NB_STR_LEN;
            data = (uint8_t *)LN_DIS_SERIAL_NB_STR;
        } break;

        case DIS_HARD_REV_STR_CHAR:
        {
            // Set information
            len = LN_DIS_HARD_REV_STR_LEN;
            data = (uint8_t *)LN_DIS_HARD_REV_STR;
        } break;

        case DIS_FIRM_REV_STR_CHAR:
        {
            // Set information
            len = LN_DIS_FIRM_REV_STR_LEN;
            data = (uint8_t *)LN_DIS_FIRM_REV_STR;
        } break;

        case DIS_SW_REV_STR_CHAR:
        {
            // Set information
            len = LN_DIS_SW_REV_STR_LEN;
            data = (uint8_t *)LN_DIS_SW_REV_STR;
        } break;

        case DIS_IEEE_CHAR:
        {
            // Set information
            len = LN_DIS_IEEE_LEN;
            data = (uint8_t *)LN_DIS_IEEE;
        } break;
        default:
            ASSERT_ERR(0);
            break;
    }
    struct diss_value_cfm *p_cfm = blib_malloc(sizeof(struct diss_value_cfm)  + len);
    // Set parameters
    p_cfm->value = param->value;
    p_cfm->length = len;
    if (len)
    {
        // Copy data
        memcpy(&p_cfm->data[0], data, len);
    }
    // Send message
    ln_app_diss_value_cfm(conidx,p_cfm);
    blib_free(p_cfm);
    return (KE_MSG_CONSUMED);
}

#endif
#if BLE_DIS_CLIENT
/*
 * DISC CALLBACK FUNCTIONS
 ****************************************************************************************
 */
int disc_enable_rsp_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
	struct disc_enable_rsp *param = (struct disc_enable_rsp *)p_param;
	uint8_t conidx = KE_IDX_GET(src_id);    
#if (TRACE_ENABLE)
	LOG(LOG_LVL_INFO,"disc_enable_rsp_handler,status:0x%x\r\n",param->status);
#endif
    struct ln_disc_enable_rsp *data = blib_malloc(sizeof(struct ln_disc_enable_rsp ));
    data->conidx = conidx;
    usr_queue_msg_send(BLE_MSG_DISC_ENABLE,sizeof(struct ln_disc_enable_rsp ),data);
	ke_msg_sync_lock_release();
	return (KE_MSG_CONSUMED);
}


int disc_rd_char_ind_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    //uint8_t conidx = KE_IDX_GET(dest_id);
    
	struct disc_rd_char_ind *param = (struct disc_rd_char_ind *)p_param;	
#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"disc_rd_char_ind_handler handle=0x%x, length=%d, status=%d, value=%s\r\n",
                      param->info.handle,param->info.length, param->info.status,param->info.value);
#endif
	ke_msg_sync_lock_release();
    disc_read_char_cle = 1;
    return (KE_MSG_CONSUMED);
}

 int disc_cmp_event_handler(ke_msg_id_t const msgid,
                                    void const *p_param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    //uint8_t conidx = KE_IDX_GET(dest_id);
    
	struct disc_cmp_evt *param = (struct disc_cmp_evt *)p_param;	
#if (TRACE_ENABLE)
    LOG(LOG_LVL_INFO,"disc_cmp_event_handler status=0x%x, operation=%d\r\n",param->status,param->operation);
#endif

    return (KE_MSG_CONSUMED);
}



#endif    //#if (BLE_DIS_CLIENT)




