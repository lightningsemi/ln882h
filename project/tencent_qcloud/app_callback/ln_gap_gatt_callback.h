#ifndef _LN_GAP_GATT_CALLBACK_H_
#define _LN_GAP_GATT_CALLBACK_H_

#include <stdint.h>
#include "ke_msg.h"

/* gap callback */
int gapm_activity_created_ind_callback   (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapm_activity_stopped_ind_callback   (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapm_ext_adv_report_ind_callback     (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapm_scan_request_ind_callback       (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapm_profile_added_ind_callback      (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapm_cmp_evt_callback                (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapm_dev_version_ind_callback        (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapm_dev_bdaddr_ind_callback         (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_get_dev_info_req_ind_callback   (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_set_dev_info_req_ind_callback   (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_connection_req_ind_callback     (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_param_update_req_ind_callback   (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_param_updated_ind_callback      (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_cmp_evt_callback                (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_disconnect_ind_callback         (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_con_rssi_ind_callback           (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gapc_le_pkt_size_ind_callback        (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

/* gatt callback */
int gattm_add_svc_rsp_callback              (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattm_unknown_msg_ind_callback          (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_cmp_evt_callback                  (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_mtu_changed_ind_callback          (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_read_ind_callback                 (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_read_req_ind_callback             (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_write_req_ind_callback            (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_transaction_to_error_ind_callback (ke_msg_id_t const msgid, void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);


#endif // _LN_GAP_GATT_CALLBACK_H_
