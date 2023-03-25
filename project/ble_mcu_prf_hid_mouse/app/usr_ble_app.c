#include "rwip_config.h"    // RW SW configuration TASK_APP
#include "ke_task.h"        // Kernel Task

#include "osal/osal.h"
#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "ln_utils.h"
#include "ln_misc.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gap.h"
#include "prf_types.h"

#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "ln_ble_advertising.h"
#include "ln_ble_scan.h"
#include "ln_ble_rw_app_task.h"
#include "ln_ble_app_kv.h"
#include "ln_ble_connection_manager.h"
#include "ln_ble_event_manager.h"
#include "ln_ble_smp.h"
#include "usr_ble_app.h"
#include "ln_ble_app_default_cfg.h"


enum ble_usr_msg_type
{
    BLE_MSG_CONN_IND,
    BLE_MSG_DISCONN_IND,
    BLE_MSG_SCAN_REPORT,

};

typedef struct
{
    uint16_t  id;
    uint16_t  len;
    void     *msg;
} ble_usr_msg_t;

#define BLE_USR_MSG_QUEUE_SIZE          (96)
#define BLE_USR_APP_TASK_STACK_SIZE     (1024)

static OS_Thread_t ble_g_usr_app_thread;
static OS_Queue_t ble_usr_queue;
static OS_Timer_t ble_usr_timer;

static uint8_t ext_adv_data[ADV_DATA_EXTEND_MAX] = {0};

static void usr_queue_creat(void)
{
    if(OS_OK != OS_QueueCreate(&ble_usr_queue, BLE_USR_MSG_QUEUE_SIZE, sizeof(ble_usr_msg_t)))
    {
        LOG(LOG_LVL_INFO, "usr QueueCreate rw_queue failed!!!\r\n");
    }
}

void usr_queue_msg_send(uint16_t id, uint16_t length, void *msg)
{
    ble_usr_msg_t usr_msg;
    usr_msg.id = id;
    usr_msg.len = length;
    usr_msg.msg = msg;
    OS_QueueSend(&ble_usr_queue, &usr_msg, OS_WAIT_FOREVER);
}

int usr_queue_msg_recv(void *msg, uint32_t timeout)
{
    return OS_QueueReceive(&ble_usr_queue, msg, timeout);
}

void user_timer_callback (void *arg)
{
    LOG(LOG_LVL_TRACE,"user_timer_callback\r\n");
}

static int user_timer_creat(void)
{
     if(OS_OK != OS_TimerCreate(&ble_usr_timer, OS_TIMER_PERIODIC, user_timer_callback, NULL, 1000))
     {
          LOG(LOG_LVL_TRACE,"usr timer create fail\r\n");
          return 1;
     }
    return 0;
}

int user_timer_start(void *timer)
{
    if (OS_OK != OS_TimerStart((OS_Timer_t *)timer))
    {
        LOG(LOG_LVL_TRACE,"user timer start fail\r\n");
        return 1;
    }

    return 0;
}

int user_timer_stop(void *timer)
{
    if (OS_OK != OS_TimerStop((OS_Timer_t *)timer))
    {
        LOG(LOG_LVL_TRACE,"user timer stop fail\r\n");
        return 1;
    }

    return 0;
}

int user_timer_delete(void *timer)
{
    if (OS_OK != OS_TimerDelete((OS_Timer_t *)timer))
    {
        LOG(LOG_LVL_TRACE,"user timer delete fail\r\n");
        return 1;
    }
    return 0;
}

#ifdef BLE_USE_LEGACY_ADV
static void app_set_adv_data(void)
{
    //adv data: adv length--adv type--adv string ASCII
    uint8_t adv_data_len = 0;
    uint8_t adv_data[ADV_DATA_LEGACY_MAX] = {0};
    ln_adv_data_t adv_data_param;

    uint16_t name_len = 0;
    uint8_t *name = ln_kv_ble_dev_name_get(&name_len);

    if(name_len)
    {
        if(name_len > (ADV_DATA_LEGACY_MAX-2))
            name_len = ADV_DATA_LEGACY_MAX-2;
        /*add item ble_name*/
        adv_data[0] = name_len + 1;
        adv_data[1] = GAP_AD_TYPE_COMPLETE_NAME;  //adv type :local name
        adv_data_len += 2;
        memcpy(&adv_data[2], name, name_len);
        adv_data_len += name_len;
    }

    adv_data_param.length = adv_data_len;
    adv_data_param.data = adv_data;
    ln_ble_adv_data_set(&adv_data_param);
    
    hexdump(LOG_LVL_INFO, (const char *)"legacy_adv", adv_data, adv_data_len);
}

