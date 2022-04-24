#include <stdio.h>
#include <string.h>
#include "iot_import.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "utils/debug/log.h"

#ifndef IP_PKTINFO
#define IP_PKTINFO  IP_MULTICAST_IF
#endif

int HAL_UDP_close_without_connect(intptr_t sockfd)
{
    return close((int)sockfd);
}

/**
 * @brief Create a UDP socket.
 *
 * @param [in] port: @n Specify the UDP port of UDP socket
 *
 * @retval  < 0 : Fail.
 * @retval >= 0 : Success, the value is handle of this UDP socket.
 * @see None.
 */
intptr_t HAL_UDP_create(char *host, unsigned short port)
{
    int                 rc = -1;
    char                addr[NETWORK_ADDR_LEN] = { 0 };
    char                port_ptr[6] = { 0 };
    long                socket_id = -1;
    struct addrinfo     hints;
    struct addrinfo     *res = NULL, *ainfo = NULL;
    struct sockaddr_in  *sa = NULL;

    if (NULL == host) {
        return -1;
    }

    sprintf(port_ptr, "%u", port);

    memset((char *)&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    rc = getaddrinfo(host, port_ptr, &hints, &res);
    if (0 != rc) {
        LOG(LOG_LVL_ERROR, "%s | getaddrinfo error: %d\r\n", __FUNCTION__, rc);
        return -1;
    }

    for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
        if (AF_INET == ainfo->ai_family) {
            sa = (struct sockaddr_in *)ainfo->ai_addr;
            inet_ntop(AF_INET, &sa->sin_addr, addr, NETWORK_ADDR_LEN);

            socket_id = socket(ainfo->ai_family, ainfo->ai_socktype, ainfo->ai_protocol);
            if (socket_id < 0) {
                LOG(LOG_LVL_ERROR, "%s | create socket error.\r\n", __FUNCTION__);
            }
            if (0 == connect(socket_id, ainfo->ai_addr, ainfo->ai_addrlen)) {
                break;
            }

            close(socket_id);
        }
    }

    freeaddrinfo(res);

    return socket_id;
}

