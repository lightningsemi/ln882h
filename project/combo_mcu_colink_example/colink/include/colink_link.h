/**
 * @file colink_link.h
 *
 * Show profile of distributing network.
 * It shows the detail of distributing network struct, enmu and API.
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
#ifndef COLINK_LINK_H
#define COLINK_LINK_H

#include <stdint.h>
#include "colink_typedef.h"

/**
 * @defgroup CoLinkLinkState
 * @{
 */
#define COLINK_LINK_INIT_INVALID         (0)    /**< 未复位配网初始状态 */
#define COLINK_LINK_RES_DEV_INFO_OK      (1)    /**< 响应设备信息成功 */
#define COLINK_LINK_RES_DEVICEID_ERROR   (2)    /**< 响应设备信息失败 */
#define COLINK_LINK_GET_INFO_OK          (3)    /**< 获取配网信息成功 */
#define COLINK_LINK_GET_INFO_ERROR       (4)    /**< 获取配网信息失败 */
/** @} */

/**
 * 配网信息结构体。
 */
typedef struct
{
    char ssid[32];              /**< 路由器ssid */
    char password[64];          /**< 路由器密码 */
    char distor_domain[32];     /**< 分配服务器域名 */
    uint16_t distor_port;       /**< 分配服务器端口 */
}ColinkLinkInfo;

/**
 * @brief 初始化配网。
 *
 * @par 描述:
 * 在colinkLinkReset之前调用初始化。
 *
 * @param deviceid      [IN] 设备ID。
 * @param apikey        [IN] 设备密钥。
 * @param chipid        [IN] 设备唯一标识字符串。
 *
 * @retval ColinkLinkErrorCode       colink配网错误码。
 * 
 * @see ColinkLinkErrorCode
 */
int32_t colinkLinkInit(char *deviceid, char *apikey, char *chipid);

/**
 * @brief 复位配网阶段到初始状态。
 *
 * @par 描述:
 * 每次进入配网阶段前都需要调用colinkLinkReset，
 * 使配网进入初始状态。
 *
 * @param 无。
 *
 */
void colinkLinkReset(void);

/**
 * @brief 用在与APP交互阶段，负责解析并输出响应APP的数据。
 *
 * @par 描述:
 * 在每次进入配网流程前需要调用colinkLinkReset。每收到来自APP的数据，就调用一次
 * colinkLinkParse，并将输出数据发送给APP。正常流程会依次返回结果
 * COLINK_LINK_RES_DEV_INFO_OK和COLINK_LINK_GET_INFO_OK，当返回
 * COLINK_LINK_GET_INFO_OK时即可结束配网流程。
 *
 * @param in            [IN] 输入数据的指针。
 * @param in_len        [IN] 输入数据的长度。
 * @param out           [IN] 输出数据的指针。
 * @param out_buf_len   [IN] 输出数据缓冲区的长度。
 * @param out_len       [IN] 输出数据的的长度。
 *
 * @retval CoLinkLinkState  配网结果枚举类型。
 *
 * @see CoLinkLinkState
 */
int32_t colinkLinkParse(uint8_t *in, 
            uint16_t in_len, 
            uint8_t *out, 
            uint16_t out_buf_len, 
            uint16_t *out_len);

/**
 * @brief 配网成功后获取分配服务器信息。
 *
 * @par 描述:
 * 当colinkLinkParse返回COLINK_LINK_GET_INFO_OK后，直接调用
 * 此函数获取分配服务器信息。否则返回COLINK_LINK_OPERATION_ERROR错误类型。
 *
 * @param info         [OUT] 输出配对结果信息的指针。
 *
 * @retval ColinkLinkErrorCode       colink配网错误码。
 *
 * @see ColinkLinkErrorCode
 */
int32_t colinkLinkGetInfo(ColinkLinkInfo *info);

#endif    /* COLINK_LINK_H */