static void app_set_scan_resp_data(void)
{
    //adv data: adv length--adv type--adv string ASCII
    uint8_t adv_data_len = 0;
    uint8_t adv_data[ADV_DATA_LEGACY_MAX] = {0};
    ln_adv_data_t adv_data_param;
    bd_addr_t addr = {0};

    ln_bd_addr_t *kv_addr = ln_kv_ble_pub_addr_get();
    memcpy(addr.addr, kv_addr->addr, 6);
    
    /*add item ble_addr*/
    adv_data[0] = BD_ADDR_LEN + 1;
    adv_data[1] = GAP_AD_TYPE_LE_BT_ADDR;
    adv_data_len += 2;
    memcpy(&adv_data[2], addr.addr, BD_ADDR_LEN);
    adv_data_len += BD_ADDR_LEN;

    adv_data_param.length = adv_data_len;
    adv_data_param.data = adv_data;
    ln_ble_adv_scan_rsp_data_set(&adv_data_param);
    
    hexdump(LOG_LVL_INFO, (const char *)"legacy_scan_rsp", adv_data, adv_data_len);
}
#else
static void app_set_ext_adv_data(void)
{
    //adv data: adv length--adv type--adv string ASCII
    uint16_t adv_data_len = 0;
    ln_adv_data_t adv_data_param;

    uint16_t name_len = 0;
    uint8_t *name = ln_kv_ble_dev_name_get(&name_len);

    if(name_len)
    {
        if(name_len > BLE_DEV_NAME_MAX_LEN)
            name_len = BLE_DEV_NAME_MAX_LEN;
        /*add item ble_name*/
        ext_adv_data[0] = name_len + 1;
        ext_adv_data[1] = GAP_AD_TYPE_COMPLETE_NAME;  //adv type :local name
        adv_data_len += 2;
        memcpy(&ext_adv_data[2], name, name_len);
        adv_data_len += name_len;
    }
    ext_adv_data[adv_data_len++] = 201;
    ext_adv_data[adv_data_len++] = 0xff;
    for(int i = adv_data_len; i < adv_data_len + 200; i++)
    {
        ext_adv_data[i] = i;
    }
    adv_data_len += 200;

    adv_data_param.length = adv_data_len;
    adv_data_param.data = ext_adv_data;
    ln_ble_adv_data_set(&adv_data_param);
    
    hexdump(LOG_LVL_INFO, (const char *)"ext_adv", ext_adv_data, adv_data_len);
}

static void app_set_ext_scan_resp_data(void)
{
    //adv data: adv length--adv type--adv string ASCII
    uint16_t adv_data_len = 0;
    ln_adv_data_t adv_data_param;

    uint16_t name_len = 0;
    uint8_t *name = ln_kv_ble_dev_name_get(&name_len);

    if(name_len)
    {
        if(name_len > BLE_DEV_NAME_MAX_LEN)
            name_len = BLE_DEV_NAME_MAX_LEN;
        /*add item ble_name*/
        ext_adv_data[0] = name_len + 1;
        ext_adv_data[1] = GAP_AD_TYPE_COMPLETE_NAME;  //adv type :local name
        adv_data_len += 2;
        memcpy(&ext_adv_data[2], name, name_len);
        adv_data_len += name_len;
    }
    ext_adv_data[adv_data_len++] = 201;
    ext_adv_data[adv_data_len++] = 0xff;
    for(int i = adv_data_len; i < adv_data_len + 200; i++)
    {
        ext_adv_data[i] = i;
    }
    adv_data_len += 200;

    adv_data_param.length = adv_data_len;
    adv_data_param.data = ext_adv_data;
    ln_ble_adv_scan_rsp_data_set(&adv_data_param);
}
#endif

static void ln_ble_connect_cb(void *arg)
{
    ble_evt_connected_t * evt_conn = (ble_evt_connected_t *)arg;

    uint8_t conn_idx = evt_conn->conn_idx;
    LOG(LOG_LVL_TRACE, "ln_ble_connect conn_id=%d\r\n", conn_idx);

    ble_evt_connected_t *data = blib_malloc(sizeof(ble_evt_connected_t));
    memcpy(data, evt_conn, sizeof(ble_evt_connected_t));
    usr_queue_msg_send(BLE_MSG_CONN_IND, sizeof(ble_evt_connected_t), data);

#if (BLE_HID_DEVICE)
    ln_hid_svr_enable(conn_idx);
#endif
#if (BLE_HID_REPORT_HOST)
    if(ln_kv_ble_usr_data_get()->ble_role & BLE_ROLE_CENTRAL)
    {
        ln_hid_cli_discover_server(conn_idx);
    }
#endif
}

