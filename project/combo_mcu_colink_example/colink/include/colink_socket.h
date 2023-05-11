/**
 * @file colink_socket.h
 *
 * Show profile of colink socket.
 * It shows the detail of colink socket API.
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
#ifndef COLINK_SOCKET_H
#define COLINK_SOCKET_H

#include <stdint.h>
#include "colink_typedef.h"

/**
 * @brief 创建TCP服务
 * 
 * @par 描述:
 * 创建非阻塞模式tcp服务
 * 
 * @param port   [IN] 接收方端口号。
 * 
 * @retval COLINK_TCP_CREATE_SERVER_ERR 建立tcp服务出错
 * @retval 大于等于0  建立成功，返回tcp套接字，使用colinkTcpState判断是否有外部连接建立
 */
int32_t colinkCreateTcpServer(uint16_t port);

/**
 * @brief tcp服务状态
 * 
 * @par 描述:
 * 获取tcp服务状态
 * 
 * @param fd  tcp套接字
 * 
 * @retval COLINK_TCP_NO_ERROR 与客户端建立连接成功
 * @retval COLINK_TCP_SERVER_WAIT_CONNECT 等待建立连接
 */
int32_t colinkTcpServerGetState(int32_t fd);

/**
 * @brief 创建tcp连接
 *
 * @par 描述:
 * 创建非阻塞模式tcp连接
 *
 * @param dst      [IN] 接收方ip地址。
 * @param port     [IN] 接收方端口号。
 *
 * @retval COLINK_TCP_ARG_INVALID           dst为空。
 * @retval COLINK_TCP_CREATE_CONNECT_ERR    创建连接失败。
 * @retval 大于0  连接成功,返回tcp套接字，然后使用colinkTcpState判断连接是否完全建立。
 */
int32_t colinkTcpConnect(const char* dst, uint16_t port);

/**
 * @brief tcp连接状态
 *
 * @par 描述:
 * 查询tcp连接状态
 *
 * @param fd      [IN] tcp套接字。
 *
 * @retval ColinkTcpErrorCode   colink tcp错误码。
 * @see ColinkTcpErrorCode
 */
int32_t colinkTcpState(int32_t fd);

/**
 * @brief 断开tcp连接。
 *
 * @par 描述:
 * 断开tcp连接。
 *
 * @param fd   [IN] colinkTcpConnect创建的套接字。
 *
 * @retval 无。
 */
void colinkTcpDisconnect(int32_t fd);

/**
 * @brief 发送tcp数据
 *
 * @par 描述:
 * 非阻塞发送tcp数据
 *
 * @param fd      [IN] tcp套接字。
 * @param buf     [IN] 指向待发送数据缓冲区的指针。
 * @param len     [IN] 待发送数据的长度，范围为[0，512)。
 *
 * @retval ColinkTcpErrorCode  colink tcp错误码。
 * @see ColinkTcpErrorCode
 */
int32_t colinkTcpSend(int32_t fd, const uint8_t* buf, uint16_t len);

/**
 * @brief 读取tcp数据
 *
 * @par 描述:
 * 非阻塞读取tcp数据
 *
 * @param fd      [IN] tcp套接字。
 * @param buf     [IN] 指向存放接收数据缓冲区的指针。
 * @param len     [IN] 存放接收数据缓冲区的最大长度，范围为[0，512)。
 *
 * @retval ColinkTcpErrorCode  colink tcp错误码。
 * @see ColinkTcpErrorCode
 */
int32_t colinkTcpRead(int32_t fd, uint8_t* buf, uint16_t len);

/**
 * @brief 创建加密tcp连接
 *
 * @par 描述:
 * 创建非阻塞模式加密tcp连接
 *
 * @param dst      [IN] 接收方ip地址。
 * @param port     [IN] 接收方端口号。
 *
 * @retval COLINK_TCP_ARG_INVALID           dst为空。
 * @retval COLINK_TCP_CREATE_CONNECT_ERR    创建连接失败。
 * @retval 大于0  连接成功,返回tcp套接字，然后使用colinkTcpSslState判断连接是否完全建立。
 */
int32_t colinkTcpSslConnect(const char* dst, uint16_t port);

/**
 * @brief 加密tcp连接状态
 *
 * @par 描述:
 * 查询加密tcp连接状态
 *
 * @param fd      [IN] tcp套接字。
 *
 * @retval ColinkTcpErrorCode   colink tcp错误码。
 * @see ColinkTcpErrorCode
 */
int32_t colinkTcpSslState(int32_t fd);

/**
 * @brief 断开加密tcp连接。
 *
 * @par 描述:
 * 断开加密tcp连接。
 *
 * @param fd   [IN] colinkTcpSslConnect创建的套接字。
 *
 * @retval 无。
 */
void colinkTcpSslDisconnect(int32_t fd);

/**
 * @brief 发送加密tcp数据
 *
 * @par 描述:
 * 非阻塞发送加密tcp数据
 *
 * @param fd      [IN] tcp套接字。
 * @param buf     [IN] 指向待发送数据缓冲区的指针。
 * @param len     [IN] 待发送数据的长度，范围为[0，512)。
 *
 * @retval ColinkTcpErrorCode  colink tcp错误码。
 * @see ColinkTcpErrorCode
 */
int32_t colinkTcpSslSend(int32_t fd, const uint8_t* buf, uint16_t len);

/**
 * @brief 读取加密tcp数据
 *
 * @par 描述:
 * 非阻塞读取加密tcp数据
 *
 * @param fd      [IN] tcp套接字。
 * @param buf     [IN] 指向存放接收数据缓冲区的指针。
 * @param len     [IN] 存放接收数据缓冲区的最大长度，范围为[0，512)。
 *
 * @retval ColinkTcpErrorCode  colink tcp错误码。
 * @see ColinkTcpErrorCode
 */
int32_t colinkTcpSslRead(int32_t fd, uint8_t* buf, uint16_t len);

/**
 * @brief 获取远端主机ip地址。
 *
 * @par 描述:
 * 通过DNS域名解析，获取远端主机ip地址，此接口实现为非阻塞。
 *
 * @param hostname    [IN] 远端主机名称。
 * @param num         [IN] 远端主机名称长度。
 *
 * @retval ColinkTcpErrorCode   colink tcp错误码。
 * @see ColinkTcpErrorCode
 */
int32_t colinkGethostbyname(const char* hostname, uint8_t len);

/**
 * @brief 获取DNS解析状态。
 *
 * @par 描述:
 * 获取DNS解析状态
 *
 * @param ip [OUT] 解析后的ip地址字符串 ("xxx.xxx.xxx.xxx")。
 *
 * @retval COLINK_TCP_NO_ERROR 解析成功
 * @retval COLINK_TCP_DNS_PARSING 解析中
 * @retval COLINK_TCP_DNS_PARSE_ERR 解析失败
 * @retval COLINK_TCP_DNS_PARSE_TIMEOUT 解析超时
 */
int32_t colinkGethostbynameState(char *ip);

#endif
