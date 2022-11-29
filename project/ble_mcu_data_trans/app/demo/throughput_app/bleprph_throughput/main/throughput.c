
#include "FreeRTOSConfig.h"
#include "ln_hci_adapter.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"
#include "gatts_sens.h"
#include "../src/ble_hs_hci_priv.h"
#include "utils/debug/log.h"
#include "osal/osal.h"
#define NOTIFY_THROUGHPUT_PAYLOAD 120 
#define MIN_REQUIRED_MBUF         1   //2 /* Assuming payload of 500Bytes and each mbuf can take 292Bytes.  */
#define PREFERRED_MTU_VALUE       512
#define LL_PACKET_TIME            2120
#define LL_PACKET_LENGTH          251
#define MTU_DEF                   512

static const char *device_name = "nimble_mj";
static SemaphoreHandle_t notify_sem;
static bool notify_state = false;
static int notify_test_time = (60);
static uint16_t conn_handle;
/* Dummy variable */
static uint8_t dummy;
static uint8_t gatts_addr_type;

static int gatts_gap_event(struct ble_gap_event *event, void *arg);

/**
 * Utility function to log an array of bytes.
 */

void
print_bytes(const uint8_t *bytes, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        LOG(LOG_LVL_INFO, "%s0x%02x", i != 0 ? ":" : "", bytes[i]);
    }
}

void
print_addr(const void *addr)
{
    const uint8_t *u8p;

    u8p = addr;
    LOG(LOG_LVL_INFO, "%02x:%02x:%02x:%02x:%02x:%02x\r\n",
             u8p[5], u8p[4], u8p[3], u8p[2], u8p[1], u8p[0]);
}

static void
bleprph_print_conn_desc(struct ble_gap_conn_desc *desc)
{
    LOG(LOG_LVL_INFO, "handle=%d our_ota_addr_type=%d our_ota_addr=",
                desc->conn_handle, desc->our_ota_addr.type);
    print_addr(desc->our_ota_addr.val);
    LOG(LOG_LVL_INFO, " our_id_addr_type=%d our_id_addr=",
                desc->our_id_addr.type);
    print_addr(desc->our_id_addr.val);
    LOG(LOG_LVL_INFO, " peer_ota_addr_type=%d peer_ota_addr=",
                desc->peer_ota_addr.type);
    print_addr(desc->peer_ota_addr.val);
    LOG(LOG_LVL_INFO, " peer_id_addr_type=%d peer_id_addr=",
                desc->peer_id_addr.type);
    print_addr(desc->peer_id_addr.val);
    LOG(LOG_LVL_INFO, " conn_itvl=%d conn_latency=%d supervision_timeout=%d"
                "encrypted=%d authenticated=%d bonded=%d\r\n",
                desc->conn_itvl, desc->conn_latency,
                desc->supervision_timeout,
                desc->sec_state.encrypted,
                desc->sec_state.authenticated,
                desc->sec_state.bonded);
}

/*
 * Enables advertising with parameters:
 *     o General discoverable mode
 *     o Undirected connectable mode
 */
static void
gatts_advertise(void)
{
    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    /*
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info)
     *     o Advertising tx power
     *     o Device name
     */
    memset(&fields, 0, sizeof(fields));

    /*
     * Advertise two flags:
     *      o Discoverability in forthcoming advertisement (general)
     *      o BLE-only (BR/EDR unsupported)
     */
    fields.flags = BLE_HS_ADV_F_DISC_GEN |
                   BLE_HS_ADV_F_BREDR_UNSUP;

    /*
     * Indicate that the TX power level field should be included; have the
     * stack fill this value automatically.  This is done by assigning the
     * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
     */
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        LOG(LOG_LVL_INFO, "Error setting advertisement data; rc=%d\r\n", rc);
        return;
    }

    /* Begin advertising */
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    rc = ble_gap_adv_start(gatts_addr_type, NULL, BLE_HS_FOREVER,
                           &adv_params, gatts_gap_event, NULL);
    if (rc != 0) {
        LOG(LOG_LVL_INFO, "Error enabling advertisement; rc=%d\r\n", rc);
        return;
    }
}

