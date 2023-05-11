/**
 * @file colink_gateway_profile.h
 *
 * Show profile of colinkgateway.
 * It shows the detail of colinkgateway struct and API.
 *
 * @author Wu Jiale
 * @date 2018.09.06
 * 
 * @copyright 
 * Copyright (C) 2018 Coolkit Technology Co.,Ltd
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef COLINK_GATEWAY_PROFILE_H
#define COLINK_GATEWAY_PROFILE_H

#include "colink_typedef.h"

/**
 * @defgroup ColinkGatewayErrorCode
 * @{
 */
#define COLINK_GATEWAY_NO_ERROR            (0)       /**< 无错误 */
#define COLINK_GATEWAY_OPERATE_FAILED     (-1)       /**< 操作失败 */
#define COLINK_GATEWAY_ARG_INVALID        (-2)       /**< 无效的参数 */
#define COLINK_GATEWAY_NET_ERROR        (-100)       /**< 网络异常 */
/** @} */

/**
 * @defgroup ColinkSubDevResultCode
 * @{
 */
#define COLINK_SUB_DEV_NO_ERROR             (0)       /**< 操作成功 */
#define COLINK_SUB_DEV_NUMBER_ERROR      (-414)       /**< 子设备数量超过限制 */
/** @} */

/**
 * 子设备地址的结构体。
 */
typedef struct
{
    uint8_t mac[8];    /**< 子设备MAC地址,仅支持6字节和8字节mac */
}ColinkSubDevAddr;

/**
 * 子设备的结构体。
 */
typedef struct
{
    ColinkSubDevAddr addr;    /**< 子设备地址 */
    uint32_t uiid;            /**< 子设备uiid */
    bool onlineState;         /**< 子设备在线状态 */
}ColinkSubDevice;

/**
 * 子设备列表的结构体。
 */
typedef struct
{
    ColinkSubDevice dev;    /**< 子设备 */
    char deviceid[11];      /**< 子设备id */
}ColinkSubDeviceList;

