#include <stdio.h>
#include <string.h>
#include "iot_import.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "utils/debug/log.h"

static uint64_t _platform_time_left(uint64_t t_end, uint64_t t_now)
{
    uint64_t t_left;

    if (t_end > t_now) {
        t_left = t_end - t_now;
    } else {
        t_left = 0;
    }

    return t_left;
}

/**
 * @brief Establish a TCP connection.
 *
 * @param [in] host: @n Specify the hostname(IP) of the TCP server
 * @param [in] port: @n Specify the TCP port of TCP server
 *
 * @return The handle of TCP connection.
   @retval   0 : Fail.
   @retval > 0 : Success, the value is handle of this TCP connection.
 */
uintptr_t HAL_TCP_Establish(const char *host, uint16_t port)
{
    struct addrinfo hints;
    struct addrinfo *addrInfoList = NULL;
    struct addrinfo *cur = NULL;
    int fd = 0;
    int rc = -1;
    char service[6] = { 0 };

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    sprintf(service, "%u", port);

    if ((rc = getaddrinfo(host, service, &hints, &addrInfoList)) != 0) {
        LOG(LOG_LVL_ERROR, "%s | getaddrinfo() failed!\r\n", __FUNCTION__);
        return (uintptr_t)-1;
    }

    for (cur = addrInfoList; cur != NULL; cur = cur->ai_next) {
        if (cur->ai_family != AF_INET) {
            LOG(LOG_LVL_ERROR, "%s | socket type error!\r\n", __FUNCTION__);
            rc = -1;
            continue;
        }

        fd = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
        if (fd  < 0) {
            LOG(LOG_LVL_ERROR, "%s | create socket error!\r\n", __FUNCTION__);
            rc = -1;
            continue;
        }

        if (connect(fd, cur->ai_addr, cur->ai_addrlen) == 0) {
            rc = fd;
            break;
        }

        close(fd);
        LOG(LOG_LVL_ERROR, "%s | connect error!\r\n", __FUNCTION__);
        rc = -1;
    }

    if (-1 == rc) {
        LOG(LOG_LVL_ERROR, "%s | failed to establish tcp.\r\n", __FUNCTION__);
    } else {
        LOG(LOG_LVL_INFO, "%s | succeed to establish tcp, fd = %d\r\n", rc);
    }

    freeaddrinfo(addrInfoList);

    return (uintptr_t)rc;
}

/**
 * @brief Destroy the specific TCP connection.
 *
 * @param [in] fd: @n Specify the TCP connection by handle.
 *
 * @return The result of destroy TCP connection.
 * @retval < 0 : Fail.
 * @retval   0 : Success.
 */
int32_t HAL_TCP_Destroy(uintptr_t fd)
{
    int rc;

    rc = shutdown((int)fd, 2);
    if (0 != rc) {
        LOG(LOG_LVL_ERROR, "%s | shutdown error!\r\n", __FUNCTION__);
        return -1;
    }

    rc = close((int)fd);
    if (0 != rc) {
        LOG(LOG_LVL_ERROR, "%s | close socket error!\r\n", __FUNCTION__);
        return -1;
    }

    return 0;
}

/**
 * @brief Write data into the specific TCP connection.
 *        The API will return immediately if 'len' be written into the specific TCP connection.
 *
 * @param [in] fd @n A descriptor identifying a connection.
 * @param [in] buf @n A pointer to a buffer containing the data to be transmitted.
 * @param [in] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 *
 * @retval      < 0 : TCP connection error occur..
 * @retval        0 : No any data be write into the TCP connection in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be written in 'timeout_ms' timeout period.

 * @see None.
 */
