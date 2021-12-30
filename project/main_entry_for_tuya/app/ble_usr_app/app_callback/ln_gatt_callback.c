/**
 ****************************************************************************************
 *
 * @file ln_gatt_callback.c
 *
 * @brief GATT callback funcion source code
 *
 *Copyright (C) 2021.Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */
#include "gattm_task.h"               // GATT Manager Task API
#include "gattc_task.h"               // GATT Controller Task API
#include "tuya_os_adapter.h"


#define TRACE_ENABLE    (1)

#if (TRACE_ENABLE)
#include "utils/debug/log.h"
#endif


extern uint16_t svc_data_transfer_hdl; // Used in tuya bt adapter

int gattm_add_svc_rsp_handler(ke_msg_id_t const msgid,
                              void const *p_param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    struct gattm_add_svc_rsp *param = (struct gattm_add_svc_rsp *)p_param;
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gattm_add_svc_rsp_handler ,start_handler:0x%x,status:0x%x\r\n",param->start_hdl,param->status);
#endif
    ke_msg_sync_lock_release();

    svc_data_transfer_hdl = param->start_hdl;
    return (KE_MSG_CONSUMED);
}

int gattc_cmp_evt_handler(ke_msg_id_t const msgid,
                          void const *p_param,
                          ke_task_id_t const dest_id,
                          ke_task_id_t const src_id)
{
    struct gattc_cmp_evt *param = (struct gattc_cmp_evt *)p_param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gattc_cmp_evt_handler   conidx:0x%x,status:0x%x,operaton:0x%x\r\n",conidx,param->status,param->operation);
#endif
    ke_msg_sync_lock_release();
    return (KE_MSG_CONSUMED);
}

int gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid,
                                  void const *p_param,
                                  ke_task_id_t const dest_id,
                                  ke_task_id_t const src_id)
{
    struct gattc_mtu_changed_ind *param = (struct gattc_mtu_changed_ind *)p_param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gattc_mtu_changed_ind_handler  mtu:%d conidx:0x%x\r\n",param->mtu,conidx);
#endif
    return (KE_MSG_CONSUMED);
}

int gattc_read_req_ind_handler(ke_msg_id_t const msgid,
                               void const *p_param,
                               ke_task_id_t const dest_id,
                               ke_task_id_t const src_id)
{
    struct gattc_read_req_ind const *param = (struct gattc_read_req_ind const *)p_param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gattc_read_req_ind_handler   conidx:0x%x,handler:0x%x\r\n",conidx,param->handle);
#endif
    struct gattc_read_cfm *p_cfm=KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, 2);
    p_cfm->handle = param->handle;
    p_cfm->status =GAP_ERR_NO_ERROR;
    p_cfm->length = 1;
    p_cfm->value[0]=0;
    ke_msg_send(p_cfm);
    return (KE_MSG_CONSUMED);
}


int gattc_write_req_ind_handler(ke_msg_id_t const msgid,
                                void const *p_param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{

    struct gattc_write_req_ind const *param = (struct gattc_write_req_ind const *)p_param;
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gattc_write_req_ind_handler   conidx:0x%x,handler:0x%x,length:0x%x\r\n",conidx,param->handle,param->length);
#endif
    struct gattc_write_cfm *  p_cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, src_id, dest_id, gattc_write_cfm);
    p_cfm->handle = param->handle;
    p_cfm->status = GAP_ERR_NO_ERROR;
    ke_msg_send(p_cfm);

    hexdump(LOG_LVL_INFO, "[recv data]", (void *)param->value, param->length);

    {
        tuya_ble_data_buf_t data;
        data.data = param->value;
        data.len = param->length;

        extern TY_BT_MSG_CB ty_bt_msg_cb;
        if(ty_bt_msg_cb!=NULL) {
            ty_bt_msg_cb(0, TY_BT_EVENT_RX_DATA, &data);
        }
    }

    return (KE_MSG_CONSUMED);
}

int gattc_transaction_to_error_ind_handler(ke_msg_id_t const msgid,
        void const *p_param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    uint8_t conidx = KE_IDX_GET(src_id);
#if (TRACE_ENABLE)
    LOG(LOG_LVL_TRACE,"gattc_transaction_to_error_ind_handler   conidx:0x%x\r\n",conidx);
#endif
    return (KE_MSG_CONSUMED);
}
