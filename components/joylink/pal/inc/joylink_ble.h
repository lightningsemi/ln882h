/**************************************************************************************************
  Filename:       joylink_ble.h
  Revised:        $Date: 2015-10-14
  Revision:       $Revision: 1.0, Zhang Hua

  Description:    This file contains the Joylink profile definitions and
                  prototypes.

  Copyright 2010 - 2013 JD.COM. All rights reserved.

**************************************************************************************************/


#ifndef __JOYLINK_BLE_H__
#define __JOYLINK_BLE_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "joylink_stdint.h"

typedef struct{
    uint32_t second;
    uint32_t usecond;
}jl_timestamp_t;

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
int jl_ble_send_data(uint8_t *buf, uint32_t size);

/*************************************************
Function: jl_ble_set_config_data
Description: SDK适配接口，获取配网与绑定参数
Calls: 配网与绑定接口
Called By: @jl_process_user_data：SDK内部函数
Input: @data->ssid：配网数据，WiFi SSID
       @data->password：配网数据，WiFi密码
Output: None
Return: 0：成功
       -1：失败
*************************************************/
int jl_ble_set_config_data(uint8_t *ssid, uint8_t *password);

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
int jl_ble_set_system_time(jl_timestamp_t *time);

#ifdef __cplusplus
}
#endif

#endif /* __JOYLINK_ADAPTER_H__ */
