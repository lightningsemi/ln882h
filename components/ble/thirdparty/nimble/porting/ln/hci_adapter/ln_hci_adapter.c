#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "stdint.h"
#include "sysinit/sysinit.h"
#include "hci.h"
#include "ln_hci_adapter.h"
#include "nimble/hci_common.h"
#include "nimble/ble_hci_trans.h"
#include "host/ble_hs.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "ln_nimble_mem.h"
#include "semphr.h"
#include "osal/osal.h"

#define NIMBLE_VHCI_TIMEOUT_MS              2000
#define BLE_HCI_EVENT_HDR_LEN               (2)
#define BLE_HCI_CMD_HDR_LEN                 (3)

static ble_hci_trans_rx_cmd_fn *ble_hci_rx_cmd_hs_cb = NULL;
static void *ble_hci_rx_cmd_hs_arg = NULL;

static ble_hci_trans_rx_acl_fn *ble_hci_rx_acl_hs_cb = NULL;
static void *ble_hci_rx_acl_hs_arg = NULL;

static struct os_mbuf_pool ble_hci_acl_mbuf_pool;
static struct os_mempool_ext ble_hci_acl_pool;

#define ACL_BLOCK_SIZE  OS_ALIGN(MYNEWT_VAL(BLE_ACL_BUF_SIZE)   \
        + BLE_MBUF_MEMBLOCK_OVERHEAD                            \
        + BLE_HCI_DATA_HDR_SZ, OS_ALIGNMENT)

static os_membuf_t *ble_hci_acl_buf = NULL;

static struct os_mempool ble_hci_cmd_pool;
static os_membuf_t *ble_hci_cmd_buf = NULL;

static struct os_mempool ble_hci_evt_hi_pool;
static os_membuf_t *ble_hci_evt_hi_buf = NULL;

static struct os_mempool ble_hci_evt_lo_pool;
static os_membuf_t *ble_hci_evt_lo_buf = NULL;

static SemaphoreHandle_t vhci_send_sem;

int os_msys_buf_alloc(void);
void os_msys_buf_free(void);


void ble_hci_trans_cfg_hs(ble_hci_trans_rx_cmd_fn *cmd_cb, void *cmd_arg, ble_hci_trans_rx_acl_fn *acl_cb, void *acl_arg)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    ble_hci_rx_cmd_hs_cb    = cmd_cb;
    ble_hci_rx_cmd_hs_arg   = cmd_arg;
    ble_hci_rx_acl_hs_cb    = acl_cb;
    ble_hci_rx_acl_hs_arg   = acl_arg;
}

int ble_hci_trans_hs_cmd_tx(uint8_t *cmd)
{
    LN_ASSERT(cmd != NULL);
    
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    uint16_t len;
    uint8_t rc = 0;

    *cmd = BLE_HCI_UART_H4_CMD;
    len = BLE_HCI_CMD_HDR_LEN + cmd[3] + 1;
    
    //hexdump(LOG_LVL_DEBUG, "=>", cmd, len);
    
    if (xSemaphoreTake(vhci_send_sem, NIMBLE_VHCI_TIMEOUT_MS / portTICK_PERIOD_MS) == pdTRUE){
        rc = hci_h4tl_receive(cmd, len);
    } 
    else{
        rc = BLE_HS_ETIMEOUT_HCI;
    }
    
    ble_hci_trans_buf_free(cmd);
    
    return rc;
}

int ble_hci_trans_ll_evt_tx(uint8_t *hci_ev)
{
    LN_ASSERT(hci_ev != NULL);
    
    int rc = -1;

    if (ble_hci_rx_cmd_hs_cb){
        rc = ble_hci_rx_cmd_hs_cb(hci_ev, ble_hci_rx_cmd_hs_arg);
    }
    return rc;
}

