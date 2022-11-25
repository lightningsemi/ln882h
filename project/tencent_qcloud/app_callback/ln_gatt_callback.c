#include <string.h>
#include "gattm_task.h"               // GATT Manager Task API
#include "gattc_task.h"               // GATT Controller Task API

//#include "usr_ble_app.h"

#include "ln_app_gatt.h"
#include "ln_gap_gatt_callback.h"

#include "utils/debug/log.h"
#include "ble_qiot_export.h"

extern void    _char_svc_handle_set(uint8_t start_hdl);
extern uint8_t _is_char_cccd_handle(uint8_t hdl);
extern uint8_t _is_char_write_handle(uint8_t hdl);
extern uint8_t _char_notify_handle_get(void);
extern uint8_t _char_write_handle_get(void);
extern void    _char_cccd_enable_set(uint8_t en);


///////////////////////////////////////////////
int gattm_add_svc_rsp_callback(ke_msg_id_t const msgid, void const *p_param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattm_add_svc_rsp *param = (struct gattm_add_svc_rsp *)p_param;
    
    _char_svc_handle_set(param->start_hdl);

    return (KE_MSG_CONSUMED);
}

int gattm_unknown_msg_ind_callback(ke_msg_id_t const msgid, void const *p_param,
                                   ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattm_unknown_msg_ind *param = (struct gattm_unknown_msg_ind *)p_param;

    return (KE_MSG_CONSUMED);
}

int gattc_cmp_evt_callback(ke_msg_id_t const msgid, void const *p_param,
                           ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_cmp_evt *param = (struct gattc_cmp_evt *)p_param;

    return (KE_MSG_CONSUMED);
}

int gattc_mtu_changed_ind_callback(ke_msg_id_t const msgid, void const *p_param,
                                   ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_mtu_changed_ind *param = (struct gattc_mtu_changed_ind *)p_param;
    uint8_t conidx = KE_IDX_GET(src_id);

    extern uint16_t llsync_mtu;
    llsync_mtu = param->mtu;

    ble_event_sync_mtu(llsync_mtu);

    return (KE_MSG_CONSUMED);
}

//clint send read command ,receive Read responsethen, receive read data
int gattc_read_ind_callback(ke_msg_id_t const msgid, void const *p_param,
                            ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_read_ind *param = (struct gattc_read_ind *)p_param;

    return (KE_MSG_CONSUMED);
}

//service receive this msg,the then send cfm reponse the data 
int gattc_read_req_ind_callback(ke_msg_id_t const msgid, void const *p_param,
                                ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_read_req_ind const *param = (struct gattc_read_req_ind const *)p_param;
    uint8_t conidx = KE_IDX_GET(src_id);
    LOG(LOG_LVL_INFO,"[%s] conidx:0x%x, handler:0x%x\r\n", __func__, conidx,param->handle);
    struct gattc_read_cfm *p_cfm=KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, 2);
    p_cfm->handle = param->handle;
    p_cfm->status = GAP_ERR_NO_ERROR;
    p_cfm->length = 1;
    p_cfm->value[0] = 0;
    ke_msg_send(p_cfm);
    return (KE_MSG_CONSUMED);
}

int gattc_write_req_ind_callback(ke_msg_id_t const msgid, void const *p_param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct gattc_write_req_ind const *param = (struct gattc_write_req_ind const *)p_param;
    uint8_t conidx = KE_IDX_GET(src_id);

    struct gattc_write_cfm *  p_cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, src_id, dest_id, gattc_write_cfm);
    p_cfm->handle = param->handle;
    p_cfm->status = GAP_ERR_NO_ERROR;
    ke_msg_send(p_cfm);
    
    hexdump(LOG_LVL_INFO, "[recv data]", (void *)param->value, param->length);

    if (_is_char_write_handle(param->handle)) {
        ble_device_info_write_cb((const uint8_t *)param->value, param->length);
    } else if (_is_char_cccd_handle(param->handle)) {
        _char_cccd_enable_set(param->handle);
    } else {
        LOG(LOG_LVL_INFO,"%s | unkown handler\r\n", __FUNCTION__);
    }
    
    return (KE_MSG_CONSUMED);
}

int gattc_transaction_to_error_ind_callback(ke_msg_id_t const msgid, void const *p_param,
                                            ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    return (KE_MSG_CONSUMED);
}