static void ln_ble_disconnect_cb(void *arg)
{
    ble_evt_disconnected_t *evt_disconn = (ble_evt_disconnected_t *)arg;

    LOG(LOG_LVL_TRACE, "ln_ble_disconnect conn_id=%d\r\n", evt_disconn->conn_idx);

    if(BLE_CONFIG_AUTO_ADV && LE_ADV_STATE_STOPED == le_adv_state_get()
                && (ln_kv_ble_usr_data_get()->ble_role & BLE_ROLE_PERIPHERAL))
    {
        ln_ble_adv_start();
    }
}

static void ln_ble_scan_report_cb(void *arg)
{
    ble_evt_scan_report_t *p_scan_rpt = (ble_evt_scan_report_t *)arg;

    LOG(LOG_LVL_TRACE, "\r\n--------------adv_report-------------\r\n");
    LOG(LOG_LVL_TRACE, "rssi=%d, tx_pwr=%d\r\n", p_scan_rpt->rssi, p_scan_rpt->tx_pwr);
    LOG(LOG_LVL_TRACE, "addr_type=%d, addr=0x%02x:0x%02x:0x%02x:0x%02x:0x%02x:0x%02x\r\n",
            p_scan_rpt->trans_addr_type, p_scan_rpt->trans_addr[0], p_scan_rpt->trans_addr[1],
            p_scan_rpt->trans_addr[2], p_scan_rpt->trans_addr[3], p_scan_rpt->trans_addr[4], p_scan_rpt->trans_addr[5]);
    hexdump(LOG_LVL_INFO, "adv data: ", (void *)p_scan_rpt->data, p_scan_rpt->length);
    LOG(LOG_LVL_TRACE, "\r\n");
}

static void ln_ble_gatt_read_req_cb(void *arg)
{
    ble_evt_gatt_read_req_t *p_gatt_read = (ble_evt_gatt_read_req_t *)arg;
    uint8_t send_data[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37};

    LOG(LOG_LVL_TRACE, "ln_ble_gatt_read conn_id=%d,handle=%d",p_gatt_read->conidx, p_gatt_read->handle);

    p_gatt_read->length = sizeof(send_data);
    p_gatt_read->value = blib_malloc(p_gatt_read->length);
    if(NULL != p_gatt_read->value && p_gatt_read->length)
    {
        memcpy(p_gatt_read->value, send_data, p_gatt_read->length);
    }
}

static void ln_ble_gatt_write_req_cb(void *arg)
{
    ble_evt_gatt_write_req_t *p_gatt_write = (ble_evt_gatt_write_req_t *)arg;
    LOG(LOG_LVL_TRACE, "ln_ble_gatt_write conn_id=%d,handle=%d\r\n",p_gatt_write->conidx,p_gatt_write->handle);
    hexdump(LOG_LVL_INFO, "[recv data]", (void *)p_gatt_write->value, p_gatt_write->length);
}

static void ln_ble_stack_init(void)
{
    //1.controller init
    ln_bd_addr_t bt_addr = {0};
#ifdef BLE_USE_STATIC_PUBLIC_ADDR
    ln_bd_addr_t *kv_addr = ln_kv_ble_pub_addr_get();
    memcpy(&bt_addr, kv_addr, sizeof(ln_bd_addr_t));
#else
    ln_bd_addr_t *kv_addr = ln_kv_ble_pub_addr_get();
    memcpy(&bt_addr, kv_addr, sizeof(ln_bd_addr_t));
    ln_bd_addr_t static_addr = {BLE_DEFAULT_PUBLIC_ADDR};

    if(!memcmp(kv_addr->addr, static_addr.addr, LN_BD_ADDR_LEN)) {
        ln_generate_random_mac(bt_addr.addr);
        bt_addr.addr[5] |= 0xC0; // This address is random generated, so assign 0x11 => Static Random Address
        ln_kv_ble_addr_store(bt_addr);
    }
#endif
    extern void rw_init(uint8_t mac[6]);
    rw_init(bt_addr.addr);

    //2.host init
    ln_gap_app_init();
    ln_gatt_app_init();

    //3.app component init
    ln_ble_conn_mgr_init();
    ln_ble_evt_mgr_init();
    ln_ble_smp_init();
    uint8_t role = ln_kv_ble_usr_data_get()->ble_role;
    if(role & BLE_ROLE_PERIPHERAL)
        ln_ble_adv_mgr_init();
    if((role & BLE_ROLE_CENTRAL))
        ln_ble_scan_mgr_init();

#if (BLE_HID_DEVICE)
    ln_hid_server_init();
#endif
#if (BLE_HID_REPORT_HOST)
    ln_hid_client_init();
#endif

    ln_rw_app_task_init();

    //4.stack start
    ln_gap_reset();
    
    uint8_t *mac = bt_addr.addr;
    LOG(LOG_LVL_INFO, "+--------------- ble stack init ok -----------+\r\n");
    LOG(LOG_LVL_INFO, "|ble role : %-22d            |\r\n",  role);
    LOG(LOG_LVL_INFO, "|ble mac  : [%02X:%02X:%02X:%02X:%02X:%02X] %-13s |\r\n", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0], "");
    LOG(LOG_LVL_INFO, "+---------------------------------------------+\r\n");
}

