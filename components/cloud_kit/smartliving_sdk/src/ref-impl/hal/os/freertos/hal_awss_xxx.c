#include <stdio.h>
#include <stdbool.h>
#include "iot_import.h"

#include "netif/ethernetif.h"
#include "wifi_lib_export/wifi.h"
#include "wifi_lib_import/ln_wifi_err.h"
#include "wifi_manager/wifi_manager.h"
#include "utils/system_parameter.h"
#include "utils/sysparam_factory_setting.h"
#include "utils/ln_misc.h"
#include "utils/ln_psk_calc.h"
#include "utils/debug/log.h"

static awss_recv_80211_frame_cb_t sg_awss_recv_80211_cb = NULL;

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////  LN PORTING APIs  //////////////////////////////
////////////////////////////////////////////////////////////////////////////////
static uint8_t sg_mac_addr[6]   = {0x00, 0x50, 0xC2, 0x5E, 0x88, 0x99};

static uint8_t psk_value[40]    = {0x0};

static uint8_t sg_sta_got_ip    = 0;

static void _netdev_sta_get_ip_cb(struct netif *nif)
{
    if (NULL == nif) {
        return;
    }

    if (netdev_get_active() == NETIF_IDX_STA) {
        sg_sta_got_ip = 1;
    }
}

/**
 * @brief
 *
 * @return int
 */
static int _port_wifi_sta_init(void)
{

    //1. sta mac get
    if (SYSPARAM_ERR_NONE != sysparam_sta_mac_get(sg_mac_addr)) {
        LOG(LOG_LVL_ERROR, "Failed to get STA MAC.");
        return -1;
    }

    if (sg_mac_addr[0] == STA_MAC_ADDR0 &&
        sg_mac_addr[1] == STA_MAC_ADDR1 &&
        sg_mac_addr[2] == STA_MAC_ADDR2 &&
        sg_mac_addr[3] == STA_MAC_ADDR3 &&
        sg_mac_addr[4] == STA_MAC_ADDR4 &&
        sg_mac_addr[5] == STA_MAC_ADDR5) {
        ln_generate_random_mac(sg_mac_addr);
        sysparam_sta_mac_update((const uint8_t *)sg_mac_addr);
    }

    wifi_mode_t  cur_mode = wifi_current_mode_get();
    LOG(LOG_LVL_DEBUG, "current wifi mdoe: %d", (int) cur_mode);

    if (WIFI_MODE_STATION != cur_mode) {
        wifi_stop();
    }

    //2. net device(lwip)
    netdev_set_mac_addr(NETIF_IDX_STA, sg_mac_addr);
    netdev_set_active(NETIF_IDX_STA);
    netdev_get_ip_cb_set(_netdev_sta_get_ip_cb);

    //3. wifi start
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, NULL);

    if(WIFI_ERR_NONE != wifi_sta_start(sg_mac_addr, WIFI_NO_POWERSAVE)){
        LOG(LOG_LVL_ERROR, "Failed to start wifi");
        return -1;
    }

    return 0;
}

