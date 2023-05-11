/**
 * @file colink_error.h
 *
 * Show profile of colink error code.
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
#ifndef COLINK_ERROR_H
#define COLINK_ERROR_H

/**
 * @defgroup ColinkInitErrorCode
 * @{
 */
#define COLINK_INIT_NO_ERROR      (0)      /**< 无错误 */
#define COLINK_INIT_FAILED       (-1)      /**< 初始化失败 */
#define COLINK_INIT_ARG_INVALID  (-2)      /**< 无效的参数 */
/** @} */

/**
 * @defgroup ColinkErrorCode
 * @{
 */
#define COLINK_NO_ERROR           (0)        /**< 无错误 */
#define COLINK_ARG_INVALID       (-2)        /**< 无效的参数 */
#define COLINK_JSON_INVALID      (-3)        /**< 无效的JSON格式 */
#define COLINK_JSON_CREATE_ERR   (-4)        /**< 创建JSON对象错误 */
#define COLINK_DATA_SEND_ERROR   (-5)        /**< 发送数据出错 */
#define COLINK_DEV_TYPE_ERROR    (-6)        /**< 设备类型错误 */
/** @} */

/**
 * @defgroup ColinkProcessErrorCode
 * @{
 */
#define COLINK_PROCESS_NO_ERROR          (0)      /**< 无错误 */
#define COLINK_PROCESS_INIT_INVALID    (-12)      /**< colinkInit未初始化成功 */
#define COLINK_PROCESS_TIMEOUT         (-13)      /**< 长时间未被调用colinkProcess */
#define COLINK_PROCESS_MEMORY_ERROR    (-14)      /**< 内存分配错误 */
/** @} */

/**
 * @defgroup ColinkTcpErrorCode
 * @{
 */
#define COLINK_TCP_NO_ERROR                 (0)      /**< 无错误 */
#define COLINK_TCP_ARG_INVALID             (-2)      /**< 无效的参数 */
#define COLINK_TCP_CREATE_CONNECT_ERR     (-21)      /**< 创建TCP连接错误 */
#define COLINK_TCP_SEND_ERR               (-23)      /**< TCP发送失败 */
#define COLINK_TCP_READ_ERR               (-25)      /**< TCP读取失败 */
#define COLINK_TCP_CONNECT_TIMEOUT        (-26)      /**< TCP连接超时 */
#define COLINK_TCP_CONNECT_ERR            (-27)      /**< TCP连接失败 */
#define COLINK_TCP_CONNECTING             (-28)      /**< TCP连接中    */
#define COLINK_TCP_READ_INCOMPLETED       (-29)      /**< TCP读包不完整 */
#define COLINK_TCP_CREATE_SERVER_ERR      (-31)      /**< 创建TCP服务错误 */
#define COLINK_TCP_SERVER_WAIT_CONNECT    (-32)      /**< tcp等待客户端连接 */
#define COLINK_TCP_DNS_PARSING            (-35)      /**< DNS解析中 */
#define COLINK_TCP_DNS_PARSE_ERR          (-36)      /**< DNS解析失败 */
#define COLINK_TCP_DNS_PARSE_TIMEOUT      (-37)      /**< DNS解析超时 */
/** @} */

/**
 * @defgroup ColinkReqResultCode
 * @{
 */
#define COLINK_REQ_RESULT_NO_ERROR            (0)       /**< 操作成功 */
#define COLINK_REQ_RESULT_NUMBER_ERROR     (-414)       /**< 子设备数量超过限制 */
/** @} */

/**
 * @defgroup ColinkOtaResCode
 * @{
 */
#define COLINK_OTA_NO_ERROR               (0)       /**< OTA升级成功 */
#define COLINK_OTA_DOWNLOAD_ERROR      (-404)       /**< OTA文件下载失败 */
#define COLINK_OTA_MODEL_ERROR         (-406)       /**< 设备型号不正确 */
#define COLINK_OTA_DIGEST_ERROR        (-409)       /**< 固件校验失败 */
/** @} */

/**
 * @defgroup ColinkLinkErrorCode
 * @{
 */
#define COLINK_LINK_NO_ERROR            (0)        /**< 无错误 */
#define COLINK_LINK_ARG_INVALID        (-2)        /**< 无效的参数 */
#define COLINK_LINK_OPERATION_ERROR   (-50)        /**< 流程操作不对 */
/** @} */

#endif /* #ifndef COLINK_ERROR_H */
