/**
 * @file colink_profile.h
 *
 * Show profile of colink.
 * It shows the detail of colink struct, enmu and API.
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
#ifndef COLINK_PROFILE_H
#define COLINK_PROFILE_H

#include "colink_typedef.h"
#include "colink_gateway_profile.h"

/**
 * @defgroup ColinkDevType
 * @{
 */
#define COLINK_SINGLE     (0)    /**< 普通设备 */
#define COLINK_GATEWAY    (1)    /**< 网关设备 */
/** @} */

/**
 * 设备信息的结构体。
 * 
 * @see ColinkDevType
 */
typedef struct
{
    char deviceid[11];           /**< 设备ID */
    char apikey[37];             /**< 设备密钥 */
    char model[20];              /**< 设备型号 */
    char chipid[50];             /**< 设备硬件唯一标识(例如芯片上的chipid，GSM模块上的IMEI) */
    char distor_domain[32];      /**< 分配服务器域名，由APP下发给设备 */
    uint16_t distor_port;        /**< 分配服务器的端口号，由APP下发给设备 */
    char version[12];            /**< 固件版本 */
    bool ssl_enable;             /**< 是否使能SSL */
    int32_t dev_type;            /**< 设备类型，参见ColinkDevType */
    uint32_t server_res_timeout; /**< 服务器响应超时 */
    bool temporary_server_enable;/**< 是否使能临时服务器进程获取distor info。当配网无法获取到disto  r info时（如WSS配网），需使能此字段。
                                      当此字段为true时，distor_domain和distor_port不需赋值。colink进程向临时服务索要，通过colinkRecvResetDispatchRequestCb回调传出。
                                      当此字段为false时，须填入正确的distor_domain和distor_port，colink进程才能正常运行。
                                      */
    int32_t __pad[2];
}ColinkDev;

/**
 * @defgroup ColinkDevStatus
 * @{
 */
#define DEVICE_OFFLINE       (0)    /**< 设备离线 */
#define DEVICE_ONLINE        (1)    /**< 设备在线 */
#define DEVICE_UNREGISTERED  (2)    /**< 设备未注册 */
#define DEVICE_BE_DELETED    (3)    /**< 设备被删除 */
/** @} */

/**
 * @defgroup ColinkDevEvent
 * @{
 */
#define DEVICE_EVT_TMP_SERVER_CONNECTED       (0)    /**< 设备连上临时服务器              */
#define DEVICE_EVT_CONNECT_DISPATCH_SERVER    (1)    /**< 设备开始连接分配服务器 */
/** @} */


/**
 * OTA固件信息的结构体。
 */
typedef struct
{
    char name[20];             /**< 固件名称 */
    char download_url[150];    /**< 下载链接 */
    char digest[65];           /**< 固件的SHA256值 */
}ColinkOtaInfo;

/**
 * 设备事件回调的结构体。
 */
