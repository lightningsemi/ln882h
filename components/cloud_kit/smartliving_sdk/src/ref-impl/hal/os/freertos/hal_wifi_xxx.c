#include <string.h>
#include "iot_import.h"

// LN headers
#include "ln_utils.h"
#include "wifi_lib_export/wifi.h"
#include "wifi_lib_import/ln_wifi_err.h"
#include "wifi_manager/wifi_manager.h"
#include "netif/ethernetif.h"
#include "utils/system_parameter.h"
#include "utils/debug/log.h"
#include "utils/sysparam_factory_setting.h"

// `HAL_Wifi_Enable_Mgmt_Frame_Filter()` monitor adapter
static  awss_wifi_mgmt_frame_cb_t   sg_monitor_cb = NULL;
static void _ln_wifi_monitor_cb_adapter(void *buf, uint16_t len, wifi_pkt_type_t pkt_type, int8_t rssi_db);
static void _ln_wifi_monitor_cb_empty(void *buf, uint16_t len, wifi_pkt_type_t pkt_type, int8_t rssi_db);

// `HAL_Wifi_Scan()` 是一个阻塞型的API，必须把扫描到的每个AP信息都处理一遍然后才能退出当前API。
static             OS_Semaphore_t   sg_sta_scan_deal_sem = { 0 };
static awss_wifi_scan_result_cb_t   sg_scan_result_cb = NULL;

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////  LN WiFi adapter  ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
int auth_mode_to_ln(int auth_type);
int auth_mode_from_ln(int auth_mode);


/**
 * @brief Authenticate mode conversion from LN `wifi_auth_mode_t` to
 * IOT `enum AWSS_AUTH_TYPE`.
 *
 * @param auth_mode
 * @return int
 */
int auth_mode_from_ln(int auth_mode)
{
    enum AWSS_AUTH_TYPE ret = AWSS_AUTH_TYPE_INVALID;

    switch ( (wifi_auth_mode_t) auth_mode)
    {
        case WIFI_AUTH_OPEN: {
            ret = AWSS_AUTH_TYPE_OPEN;
            break;
        }
        case WIFI_AUTH_WEP: {
            ret = AWSS_AUTH_TYPE_SHARED;
            break;
        }
        case WIFI_AUTH_WPA_PSK: {
            ret = AWSS_AUTH_TYPE_WPAPSK;
            break;
        }
        case WIFI_AUTH_WPA2_PSK: {
            ret = AWSS_AUTH_TYPE_WPA2PSK;
            break;
        }
        case WIFI_AUTH_WPA_WPA2_PSK: {
            ret = AWSS_AUTH_TYPE_WPAPSKWPA2PSK;
            break;
        }
        case WIFI_AUTH_WPA2_ENTERPRISE: {
            break;
        }
        case WIFI_AUTH_MAX:
        default:
            break;
    }

    return (int)ret;
}


int auth_mode_to_ln(int auth_type)
{
    wifi_auth_mode_t ret = WIFI_AUTH_MAX;
    switch ( (enum AWSS_AUTH_TYPE)auth_type ) {
        case AWSS_AUTH_TYPE_OPEN: {
            ret = WIFI_AUTH_OPEN;
            break;
        }
        case AWSS_AUTH_TYPE_SHARED: {
            ret = WIFI_AUTH_WEP;
            break;
        }
        case AWSS_AUTH_TYPE_WPAPSK: {
            ret = WIFI_AUTH_WPA_PSK;
            break;
        }
        case AWSS_AUTH_TYPE_WPA8021X: {

            break;
        }
        case AWSS_AUTH_TYPE_WPA2PSK: {
            ret = WIFI_AUTH_WPA2_PSK;
            break;
        }
        case AWSS_AUTH_TYPE_WPA28021X: {

            break;
        }
        case AWSS_AUTH_TYPE_WPAPSKWPA2PSK: {
            ret = WIFI_AUTH_WPA_WPA2_PSK;
            break;
        }
        default:
            break;
    }

    return (int)ret;
}


void _ln_wifi_monitor_cb_adapter(void *buf, uint16_t len, wifi_pkt_type_t pkt_type, int8_t rssi_db)
{
    int buffer_type = 0; // 0 -- 80211 frame; 1 -- only contain IE info.
    char rssi = rssi_db;

    if ( (NULL == buf) || (0 == len)) {
        return;
    }

    if (NULL == sg_monitor_cb) {
        return;
    }

    // hexdump(LOG_LVL_INFO, "monitor data", buf, (len > 64) ? (64) : (len));

    switch (pkt_type) {
        case WIFI_PKT_TYPE_MGMT: {
            LOG(LOG_LVL_INFO, "WIFI_PKT_TYPE_MGMT\r\n");
            sg_monitor_cb( (uint8_t *) buf, (int) len, (signed char) rssi, buffer_type);
            break;
        }
        case WIFI_PKT_TYPE_CTRL: {
            LOG(LOG_LVL_INFO, "WIFI_PKT_TYPE_CTRL\r\n");
            break;
        }
        case WIFI_PKT_TYPE_DATA: {
            // LOG(LOG_LVL_INFO, "WIFI_PKT_TYPE_DATA\r\n");
            sg_monitor_cb( (uint8_t *) buf, (int) len, (signed char) rssi, buffer_type);

            break;
        }
        default: {

            break;
        }
    }
}

