#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "utils/system_parameter.h"
#include "wifi.h"
#include "ln_wifi_err.h"
#include "netif/ethernetif.h"
#include "dhcpd_api.h"
#include "wifi_manager.h"

#include "ln_app_gap.h"
#include "gapm_task.h"
#include "ln_app_gatt.h"
#include "ln_app_callback.h"
#include "usr_ble_app.h"
#include "usr_send_data.h"

#include "ln_joylink.h"

#include "joylink_ble_sdk.h"
#include "joylink_stdio.h"
#include "joylink_thread.h"
#include "joylink_extern.h"
#include "joylink.h"
#include "joylink_ble_sdk.h"


extern uint8_t svc_uuid[16];
extern uint8_t con_num;
static uint8_t m_ble_con = 0;
static int m_ble_state = -1;
uint8_t adv_actv_idx  = 0;
uint8_t init_actv_idx = 0;
static wifi_softap_cfg_t softap_cfg = {
    .ssid            = LN_AP_SSID,
    .pwd             = "",
    .bssid           = NULL,
    .ext_cfg = {
        .channel         = 6,
        .authmode        = WIFI_AUTH_OPEN,
        .ssid_hidden     = 0,
        .beacon_interval = 100,
        .psk_value = NULL,
    }
};

static int m_ln_net_config = JL_NET_CONFIG_END;

void ln_set_ble_connect_flag(int flag)
{
    m_ble_state = flag;
}

int ln_get_ble_connect_flag(void)
{
    return m_ble_state;
}

int ln_get_net_config_state(void)
{
    return m_ln_net_config;
}

void ln_set_ble_con(uint8_t conidx)
{
    m_ble_con =conidx;
    LOG(LOG_LVL_INFO, "[%s:%d]ble_con=%d\r\n", __func__, __LINE__,m_ble_con);
}


uint8_t ln_get_ble_con(void)
{
    LOG(LOG_LVL_INFO, "[%s:%d]ble_con=%d\r\n", __func__, __LINE__, m_ble_con);
    return m_ble_con;
}

uint16_t ln_get_handle_ind(void)
{
    uint16_t handle = gattm_get_start_hdl() + DATA_TRANS_CHAR_VAL_TX + 1;
    LOG(LOG_LVL_INFO, "[%s:%d]handle=%d\r\n", __func__, __LINE__, handle);
    return handle;
}

int32_t ln_send_net_config_state(E_JL_NET_CONF_ST st, uint8_t* data, int32_t data_len)
{
    if(LN_NET_CONFIG_MODE == JL_NET_CONFIG_BLE && ln_get_ble_connect_flag() == 0)
    {
        jl_send_net_config_state(st, data, data_len);
    }
}