int ble_hci_trans_hs_acl_tx(struct os_mbuf *om)
{
    LN_ASSERT(om != NULL);
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    uint16_t len = 0;
    uint8_t data[MYNEWT_VAL(BLE_ACL_BUF_SIZE) + 1], rc = 0;

    if (OS_MBUF_PKTLEN(om) == 0){
        os_mbuf_free_chain(om);
        return 0;
    }
    data[0] = BLE_HCI_UART_H4_ACL;
    len++;

    os_mbuf_copydata(om, 0, OS_MBUF_PKTLEN(om), &data[1]);
    len += OS_MBUF_PKTLEN(om);
    
    //hexdump(LOG_LVL_DEBUG, "=>", data, len);
    
    if (xSemaphoreTake(vhci_send_sem, NIMBLE_VHCI_TIMEOUT_MS / portTICK_PERIOD_MS) == pdTRUE){
        hci_h4tl_receive(data, len);
    }else{
        rc = BLE_HS_ETIMEOUT_HCI;
    }

    os_mbuf_free_chain(om);

    return rc;
}

int ble_hci_trans_ll_acl_tx(struct os_mbuf *om)
{
    LN_ASSERT(om != NULL);
    
    int rc = -1;

    if (ble_hci_rx_acl_hs_cb){
        rc = ble_hci_rx_acl_hs_cb(om, ble_hci_rx_acl_hs_arg);
    }
    
    return rc;
}

uint8_t *ble_hci_trans_buf_alloc(int type)
{
    uint8_t *buf;

    switch (type){
        case BLE_HCI_TRANS_BUF_CMD:
            buf = os_memblock_get(&ble_hci_cmd_pool);
            break;

        case BLE_HCI_TRANS_BUF_EVT_HI:
            buf = os_memblock_get(&ble_hci_evt_hi_pool);
            if (buf == NULL){
                buf = ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_LO);
            }
            break;

        case BLE_HCI_TRANS_BUF_EVT_LO:
            buf = os_memblock_get(&ble_hci_evt_lo_pool);
            break;

        default:
            LN_ASSERT(0);
            buf = NULL;
    }

    return buf;
}

void ble_hci_trans_buf_free(uint8_t *buf)
{
    LN_ASSERT(buf != NULL);
    int rc;
    if (os_memblock_from(&ble_hci_evt_hi_pool, buf)){
        rc = os_memblock_put(&ble_hci_evt_hi_pool, buf);
        LN_ASSERT(rc == 0);
    } else if (os_memblock_from(&ble_hci_evt_lo_pool, buf)){
        rc = os_memblock_put(&ble_hci_evt_lo_pool, buf);
        LN_ASSERT(rc == 0);
    } else{
        LN_ASSERT(os_memblock_from(&ble_hci_cmd_pool, buf));
        rc = os_memblock_put(&ble_hci_cmd_pool, buf);
        LN_ASSERT(rc == 0);
    }
}

int ble_hci_trans_set_acl_free_cb(os_mempool_put_fn *cb, void *arg)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    ble_hci_acl_pool.mpe_put_cb = cb;
    ble_hci_acl_pool.mpe_put_arg = arg;
    return 0;
}

int ble_hci_trans_reset(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    return 0;
}


static struct os_mbuf *ble_hci_trans_acl_buf_alloc(void)
{
    struct os_mbuf *m;
    uint8_t usrhdr_len;

#if MYNEWT_VAL(BLE_DEVICE)
    usrhdr_len = sizeof(struct ble_mbuf_hdr);
#elif MYNEWT_VAL(BLE_HS_FLOW_CTRL)
    usrhdr_len = BLE_MBUF_HS_HDR_LEN;
#else
    usrhdr_len = 0;
#endif

    m = os_mbuf_get_pkthdr(&ble_hci_acl_mbuf_pool, usrhdr_len);
    return m;
}