typedef struct
{
     /**
     * @brief 接收请求数据的回调函数。
     *
     * @par 描述:
     * 当APP需要改变设备状态时，子设备通过此回调函数获取数据。
     * 需要调用colinkSubDevSendRes来响应子设备是否操作成功。
     *
     * @param sub_dev       [IN] 子设备对应的指针。
     * @param data          [IN] 收到请求的数据。
     * @param req_sequence  [IN] 收到的sequence。
     *
     */
    void (*colinkSubDevRecvReqCb)(ColinkSubDevice *sub_dev, char* data, char req_sequence[]);

    /**
     * @brief 接收服务器响应的回调函数。
     *
     * @par 描述:
     * 当调用colinkSubDevSendReq发送数据，随后产生此回调函数来获取服务器响应的结果。
     * 子设备可以通过此回调判断数据是否发送成功。
     *
     * @param sub_dev       [IN] 子设备对应的指针。
     * @param error_code    [IN] 收到服务器响应的错误码。参见ColinkSubDevResultCode。
     *
     * @see ColinkSubDevResultCode
     */
    void (*colinkSubDevRecvResCb)(ColinkSubDevice *sub_dev, int32_t error_code);

    /**
     * @brief 注册新子设备时服务器响应的回调函数。
     *
     * @par 描述:
     * 当调用colinkAddSubDev注册多个新子设备，随后产生此回调函数来获取服务器响应的结果。
     * 子设备可以通过此回调判断是否注册成功。
     *
     * @param sub_dev       [IN] 子设备对应的数组列表。
     * @param error_code    [IN] 收到服务器响应的错误码列表。参见ColinkSubDevResultCode。
     * @param num           [IN] 子设备数量。
     *
     * @see ColinkSubDevResultCode
     */
    void (*colinkAddSubDevResultCb)(ColinkSubDevice sub_dev[], int32_t error_code[], uint16_t num);

    /**
     * @brief 删除子设备时服务器响应的回调函数。
     *
     * @par 描述:
     * 当调用colinkDelSubDev删除多个子设备，随后产生此回调函数来获取服务器响应的结果。
     * 子设备可以通过此回调判断是否删除成功。
     *
     * @param sub_dev       [IN] 子设备对应的数组列表。
     * @param error_code    [IN] 收到服务器响应的错误码列表。参见ColinkSubDevResultCode。
     * @param num           [IN] 子设备数量。
     *
     * @see ColinkSubDevResultCode
     */
    void (*colinkDelSubDevResultCb)(ColinkSubDevice sub_dev[], int32_t error_code[], uint16_t num);

    /**
     * @brief 上报子设备在线时服务器响应的回调函数。
     *
     * @par 描述:
     * 当调用colinkOnlineSubDev上报多个子设备在线，随后产生此回调函数来获取服务器响应的结果。
     * 子设备可以通过此回调判断是否删除成功。
     *
     * @param sub_dev       [IN] 子设备对应的数组列表。
     * @param error_code    [IN] 收到服务器响应的错误码列表。参见ColinkSubDevResultCode。
     * @param num           [IN] 子设备数量。
     *
     * @see ColinkSubDevResultCode
     */
    void (*colinkOnlineSubDevResultCb)(ColinkSubDevice sub_dev[], int32_t error_code[], uint16_t num);

    /**
     * @brief 上报子设备离线时服务器响应的回调函数。
     *
     * @par 描述:
     * 当调用colinkOfflineSubDev上报多个子设备离线，随后产生此回调函数来获取服务器响应的结果。
     * 子设备可以通过此回调判断是否删除成功。
     *
     * @param sub_dev       [IN] 子设备对应的数组列表。
     * @param error_code    [IN] 收到服务器响应的错误码列表。参见ColinkSubDevResultCode。
     * @param num           [IN] 子设备数量。
     *
     * @see ColinkSubDevResultCode
     */
    void (*colinkOfflineSubDevResultCb)(ColinkSubDevice sub_dev[], int32_t error_code[], uint16_t num);

    /**
     * @brief 服务器发送删除子设备指令，通知网关处理的回调函数。
     *
     * @par 描述:
     * 已注册当前网关的子设备连接其它网关或者app端删除子设备的时候，服务器会发送删除子设备指令，通过回调函数来得知哪些子设备已被删除。
     *
     * @param sub_dev       [IN] 通知删除的子设备。
     *
     */
    void (*colinkServerDelSubDevCb)(ColinkSubDevice *sub_dev);

    /**
     * @brief 网关设备批量上报子设备状态时服务器响应的回调函数。
     * 
     * @par 描述:
     * 当调用colinkGatewayReportSubDevState批量上报子设备状态后，随后产生此回调来获取服务器响应结果。
     * 
     * @param error_code    [IN] 收到服务器响应的错误码。参见ColinkSubDevResultCode。
     * 
     * @see ColinkSubDevResultCode
     */
    void (*colinkReportSubDevStateCb)(int32_t error_code);

    /**
     * @brief 网关设备收到服务器发来的批量上报子设备状态请求时的回调函数。
     * 
     * @par 描述:
     * 当服务器需要网关设备批量上报子设备状态时，会产生此回调，需将本地子设备状态上报给服务器。
     * 
     * @param 无
     */
    void (*colinkRecvReportSubDevStateCb)(void);
}ColinkGatewayEvent;

/**
 * @brief 初始化网关功能。
 *
 * @par 描述:
 * 初始化网关功能。
 * 
 * @param device_id 设备id
 * @param event 网关设备事件回调
 * 
 * @retval ColinkGatewayErrorCode
 */
int32_t colinkGatewayInit(const char* device_id, ColinkGatewayEvent *event);