int port_wifi_sta_start_connect(const char* ssid, const char*pwd, uint8_t chan)
{
    if ( (NULL == ssid) || (NULL == pwd) ) {
        return -1;
    }

    wifi_scan_cfg_t scan_cfg = {
            .channel   = chan,
            .scan_type = WIFI_SCAN_TYPE_ACTIVE,
            .scan_time = 20,
    };

    wifi_sta_connect_t sta_connect_cfg = {
            .ssid    = (char *)ssid,
            .pwd     = (char *)pwd,
            .bssid   = NULL,
            .psk_value = NULL,
    };


    sta_connect_cfg.psk_value = NULL;
    if (strlen(sta_connect_cfg.pwd) != 0) {
        if (0 == ln_psk_calc(sta_connect_cfg.ssid, sta_connect_cfg.pwd, psk_value, sizeof (psk_value))) {
            sta_connect_cfg.psk_value = psk_value;
            // hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    scan_cfg.channel = chan;

    if (0 != _port_wifi_sta_init()) {
        return -1;
    }

    if ( 0 != wifi_sta_connect(&sta_connect_cfg, &scan_cfg)) {
        return -1;
    } else {
        return 0;
    }
}

bool port_wifi_is_sta_got_ip(void)
{
    return (sg_sta_got_ip == 1);
}


/**
 * @brief 中间层，其目的是为了调用 awss_recv_80211_frame_cb_t 类型的回调函数
 *
 * @param buffer
 * @param len
 * @param rssi_dbm
 * @param buffer_type
 */
static void _ln_wifi_mgmt_frame_cb (uint8_t *buffer, int len, signed char rssi_dbm, int buffer_type)
{
    enum AWSS_LINK_TYPE link_type = AWSS_LINK_TYPE_NONE;
    int with_fcs = 0;

    // hexdump(LOG_LVL_INFO, "_ln_wifi_mgmt_frame_cb", buffer, (len > 64) ? (64) : (len));

    if (NULL != sg_awss_recv_80211_cb) {
        sg_awss_recv_80211_cb((char *)buffer, len, link_type, with_fcs, rssi_dbm);
    }
}

/**
 * @brief   设置Wi-Fi网卡工作在监听(Monitor)模式, 并在收到802.11帧的时候调用被传入的回调函数
 *
 * @param[in] cb @n A function pointer, called back when wifi receive a frame.
 */
void HAL_Awss_Open_Monitor(_IN_ awss_recv_80211_frame_cb_t cb)
{
    if (NULL == cb) {
        return;
    }

    if (WIFI_MODE_AP == wifi_current_mode_get()) {
        wifi_stop();
    }
    _port_wifi_sta_init();

    sg_awss_recv_80211_cb = cb;
    HAL_Wifi_Enable_Mgmt_Frame_Filter(0, NULL, _ln_wifi_mgmt_frame_cb);
    HAL_Awss_Switch_Channel(1, 0, NULL);
}

/**
 * @brief   设置Wi-Fi网卡离开监听(Monitor)模式, 并开始以站点(Station)模式工作
 */
void HAL_Awss_Close_Monitor(void)
{
    HAL_Wifi_Enable_Mgmt_Frame_Filter(0, NULL, NULL);
    sg_awss_recv_80211_cb = NULL;
    // TODO: 切换到 STA 模式
}

/**
 * @brief   要求Wi-Fi网卡连接指定热点(Access Point)的函数
 *
 * @param[in] connection_timeout_ms @n AP connection timeout in ms or HAL_WAIT_INFINITE
 * @param[in] ssid @n AP ssid
 * @param[in] passwd @n AP passwd
 * @param[in] auth @n optional(AWSS_AUTH_TYPE_INVALID), AP auth info
 * @param[in] encry @n optional(AWSS_ENC_TYPE_INVALID), AP encry info
 * @param[in] bssid @n optional(NULL or zero mac address), AP bssid info
 * @param[in] channel @n optional, AP channel info
 * @return
   @verbatim
     = 0: connect AP & DHCP success
     = -1: connect AP or DHCP fail/timeout
   @endverbatim
 * @see None.
 * @note
 *      If the STA connects the old AP, HAL should disconnect from the old AP firstly.
 *      If bssid specifies the dest AP, HAL should use bssid to connect dest AP.
 */
int HAL_Awss_Connect_Ap(
    _IN_ uint32_t connection_timeout_ms,
    _IN_ char ssid[HAL_MAX_SSID_LEN],
    _IN_ char passwd[HAL_MAX_PASSWD_LEN],
    _IN_OPT_ enum AWSS_AUTH_TYPE auth,
    _IN_OPT_ enum AWSS_ENC_TYPE encry,
    _IN_OPT_ uint8_t bssid[ETH_ALEN],
    _IN_OPT_ uint8_t channel)
{
    wifi_stop();

    port_wifi_sta_start_connect(ssid, passwd, channel);
    return SUCCESS_RETURN;
}

/**
 * @brief   获取在每个信道(`channel`)上扫描的时间长度, 单位是毫秒
 *
 * @return  时间长度, 单位是毫秒
 * @note    推荐时长是200毫秒到400毫秒
 */
int HAL_Awss_Get_Channelscan_Interval_Ms(void)
{
    int scan_time = STA_SCAN_TIME;
    wifi_scan_cfg_t cfg = { 0 };

    if ( WIFI_ERR_NONE == wifi_get_sta_scan_cfg(&cfg) ) {
        scan_time = cfg.scan_time;
    }

    scan_time = 200; // TODO: 根据需要调整

    return scan_time;
}

/**
 * @brief   获取配网服务(`AWSS`)的超时时间长度, 单位是毫秒
 *
 * @return  超时时长, 单位是毫秒
 * @note    推荐时长是60,0000毫秒
 */
int HAL_Awss_Get_Timeout_Interval_Ms(void)
{
    return 1000 * 60 * 3;
}

/**
 * @brief   获取`smartconfig`服务的安全等级
 *
 * @param None.
 * @return The security level:
   @verbatim
    0: open (no encrypt)
    1: aes256cfb with default aes-key and aes-iv
    2: aes128cfb with default aes-key and aes-iv
    3: aes128cfb with aes-key per product and aes-iv = 0
    4: aes128cfb with aes-key per device and aes-iv = 0
    5: aes128cfb with aes-key per manufacture and aes-iv = 0
    others: invalid
   @endverbatim
 * @see None.
 */
int HAL_Awss_Get_Encrypt_Type(void)
{
    return 3;
}

/**
 * @brief    Get Security level for wifi configuration with connection.
 *           Used for AP solution of router and App.
 *
 * @param None.
 * @return The security level:
   @verbatim
    3: aes128cfb with aes-key per product and aes-iv = random
    4: aes128cfb with aes-key per device and aes-iv = random
    5: aes128cfb with aes-key per manufacture and aes-iv = random
    others: invalid
   @endverbatim
 * @see None.
 */
int HAL_Awss_Get_Conn_Encrypt_Type(void)
{
    // TODO: 热点配网的安全等级
    char invalid_ds[DEVICE_SECRET_LEN + 1] = {0};
    char ds[DEVICE_SECRET_LEN + 1] = {0};

    HAL_GetDeviceSecret(ds);

    if (memcmp(invalid_ds, ds, sizeof(ds)) == 0)
        return 3;

    memset(invalid_ds, 0xff, sizeof(invalid_ds));
    if (memcmp(invalid_ds, ds, sizeof(ds)) == 0)
        return 3;

    return 4;
}

static void ap_startup_cb(void * arg)
{
    netdev_set_state(NETIF_IDX_AP, NETDEV_UP);
}

/**
  * @brief   开启设备热点（SoftAP模式）
  *
  * @param[in] ssid @n 热点的ssid字符；
  * @param[in] passwd @n 热点的passwd字符；
  * @param[in] beacon_interval @n 热点的Beacon广播周期（广播间隔）；
  * @param[in] hide @n 是否是隐藏热点，hide:0, 非隐藏, 其它值：隐藏；
  * @return，
 @verbatim
    = 0: success
    = -1: unsupported
    = -2: failure with system error
    = -3: failure with no memory
    = -4: failure with invalid parameters
 @endverbatim
  * @Note:
  *       1）ssid和passwd都是以'\0'结尾的字符串，如果passwd字符串的
  *          长度为0，表示该热点采用Open模式（不加密）；
  *       2）beacon_interval表示热点的Beacon广播间隔（或周期），单
  *          位为毫秒，一般会采用默认100ms；
  *       3）hide表示创建的热点是否是隐藏热点，hide=0表示非隐藏热
  *         点，其他值表示隐藏热点；
  */
int HAL_Awss_Open_Ap(const char *ssid, const char *passwd, int beacon_interval, int hide)
{
    // FIXME: 建立 softAP
    if (wifi_current_mode_get() == WIFI_MODE_STATION) {
        wifi_stop();
    }

    if (NULL == ssid) {
        return -4;
    }
    static uint8_t mac_addr[6]        = {0x00, 0x50, 0xC2, 0x5E, 0x88, 0x99};
    tcpip_ip_info_t  ip_info;
    server_config_t  server_config;
    wifi_softap_cfg_t ap_cfg = {
            .ssid            = "LN_AP",
            .pwd             = "12345678",
            .bssid           = mac_addr,
            .ext_cfg = {
                    .channel         = 6,
                    .authmode        = WIFI_AUTH_WPA_WPA2_PSK,//WIFI_AUTH_OPEN,
                    .ssid_hidden     = 0,
                    .beacon_interval = 100,
                    .psk_value = NULL,
            }
    };

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

    ap_cfg.ssid = ssid;
    if ( (NULL != passwd) && strlen(passwd) > 0) {
        ap_cfg.pwd = passwd;
    } else {
        ap_cfg.ext_cfg.authmode = WIFI_AUTH_OPEN;
    }
    ap_cfg.ext_cfg.beacon_interval = beacon_interval;
    ap_cfg.ext_cfg.ssid_hidden = hide;

    //1. net device(lwip).
    netdev_set_mac_addr(NETIF_IDX_AP, mac_addr);
    netdev_set_ip_info(NETIF_IDX_AP, &ip_info);
    netdev_set_active(NETIF_IDX_AP);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_STARTUP, &ap_startup_cb);

    sysparam_softap_mac_update((const uint8_t *)mac_addr);

    ap_cfg.ext_cfg.psk_value = NULL;
    if ((strlen(ap_cfg.pwd) != 0) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_OPEN) &&
        (ap_cfg.ext_cfg.authmode != WIFI_AUTH_WEP)) {
        memset(psk_value, 0, sizeof(psk_value));
        if (0 == ln_psk_calc(ap_cfg.ssid, ap_cfg.pwd, psk_value, sizeof (psk_value))) {
            ap_cfg.ext_cfg.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

    //2. wifi
    if(WIFI_ERR_NONE !=  wifi_softap_start(&ap_cfg)){
        LOG(LOG_LVL_ERROR, "[%s, %d]wifi_start() fail.\r\n", __func__, __LINE__);
    }

    return 0;
}

/**
  * @brief   关闭当前设备热点，并把设备由SoftAP模式切换到Station模式
  *
  * @return，
 @verbatim
    = 0: success
    = -1: unsupported
    = -2: failure
 @endverbatim
  * @Note:
  *       1）如果当前设备已经开启热点，关闭当前热点，如果当前设备正
  *          在开热点，取消开热点的操作；
  *       2）如果当前设备不是以Station模式（包括Station+SoftAP模式和
  *          SoftAP模式）工作，设备必须切换到Station模式；
  *       3）Wi-Fi状态机需要切换到初始化状态，因为接下来很可能进行
  *          连接某一个路由器操作；
  */
int HAL_Awss_Close_Ap(void)
{
    wifi_stop();

    // NOTE: 以STA模式工作
    _port_wifi_sta_init();
    return 0;
}

/**
 * @brief   设置Wi-Fi网卡切换到指定的信道(channel)上
 *
 * @param[in] primary_channel @n Primary channel.
 * @param[in] secondary_channel @n Auxiliary channel if 40Mhz channel is supported, currently
 *              this param is always 0.
 * @param[in] bssid @n A pointer to wifi BSSID on which awss lock the channel, most HAL
 *              may ignore it.
 */
void HAL_Awss_Switch_Channel(char primary_channel, char secondary_channel, uint8_t bssid[ETH_ALEN])
{
    wifi_set_channel( (uint8_t)primary_channel );
}