static void ble_hci_rx_acl(uint8_t *data, uint16_t len)
{
    struct os_mbuf *m;
    int rc;
    int sr;
    if (len < BLE_HCI_DATA_HDR_SZ || len > MYNEWT_VAL(BLE_ACL_BUF_SIZE)) {
        return;
    }

    m = ble_hci_trans_acl_buf_alloc();

    if (!m) {
        LOG(LOG_LVL_ERROR, "%s failed to allocate ACL buffers; increase ACL_BUF_COUNT", __func__);
        return;
    }
    if ((rc = os_mbuf_append(m, data, len)) != 0) {
        LOG(LOG_LVL_ERROR, "%s failed to os_mbuf_append; rc = %d", __func__, rc);
        os_mbuf_free_chain(m);
        return;
    }
    OS_ENTER_CRITICAL();
    if (ble_hci_rx_acl_hs_cb) {
        ble_hci_rx_acl_hs_cb(m, NULL);
    }
    OS_EXIT_CRITICAL();
}

static void ble_hci_transport_init(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    
    int rc;

    rc = os_mempool_ext_init(&ble_hci_acl_pool, MYNEWT_VAL(BLE_ACL_BUF_COUNT),
                             ACL_BLOCK_SIZE, ble_hci_acl_buf, "ble_hci_acl_pool");
    LN_ASSERT(rc == 0);

    rc = os_mbuf_pool_init(&ble_hci_acl_mbuf_pool, &ble_hci_acl_pool.mpe_mp,
                            ACL_BLOCK_SIZE, MYNEWT_VAL(BLE_ACL_BUF_COUNT));
    LN_ASSERT(rc == 0);

    rc = os_mempool_init(&ble_hci_cmd_pool, 1, BLE_HCI_TRANS_CMD_SZ,
                          ble_hci_cmd_buf, "ble_hci_cmd_pool");
    LN_ASSERT(rc == 0);

    rc = os_mempool_init(&ble_hci_evt_hi_pool, MYNEWT_VAL(BLE_HCI_EVT_HI_BUF_COUNT), 
                          MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE), ble_hci_evt_hi_buf, "ble_hci_evt_hi_pool");
    LN_ASSERT(rc == 0);

    rc = os_mempool_init(&ble_hci_evt_lo_pool, MYNEWT_VAL(BLE_HCI_EVT_LO_BUF_COUNT),
                          MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE), ble_hci_evt_lo_buf, "ble_hci_evt_lo_pool");
    LN_ASSERT(rc == 0);
}

static void controller_rcv_pkt_ready(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    if (vhci_send_sem){
        xSemaphoreGive(vhci_send_sem);
    }
}

static int host_rcv_pkt(uint8_t *data, uint16_t len)
{
    //hexdump(LOG_LVL_INFO, "<=", data, len);
    if (data[0] == BLE_HCI_UART_H4_EVT){
        uint8_t *evbuf;
        int totlen;
        int rc;

        totlen = BLE_HCI_EVENT_HDR_LEN + data[2];
        LN_ASSERT(totlen <= UINT8_MAX + BLE_HCI_EVENT_HDR_LEN);

        if (totlen > MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE)){
            LOG(LOG_LVL_DEBUG, "Received HCI data length at host (%d) exceeds maximum configured HCI event buffer size (%d).", totlen, MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE));
            ble_hs_sched_reset(BLE_HS_ECONTROLLER);
            return 0;
        }

        if (data[1] == BLE_HCI_EVCODE_HW_ERROR){
            LN_ASSERT(0);
        }

        if ((data[1] == BLE_HCI_EVCODE_LE_META) && (data[3] == BLE_HCI_LE_SUBEV_ADV_RPT)){
            evbuf = ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_LO);
            if (!evbuf) {
                return 0;
            }
        } else {
            evbuf = ble_hci_trans_buf_alloc(BLE_HCI_TRANS_BUF_EVT_HI);
            LN_ASSERT(evbuf != NULL);
        }

        memcpy(evbuf, &data[1], totlen);
        rc = ble_hci_trans_ll_evt_tx(evbuf);
        LN_ASSERT(rc == 0);
    } else if (data[0] == BLE_HCI_UART_H4_ACL){
        ble_hci_rx_acl(data + 1, len - 1);
    }
    return 0;
}

