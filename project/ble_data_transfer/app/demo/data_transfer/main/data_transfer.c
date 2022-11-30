
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
#include "host/ble_gap.h"
#include "osal/osal.h"
#include "utils/debug/log.h"

//bonding enable or disable
#define USER_CONF_BONDING_EN

#define MIN_REQUIRED_MBUF         1   //2 /* Assuming payload of 500Bytes and each mbuf can take 292Bytes.  */
#define PREFERRED_MTU_VALUE       512
#define LL_PACKET_TIME            2120
#define LL_PACKET_LENGTH          251
#define MTU_DEF                   512


#define TX_BUFF_LEN     20
static uint8_t g_tx_buff[TX_BUFF_LEN] = {0};


static OS_Thread_t g_send_thread;

static const char *device_name = "mingjun_data_transfer";
static bool indicate_state = false;
static uint16_t conn_handle = 0;
static uint8_t gatts_addr_type;
static int gatts_gap_event(struct ble_gap_event *event, void *arg);

void print_bytes(const uint8_t *bytes, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        LOG(LOG_LVL_INFO, "%s0x%02x", i != 0 ? ":" : "", bytes[i]);
    }
}

void print_addr(const void *addr)
{
    const uint8_t *u8p;

    u8p = addr;
    LOG(LOG_LVL_INFO, "[%02x:%02x:%02x:%02x:%02x:%02x]\r\n",
             u8p[5], u8p[4], u8p[3], u8p[2], u8p[1], u8p[0]);
}

static void bleprph_print_conn_desc(struct ble_gap_conn_desc *desc)
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

int ln_set_random_addr(void)
{
    ble_addr_t addr;
    // Generates a new static random address
    int rc = ble_hs_id_gen_rnd(0, &addr);
    if (rc != 0)
    {
        LOG(LOG_LVL_INFO, "ble_hs_id_gen_rnd err, rc = %d\r\n", rc);
        return -1;
    }
    // Set generated address
    rc = ble_hs_id_set_rnd(addr.val);
    if (rc != 0)
    {
        LOG(LOG_LVL_INFO, "ble_hs_id_set_rnd err, rc = %d\r\n", rc);
        return -1;
    }
    // Try to configure the device with random static address
    rc = ble_hs_util_ensure_addr(1);
    if (rc != 0)
    {
      LOG(LOG_LVL_INFO, "ble_hs_util_ensure_addr err, rc = %d\r\n", rc);
        return -1;
    }
    LOG(LOG_LVL_INFO, "device random static address:\r\n");
    print_addr(addr.val);
    
    return 0;
}

static int ln_adv_data_config(void)
{
    int rc = 0;
    uint8_t adv_data[31];
    uint8_t index = 0;
    
    rc = ble_svc_gap_device_name_set(device_name);
    if(rc != 0){
        LOG(LOG_LVL_INFO, "ble_svc_gap_device_name_set err, rc = %d\r\n", rc);
        return -1;
    }
    
    memset(adv_data, 0, sizeof(adv_data));
    
    adv_data[index++] = 0x02;       // length
    adv_data[index++] = 0x01;       // AD type : flags
    adv_data[index++] = 0x06;       // AD value
    adv_data[index++] = 18;         // length
    adv_data[index++] = 0x16;       // AD type: (Service Data - 16-bit UUID)
    adv_data[index++] = 0xF6;       // AD value
    adv_data[index++] = 0xFF;       // AD value
    for (uint8_t i = 0; i < 15; i++){
        adv_data[index + i] = 0x55;
    }

// for adv name visual
//    adv_data[index++] = strlen(device_name) + 1;         
//    adv_data[index++] = 0x09;      
//    memcpy(adv_data + index, device_name, strlen(device_name));
   
    rc = ble_gap_adv_set_data(adv_data, sizeof(adv_data));
    if(rc != 0){
        LOG(LOG_LVL_INFO, "ble_gap_adv_set_data err, rc = %d\r\n", rc);
        return -1;
    }
    return 0; 
}

static uint8_t own_addr_type = BLE_OWN_ADDR_RANDOM;
static int ln_adv_start(void)
{
    int rc = 0;
    struct ble_gap_adv_params  adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.itvl_min = 80;
    adv_params.itvl_max = 120;
    if(ble_gap_adv_active()){
        rc = ble_gap_adv_stop();
        if(rc != 0){
        LOG(LOG_LVL_INFO, "ble_gap_adv_stop err, rc = %d\r\n", rc);
        return -1;
    }
    }
    rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER, &adv_params, gatts_gap_event, NULL);
    if(rc != 0){
        LOG(LOG_LVL_INFO, "ble_gap_adv_start err, rc = %d\r\n", rc);
        return -1;
    }
    return 0;
}

/*
 * Enables advertising with parameters:
 *     o General discoverable mode
 *     o Undirected connectable mode
 */
static void gatts_advertise(void)
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
extern uint16_t txchar_attrhandle;
static void char_tx_cccd_write_process(bool ind)
{
    indicate_state = ind;
    LOG(LOG_LVL_INFO, "indicate_state = %d\r\n", indicate_state);
}