typedef struct
{
    /**
     * @brief 接收update字段的回调函数。
     *
     * @par 描述:
     * 当APP需要改变设备状态时，通过此回调函数获取数据。
     *
     * @param data     [IN] 收到的字符串数据，以Json格式表示。
     *
     */
    void (*colinkRecvUpdateCb)(char* data);

    /**
     * @brief 接收update字段的回调函数2，较colinkRecvUpdateCb新增参数sequence。
     *
     * @par 描述:
     * 当APP需要改变设备状态时，通过此回调函数获取数据。
     *
     * @param data      [IN] 收到的字符串数据，以Json格式表示。
     * @param sequence  [IN] 控制端的指令序列号，字符串格式。用于指示指令的唯一性和先后顺序。
     *
     * @note 与上述colinkRecvUpdateCb功能一致，二选一使用。
     */
    void (*colinkRecvUpdateWithSeqCb)(char* data, char* sequence);
    
    /**
     * @brief 设备状态发生改变时的回调函数。
     *
     * @par 描述:
     * 当设备状态发生改变时会产生回调函数。
     *
     * @param status   [IN] 设备状态。参见ColinkDevStatus。
     *
     * @see ColinkDevStatus
     */
    void (*colinkNotifyDevStatusCb)(int32_t status);

    /**
     * @brief 设备收到请求上报用电量的回调函数。
     *
     * @par 描述:
     * 当设备收到请求上报用电量时会产生该回调。
     *
     * @param data   [IN] json字符串。
     * @param seq    [IN] 随机数字符串，需要在调用
     * 
     */
    void (*colinkRecvPowerDataGetCmdCb)(char *data, char *seq);

    /**
     * @brief 升级通知的回调函数。
     *
     * @par 描述:
     * 当APP发出升级通知后，通过此回调函数获取升级信息。
     * 当升级完成后需要调用colinkUpgradeRes通知服务器升级完成。
     *
     * @param new_ver       [IN] 新固件的版本号。
     * @param file_list     [IN] OTA固件信息列表。
     * @param file_num      [IN] OTA固件的数量。
     * @param sequence      [IN] 唯一标识字符串，需要保存下来，
     * 在通过colinkUpgradeRes通知服务器时传入
     *
     */
    void (*colinkUpgradeRequestCb)(char *new_ver, ColinkOtaInfo file_list[], uint8_t file_num, char *sequence);
 
    /**
     * @brief 发送update字段数据的回调函数。
     *
     * @par 描述:
     * 当发送update字段数据后需要调用colinkSendUpdateCb获取服务器应答。
     *
     * @param new_ver       [IN] 新固件的版本号。
     *
     */
    void (*colinkSendUpdateCb)(int32_t error_code);

    /**
     * @brief 向服务器请求UTC时间的回调函数。
     *
     * @par 描述:
     * 当设备调用colinkSendUTCRequest发送获取UTC时间后，
     * 通过此回调函数获取请求结果。
     *
     * @param error_code    [IN] 收到服务器响应的错误码。参见ColinkReqResultCode。
     * @param utc_str       [IN] UTC时间字符串。
     * 
     * @see ColinkReqResultCode
     */
    void (*colinkSendUTCRequestCb)(int32_t error_code, char utc_str[]);

    /**
     * @brief 向服务器发起查询申请的回调函数。
     * 
     * @par 描述:
     * 当设备调用colinkSendQuery发送查询请求后，
     * 通过此回调函数获取请求结果。
     * 
     * @param error_code    [IN] 收到服务器响应的错误码。参见ColinkReqResultCode。
     * @param params        [IN] 查询返回的结果。
     * 
     * @see ColinkReqResultCode
     */
    void (*colinkSendQueryCb)(int32_t error_code, char *params);

    /**
     * @brief 服务器请求重置分配服务器
     * 
     * @par 描述:
     * 当分配服务器不可用时，长连接服务器会下发该请求，请求重置分配服务器。
     * 当使能
     *
     * @param dispatchUrl  [IN] 新的分配服务器链接
     * @param dispatchPort [IN] 新的分配服务器端口
     * 
     * @retval 无
     */
    void (*colinkRecvResetDispatchRequestCb)(char *dispatchUrl, uint16_t dispatchPort);

    /**
     * @brief 服务器请求重置分配服务器区域
     * 
     * @par 描述:
     * 当分配服务器不可用时，长连接服务器会下发该请求，请求重置分配服务器区域。
     * 
     * @param region  [IN] 新的分配服务器区域
     * 
     * @retval 无
     */
    void (*colinkRecvResetDispatchRegionRequestCb)(char *region);

    /**
     * @brief colink流程事件回调函数。
     *
     * @par 描述:
     * 将colink事件传给上层
     *
     * @param event [IN] colink事件。现只有两种，参见ColinkDevEvent
     *
     */
    void (*colinkNotifyDevEventCb)(int32_t event);
}ColinkEvent;

/**
 * @brief 初始化colink Device。
 *
 * @par 描述:
 * 初始化colink设备信息和注册回调事件。
 *
 * @param dev_data       [IN] 设备信息结构体指针。
 * @param reg_event      [IN] 注册回调事件结构体指针。
 *
 * @retval ColinkInitErrorCode colink初始化错误码。
 * @see ColinkInitErrorCode
 */
int32_t colinkInit(ColinkDev *dev_data, ColinkEvent *reg_event);

/**
 * @brief 反初始化colink Device。
 *
 * @par 描述:
 * 反初始化colink，释放当前colink占用的资源。
 * 用于使用过程中，设备需要重新配网或者出现问题需要重新初始化colink的状况下，须先
 * 把colinkProcess进程结束，然后调用该接口释放colink占用的资源。
 *
 * @param 无。
 *
 * @retval true      反初始化成功。
 * @retval false     反初始化失败。
 */
bool colinkDeInit(void);

/**
 * @brief 获取用户ApiKey。
 *
 * @par 描述:
 * 获取服务器下发的用户ApiKey。
 *
 * @param 无。
 *
 * @retval 服务器下发的用户ApiKey。
 */
char *colinkGetUserApiKey(void);

/**
 * @brief colink事件处理。
 *
 * @par 描述:
 * 启动colink事件处理，必须在colinkInit初始化成功后调用！
 * 建议调用间隔最大25~50毫秒。若该函数长时间未被调用，则可能返回异常信息。
 * 调用该接口的任务分配栈空间建议4096字节！！！
 *
 * @param 无。
 *
 * @retval ColinkProcessErrorCode       colink进程运行错误码。
 * @see ColinkProcessErrorCode
 */