static const ln_vhci_host_callback_t vhci_host_cb = {
    .notify_host_send_available = controller_rcv_pkt_ready,
    .notify_host_recv = host_rcv_pkt,
};

static void ble_buf_free(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    
    os_msys_buf_free();

    nimble_platform_mem_free(ble_hci_evt_hi_buf);
    ble_hci_evt_hi_buf = NULL;
    nimble_platform_mem_free(ble_hci_evt_lo_buf);
    ble_hci_evt_lo_buf = NULL;
    nimble_platform_mem_free(ble_hci_cmd_buf);
    ble_hci_cmd_buf = NULL;
    nimble_platform_mem_free(ble_hci_acl_buf);
    ble_hci_acl_buf = NULL;
}

static int ble_buf_alloc(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    
    if (os_msys_buf_alloc()){
        return -2;
    }

    ble_hci_evt_hi_buf = (os_membuf_t *)nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_HCI_EVT_HI_BUF_COUNT),
        MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE))));

    ble_hci_evt_lo_buf = (os_membuf_t *)nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_HCI_EVT_LO_BUF_COUNT),
        MYNEWT_VAL(BLE_HCI_EVT_BUF_SIZE))));

    ble_hci_cmd_buf = (os_membuf_t *)nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(1, BLE_HCI_TRANS_CMD_SZ)));

    ble_hci_acl_buf = (os_membuf_t *)nimble_platform_mem_calloc(1,
        (sizeof(os_membuf_t) * OS_MEMPOOL_SIZE(MYNEWT_VAL(BLE_ACL_BUF_COUNT),
        ACL_BLOCK_SIZE)));

    if (!ble_hci_evt_hi_buf || !ble_hci_evt_lo_buf || !ble_hci_cmd_buf || !ble_hci_acl_buf){
        ble_buf_free();
        return -2;
    }
    return 0;
}

int ln_nimble_hci_init(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    
    int ret = 0;

    ret = ble_buf_alloc();
    if (ret != 0){
        goto err;
    }
    if ((ret = ln_vhci_host_register_callback(&vhci_host_cb)) != 0){
        goto err;
    }

    ble_hci_transport_init();

    vhci_send_sem = xSemaphoreCreateBinary();
    if (vhci_send_sem == NULL){
        ret = -2;
        goto err;
    }

    xSemaphoreGive(vhci_send_sem);

    return ret;
err:
    ble_buf_free();
    return ret;
}

int ln_nimble_hci_and_controller_init(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    
    //todo: controller init
    
    return ln_nimble_hci_init();
}

static int ble_hci_transport_deinit(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    int ret = 0;

    ret += os_mempool_clear(&ble_hci_evt_lo_pool);

    ret += os_mempool_clear(&ble_hci_evt_hi_pool);

    ret += os_mempool_clear(&ble_hci_cmd_pool);

    ret += os_mempool_ext_clear(&ble_hci_acl_pool);

    if (ret) {
        return -1;
    } else {
        return 0;
    }
}

int ln_nimble_hci_deinit(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    if (vhci_send_sem){
        /* Dummy take & give semaphore before deleting */
        xSemaphoreTake(vhci_send_sem, portMAX_DELAY);
        xSemaphoreGive(vhci_send_sem);
        vSemaphoreDelete(vhci_send_sem);
        vhci_send_sem = NULL;
    }
    int ret = ble_hci_transport_deinit();
    if (ret != 0){
        return ret;
    }

    ble_buf_free();

    return 0;
}

int ln_nimble_hci_and_controller_deinit(void)
{
    LOG(LOG_LVL_DEBUG, "%s\r\n", __func__);
    int ret;
    ret = ln_nimble_hci_deinit();
    if (ret != 0){
        return ret;
    }
    //todo: controller deinit
    
    return 0;
}