void _ln_wifi_monitor_cb_empty(void *buf, uint16_t len, wifi_pkt_type_t pkt_type, int8_t rssi_db)
{
    return;
}

/**
 * @brief   使能或禁用对管理帧的过滤
 *
 * @param[in] filter_mask @n see mask macro in enum HAL_Awss_frame_type,
 *                      currently only FRAME_PROBE_REQ_MASK & FRAME_BEACON_MASK is used
 * @param[in] vendor_oui @n oui can be used for precise frame match, optional
 * @param[in] callback @n see awss_wifi_mgmt_frame_cb_t, passing 80211
 *                      frame or ie to callback. when callback is NULL
 *                      disable sniffer feature, otherwise enable it.
 * @return
   @verbatim
   =  0, success
   = -1, fail
   = -2, unsupported.
   @endverbatim
 * @see None.
 * @note awss use this API to filter specific mgnt frame in wifi station mode
 */
int HAL_Wifi_Enable_Mgmt_Frame_Filter(
    _IN_ uint32_t filter_mask,
    _IN_OPT_ uint8_t vendor_oui[3],
    _IN_ awss_wifi_mgmt_frame_cb_t callback)
{

    sg_monitor_cb = callback;

    if (callback != NULL) { // enable
        sniffer_cfg_t cfg = {
            .enable = 1,
            .type =0,
            // .filter_mask = WIFI_SNIFFER_FILTER_MASK_DATA | WIFI_SNIFFER_FILTER_MASK_MGMT | WIFI_SNIFFER_FILTER_MASK_CTRL,
            .filter_mask = WIFI_SNIFFER_FILTER_MASK_DATA,
        };
        wifi_sta_set_sniffer_cfg(&cfg, _ln_wifi_monitor_cb_adapter);
    } else { // disable
        sniffer_cfg_t cfg = {
            .enable = 0,
        };
        wifi_sta_set_sniffer_cfg(&cfg, &_ln_wifi_monitor_cb_empty);
    }
    return 0;
}

/**
 * @brief   获取所连接的热点(Access Point)的信息
 *
 * @param[out] ssid: array to store ap ssid. It will be null if ssid is not required.
 * @param[out] passwd: array to store ap password. It will be null if ap password is not required.
 * @param[out] bssid: array to store ap bssid. It will be null if bssid is not required.
 * @return
   @verbatim
     = 0: succeeded
     = -1: failed
   @endverbatim
 * @see None.
 * @note
 *     If the STA dosen't connect AP successfully, HAL should return -1 and not touch the ssid/passwd/bssid buffer.
 */
int HAL_Wifi_Get_Ap_Info(char ssid[HAL_MAX_SSID_LEN], char passwd[HAL_MAX_PASSWD_LEN], uint8_t bssid[ETH_ALEN])
{
    int ret = 0;
    wifi_mode_t mode = WIFI_MODE_MAX;

    if ( (NULL == ssid) || (NULL == passwd) || (NULL == bssid) ) {
        ret = -1;
        return ret;
    }

    mode = wifi_current_mode_get();
    if (WIFI_MODE_STATION != mode) {
        ret = -1;
        return ret;
    }

    if (NETDEV_LINK_UP != netdev_get_link_state(netdev_get_active())) {
        ret = -1;
        return ret;
    }

    char *_ssid = NULL;
    char *_pwd = NULL;
    uint8_t _pwd_len = 0;
    uint8_t *_psk = NULL;

    if (WIFI_ERR_NONE == wifi_get_psk_info((const char **)&_ssid,
        (const uint8_t **)&_pwd, &_pwd_len, (const uint8_t **) &_psk)) {
        memset(ssid, 0, HAL_MAX_SSID_LEN);
        memcpy(ssid, _ssid, strlen(_ssid));


        memset(passwd, 0, HAL_MAX_PASSWD_LEN);
        memcpy(passwd, _pwd, strlen(_pwd));
    }

    // TODO: bssid not touched.

    return ret;
}

