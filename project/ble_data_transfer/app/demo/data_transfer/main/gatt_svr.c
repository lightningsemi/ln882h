/*
 * SPDX-FileCopyrightText: 2015-2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "host/ble_hs.h"
#include "host/ble_uuid.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "gatts_sens.h"
#include "utils/debug/log.h"

#define RX_BUFF_LEN     512
static uint8_t g_rx_buff[RX_BUFF_LEN] = {0};

struct ble_gatt_char_context
{
    uint16_t conn_handle;
    uint16_t attr_handle;
    struct ble_gatt_access_ctxt *ctxt;
    void *arg;
};

const ble_uuid16_t uuid16_chipoble_service = {
    BLE_UUID_TYPE_16, 0xFFF6
};
const ble_uuid128_t uuid128_chipoble_char_rx = {
    BLE_UUID_TYPE_128, {0x11, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18}
};
const ble_uuid128_t uuid128_chipoble_char_tx   = {
    BLE_UUID_TYPE_128, {0x12, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18}
};

static uint16_t service_attrhandle;
static uint16_t rxchar_attrhandle;
uint16_t txchar_attrhandle;

static int char_rx_write_process(struct ble_gatt_char_context *param)
{
    uint16_t data_len = 0;
    uint16_t out_len = 0;
    data_len = OS_MBUF_PKTLEN(param->ctxt->om);
    if (data_len > RX_BUFF_LEN){
        return -1;
    }
    ble_hs_mbuf_to_flat(param->ctxt->om, g_rx_buff, data_len, &out_len);
    LOG(LOG_LVL_INFO, "len = %d, %d\r\n", data_len, out_len);
    hexdump(LOG_LVL_INFO, "rx", g_rx_buff, out_len);
    
    return 0;
}

static int gatt_svr_chr_access(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    struct ble_gatt_char_context param;
    switch (ctxt->op)
    {
        case BLE_GATT_ACCESS_OP_READ_CHR:
            LOG(LOG_LVL_INFO, "BLE_GATT_ACCESS_OP_READ_CHR, but not supported\r\n");
            break;

        case BLE_GATT_ACCESS_OP_READ_DSC:
            LOG(LOG_LVL_INFO, "BLE_GATT_ACCESS_OP_READ_DSC, but not supported\r\n");
            break;

        case BLE_GATT_ACCESS_OP_WRITE_CHR:
            LOG(LOG_LVL_INFO, "BLE_GATT_ACCESS_OP_WRITE_CHR\r\n");
            param.conn_handle = conn_handle;
            param.attr_handle = attr_handle;
            param.ctxt        = ctxt;
            param.arg         = arg;
            char_rx_write_process(&param);
        
            break;

        default:
            break;
    }

    return 0;
}


const struct ble_gatt_svc_def chipoble_gatt_attrs[] = {
    { 
        .type = BLE_GATT_SVC_TYPE_PRIMARY,
        .uuid = (ble_uuid_t *) (&uuid16_chipoble_service),
        .characteristics =
          (struct ble_gatt_chr_def[]){
              {
                  .uuid       = (ble_uuid_t *)(&uuid128_chipoble_char_rx),
                  .access_cb  = gatt_svr_chr_access,
                  .flags      = BLE_GATT_CHR_F_WRITE,
                  .val_handle = &rxchar_attrhandle,
              },
              {
                  .uuid       = (ble_uuid_t *) (&uuid128_chipoble_char_tx),
                  .access_cb  = gatt_svr_chr_access,
                  .flags      = BLE_GATT_CHR_F_READ | BLE_GATT_CHR_F_INDICATE,
                  .val_handle = &txchar_attrhandle,
              },
              {
                  0, /* No more characteristics in this service */
              },
          } 
    },
    {
        0, /* No more services. */
    },
};

void gatt_svr_register_cb(struct ble_gatt_register_ctxt *ctxt, void *arg)
{
    char buf[BLE_UUID_STR_LEN];

    switch (ctxt->op) {
    case BLE_GATT_REGISTER_OP_SVC:
        LOG(LOG_LVL_INFO, "registered service %s with handle=%d\n",
                 ble_uuid_to_str(ctxt->svc.svc_def->uuid, buf),
                 ctxt->svc.handle);
        break;

    case BLE_GATT_REGISTER_OP_CHR:
        LOG(LOG_LVL_INFO, "registering characteristic %s with "
                 "def_handle=%d val_handle=%d\n",
                 ble_uuid_to_str(ctxt->chr.chr_def->uuid, buf),
                 ctxt->chr.def_handle,
                 ctxt->chr.val_handle);
        break;

    case BLE_GATT_REGISTER_OP_DSC:
        LOG(LOG_LVL_INFO, "registering descriptor %s with handle=%d\n",
                 ble_uuid_to_str(ctxt->dsc.dsc_def->uuid, buf),
                 ctxt->dsc.handle);
        break;

    default:
        assert(0);
        break;
    }
}

int gatt_svr_init(void)
{
    int rc;

    ble_svc_gap_init();
    ble_svc_gatt_init();

    rc = ble_gatts_count_cfg(chipoble_gatt_attrs);
    if (rc != 0) {
        return rc;
    }

    rc = ble_gatts_add_svcs(chipoble_gatt_attrs);
    if (rc != 0) {
        return rc;
    }

    return 0;
}