int32_t colinkProcess(void);

/**
 * @brief 发送update字段的数据。
 *
 * @par 描述:
 * 当设备状态发生改变时主动发到云平台，云平台再转发到APP同步设备状态。
 *
 * @param data          [IN] 以Json格式表示的字符串数据。
 * @retval ColinkErrorCode     colink错误码。
 * @see ColinkErrorCode
 */
int32_t colinkSendUpdate(char* data);

/**
 * @brief eWeLink App局域网控制引起的设备状态改变时发送update字段数据。
 *
 * @par 描述:
 * eWeLink App局域网控制引起的设备状态改变时主动发到云平台，云平台再转发到APP同步设备状态。
 *
 * @param data          [IN] 以Json格式表示的字符串数据。
 * @param selfApikey    [IN] 局域网控制端的账户信息，字符串格式。NULL表示无此字段需求。                      
 * @param sequence      [IN] 局域网控制端的指令序列号，字符串格式。NULL表示无此字段需求。
 * @param seq           [IN] 设备端指令序列号。0xFFFFFFFF表示无此字段需求。
 *                           建议为从1开始递增的正整数。
 * @note 
 *      当设备开放eWeLink局域网控制功能时，设备信息的更新须调用colinkSendUpdateByLan
 *  接口上报给云平台（指令必须包含表示设备状态更新先后顺序的 seq  字段）。其中，
 *  当更新由 eWeLink App局域网控制引起时，update指令还必须同时包含sequence和selfApikey字段，
 *  其值分别等于局域网控制请求体中的sequence和selfApikey字段值。
 *      对于相同的seq包，eWeLink App仅处理一次。
 *
 * @retval ColinkErrorCode     colink错误码。
 * @see ColinkErrorCode
 */
int32_t colinkSendUpdateByLan(char* data, char* selfApikey, char* sequence, uint32_t seq);

/**
 * @brief 获取设备的状态。
 *
 * @par 描述:
 * 获取设备的离线上线状态，开发者通过返回值作相应处理。
 *
 * @param 无。
 *
 * @retval ColinkDevStatus  设备状态。
 * 
 * @see ColinkDevStatus
 */
int32_t colinkGetDevStatus(void); 

/**
 * @brief 发送用电量的数据。
 *
 * @par 描述:
 * 当设备收到获取用电量请求时，需要调用此函数将用电量上报给服务器。
 *
 * @param data   [IN] 以Json格式表示的字符串数据。
 *
 * @retval ColinkErrorCode    colink错误码。
 * @see ColinkErrorCode
 */
int32_t colinkResponsePowerData(char *data, char *seq);

/**
 * @brief 响应升级的结果。
 *
 * @par 描述:
 * 当收到升级通知后（即colinkUpgradeRequestCb），
 * 升级完成需要调用此接口来响应服务器是否升级成功的结果。
 *
 * @param error_code   [IN] 响应错误码值。参考ColinkOtaResCode。
 *
 * @retval ColinkErrorCode     colink错误码。
 * 
 * @see ColinkOtaResCode
 * @see ColinkErrorCode
 */
int32_t colinkUpgradeRes(int32_t error_code, char *seqeuence);

/**
 * @brief 向服务器发送获取UTC时间请求。
 *
 * @par 描述:
 * 当设备需要同步服务器UTC时间时，调用此接口发送请求，
 * 发送请求成功后通过colinkSendUTCRequestCb获取请求结果。
 *
 * @param error_code   [IN] 响应错误码值。参考ColinkOtaResCode。
 *
 * @retval ColinkErrorCode     colink错误码。
 * 
 * @see ColinkOtaResCode
 * @see ColinkErrorCode
 */
int32_t colinkSendUTCRequest(void);

/**
 * @brief 发送query字段的数据。
 *
 * @par 描述:
 * 当设备需要查询定时器等信息时，向服务器发送查询请求。
 *
 * @param params   [IN] 以Json格式表示的字符串数据。
 *
 * @retval ColinkErrorCode     colink错误码。
 * 
 * @see ColinkErrorCode
 */
int32_t colinkSendQuery(char *params);

/**
 * @brief 获取colink版本号。
 *
 * @par 描述:
 * 获取colink版本号。
 *
 * @param 无。
 *
 * @retval 非NULL     成功获取colink版本。
 * @retval NULL       获取版本失败。
 */
const char *colinkGetVersion(void);

#endif    /* COLINK_PROFILE_H */