int32_t HAL_TCP_Write(uintptr_t fd, const char *buf, uint32_t len, uint32_t timeout_ms)
{
    int         ret, err_code;
    uint32_t    len_sent;
    uint64_t    t_end, t_left;
    fd_set      sets;

    if (fd >= FD_SETSIZE) {
        return -1;
    }

    t_end = HAL_UptimeMs() + timeout_ms;
    len_sent = 0;
    err_code = 0;
    ret = 1;

    do {
        t_left = _platform_time_left(t_end, HAL_UptimeMs());

        if (0 != t_left) {
            struct timeval timeout;

            FD_ZERO(&sets);
            FD_SET(fd, &sets);

            timeout.tv_sec = t_left / 1000;
            timeout.tv_usec = (t_left % 1000) * 1000;
            ret = select(fd + 1, NULL, &sets, NULL, &timeout);
            if (ret > 0) {
                if (0 == FD_ISSET(fd, &sets)) {
                    LOG(LOG_LVL_ERROR, "%s | should not arrive!\r\n", __FUNCTION__);
                    ret = 0;
                    continue;
                }
            } else if (0 == ret) {
                break;
            } else {
                if (EINTR == errno) {
                    LOG(LOG_LVL_ERROR, "%s | EINTR be caught\r\n", __FUNCTION__);
                    continue;
                }

                err_code = -1;
                LOG(LOG_LVL_ERROR, "%s | select-write failed!\r\n", __FUNCTION__);
                break;
            }
        }

        if (ret > 0) {
            ret = send(fd, buf + len_sent, len - len_sent, 0);
            if (ret > 0) {
                len_sent += ret;
            } else if(0 == ret) {
                LOG(LOG_LVL_ERROR, "%s | no data be sent.\r\n", __FUNCTION__);
            } else {
                if (EINTR == errno) {
                    LOG(LOG_LVL_ERROR, "%s | EINTR be caught.\r\n", __FUNCTION__);
                    continue;
                }

                err_code = -1;
                LOG(LOG_LVL_ERROR, "%s | send failed!\r\n", __FUNCTION__);
                break;
            }
        }
    } while( (len_sent < len) && ( _platform_time_left(t_end, HAL_UptimeMs()) > 0 ) );

    return err_code == 0 ? len_sent : err_code;
}

/**
 * @brief Read data from the specific TCP connection with timeout parameter.
 *        The API will return immediately if 'len' be received from the specific TCP connection.
 *
 * @param [in] fd @n A descriptor identifying a TCP connection.
 * @param [out] buf @n A pointer to a buffer to receive incoming data.
 * @param [out] len @n The length, in bytes, of the data pointed to by the 'buf' parameter.
 * @param [in] timeout_ms @n Specify the timeout value in millisecond. In other words, the API block 'timeout_ms' millisecond maximumly.
 *
 * @retval       -2 : TCP connection error occur.
 * @retval       -1 : TCP connection be closed by remote server.
 * @retval        0 : No any data be received in 'timeout_ms' timeout period.
 * @retval (0, len] : The total number of bytes be received in 'timeout_ms' timeout period.

 * @see None.
 */
int32_t HAL_TCP_Read(uintptr_t fd, char *buf, uint32_t len, uint32_t timeout_ms)
{
    int             ret, err_code;
    uint32_t        len_recv;
    uint64_t        t_end, t_left;
    fd_set          sets;
    struct timeval  timeout;

    if (fd >= FD_SETSIZE) {
        LOG(LOG_LVL_ERROR, "%s | error: fd (%d) >= FD_SETSIZE (%d)!\r\n",
            __FUNCTION__, fd, FD_SETSIZE);
        return -1;
    }

    t_end = HAL_UptimeMs() + timeout_ms;
    len_recv = 0;
    err_code = 0;

    do {
        t_left = _platform_time_left(t_end, HAL_UptimeMs());
        if (0 == t_left) {
            break;
        }

        FD_ZERO(&sets);
        FD_SET(fd, &sets);

        timeout.tv_sec = t_left / 1000;
        timeout.tv_usec = (t_left % 1000) * 1000;

        ret = select(fd + 1, &sets, NULL, NULL, &timeout);
        if (ret > 0) {
            if (0 == FD_ISSET(fd, &sets)) {
                LOG(LOG_LVL_ERROR, "%s | no data for fd (%d)!\r\n", __FUNCTION__, fd);
                ret = 0;
                continue;
            }

            ret = recv(fd, buf + len_recv, len - len_recv, 0);
            if (ret > 0) {
                len_recv += ret;
            } else {
                if ( (EINTR == errno) || (EAGAIN == errno) || (EWOULDBLOCK == errno) ||
                    (EPROTOTYPE == errno) || (EALREADY == errno) || (EINPROGRESS == errno) ) {
                    continue;
                }

                LOG(LOG_LVL_ERROR, "%s | recv fail (fd: %d), errno: %d, ret: %d\r\n",
                            __FUNCTION__, fd, errno, ret);
                err_code = -2;
                break;
            }
        } else if (0 == ret) {
            break;
        } else {
            if (EINTR == errno) {
                continue;
            }
            LOG(LOG_LVL_ERROR, "%s | select-recv (fd: %d) failed errno=%d\r\n",
                    __FUNCTION__, fd, errno);
            err_code = -2;
            break;
        }
    } while( len_recv < len );

    return (0 != len_recv) ? len_recv : err_code;
}