/**
 * @brief   获取Wi-Fi网口的IP地址, 点分十进制格式保存在字符串数组出参, 二进制格式则作为返回值, 并以网络字节序(大端)表达
 *
 * @param   ifname : 指定Wi-Fi网络接口的名字
 * @param   ip_str : 存放点分十进制格式的IP地址字符串的数组
 * @return  二进制形式的IP地址, 以网络字节序(大端)组织
 */
uint32_t HAL_Wifi_Get_IP(char ip_str[NETWORK_ADDR_LEN], const char *ifname)
{
    wifi_mode_t mode = WIFI_MODE_MAX;
    wifi_interface_t if_index;
    tcpip_ip_info_t ip_info = { 0 };
    char *ip_format = NULL;
    uint32_t addr = 0;

    mode = wifi_current_mode_get();

    if (WIFI_MODE_STATION == mode) {
        if_index = STATION_IF;
    } else if(WIFI_MODE_AP == mode) {
        if_index = SOFT_AP_IF;
    }

    if (NETDEV_LINK_UP != netdev_get_link_state(netdev_get_active())) {
        return addr;
    }

    netdev_get_ip_info(if_index, &ip_info);
    ip_format = ip4addr_ntoa(&ip_info.ip);
    if (NULL != ip_format) {
        strcpy(ip_str, ip_format);
    }
    addr = ipaddr_addr(ip_format);
    return addr;
}

/**
 * @brief   获取Wi-Fi网口的MAC地址, 格式应当是"XX:XX:XX:XX:XX:XX"
 *
 * @param   mac_str : 用于存放MAC地址字符串的缓冲区数组
 * @return  指向缓冲区数组起始位置的字符指针
 */
char *HAL_Wifi_Get_Mac(char mac_str[HAL_MAC_LEN])
{
    uint8_t _mac_addr_buf[MAC_ADDRESS_LEN] = { 0 };
    wifi_interface_t if_index;
    wifi_mode_t mode = wifi_current_mode_get();

    if (WIFI_MODE_STATION == mode) {
        if_index = STATION_IF;
    } else if(WIFI_MODE_AP == mode) {
        if_index = SOFT_AP_IF;
    }

    // get mac addr, which is 6 bytes array.
    wifi_get_macaddr(if_index, _mac_addr_buf);

    // format to str XX:XX:XX:XX:XX:XX
    snprintf(mac_str, HAL_MAC_LEN, "%02X:%02X:%02X:%02X:%02X:%02X",
        _mac_addr_buf[0], _mac_addr_buf[1], _mac_addr_buf[2],
        _mac_addr_buf[3], _mac_addr_buf[4], _mac_addr_buf[5]);

    return (char *)mac_str;
}

static void *sg_scan_sem = NULL;
static awss_wifi_scan_result_cb_t sg_awss_scan_result_cb = NULL;


static void wifi_scan_complete_cb(void * arg)
{
    LN_UNUSED(arg);

    HAL_SemaphorePost(sg_scan_sem);
}

static void _deal_with_scan_result_list(void)
{
    ln_list_t *list;
    uint8_t node_count = 0;
    uint8_t node_at = 0;
    ap_info_node_t *pnode;

    if (NULL == sg_awss_scan_result_cb) {
        return;
    }

    wifi_manager_ap_list_update_enable(LN_FALSE);
    wifi_manager_get_ap_list(&list, &node_count);

    LN_LIST_FOR_EACH_ENTRY(pnode, ap_info_node_t, list,list)
    {
        uint8_t * mac = (uint8_t*)pnode->info.bssid;
        ap_info_t *info = &pnode->info;

        char *ssid = (char *)info->ssid;
        char *bssid = (char *)info->bssid;
        enum AWSS_AUTH_TYPE auth = (enum AWSS_AUTH_TYPE) auth_mode_from_ln(info->authmode);
        enum AWSS_ENC_TYPE encry = 0; // FIXME: 用什么类型?
        uint8_t channel = info->channel;
        signed char rssi = info->rssi;
        int is_last_ap = (node_at++ >= node_count) ? 1 : 0;

        sg_awss_scan_result_cb((const char *)ssid, (const char *)bssid, auth, encry, channel, rssi, is_last_ap);
    }

    wifi_manager_ap_list_update_enable(LN_TRUE);
}

/**
 * @brief   启动一次Wi-Fi的空中扫描(Scan)
 *
 * @param[in] cb @n pass ssid info(scan result) to this callback one by one
 * @return 0 for wifi scan is done, otherwise return -1
 * @see None.
 * @note
 *      This API should NOT exit before the invoking for cb is finished.
 *      This rule is something like the following :
 *      HAL_Wifi_Scan() is invoked...
 *      ...
 *      for (ap = first_ap; ap <= last_ap; ap = next_ap){
 *        cb(ap)
 *      }
 *      ...
 *      HAL_Wifi_Scan() exit...
 */