static void ln_ble_app_init(void)
{
    //full stack init
    ln_ble_stack_init();

    uint8_t role = ln_kv_ble_usr_data_get()->ble_role;
    LOG(LOG_LVL_TRACE, "ble_app_init role=%d\r\n", role);

    ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_CONNECTED,    ln_ble_connect_cb);
    ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_DISCONNECTED, ln_ble_disconnect_cb);

    if(role & BLE_ROLE_PERIPHERAL)
    {
#if (BLE_HID_DEVICE)
        ln_hid_server_add();
#endif
        ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_GATT_READ_REQ,  ln_ble_gatt_read_req_cb);
        ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_GATT_WRITE_REQ, ln_ble_gatt_write_req_cb);
        /*advertising activity init*/
        adv_param_t *adv_param = &le_adv_mgr_info_get()->adv_param;
#ifdef BLE_USE_LEGACY_ADV
        adv_param->adv_type = GAPM_ADV_TYPE_LEGACY;
        adv_param->adv_prop = GAPM_ADV_PROP_UNDIR_CONN_MASK;
        ln_ble_adv_actv_creat(adv_param);
        /*set advertising data*/
        app_set_adv_data();
        /*set scan respond data*/
        app_set_scan_resp_data();
#else
        adv_param->adv_type = GAPM_ADV_TYPE_EXTENDED;
        adv_param->adv_prop = GAPM_EXT_ADV_PROP_UNDIR_CONN_MASK;
        ln_ble_adv_actv_creat(adv_param);
        /*set extended advertising data*/
        app_set_ext_adv_data();
        /*set extended scan respond data*/
        //app_set_ext_scan_resp_data();
#endif
        /*start advertising*/
        ln_ble_adv_start();
    }

    if((role & BLE_ROLE_CENTRAL))
    {
#if (BLE_HID_REPORT_HOST)
        ln_hid_client_add();
#endif
        ln_ble_evt_mgr_reg_evt(BLE_EVT_ID_SCAN_REPORT, ln_ble_scan_report_cb);

        /*scan activity init*/
        ln_ble_scan_actv_creat();
        /*start scan*/
        ln_ble_scan_start(&le_scan_mgr_info_get()->scan_param);
        /*connect activity init*/
        ln_ble_init_actv_creat();
    }
}

void ble_app_task_entry(void *params)
{
    ble_usr_msg_t usr_msg;

    ln_kv_ble_app_init();

    usr_queue_creat();
    user_timer_creat();
    ln_ble_app_init();

    while(1)
    {
        if(OS_OK == usr_queue_msg_recv((void *)&usr_msg, OS_WAIT_FOREVER))
        {
            switch(usr_msg.id)
            {
                case BLE_MSG_CONN_IND:
                {
                    ble_evt_connected_t *p_param = (ble_evt_connected_t *)(usr_msg.msg);
                    ln_gatt_exc_mtu(p_param->conn_idx);

                    ln_gap_set_le_pkt_size_t pkt_size;
                    pkt_size.tx_octets = 251;
                    pkt_size.tx_time   = 2120;
                    ln_gap_set_le_pkt_size(p_param->conn_idx,  &pkt_size);

                    if(!p_param->role)
                        OS_MsDelay(100);
                    else
                        OS_MsDelay(1000);
                    ln_ble_conn_param_t conn_param;
                    conn_param.intv_min = 80;
                    conn_param.intv_max = 90;
                    conn_param.latency = 10;
                    conn_param.time_out = 3000;
                    ln_ble_conn_param_update(p_param->conn_idx, &conn_param);
                }
                    break;

                case BLE_MSG_DISCONN_IND:
                    break;

                default:
                    break;
            }
            blib_free(usr_msg.msg);
        }
    }
}

void ble_creat_usr_app_task(void)
{
    if(OS_OK != OS_ThreadCreate(&ble_g_usr_app_thread, "BleUsrAPP", ble_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, BLE_USR_APP_TASK_STACK_SIZE)) 
    {
        LN_ASSERT(1);
    }
}