static void ln_ble_create_advertising(void)
{
#define APP_ADV_CHMAP                (0x07)  // Advertising channel map - 37, 38, 39
#define APP_ADV_INT_MIN              (640)   // Advertising minimum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MAX              (640)   // Advertising maximum interval - 40ms (64*0.625ms)

    struct ln_gapm_activity_create_adv_cmd  adv_creat_param = {0};

    adv_creat_param.own_addr_type                     = GAPM_STATIC_ADDR;
    adv_creat_param.adv_param.type                    = GAPM_ADV_TYPE_LEGACY;//GAPM_ADV_TYPE_EXTENDED;//GAPM_ADV_TYPE_LEGACY;
    adv_creat_param.adv_param.filter_pol              = ADV_ALLOW_SCAN_ANY_CON_ANY;
    adv_creat_param.adv_param.prim_cfg.chnl_map       = APP_ADV_CHMAP;
    adv_creat_param.adv_param.prim_cfg.phy            = GAP_PHY_1MBPS;
    adv_creat_param.adv_param.prop                    = GAPM_ADV_PROP_UNDIR_CONN_MASK;//GAPM_ADV_PROP_NON_CONN_SCAN_MASK;//GAPM_ADV_PROP_UNDIR_CONN_MASK;//GAPM_ADV_PROP_UNDIR_CONN_MASK;//GAPM_EXT_ADV_PROP_UNDIR_CONN_MASK;//GAPM_ADV_PROP_UNDIR_CONN_MASK;
    adv_creat_param.adv_param.disc_mode               = GAPM_ADV_MODE_GEN_DISC;
    adv_creat_param.adv_param.prim_cfg.adv_intv_min   = APP_ADV_INT_MIN;
    adv_creat_param.adv_param.prim_cfg.adv_intv_max   = APP_ADV_INT_MAX;
    adv_creat_param.adv_param.max_tx_pwr              = 0;
    //adv_creat_param.adv_param.second_cfg.phy        = GAP_PHY_1MBPS;//GAP_PHY_1MBPS;//GAP_PHY_CODED;
    adv_creat_param.adv_param.second_cfg.max_skip     = 0x00;
    adv_creat_param.adv_param.second_cfg.phy          = 0x01;
    adv_creat_param.adv_param.second_cfg.adv_sid      = 0x00;
    adv_creat_param.adv_param.period_cfg.adv_intv_min = 0x0400;
    adv_creat_param.adv_param.period_cfg.adv_intv_max = 0x0400;
    ln_app_advertise_creat(&adv_creat_param);
}

static void ln_ble_set_adv_data(void)
{
    int i = 0;
    //adv data: adv length--adv type--adv string ASCII
    uint8_t adv_data[ADV_DATA_MAX_LENGTH] = {0};
    jl_gap_data_t gap_data;
    jl_get_gap_config_data(&gap_data);
    adv_data[0] = 3;
    adv_data[1] = 0x03;  //adv type :local name
    memcpy(&adv_data[2],gap_data.service_uuid16,2);
    
    adv_data[4] = DEVICE_NAME_LEN + 1;
    adv_data[5] = 0x09;  //adv type :local name
    memcpy(&adv_data[6],DEVICE_NAME,DEVICE_NAME_LEN);
    
    adv_data[6+DEVICE_NAME_LEN ] = 14 + 1;
    adv_data[6+DEVICE_NAME_LEN + 1] = 0xFF;
    memcpy(&adv_data[5+DEVICE_NAME_LEN + 3],gap_data.manufacture_data,14);

    struct ln_gapm_set_adv_data_cmd adv_data_param;
    adv_data_param.actv_idx = adv_actv_idx;
    adv_data_param.length = sizeof(adv_data);
    adv_data_param.data = adv_data;
    ln_app_set_adv_data(&adv_data_param);
}

static void ln_ble_start_advertising(void)
{
    struct ln_gapm_activity_start_cmd  adv_start_param;
    adv_start_param.actv_idx = adv_actv_idx;
    adv_start_param.u_param.adv_add_param.duration = 0;
    adv_start_param.u_param.adv_add_param.max_adv_evt = 0;
    ln_app_advertise_start(&adv_start_param);
}

void ln_ble_stop_advertising(void)
{
    ln_app_activity_stop(adv_actv_idx);
}

void ln_ble_restart_adv(void)
{
    ln_ble_start_advertising();
}

static void ln_ble_start_adv(void)
{
    ln_ble_create_advertising();
    ln_ble_set_adv_data();
    ln_ble_start_advertising();
}

static OS_Queue_t ble_usr_queue;
static OS_Semaphore_t usr_semaphore;