int HAL_Wifi_Scan(awss_wifi_scan_result_cb_t cb)
{
    // NOTE: 启动一次Wi-Fi的空中扫描，该API是一个阻塞操作，扫描没有完成不能结束。
    // 所有的AP列表收集完成后，一个一个通过回调函数告知AWSS，最好不要限制AP的数量，
    // 否则可能导致中文GBK编码的SSID热点配网失败。

    #define CONNECTED_SCAN_TIMES     (6)
    #define DEFAULT_SCAN_TIMES       (1)
    #define SCAN_TIMEOUT             (1500)

    int ret = -1;
    int8_t scan_cnt = DEFAULT_SCAN_TIMES;
    wifi_sta_status_t sta_status = WIFI_STA_STATUS_STARTUP;
    wifi_scan_cfg_t scan_cfg = { 0 };


    wifi_mode_t mode = wifi_current_mode_get();
    if (WIFI_MODE_STATION != mode) {
        LOG(LOG_LVL_ERROR, "%s | WiFi not in STA mode!!!\r\n", __FUNCTION__);
        return -1;
    }

    if (SYSPARAM_ERR_NONE != sysparam_sta_scan_cfg_get(&scan_cfg)) {
        scan_cfg.channel = STA_SCAN_CHANNEL;
        scan_cfg.scan_type = STA_SCAN_TYPE;
        scan_cfg.scan_time = STA_SCAN_TIME;
    }

    sg_scan_sem = HAL_SemaphoreCreate();
    if (NULL == sg_scan_sem) { // error
        LOG(LOG_LVL_ERROR, "%s | failed to create sem.\r\n", __FUNCTION__);
        return ret;
    }

    // typedef void (*wifi_mgr_event_cb_t)(void * arg);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);
    sg_awss_scan_result_cb = cb;

    wifi_get_sta_status(&sta_status);
    if (sta_status == WIFI_STA_STATUS_CONNECTED || \
        sta_status == WIFI_STA_STATUS_DISCONNECTING)
    {
        scan_cnt = CONNECTED_SCAN_TIMES;
    }
    LOG(LOG_LVL_INFO, "Scan cnt:%d; scan timeout:%d\r\n", scan_cnt, SCAN_TIMEOUT);

    for (; scan_cnt > 0; scan_cnt--)
    {
        wifi_sta_scan(&scan_cfg);
        HAL_SemaphoreWait(sg_scan_sem, SCAN_TIMEOUT);
    }

    _deal_with_scan_result_list();

    sg_awss_scan_result_cb = NULL;
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, NULL);

    HAL_SemaphoreDestroy(sg_scan_sem);
    sg_scan_sem = NULL;

    return 0;
}

/**
 * @brief   在当前信道(channel)上以基本数据速率(1Mbps)发送裸的802.11帧(raw 802.11 frame)
 *
 * @param[in] type @n see enum HAL_Awss_frame_type, currently only FRAME_BEACON
 *                      FRAME_PROBE_REQ is used
 * @param[in] buffer @n 80211 raw frame, include complete mac header & FCS field
 * @param[in] len @n 80211 raw frame length
 * @return
   @verbatim
   =  0, send success.
   = -1, send failure.
   = -2, unsupported.
   @endverbatim
 * @see None.
 * @note awss use this API send raw frame in wifi monitor mode & station mode
 */
int HAL_Wifi_Send_80211_Raw_Frame(_IN_ enum HAL_Awss_Frame_Type type,
                                  _IN_ uint8_t *buffer, _IN_ int len)
{
    return wifi_send_80211_mgmt_raw_frame(buffer, len);
}

/**
 * @brief   获取当前Station模式与AP连接状态的信息
 *
 * @param[out] p_rssi: rssi value of current link
 * @param[out] p_channel: channel of current link
 *
 * @return
   @verbatim
     = 0: succeeded
     = -1: failed
   @endverbatim
 * @see None.
 * @note None.
 * @note awss use this API to get rssi and channel of current link
 */
DLL_HAL_API int HAL_Wifi_Get_Link_Stat(_OU_ int *p_rssi,
                                       _OU_ int *p_channel) {
    int8_t rssi = 0;
    uint8_t chan = 0;

    if (WIFI_ERR_NONE != wifi_sta_get_rssi(&rssi)) {
        *p_rssi = rssi;
    } else {
        return -1;
    }

    if (WIFI_ERR_NONE != wifi_get_channel(&chan)) {
        *p_channel = chan;
    } else {
        return -1;
    }

    return SUCCESS_RETURN;
}