static int gatts_gap_event(struct ble_gap_event *event, void *arg)
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
            //gatts_advertise();
            ln_adv_start();
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
        LOG(LOG_LVL_INFO, "disconnect; reason = 0x%x\r\n", event->disconnect.reason);

        /* Connection terminated; resume advertising */
        //gatts_advertise();
        ln_adv_start();
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
        //gatts_advertise();
        ln_adv_start();
        break;
    
    case BLE_GAP_EVENT_ENC_CHANGE:
        LOG(LOG_LVL_INFO, "encryption change event; status = %d \r\n", event->enc_change.status);
        rc = ble_gap_conn_find(event->enc_change.conn_handle, &desc);
        assert(rc == 0);
        bleprph_print_conn_desc(&desc);

        break;
    case BLE_GAP_EVENT_REPEAT_PAIRING:   
        LOG(LOG_LVL_INFO, "BLE_GAP_EVENT_REPEAT_PAIRING\r\n");
        rc = ble_gap_conn_find(event->repeat_pairing.conn_handle, &desc);
        assert(rc == 0);
        ble_store_util_delete_peer(&desc.peer_id_addr);
        return BLE_GAP_REPEAT_PAIRING_RETRY;
    
    case BLE_GAP_EVENT_PASSKEY_ACTION:
        LOG(LOG_LVL_INFO, "BLE_GAP_EVENT_PASSKEY_ACTION\r\n");
        break;
    
    case BLE_GAP_EVENT_SUBSCRIBE:
        LOG(LOG_LVL_INFO, "subscribe event; cur_notify=%d; value handle; " "val_handle = %d\r\n",
                           event->subscribe.cur_indicate, event->subscribe.attr_handle);
        if (event->subscribe.attr_handle == txchar_attrhandle){
            char_tx_cccd_write_process(event->subscribe.cur_indicate);
        }
        
        break;

    case BLE_GAP_EVENT_NOTIFY_TX:
        if (event->notify_tx.status == 0){
            //do nothing
        }
        else if (event->notify_tx.status == BLE_HS_EDONE){
            LOG(LOG_LVL_INFO, "send indicate ok\r\n");
        } else{
            LOG(LOG_LVL_INFO, "---- send indicate error, status = %d ----\r\n", event->notify_tx.status);
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

static void gatts_on_sync(void)
{
/*
    int rc;
    uint8_t addr_val[6] = {0};
    rc = ble_hs_id_infer_auto(0, &gatts_addr_type);
    assert(rc == 0);
    rc = ble_hs_id_copy_addr(gatts_addr_type, addr_val, NULL);
    assert(rc == 0);
    LOG(LOG_LVL_INFO, "Device Address:");
    print_addr(addr_val);
    
    //开启广播
    gatts_advertise();
*/   
    //设置随机地址
    ln_set_random_addr();
    //设置广播数据
    ln_adv_data_config();
    //开启广播
    ln_adv_start();
}

static void gatts_on_reset(int reason)
{
    LOG(LOG_LVL_INFO, "Resetting state; reason=%d\r\n", reason);
}

void gatts_host_task(void *param)
{
    LOG(LOG_LVL_INFO, "ble host start\r\n");
    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();
    nimble_port_freertos_deinit();
}
static uint16_t send_count = 0;
static bool send_indication(uint8_t *data, uint8_t len)
{
    struct os_mbuf *om;
    om = ble_hs_mbuf_from_flat(data, len);
    if (om == NULL)
    {
        LOG(LOG_LVL_INFO, "om == NULL\r\n");
        return false;
    }
    LOG(LOG_LVL_INFO, "sinding seq %d\r\n", send_count++);
    return ble_gattc_indicate_custom(conn_handle, txchar_attrhandle, om);
}

static void send_task(void *arg)
{
   static uint8_t dat = 0;
   while(1)
   {
      if (indicate_state){
          for (uint16_t i = 0; i < sizeof(g_tx_buff); i++){
              g_tx_buff[i] = dat;  
          }
          dat++;
          send_indication(g_tx_buff, sizeof(g_tx_buff));
      }
      OS_MsDelay(1000);
   }   
}
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
    
#ifdef USER_CONF_BONDING_EN   
    ble_hs_cfg.sm_bonding        = 1;
    ble_hs_cfg.sm_our_key_dist   = BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID;
    ble_hs_cfg.sm_their_key_dist = BLE_SM_PAIR_KEY_DIST_ENC | BLE_SM_PAIR_KEY_DIST_ID;
#endif    

    if(OS_OK != OS_ThreadCreate(&g_send_thread, "sendTask", send_task, NULL, OS_PRIORITY_NORMAL, 1024*4)) {
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
//    rc = ble_svc_gap_device_name_set(device_name);
//    assert(rc == 0);
//    if (rc != 0)
//    {
//        LOG(LOG_LVL_ERROR, "ble_svc_gap_device_name_set fail %x\r\n", rc);
//        return;
//    }

    /* Start the task */
    nimble_port_freertos_init(gatts_host_task);
}