/**
 * @brief 通过deviceid获取子设备的地址。
 *
 * @par 描述:
 * 通过deviceid获取子设备的地址。
 *
 * @param deviceid       [IN] deviceid字符串。
 *
 * @retval 非NULL        获取成功。
 * @retval NULL         获取失败。
 */
const ColinkSubDevAddr *colinkGetAddrByDeviceid(char deviceid[11]);

/**
 * @brief 通过子设备地址获取deviceid。
 *
 * @par 描述:
 * 通过子设备地址获取deviceid。
 *
 * @param sub_dev_addr       [IN] 子设备地址的指针。
 *
 * @retval 非NULL        获取成功。
 * @retval NULL         获取失败。
 */
const char *colinkGetDeviceidByAddr(ColinkSubDevAddr *sub_dev_addr);

/**
 * @brief 清除子设备列表。
 *
 * @par 描述:
 * 清除网关本地子设备列表。
 *
 * @param  无
 *
 * @retval 无
 */
void colinkGatewayDelAllSubDev(void);

/**
 * @brief 注册新子设备。
 *
 * @par 描述:
 * 注册多个新子设备。
 *
 * @param dev_list       [IN] 子设备数组的指针。
 * @param dev_num        [IN] 子设备数组的数量。
 *
 * @retval ColinkGatewayErrorCode        子设备操作错误码。
 * 
 * @see ColinkGatewayErrorCode
 * 
 * @warning colinkAddSubDev,colinkDelSubDev,colinkOnlineSubDev,colinkOfflineSubDev属于同类操作，
 * 不支持并发，请在某个操作的回调结束后再去进行调用同类的操作！！！
 */
int32_t colinkAddSubDev(ColinkSubDevice dev_list[], uint16_t dev_num);

/**
 * @brief 删除子设备。
 *
 * @par 描述:
 * 删除多个子设备。
 *
 * @param dev_list       [IN] 子设备数组的指针。
 * @param dev_num        [IN] 子设备数组的数量。
 *
 * @retval ColinkGatewayErrorCode        子设备操作错误码。
 * 
 * @see ColinkGatewayErrorCode
 * 
 * @warning colinkAddSubDev,colinkDelSubDev,colinkOnlineSubDev,colinkOfflineSubDev属于同类操作，
 * 不支持并发，请在某个操作的回调结束后再去进行调用同类的操作！！！
 */
int32_t colinkDelSubDev(ColinkSubDevice dev_list[], uint16_t dev_num);

/**
 * @brief 上报子设备在线。
 *
 * @par 描述:
 * 上报多个子设备在线。
 *
 * @param dev_list       [IN] 子设备数组的指针。
 * @param dev_num        [IN] 子设备数组的数量。
 *
 * @retval ColinkGatewayErrorCode        子设备操作错误码。
 * 
 * @see ColinkGatewayErrorCode
 * 
 * @warning colinkAddSubDev,colinkDelSubDev,colinkOnlineSubDev,colinkOfflineSubDev属于同类操作，
 * 不支持并发，请在某个操作的回调结束后再去进行调用同类的操作！！！
 */
int32_t colinkOnlineSubDev(ColinkSubDevice dev_list[], uint16_t dev_num);

/**
 * @brief 上报子设备离线。
 *
 * @par 描述:
 * 上报多个子设备离线。
 *
 * @param dev_list       [IN] 子设备数组的指针。
 * @param dev_num        [IN] 子设备数组的数量。
 *
 * @retval ColinkGatewayErrorCode        子设备操作错误码。
 * 
 * @see ColinkGatewayErrorCode
 * 
 * @warning colinkAddSubDev,colinkDelSubDev,colinkOnlineSubDev,colinkOfflineSubDev属于同类操作，
 * 不支持并发，请在某个操作的回调结束后再去进行调用同类的操作！！！
 */
int32_t colinkOfflineSubDev(ColinkSubDevice dev_list[], uint16_t dev_num);