void usr_creat_queue(void)
{
    if(OS_OK != OS_QueueCreate(&ble_usr_queue, BLE_USR_MSG_QUEUE_SIZE, sizeof(ble_usr_msg_t)))
    {
        BLIB_LOG(BLIB_LOG_LVL_E, "usr QueueCreate rw_queue failed!!!\r\n");
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

int ln_stop_ble_netconfig(void)
{
    if (LN_NET_CONFIG_MODE != JL_NET_CONFIG_BLE) {
        return 0;
    }
    
    if (ln_get_ble_connect_flag() == 0) {
        ln_app_disconnect(ln_get_ble_con(), 0x16);
    }
    
    if (m_ln_net_config == JL_NET_CONFIG_ING) {
        ln_ble_stop_advertising();
    }
    
    m_ln_net_config = JL_NET_CONFIG_END;
    return 0;
}

int ln_cancel_ble_netconfig(void)  //认证测试case用
{
    if (ln_get_ble_connect_flag() == 0) {
        ln_send_net_config_state(E_JL_NET_CONF_ST_EXIT, NULL, 0); 
    }
    
    ln_stop_ble_netconfig();
}

static void ln_ble_app_task_entry(void *params)
{
    int task_start = OS_GetTime();
    int curr_time = OS_GetTime();
    ble_usr_msg_t usr_msg;

    usr_creat_queue();

    extern void ble_app_init(void);
    ble_app_init();

    ln_ble_start_adv();
    data_trans_svc_add();

    m_ln_net_config = JL_NET_CONFIG_ING;
    while(1)
    {
        if(OS_OK == usr_queue_msg_recv((void *)&usr_msg, 200))
        {
            LOG(LOG_LVL_TRACE, "connect device number :%d ,msgid = %d\r\n",con_num,usr_msg.id);
            switch(usr_msg.id)
            {
                case BLE_MSG_WRITE_DATA:
                {
                    struct ln_attc_write_req_ind *p_param = (struct ln_attc_write_req_ind *)usr_msg.msg;
                    struct ln_gattc_send_evt_cmd send_data;
                    hexdump(LOG_LVL_INFO, "[recv data]", (void *)p_param->value, p_param->length);
                    
                    if(p_param->length > 2)
                    {
                        jl_write_data(p_param->value, p_param->length);
                    }
                }
                break;

                default:
                    break;
            }
            blib_free(usr_msg.msg);
        }
        else
        {
            curr_time = OS_GetTime();
            if(curr_time - task_start >= LN_NET_CONFIG_MAX_TIMEOUT/1000)
            {
                LOG(LOG_LVL_ERROR, "ln_ble_app_task_entry timeout\r\n");
                ln_stop_ble_netconfig();
                break;
            }
        }
    }

    OS_ThreadDelete(NULL);
}

#include "joylink_ble_sdk.h"
extern int joylink_util_hexStr2bytes(const char *hexStr, uint8_t *buf, int bufLen);

static void ln_start_ble(void)
{
    static OS_Thread_t ble_g_usr_app_thread = {0};

    if(OS_OK != OS_ThreadCreate(&ble_g_usr_app_thread, "BleUsrAPP", ln_ble_app_task_entry, NULL, \
                                OS_PRIORITY_BELOW_NORMAL, 2048)) 
    {
        LN_ASSERT(1);
    }
}

int ln_start_connect_ap(char *ssid, char *passwd)
{
#define WIFI_CONN_DHCP_TIMEOUT_MS    (15*1000) 

    //1. wifi,netdev shutdown
    if (LN_TRUE == wifi_is_startup()) {
        wifi_stop();
        wifi_mode_t mode = wifi_current_mode_get();
        if (mode ==WIFI_MODE_STATION) {
            netdev_set_state(NETIF_IDX_STA, NETDEV_DOWN);
        } else if(mode ==WIFI_MODE_AP) {
            netdev_set_state(NETIF_IDX_AP, NETDEV_DOWN);
        }
    }

    //2. set netdev(lwip) 
    netdev_set_mac_addr(NETIF_IDX_STA, ln_get_mac_array());
    netdev_set_active(NETIF_IDX_STA);

    //3. wifi start
    wifi_sta_start(ln_get_mac_array(), WIFI_NO_POWERSAVE);

    //4. wifi station join
    wifi_sta_connect_t conn = {0};
    uint8_t psk_value[40]   = {0};//TODO: 超大局部变量

    conn.ssid      = ssid;
    conn.pwd       = passwd;
    conn.bssid     = NULL;
    conn.psk_value = NULL;

    if (strlen(conn.pwd) != 0) {
        if (0 == ln_psk_calc(conn.ssid, conn.pwd, psk_value, sizeof (psk_value))) {
            conn.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    wifi_scan_cfg_t scan_cfg = {0};
    sysparam_sta_scan_cfg_get(&scan_cfg);

    if (WIFI_ERR_NONE != wifi_sta_connect(&conn, &scan_cfg)) {
        LOG(LOG_LVL_ERROR, "Join failed! ssid:%s, pwd:%s\r\n", conn.ssid, conn.pwd);
        return -1;
    }

    if (SYSPARAM_ERR_NONE != sysparam_sta_conn_cfg_update(&conn)) {
        LOG(LOG_LVL_ERROR, "sysparam_sta_conn_cfg_update failed\r\n");
        return -1;
    }

    //5. waite for join&DHCP succeccful
    int16_t count_100ms = WIFI_CONN_DHCP_TIMEOUT_MS / 100;

    while (NETDEV_LINK_UP != netdev_get_link_state(NETIF_IDX_STA))
    {
        OS_MsDelay(100);

        if (count_100ms-- < 0)
        {
            LOG(LOG_LVL_ERROR, "wifi connect ap timeout!\r\n");

            if (LN_NET_CONFIG_MODE == JL_NET_CONFIG_BLE) {
                ln_send_net_config_state(E_JL_NET_CONF_ST_WIFI_CONNECT_FAILED, NULL, 0);
                LOG(LOG_LVL_INFO, "jl_send_net_config_state E_JL_NET_CONF_ST_WIFI_CONNECT_FAILED\r\n");
                return -1;
            } else if (LN_NET_CONFIG_MODE == JL_NET_CONFIG_SOFTAP) {
                //TODO:
                return -1;
            } else {
                LOG(LOG_LVL_ERROR, "LN_NET_CONFIG_MODE(mode:%d) not support! \r\n", LN_NET_CONFIG_MODE);
            }

            break;
        }
    }

    return 0;
}

static void ln_ap_startup_cb(void * arg)
{
    netdev_set_state(NETIF_IDX_AP, NETDEV_UP);
}

static void ln_wifi_init_ap(void)
{
#define WIFI_PSK_LEN  (40)
    uint8_t *psk = NULL;
    tcpip_ip_info_t  ip_info = {0};
    server_config_t  server_config = {0};

    ip_info.ip.addr      = ipaddr_addr((const char *)"192.168.4.1");
    ip_info.gw.addr      = ipaddr_addr((const char *)"192.168.4.1");
    ip_info.netmask.addr = ipaddr_addr((const char *)"255.255.255.0");

    server_config.server.addr   = ip_info.ip.addr;
    server_config.port          = 67;
    server_config.lease         = 2880;
    server_config.renew         = 2880;
    server_config.ip_start.addr = ipaddr_addr((const char *)"192.168.4.100");
    server_config.ip_end.addr   = ipaddr_addr((const char *)"192.168.4.150");
    server_config.client_max    = 3;
    dhcpd_curr_config_set(&server_config);

    //1. net device(lwip).
    netdev_set_mac_addr(NETIF_IDX_AP, ln_get_mac_array());
    netdev_set_ip_info(NETIF_IDX_AP, &ip_info);
    netdev_set_active(NETIF_IDX_AP);

    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_STARTUP, &ln_ap_startup_cb);
    sysparam_softap_mac_update((const uint8_t *)ln_get_mac_array());

    softap_cfg.ext_cfg.psk_value = NULL;
    softap_cfg.bssid = ln_get_mac_array();
    if ((strlen(softap_cfg.pwd) != 0) &&
        (softap_cfg.ext_cfg.authmode != WIFI_AUTH_OPEN) &&
        (softap_cfg.ext_cfg.authmode != WIFI_AUTH_WEP))
    {
        if (NULL != (psk = OS_Malloc(WIFI_PSK_LEN))) {
            if (0 == ln_psk_calc(softap_cfg.ssid, softap_cfg.pwd, psk, WIFI_PSK_LEN)) {
                softap_cfg.ext_cfg.psk_value = psk;
                hexdump(LOG_LVL_INFO, "psk value ", psk, WIFI_PSK_LEN);
            }
            OS_Free(psk);
        } else {
            LOG(LOG_LVL_ERROR, "[%s, %d] psk malloc fail.\r\n", __func__, __LINE__);
        }
    }

    //2. wifi start
    if (WIFI_ERR_NONE !=  wifi_softap_start(&softap_cfg)) {
        LOG(LOG_LVL_ERROR, "[%s, %d]wifi_start() fail.\r\n", __func__, __LINE__);
    }
}

int ln_check_dev_bind(void)
{
    int ret = 0;
    JLPInfo_t *pjlinfo = NULL;
    wifi_sta_connect_t conn = {0};
    char ssid[SSID_MAX_LEN] = {0};    //TODO: malloc
    char pwd[PASSWORD_MAX_LEN] = {0}; //TODO: malloc
    char bssid[BSSID_LEN] = {0};
    conn.ssid  = ssid;
    conn.pwd   = pwd;
    conn.bssid = bssid;

    if (NULL == (pjlinfo = OS_Malloc(sizeof(JLPInfo_t)))) {
        LOG(LOG_LVL_ERROR, "[%s]pjlinfo malloc failed!\r\n", __func__);
        return -1;
    }

    /* 1. check device-bind */
    ret = joylink_dev_get_jlp_info(pjlinfo);
    if((ret == E_RET_OK) && \
        (strlen(pjlinfo->domain) > 0) && \
        (strlen(pjlinfo->token) > 0) && \
        (strlen(pjlinfo->feedid) > 0))
    {
        /* 2. check wifi conn param, don't check pwd!*/
        ret = sysparam_sta_conn_cfg_get(&conn);
        if ((SYSPARAM_ERR_NONE == ret) && (strlen(conn.ssid) > 0))
        {
            ln_start_connect_ap(conn.ssid, conn.pwd);
            ret = 0;
            goto __exit;
        } 
    }

    ret = -1;
__exit:
    OS_Free(pjlinfo);
    return ret;
}

static void jl_ble_init(void)
{
    char mac_str[17] = {0};
    jl_dev_info_t info = {0};

    LOG(LOG_LVL_INFO, "[%s] %s\r\n", __func__, JLP_UUID);

    jl_platform_memset(&info, 0, sizeof(jl_dev_info_t));
    jl_platform_strcpy(info.product_uuid, JLP_UUID);
    jl_platform_strcpy(info.shared_key, "1234567890123456"); //TODO:
    joylink_dev_get_user_mac(mac_str);
    joylink_util_hexStr2bytes(mac_str, info.mac, 6);
    jl_init(&info);
}

void ln_enter_netconfig_mode(void)
{
    if (LN_NET_CONFIG_MODE == JL_NET_CONFIG_SOFTAP) {
        ln_wifi_init_ap();
        jl_softap_enter_ap_mode();//阻塞 timeout
    } else if (LN_NET_CONFIG_MODE == JL_NET_CONFIG_BLE) {
        jl_ble_init();
        ln_start_ble(); //TODO: maybe no need to creat ble task
    } else {
        LOG(LOG_LVL_ERROR, "LN_NET_CONFIG_MODE(mode:%d) not support!\r\n", LN_NET_CONFIG_MODE);
        LN_ASSERT(1);
    }
}