/* This function sends notifications to the client */
static void
notify_task(void *arg)
{
    static uint8_t payload[NOTIFY_THROUGHPUT_PAYLOAD] = {0};/* Data payload */
    int rc, notify_count = 0;
    int64_t start_time, end_time, notify_time = 0;
    struct os_mbuf *om;

    payload[0] = dummy; /* storing dummy data */
    payload[1] = 0x12;
    payload[19] = 0x34;
    LOG(LOG_LVL_INFO, "%s\r\n",__func__);

    while (!notify_state) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        LOG(LOG_LVL_INFO, "ntf wait\r\n");
    }
    LOG(LOG_LVL_INFO, "ntf start %d\r\n", notify_test_time);
    while (1) {
        switch (notify_test_time) {

        case 0:
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            break;
        default:
            start_time = OS_GetTicks();

            if (!notify_state) {
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                break;
            }
            notify_time = 0;
            notify_count = 0;
            notify_state = 0;
            
            while (notify_time < (notify_test_time)) {
                /* We are anyway using counting semaphore for sending
                 * notifications. So hopefully not much waiting period will be
                 * introduced before sending a new notification. Revisit this
                 * counter if need to do away with semaphore waiting. XXX */
                xSemaphoreTake(notify_sem, portMAX_DELAY);

                if (dummy == 200) {
                    dummy = 0;
                }
                dummy++;

                /* Check if the MBUFs are available */
                if (os_msys_num_free() >= MIN_REQUIRED_MBUF) {
                    om = ble_hs_mbuf_from_flat(payload, sizeof(payload));
                    if (om == NULL) {
                        /* Memory not available for mbuf */
                        LOG(LOG_LVL_INFO, "No MBUFs available from pool, retry..\r\n");
                        vTaskDelay(100 / portTICK_PERIOD_MS);
                        om = ble_hs_mbuf_from_flat(payload, sizeof(payload));
                        assert(om != NULL);
                    }

                    rc = ble_gattc_notify_custom(conn_handle, notify_handle, om);
                    if (rc != 0) {
                        LOG(LOG_LVL_INFO, "Error while sending notification; rc = %d\r\n", rc);
                        notify_count -= 1;
                        xSemaphoreGive(notify_sem);
                        /* Most probably error is because we ran out of mbufs (rc = 6),
                         * increase the mbuf count/size from menuconfig. Though
                         * inserting delay is not good solution let us keep it
                         * simple for time being so that the mbufs get freed up
                         * (?), of course assumption is we ran out of mbufs */
                        vTaskDelay(10 / portTICK_PERIOD_MS);
                    }
                } else {
                    LOG(LOG_LVL_INFO, "Not enough OS_MBUFs available; reduce notify count\r\n");
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                    xSemaphoreGive(notify_sem);
                    notify_count -= 1;
                }

                end_time = OS_GetTicks();
                notify_time = (end_time - start_time) / 1000 ;
                notify_count += 1;
            }

            LOG(LOG_LVL_INFO, "Notify throughput = %d bps, total(B) = %d, count = %d\r\n",
                     (notify_count * NOTIFY_THROUGHPUT_PAYLOAD * 8) / notify_test_time, notify_count * NOTIFY_THROUGHPUT_PAYLOAD, notify_count);
            LOG(LOG_LVL_INFO, " Notification test complete for stipulated time of %d sec\r\n", notify_test_time);
            //notify_test_time = 0;
            notify_count = 0;

            break;
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

static int
gatts_gap_event(struct ble_gap_event *event, void *arg)
{
    struct ble_gap_conn_desc desc;
    int rc;

    switch (event->type) {
    case BLE_GAP_EVENT_CONNECT:
        /* A new connection was established or a connection attempt failed */
        LOG(LOG_LVL_INFO, "connection %s; status = %d\r\n",
                 event->connect.status == 0 ? "established" : "failed",
                 event->connect.status);
        rc = ble_att_set_preferred_mtu(PREFERRED_MTU_VALUE);
        if (rc != 0) {
            LOG(LOG_LVL_INFO, "Failed to set preferred MTU; rc = %d\r\n", rc);
        }

        if (event->connect.status != 0) {
            /* Connection failed; resume advertising */
            gatts_advertise();
        }

        rc = ble_hs_hci_util_set_data_len(event->connect.conn_handle,
                                          LL_PACKET_LENGTH,
                                          LL_PACKET_TIME);
        if (rc != 0) {
            LOG(LOG_LVL_INFO, "Set packet length failed\r\n");
        }

        conn_handle = event->connect.conn_handle;
        break;

    case BLE_GAP_EVENT_DISCONNECT:
        LOG(LOG_LVL_INFO, "disconnect; reason = %d\r\n", event->disconnect.reason);

        /* Connection terminated; resume advertising */
        gatts_advertise();
        break;

    case BLE_GAP_EVENT_CONN_UPDATE:
        /* The central has updated the connection parameters. */
        LOG(LOG_LVL_INFO, "connection updated; status=%d\r\n", event->conn_update.status);
        rc = ble_gap_conn_find(event->conn_update.conn_handle, &desc);
        assert(rc == 0);
        bleprph_print_conn_desc(&desc);
        return 0;
     
    case BLE_GAP_EVENT_ADV_COMPLETE:
        LOG(LOG_LVL_INFO, "adv complete\r\n");
        gatts_advertise();
        break;

    case BLE_GAP_EVENT_SUBSCRIBE:
        LOG(LOG_LVL_INFO, "subscribe event; cur_notify=%d; value handle; "
                 "val_handle = %d\r\n",
                 event->subscribe.cur_notify, event->subscribe.attr_handle);
        if (event->subscribe.attr_handle == notify_handle) {
            notify_state = event->subscribe.cur_notify;
            if (arg != NULL) {
                LOG(LOG_LVL_INFO, "notify test time = %d\r\n", *(int *)arg);
                notify_test_time = *((int *)arg);
            }
            xSemaphoreGive(notify_sem);
        } else if (event->subscribe.attr_handle != notify_handle) {
            notify_state = event->subscribe.cur_notify;
        }
        break;

    case BLE_GAP_EVENT_NOTIFY_TX:
        
        if ((event->notify_tx.status == 0) ||
                (event->notify_tx.status == BLE_HS_EDONE)) {
            /* Send new notification i.e. give Semaphore. By definition,
             * sending new notifications should not be based on successful
             * notifications sent, but let us adopt this method to avoid too
             * many `BLE_HS_ENOMEM` errors because of continuous transfer of
             * notifications.XXX */
            LOG(LOG_LVL_INFO, "NOTIFY_TX success !!\r\n");
            OS_MsDelay(100);
            xSemaphoreGive(notify_sem);
        } else {
            LOG(LOG_LVL_INFO, "----------->NOTIFY_TX error status = %d\r\n", event->notify_tx.status);
        }
        break;

    case BLE_GAP_EVENT_MTU:
        LOG(LOG_LVL_INFO, "mtu update event; conn_handle = %d mtu = %d\r\n",
                 event->mtu.conn_handle,
                 event->mtu.value);
        break;
    }
    return 0;
}

static void
gatts_on_sync(void)
{
    int rc;
    uint8_t addr_val[6] = {0};

    rc = ble_hs_id_infer_auto(0, &gatts_addr_type);
    assert(rc == 0);
    rc = ble_hs_id_copy_addr(gatts_addr_type, addr_val, NULL);
    assert(rc == 0);
    LOG(LOG_LVL_INFO, "Device Address:");
    print_addr(addr_val);
    /* Begin advertising */
    gatts_advertise();
}

static void
gatts_on_reset(int reason)
{
    LOG(LOG_LVL_INFO, "Resetting state; reason=%d\r\n", reason);
}

void gatts_host_task(void *param)
{
    LOG(LOG_LVL_INFO, "ble host start\r\n");
    /* Create a counting semaphore for Notification. Can be used to track
     * successful notification txmission. Optimistically take some big number
     * for counting Semaphore */
    notify_sem = xSemaphoreCreateCounting(100, 0);
    if (notify_sem == NULL)
    {
        LOG(LOG_LVL_INFO, "xSemaphoreCreateCounting fail.\r\n");
    }
    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();
    vSemaphoreDelete(notify_sem);
    nimble_port_freertos_deinit();
}
#include "utils/debug/log.h"
#include "osal/osal.h"
static OS_Thread_t g_demo_thread;
void app_main(void)
{
    int rc;
    rc = ln_nimble_hci_and_controller_init();
    if (rc != 0)
    {
        LOG(LOG_LVL_ERROR, "ln_nimble_hci_and_controller_init fail %x\r\n", rc);
        return;
    }
    nimble_port_init();
    /* Initialize the NimBLE host configuration */
    ble_hs_cfg.sync_cb = gatts_on_sync;
    ble_hs_cfg.reset_cb = gatts_on_reset;
    ble_hs_cfg.gatts_register_cb = gatt_svr_register_cb,
    ble_hs_cfg.store_status_cb = ble_store_util_status_rr;

    /* Initialize Notify Task */
    if(OS_OK != OS_ThreadCreate(&g_demo_thread, "ntfDemo", notify_task, NULL, OS_PRIORITY_NORMAL, 1024*4)) {
       return;
    }

    rc = gatt_svr_init();
    assert(rc == 0);
    if (rc != 0)
    {
        LOG(LOG_LVL_ERROR, "gatt_svr_init fail %x\r\n", rc);
        return;
    }

    /* Set the default device name */
    rc = ble_svc_gap_device_name_set(device_name);
    assert(rc == 0);
    if (rc != 0)
    {
        LOG(LOG_LVL_ERROR, "ble_svc_gap_device_name_set fail %x\r\n", rc);
        return;
    }

    /* Start the task */
    nimble_port_freertos_init(gatts_host_task);
}