intptr_t HAL_UDP_create_without_connect(const char *host, unsigned short port)
{
    int                 flag = 1;
    int                 ret = -1;
    int                 socket_id = -1;
    struct sockaddr_in  local_addr;

    if ((socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        LOG(LOG_LVL_ERROR, "%s | socket create fialed!\r\n", __FUNCTION__);
        return (intptr_t)-1;
    }

    ret = setsockopt(socket_id, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    if (ret < 0) {
        LOG(LOG_LVL_ERROR, "%s | setsockopt() SO_RESUEADDR failed!\r\n");
        return (intptr_t)-1;
    }

    flag = 1;
    #ifdef IP_RECVPKTINFO
    if ((ret = setsockopt(socket_id, IPPROTO_IP, IP_RECVPKTINFO, &flag, sizeof(flag))) < 0) {
    #else
    if ((ret = setsockopt(socket_id, IPPROTO_IP, IP_PKTINFO, &flag, sizeof(flag))) < 0) {
    #endif /*!IP_RECVPKTINFO */
        if (ret < 0) {
            close(socket_id);
            LOG(LOG_LVL_ERROR, "%s | setsockopt() IP_PKTINFO failed!\r\n");
            return (intptr_t)-1;
        }
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    if (NULL != host) {
        inet_aton(host, &local_addr.sin_addr);
    } else {
        local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    local_addr.sin_port = htons(port);
    ret = bind(socket_id, (struct sockaddr *)&local_addr, sizeof(local_addr));

    return (intptr_t)socket_id;
}

int HAL_UDP_joinmulticast(intptr_t sockfd,
                          char *p_group)
{
    int err = -1;
    int socket_id = -1;

    if (NULL == p_group) {
        return -1;
    }

    /* set loopback */
    int loop = 1;
    socket_id = (int)sockfd;
    err = setsockopt(socket_id, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if (err < 0) {
        LOG(LOG_LVL_ERROR, "%s | setsockopt() IP_MULTICAST_LOOP failed!\r\n", __FUNCTION__);
        return err;
    }

    struct ip_mreq mreq;
    mreq.imr_multiaddr.s_addr = inet_addr(p_group);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    /* join to the multicast group */
    err = setsockopt(socket_id, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (err < 0) {
        LOG(LOG_LVL_ERROR, "%s | setsockopt() IP_ADD_MEMBERSHIP failed!\r\n");
        return err;
    }

    return 0;
}

/**
 * @brief Read data from the specific UDP connection by blocked
 *
 * @param [in] p_socket @n A descriptor identifying a UDP connection.
 * @param [in] p_data @n A pointer to a buffer to receive incoming data.
 * @param [out] datalen @n The length, in bytes, of the data pointed to by the 'p_data' parameter.
 * @return
 *
 * @retval < 0 : UDP connect error occur.
 * @retval = 0 : End of file.
 * @retval > 0 : The number of byte read.
 * @see None.
 */
int HAL_UDP_read(intptr_t p_socket,
                 unsigned char *p_data,
                 unsigned int datalen)
{
    // FIXME: 并未被使用
    return 0;
}

/**
 * @brief Read data from the specific UDP connection with timeout parameter.
 *        The API will return immediately if 'datalen' be received from the specific UDP connection.
 *
 * @param [in] p_socket @n A descriptor identifying a UDP connection.
 * @param [out] p_data @n A pointer to a buffer to receive incoming data.
 * @param [out] datalen @n The length, in bytes, of the data pointed to by the 'p_data' parameter.
 * @param [in] timeouf_ms @n Specify the timeout value in millisecond. In other words, the API block timeout_ms millisecond maximumly.
 *
 * @retval          -4 : UDP connect error occur.
 * @retval          -3 : The  call  was interrupted by a signal before any data was read.
 * @retval          -2 : No any data be received in 'timeout_ms' timeout period.
 * @retval          -1 : Invalid parameter.
 * @retval           0 : End of file.
 * @retval (0,datalen] : The number of byte read.
 * @see None.
 */
int HAL_UDP_readTimeout(intptr_t p_socket,
                        unsigned char *p_data,
                        unsigned int datalen,
                        unsigned int timeout)
{
    int ret;
    struct timeval tv;
    fd_set read_fds;
    long socket_id = -1;

    if ( (0 == p_socket) || (NULL == p_data) ) {
        return -1;
    }

    socket_id = (long)p_socket;
    if (socket_id < 0) {
        return -1;
    }

    FD_ZERO(&read_fds);
    FD_SET(socket_id, &read_fds);

    tv.tv_sec = timeout / 1000;
    tv.tv_usec = (timeout % 1000) * 1000;

    ret = select(socket_id + 1, &read_fds, NULL, NULL, timeout == 0 ? NULL : &tv);
    if (ret == 0) {
        return -2; // receive timeout.
    }

    if (ret < 0) {
        if (errno == EINTR) {
            return -3; // want read.
        }

        return -4; // receive failed.
    }

    return read((long)p_socket, p_data, datalen);



    return 0;
}

/**
 * @brief   从指定的UDP句柄接收指定长度数据到缓冲区, 阻塞时间不超过指定时长, 且指定长度若接收完需提前返回, 源地址保存在出参中
 *
 * @param   fd : UDP socket的句柄
 * @param   p_remote : 存放源网络地址的结构体首地址
 * @param   p_data : 存放被接收数据的缓冲区起始地址
 * @param   datalen : 接收并存放到缓冲区中数据的最大长度, 单位是字节(Byte)
 * @param   timeout_ms : 可能阻塞的最大时间长度, 单位是毫秒
 *
 * @retval  < 0 : 接收过程中出现错误或异常
 * @retval  0 : 在指定的'timeout_ms'时间间隔内, 没有任何数据被成功接收
 * @retval  (0, len] : 在指定的'timeout_ms'时间间隔内, 被成功接收的数据长度, 单位是字节(Byte)
 */
int HAL_UDP_recvfrom(intptr_t sockfd,
                     NetworkAddr *p_remote,
                     unsigned char *p_data,
                     unsigned int datalen,
                     unsigned int timeout_ms)
{
    int socket_id = -1;
    struct sockaddr from;
    int count = -1, ret = -1;
    socklen_t addrlen = 0;
    struct timeval tv;
    fd_set read_fds;

    if ( (NULL == p_remote) || (NULL == p_data) ) {
        return -1;
    }

    socket_id = (int) sockfd;

    FD_ZERO(&read_fds);
    FD_SET(socket_id, &read_fds);

    tv.tv_sec = timeout_ms / 1000;
    tv.tv_usec = (timeout_ms % 1000) * 1000;

    ret = select(socket_id + 1, &read_fds, NULL, NULL, timeout_ms == 0 ? NULL : &tv);
    if (ret == 0) {
        return -2; // receive timeout.
    }

    if (ret < 0) {
        if (errno == EINTR) {
            return -3; // want read.
        }

        return -4; // receive failed.
    }

    addrlen = sizeof(struct sockaddr);
    count = recvfrom(socket_id, p_data, (size_t)datalen, 0, &from, &addrlen);
    if (-1 == count) {
        return -1;
    }

    if (from.sa_family == AF_INET) {
        struct sockaddr_in *sin = (struct sockaddr_in*)&from;
        inet_ntop(AF_INET, &sin->sin_addr, (char *)p_remote->addr, NETWORK_ADDR_LEN);
        p_remote->port = ntohs(sin->sin_port);
    }

    return count;
}

/**
 * @brief   在指定的UDP socket上发送指定缓冲区的指定长度, 阻塞时间不超过指定时长, 且指定长度若发送完需提前返回
 *
 * @param   sockfd : UDP socket的句柄
 * @param   p_remote : 目标网络地址结构体的首地址
 * @param   p_data : 被发送的缓冲区起始地址
 * @param   datalen: 被发送的数据长度, 单位是字节(Byte)
 * @param   timeout_ms : 可能阻塞的最大时间长度, 单位是毫秒
 *
 * @retval  < 0 : 发送过程中出现错误或异常
 * @retval  0 : 在指定的'timeout_ms'时间间隔内, 没有任何数据被成功发送
 * @retval  (0, len] : 在指定的'timeout_ms'时间间隔内, 被成功发送的数据长度, 单位是字节(Byte)
 */
int HAL_UDP_sendto(intptr_t sockfd,
                   const NetworkAddr *p_remote,
                   const unsigned char *p_data,
                   unsigned int datalen,
                   unsigned int timeout_ms)
{
    int rc = -1;
    int socket_id = -1;
    struct sockaddr_in remote_addr;

    if ( (NULL == p_remote) || (NULL == p_data) ) {
        return -1;
    }

    socket_id = (int)sockfd;
    remote_addr.sin_family = AF_INET;

    if (1 != (rc = inet_pton(remote_addr.sin_family, (const char *)p_remote->addr,
                    &remote_addr.sin_addr.s_addr))) {
        return -1;
    }
    remote_addr.sin_port = htons(p_remote->port);
    rc = sendto(socket_id, p_data, (size_t)datalen, 0,
            (const struct sockaddr *)&remote_addr, sizeof(remote_addr));
    if (-1 == rc) {
        return -1;
    }

    return rc;
}

/**
 * @brief Write data into the specific UDP connection.
 *
 * @param [in] p_socket @n A descriptor identifying a connection.
 * @param [in] p_data @n A pointer to a buffer containing the data to be transmitted.
 * @param [in] datalen @n The length, in bytes, of the data pointed to by the 'p_data' parameter.

 * @retval          < 0 : UDP connection error occur.
 * @retval [0,datalen ] : The number of bytes sent.
 * @see None.
 */
int HAL_UDP_write(intptr_t p_socket,
                  const unsigned char *p_data,
                  unsigned int datalen)
{
    int rc = -1;
    long socket_id = -1;

    socket_id = (long) p_socket;
    rc = send(socket_id, (char *)p_data, (int)datalen, 0);
    if (-1 == rc) {
        return -1;
    }

    return rc;
}
