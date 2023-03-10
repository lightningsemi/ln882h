#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "rwip_config.h"
#include "llm_int.h"
#include "att.h"
#include "usr_send_data.h"
#include "ln_app_gatt.h"
#include "usr_ble_app.h"
#include "attm.h"

typedef struct {
    uint16_t hdl_svc;
    int conid;
    int (*rx_callback)(int, uint8_t*, int);
    uint16_t mtu;
    uint16_t value_ccc;
} data_trans_svr_t;

data_trans_svr_t  data_trans_svr = {0};

static const data_trans_attm_desc  data_trans_atts[] = {
    [DATA_TRANS_DECL_SVC] = {
        .uuid = UUID_16_DECLARATION(0x2800),
        .perm = PERM_MASK_RD,
        .ext_perm = (2 <<  PERM_POS_UUID_LEN),
        .max_size = 0,
    },
    [DATA_TRANS_DECL_CHAR_RX] = {
        .uuid = UUID_16_DECLARATION(0x2803),
        .perm = PERM_MASK_RD,
        .ext_perm = (0 <<  PERM_POS_UUID_LEN),
        .max_size = 0,
    },
    [DATA_TRANS_CHAR_VAL_RX] = {
        .uuid = DATA_TRANS_RX_UUID, 
        .perm = PERM_MASK_WRITE_REQ|PERM_MASK_WRITE_COMMAND|PERM_MASK_RD,
        .ext_perm = (2 <<PERM_POS_UUID_LEN) | (1<<PERM_POS_RI),
        .max_size = DATA_TRANS_MAX_LEN,
    },
    [DATA_TRANS_DECL_CHAR_TX] = {
        .uuid = UUID_16_DECLARATION(0x2803),
        .perm = PERM_MASK_RD,
        .ext_perm =  (0 << PERM_POS_UUID_LEN),
        .max_size = 0,
    },
    [DATA_TRANS_CHAR_VAL_TX] = {
        .uuid = DATA_TRANS_TX_UUID,
        .perm = PERM_MASK_NTF|PERM_MASK_RD,
        .ext_perm =  (2 << PERM_POS_UUID_LEN) | (1<<PERM_POS_RI),
        .max_size = DATA_TRANS_MAX_LEN,
    },
    [DATA_TRANS_CLIENT_CHAR_CFG_TX] = {
        .uuid = UUID_16_DECLARATION(0x2902), //INB_ATT_DESC_SERVER_CHAR_CFG,
        .perm = PERM_MASK_WRITE_REQ|PERM_MASK_RD,
        .ext_perm = 0,
        .max_size = 0,
    },
};

void data_trans_svc_add(void)
{
    uint16_t start_handle = HANDLE_DATA_TRANS_SERVER_START;
    uint32_t cfg_flag = DATA_TRANS_CONFIG_MASK;
    uint8_t status = ATT_ERR_NO_ERROR;

#ifdef ENABLE_DATA_TRANS_USE_UUID_16_LEN
    uint16_t data_trans_svc_uuid = DATA_TRANS_SVR_UUID;

    status = attm_svc_create_db(&start_handle, data_trans_svc_uuid, (uint8_t *)&cfg_flag,
        DATA_TRANS_IDX_MAX, NULL, TASK_APP, &data_trans_atts[0],
        (SVC_PREM_TYPE_UUID16_MASK<<5));
#else
    uint8_t data_trans_svc_uuid[ATT_UUID_128_LEN]  = DATA_TRANS_SVR_UUID;

    status = attm_svc_create_db_128(&start_handle, data_trans_svc_uuid, (uint8_t *)&cfg_flag,
        DATA_TRANS_IDX_MAX, NULL, TASK_APP, &data_trans_atts[0],
        (SVC_PREM_TYPE_UUID128_MASK<<5));
#endif
    if(ATT_ERR_NO_ERROR == status)
    {
        data_trans_svr.hdl_svc = start_handle;
        LOG(LOG_LVL_INFO, "data_trans_svc_add success, handle =:0x%x \r\n", start_handle);
    }
    else
    {
        LOG(LOG_LVL_INFO, "data_trans_svc_add err!!!\r\n");
    }
}



void in_trx_notify(int conidx, uint8_t *buf, uint32_t len)
{
  
}


