/**
 ****************************************************************************************
 *
 * @file ln_gatt_callback.h
 *
 * @brief GATT callback function.
 *
 *Copyright (C) 2021.Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */

#ifndef _LN_GATT_CALLBACK_H_
#define _LN_GATT_CALLBACK_H_

#include <stdint.h>
#include <stdbool.h>
#include "gattc_task.h"
#include "gattm_task.h"
#include "att.h"
#include "rwip_task.h" // Task definitions
#include "ke_msg.h"


int gattm_add_svc_rsp_handler(ke_msg_id_t const msgid,
        void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_cmp_evt_handler(ke_msg_id_t const msgid,
        void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_mtu_changed_ind_handler(ke_msg_id_t const msgid,
        void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_read_req_ind_handler(ke_msg_id_t const msgid,
        void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_write_req_ind_handler(ke_msg_id_t const msgid,
        void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
int gattc_transaction_to_error_ind_handler(ke_msg_id_t const msgid,
        void const *p_param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

#endif // _LN_GATT_CALLBACK_H_