/**
 * @brief 子设备响应服务器的错误码。
 *
 * @par 描述:
 * 子设备响应服务器的错误码。当收到colinkSubDevRecvReqCb回调函数后
 * 需要调用此函数响应给服务器。
 *
 * @param sub_dev       [IN] 子设备的指针。
 * @param error_code    [IN] 发送响应的错误码。参见ColinkSubDevResultCode。
 * @param req_sequence  [IN] 传入colinkSubDevRecvReqCb获取的req_sequence。
 *
 * @retval ColinkErrorCode     colink错误码。
 * 
 * @see ColinkSubDevResultCode
 * @see ColinkErrorCode
 */
int32_t colinkSubDevSendRes(ColinkSubDevice *sub_dev, int32_t error_code, char req_sequence[]);

/**
 * @brief 子设备发送请求的数据。
 *
 * @par 描述:
 * 子设备发送请求的数据。之后会产生colinkSubDevRecvResCb回调函数来获取
 * 服务器响应的错误码。每调用此函数需要等收到服务器响应才能再次调用。
 *
 * @param sub_dev       [IN] 子设备的指针。
 * @param data          [IN] 发送请求的数据。
 *
 * @retval ColinkErrorCode     colink错误码。
 * @see ColinkErrorCode
 */
int32_t colinkSubDevSendReq(ColinkSubDevice *sub_dev, char* data);

/**
 * @brief 添加多个子设备到子设备列表。
 *
 * @par 描述:
 * 添加多个子设备到本地子设备列表，用于重启时添加保存在flash中的子设备列表，需
 * 在初始化（colinkInit）之后调用。
 *
 * @param list       [IN] 欲添加进本地子设备列表的子设备列表。
 * @param num        [IN] 子设备数量。
 *
 * @retval 0          全部添加成功。
 * @retval 大于0      已添加成功的子设备数量。
 */
uint16_t colinkGatewayAddSubDev(ColinkSubDeviceList *list, uint16_t num);

/**
 * @brief 获取当前子设备数量。
 *
 * @par 描述:
 * 获取当前网关设备上子设备的数量。
 *
 * @param 无
 *
 * @retval  当前子设备数量。
 */
uint16_t colinkGatewayGetSubDevNum(void);

/**
 * @brief 获取当前子设备信息列表。
 *
 * @par 描述:
 * 获取当前网关设备上的子设备信息列表。
 *
 * @param list       [IN/OUT] 子设备信息列表缓冲区，由开发者根据实际需求分配空间。
 * @param index      [IN]     子设备信息i列表读取起始位置
 * @param num        [IN]     获取的子设备信息数量
 *
 * @retval 成功获取的子设备信息数量。
 */
uint16_t colinkGatewayGetSubDevList(ColinkSubDeviceList *list, uint16_t index, uint16_t num);

/**
 * @brief 批量上报子设备状态。
 * 
 * @par 描述：
 * 在网关设备掉线重连后，上报当前子设备的状态。
 * 
 * @param dev_list       [IN] 子设备数组的指针。
 * @param dev_num        [IN] 子设备数量。
 * @param end_report     [IN] 是否上报完成（是否为最后一次上报）。
 * 
 * @retval  成功  上报成功的子设备数量。
 *          失败  0
 */
uint16_t colinkGatewayReportSubDevState(ColinkSubDevice dev_list[], uint16_t dev_num, bool end_report);

/**
 * @brief 向服务器查询子设备的参数。
 *
 * @par 描述:
 * 当自设备需要查询定时器等信息时，向服务器发送查询请求。
 *
 * @param deviceid   [IN] 子设备的id。
 * @param params     [IN] 以Json格式表示的字符串数据。
 *
 * @retval ColinkErrorCode     colink错误码。
 * 
 * @see ColinkErrorCode
 */
int32_t colinkSendQueryWithDeviceid(char *deviceid, char *params);


#endif  /* COLINK_GATEWAY_PROFILE_H */

