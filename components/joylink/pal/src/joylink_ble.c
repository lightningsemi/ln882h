#include "joylink_ble.h"
#include "joylink_log.h"
#include "joylink_ble_sdk.h"
#include "ln_joylink.h"
#include "ln_app_gatt.h"

/*************************************************
Function: jl_ble_send_data
Description: SDK适配接口，GATTS Characteristic发送数据
Calls: GATTS Characteristic发送数据接口
Called By: @jl_send_frame：SDK内部函数
Input: @data：发送的数据
       @data_len：发送的数据长度
Output: None
Return: 0：成功
       -1：失败
Others:
*************************************************/
int jl_ble_send_data(uint8_t* data, uint32_t data_len)
{
    struct ln_gattc_send_evt_cmd p_param = {0};
    p_param.length = data_len;
    p_param.value  = data;
    p_param.handle = ln_get_handle_ind();
    hexdump(LOG_LVL_INFO, "[jl_ble_send_data]", (void *)data, data_len);
    if (ln_get_ble_connect_flag() == 0) {
        ln_app_gatt_send_ind(ln_get_ble_con(), &p_param);
    }

    //TODO:
    //OS_MsDelay(5);
    //jl_send_confirm();
    return 0;
}

/*************************************************
Function: jl_ble_set_config_data
Description: SDK适配接口，获取配网与绑定参数
Calls: 传递配网信息接口
Called By: @jl_process_user_data：SDK内部函数
Input: @data->ssid：配网数据，WiFi SSID
       @data->password：配网数据，WiFi密码
Output: None
Return: 0：成功
       -1：失败
*************************************************/
int jl_ble_set_config_data(uint8_t *ssid, uint8_t *password)
{
    log_info("ssid: %s, password: %s\r\n", ssid, password);
    
    ln_send_net_config_state(E_JL_NET_CONF_ST_WIFI_CONNECT_START, NULL, 0);
    if (0 != ln_start_connect_ap(ssid, password)) {
        return -1;
    }

    ln_send_net_config_state(E_JL_NET_CONF_ST_WIFI_CONNECT_SUCCEED, NULL, 0);
    log_info("E_JL_NET_CONF_ST_WIFI_CONNECT_SUCCEED\r\n");
    joylink_dev_lan_active_switch(1);

    return 0;
}

/*************************************************
Function: jl_ble_set_system_time
Description: SDK适配接口，设置系统时间
Calls: 设置系统时间接口
Called By: SDK内部函数
Input: @time->second：秒
       @time->usecond：微妙
Output: None
Return: 0：成功
       -1：失败
Others: None
*************************************************/
int jl_ble_set_system_time(jl_timestamp_t *time)
{
//TODO:
#if 0
    struct timeval now = { .tv_sec = time->second , .tv_usec = time->usecond};
    settimeofday(&now, NULL);

    struct tm *lt;
    lt = localtime(&lt);
    printf("%d/%d/%d %d:%d:%d\r\n",lt->tm_year+1900, lt->tm_mon, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
 #endif
    return 0;
}
