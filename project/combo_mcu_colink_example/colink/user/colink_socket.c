#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "colink_error.h"
#include "mbedtls/net.h"
#include "mbedtls/debug.h"
#include "mbedtls/ssl.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/error.h"
#include "mbedtls/certs.h"
#include "colink_socket.h"
#include "colink_sysadapter.h"
#include "usr_ctrl.h"

#include "colink_wifi_driver_port.h"

#if !defined(MBEDTLS_CONFIG_FILE)
#include "mbedtls/config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#include <stdio.h>
#include <stdlib.h>
#define mbedtls_time            time
#define mbedtls_time_t          time_t
#define mbedtls_fprintf         fprintf
#define mbedtls_printf          printf
#define mbedtls_exit            exit
#define MBEDTLS_EXIT_SUCCESS    EXIT_SUCCESS
#define MBEDTLS_EXIT_FAILURE    EXIT_FAILURE
#endif /* MBEDTLS_PLATFORM_C */


#define COLINK_SSL
#define COLINK_VERIFY

#if defined(COLINK_VERIFY)

const char colink_test_cli_key_rsa[] =
"-----BEGIN CERTIFICATE-----\r\n"
"MIICxDCCAi2gAwIBAgIJAKZOAGaffv/UMA0GCSqGSIb3DQEBBQUAMHoxCzAJBgNV\r\n"
"BAYTAmNiMQswCQYDVQQIDAJnZDELMAkGA1UEBwwCc3oxEDAOBgNVBAoMB2Nvb2xr\r\n"
"aXQxDDAKBgNVBAsMA2RldjETMBEGA1UEAwwKY29vbGtpdC5jbjEcMBoGCSqGSIb3\r\n"
"DQEJARYNMjIwMzM1QHFxLmNvbTAgFw0xNzA3MTEwOTUzNTFaGA8yMTE3MDYxNzA5\r\n"
"NTM1MVowejELMAkGA1UEBhMCY2IxCzAJBgNVBAgMAmdkMQswCQYDVQQHDAJzejEQ\r\n"
"MA4GA1UECgwHY29vbGtpdDEMMAoGA1UECwwDZGV2MRMwEQYDVQQDDApjb29sa2l0\r\n"
"LmNuMRwwGgYJKoZIhvcNAQkBFg0yMjAzMzVAcXEuY29tMIGfMA0GCSqGSIb3DQEB\r\n"
"AQUAA4GNADCBiQKBgQDNib2yd5iOrhUahGb9YPxVXJU16uBIFMgbTlfJu0JzxdOk\r\n"
"Ejt0i3+6Ijz6ISmNY+0/ojOLlXO7qPmBDl/DQtn0faigzVOtJFJZdNaiAnUkGVvp\r\n"
"/4RCIhdmHVXj3fL2Ojcuh9ua6k2MaFUIroHiyD6c0Bict8jke1hIEpP8On2anQID\r\n"
"AQABo1AwTjAdBgNVHQ4EFgQUwFM57JgjWg7fzO/tEPjZYdYDz74wHwYDVR0jBBgw\r\n"
"FoAUwFM57JgjWg7fzO/tEPjZYdYDz74wDAYDVR0TBAUwAwEB/zANBgkqhkiG9w0B\r\n"
"AQUFAAOBgQC62kzGjskLHLuPY0Em+xl26SQmnx0mJOLgzLx13lpc5xf0vWWSsiI+\r\n"
"IGCA+ybWXavTUZAbJ2waLA5eQJCGgEosnO5Nce3OR3kxfHCdW7k+fVvQqlmU0mQR\r\n"
"K8U0/gOdogOGK7McH+UK4QYjeECcFZp1WD/uinsXg4u2hiuGBw7Dwg==\r\n"
"-----END CERTIFICATE-----\r\n";

const int32_t colink_test_cas_pem_len = sizeof(colink_test_cli_key_rsa);

#endif

static int32_t dns_addr = 0;
void dns_get_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg)
{
    Log_i("%s %d %d", name, ipaddr, callback_arg);

    if (NULL == ipaddr)
    {
        dns_addr = -1;
    }
    else
    {
        Log_i("DNS get: %d.%d.%d.%d",
              *((uint8_t *) &ipaddr->addr),
              *((uint8_t *) &ipaddr->addr + 1),
              *((uint8_t *) &ipaddr->addr + 2),
              *((uint8_t *) &ipaddr->addr + 3));
        
        if (0 == ipaddr->addr)
        {
            dns_addr = -1;
        }
        else
        {
            dns_addr = ipaddr->addr;
        }
    }
}

int32_t colinkGethostbyname(const char* hostname, uint8_t len)
{
    ip_addr_t  server_ip = {0};
    
    err_t rtVal = 0;
	
	if (!get_colink_wifi_handle()->get_sta_stat())
	{
		Log_e("wait for ip timeout!");
		return COLINK_TCP_DNS_PARSE_TIMEOUT;
	}
	
    Log_i("[colinkGethostbyname] hostname:%s len:%d", hostname, len);
    rtVal = dns_gethostbyname(hostname, &(server_ip), dns_get_callback, NULL);
	
	if (0 != server_ip.addr && ERR_OK == rtVal)
    {
        Log_i("%d.%d.%d.%d", *((uint8_t *) &server_ip.addr),
              *((uint8_t *) &server_ip.addr + 1),
              *((uint8_t *) &server_ip.addr + 2),
              *((uint8_t *) &server_ip.addr + 3));
        dns_addr = server_ip.addr;
    }

    return COLINK_TCP_NO_ERROR;
}

int32_t colinkGethostbynameState(char *ip)
{
    if (-1 == dns_addr)
    {
        return COLINK_TCP_DNS_PARSE_ERR;
    }
    else if (0 != dns_addr)
    {
        colinkStrcpy(ip, inet_ntoa(dns_addr));
        return COLINK_TCP_NO_ERROR;
    }
    else
    {
        return COLINK_TCP_DNS_PARSING;
    }
}

int32_t colinkCreateTcpServer(uint16_t port)
{
    int sockfd;
    struct sockaddr_in address;
    int flags;
    int reuse;

    /* create a TCP socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        Log_e("can not create socket");
        return COLINK_TCP_CREATE_SERVER_ERR;
    }

    flags = fcntl(sockfd, F_GETFL, 0);

    if (flags < 0 || fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        Log_e("fcntl: %s", strerror(errno));
        close(sockfd);
        return COLINK_TCP_CONNECT_ERR;
    }

    reuse = 1;

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                   (const char*) &reuse, sizeof(reuse)) != 0)
    {
        close(sockfd);
        Log_e("set SO_REUSEADDR failed");
        return COLINK_TCP_CONNECT_ERR;
    } 

    /* bind to port 80 at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *)&address, sizeof (address)) < 0)
    {
        Log_e("can not bind socket");
        close(sockfd);
        return COLINK_TCP_CREATE_SERVER_ERR;
    }

    /* listen for connections (TCP listen backlog = 1) */
    listen(sockfd, 1);

    return sockfd;
}

int32_t colinkTcpServerGetState(int32_t fd)
{
    int errcode = 0;
    int tcp_fd = fd;
    int new_fd = -1;
    
    if (tcp_fd < 0)
    {
        return COLINK_TCP_CONNECT_ERR;
    }

    fd_set rset, wset;
    int ready_n;

    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_CLR(tcp_fd, &rset);
    FD_CLR(tcp_fd, &wset);
    FD_SET(tcp_fd, &rset);
    FD_SET(tcp_fd, &wset);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100;

    ready_n = select(tcp_fd + 1, &rset, &wset, NULL, &timeout);

    if (0 == ready_n)
    {
        errcode = COLINK_TCP_SERVER_WAIT_CONNECT;
    }
    else if (ready_n < 0)
    {
        Log_e("select error");
        errcode = COLINK_TCP_CONNECT_ERR;
    }
    else
    {
        if (FD_ISSET(tcp_fd, &wset) != 0)
        {
            new_fd = accept(tcp_fd, NULL, NULL);
            if (0 <= new_fd)
            {
                return new_fd;
            }
            else
            {
                errcode = COLINK_TCP_CONNECT_ERR;
            }
        }
        else
        {
            int ret;
            socklen_t len = (socklen_t) sizeof(int);

            if (0 != getsockopt(tcp_fd, SOL_SOCKET, SO_ERROR, &ret, &len))
            {
                Log_e("getsocketopt failed");
                errcode = COLINK_TCP_CONNECT_ERR;
            }

            if (0 != ret)
            {
                errcode = COLINK_TCP_CONNECT_ERR;
            }

            new_fd = accept(tcp_fd, NULL, NULL);
            if (0 <= new_fd)
            {
                return new_fd;
            }
            else
            {
                errcode = COLINK_TCP_CONNECT_ERR;
            }
        }
    }

    return errcode;
}

#if defined(COLINK_SSL)

#define DEBUG_LEVEL  0

static void my_debug( void *ctx, int level,
                      const char *file, int line,
                      const char *str )
{
    ((void) level);

    LOG(LOG_LVL_INFO, "%s:%04d: %s", file, line, str);
}

mbedtls_ssl_context ssl;
mbedtls_ssl_config conf;
mbedtls_entropy_context entropy;
mbedtls_ctr_drbg_context ctr_drbg;
mbedtls_net_context server_fd;

#if defined(COLINK_VERIFY)

mbedtls_x509_crt cacert;

#endif

int32_t colinkTcpSslConnect(const char* dst, uint16_t port)
{
    int ret;

    Log_i("colinkTcpSslConnect dst: %s port: %d", dst, port);

    if (NULL == dst)
    {
        return COLINK_TCP_ARG_INVALID;
    }

    mbedtls_debug_set_threshold(DEBUG_LEVEL);

    mbedtls_ssl_init(&ssl);

#if defined(COLINK_VERIFY)

    mbedtls_x509_crt_init( &cacert );

#endif

    mbedtls_ctr_drbg_init(&ctr_drbg);
    mbedtls_ssl_config_init(&conf);
    mbedtls_entropy_init(&entropy);

    mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );

    if ((ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                                     NULL, 0)) != 0)
    {
        return COLINK_TCP_CREATE_CONNECT_ERR;
    }

#if defined(COLINK_VERIFY)
    ret = mbedtls_x509_crt_parse( &cacert, (const unsigned char *)colink_test_cli_key_rsa,
                                   colink_test_cas_pem_len );

    if (ret < 0)
    {
        mbedtls_printf("\r\n mbedtls_x509_crt_parse returned -0x%x\n\n", -ret);
    }
#endif

    if ((ret = mbedtls_ssl_config_defaults(&conf,
                                           MBEDTLS_SSL_IS_CLIENT,
                                           MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0)
    {
        mbedtls_printf("mbedtls_ssl_config_defaults returned %d", ret);
        return COLINK_TCP_CREATE_CONNECT_ERR;
    }

#if defined(COLINK_VERIFY)

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_REQUIRED/*MBEDTLS_SSL_VERIFY_OPTIONAL*/);
    mbedtls_ssl_conf_ca_chain( &conf, &cacert, NULL );
#else

    mbedtls_ssl_conf_authmode(&conf, MBEDTLS_SSL_VERIFY_NONE);

#endif

    mbedtls_ssl_conf_rng(&conf, mbedtls_ctr_drbg_random, &ctr_drbg);
  
    mbedtls_ssl_conf_read_timeout(&conf, 0);
    
    //mbedtls_ssl_conf_dbg( &conf, my_debug, stdout );
    
    if ((ret = mbedtls_ssl_setup(&ssl, &conf)) != 0)
    {
        mbedtls_printf("mbedtls_ssl_setup returned -0x%x\n\n", -ret);
        return COLINK_TCP_CREATE_CONNECT_ERR;
    }

    mbedtls_net_init(&server_fd);

    struct sockaddr_in servaddr;
    int flags;
    int reuse = 1;

    server_fd.fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd.fd < 0)
    {
        Log_e("ssl creat socket fd failed");
        return COLINK_TCP_CREATE_CONNECT_ERR;
    }

    flags = fcntl(server_fd.fd, F_GETFL, 0);
    
    if (flags < 0 || fcntl(server_fd.fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        Log_e("ssl fcntl: %s", strerror(errno));
        close(server_fd.fd);
        return COLINK_TCP_CREATE_CONNECT_ERR;
    }

    if (setsockopt(server_fd.fd, SOL_SOCKET, SO_REUSEADDR,
                   (const char*)&reuse, sizeof(reuse)) != 0)
    {
        close(server_fd.fd);
        Log_e("ssl set SO_REUSEADDR failed");
        return COLINK_TCP_CREATE_CONNECT_ERR;
    }

    memset(&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(dst);
    servaddr.sin_port = htons(port);

    if (connect(server_fd.fd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) == 0)
    {
        Log_e("ssl dst %s errno %d", dst, errno);
    }
    else
    {
        Log_i("ssl dst %s port %d errno %d", dst, port, errno);

        if (errno == EINPROGRESS)
        {
            Log_i("ssl tcp conncet noblock");
        }
        else
        {
            close(server_fd.fd);
            return COLINK_TCP_CREATE_CONNECT_ERR;
        }
    }

    mbedtls_ssl_set_bio(&ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL);

    return (int32_t)&ssl;
}

void colinkTcpSslDisconnect(int32_t fd)
{
    mbedtls_ssl_context *pssl = (mbedtls_ssl_context *)fd;

    if (NULL == pssl)
    {
        Log_i("colinkTcpSslDisconnect ????????\r\n");
    }

    mbedtls_ssl_close_notify(pssl);

    mbedtls_net_free((mbedtls_net_context*)(pssl->p_bio));

#if defined(COLINK_VERIFY)

    mbedtls_x509_crt_free( &cacert );

#endif

    mbedtls_ssl_free( pssl);
    mbedtls_ssl_config_free( &conf );
    mbedtls_ctr_drbg_free( &ctr_drbg );
    mbedtls_entropy_free( &entropy );

    Log_i("colinkTcpSslDisconnect");
}

int32_t colinkTcpSslState(int32_t fd)
{
    int errcode = COLINK_TCP_NO_ERROR;
    mbedtls_ssl_context *pssl = (mbedtls_ssl_context *)fd;
    int tcp_fd = ((mbedtls_net_context*)(pssl->p_bio))->fd;
    int ret;

    if (tcp_fd < 0 || NULL == pssl)
    {
        return COLINK_TCP_ARG_INVALID;
    }

    fd_set rset, wset;
    int ready_n;

    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_CLR(tcp_fd, &rset);
    FD_CLR(tcp_fd, &wset);
    FD_SET(tcp_fd, &rset);
    FD_SET(tcp_fd, &wset);

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    ready_n = select(tcp_fd + 1, &rset, &wset, NULL, &timeout);

    if (0 == ready_n)
    {
        errcode = COLINK_TCP_CONNECTING;
    }
    else if (ready_n < 0)
    {
        errcode = COLINK_TCP_CONNECT_ERR;
    }
    else
    {
        if (FD_ISSET(tcp_fd, &wset) != 0)
        {
            Log_e("ssl COLINK_TCP_CONNECTING ");
            errcode = COLINK_TCP_CONNECTING;

            if(pssl->state != MBEDTLS_SSL_HANDSHAKE_OVER)
            { 
                ret = mbedtls_ssl_handshake_step( pssl );
                Log_e("mbedtls_ssl_handshake_step return = 0X%X", -ret);
                
                if ((0 != ret) && (MBEDTLS_ERR_SSL_WANT_READ != ret))
                { 
                    errcode = COLINK_TCP_CONNECT_ERR;
                }
            }
            else
            {
                errcode = COLINK_TCP_NO_ERROR;
            }
        }
        else
        {
            socklen_t len = (socklen_t) sizeof(int);

            if (0 != getsockopt(tcp_fd, SOL_SOCKET, SO_ERROR, &ret, &len))
            {
                errcode = COLINK_TCP_CONNECT_ERR;
            }

            if (0 != ret)
            {
                errcode = COLINK_TCP_CONNECT_ERR;
            }

            errcode = COLINK_TCP_CONNECT_ERR;
        }
    }

    Log_i("colinkTcpState errcode=%d", errcode);

    return errcode;
}

int32_t colinkTcpSslSend(int32_t fd, const uint8_t* buf, uint16_t len)
{
    int ret = 0;
    mbedtls_ssl_context *pssl = (mbedtls_ssl_context *)fd;

    if (NULL == buf || NULL == pssl)
    {
        return COLINK_TCP_ARG_INVALID;
    }

    ret = mbedtls_ssl_write(pssl, buf, len);
    
    if(ret > 0)
    {
        return ret;
    }
    else if(MBEDTLS_ERR_SSL_WANT_WRITE == ret)
    {
        return 0;
    }
    else
    {
        Log_i("colinkTcpSslsend error ret = 0X%X", -ret);
        return COLINK_TCP_SEND_ERR;
    }
}

int32_t colinkTcpSslRead(int32_t fd, uint8_t* buf, uint16_t len)
{
    int ret = 0;
    mbedtls_ssl_context *pssl = (mbedtls_ssl_context *)fd;

    if (NULL == buf || NULL == pssl)
    {
        return COLINK_TCP_ARG_INVALID;
    }

    ret = mbedtls_ssl_read(pssl, buf, len);

    if(ret > 0)
    {
        return ret;
    }
    else if(MBEDTLS_ERR_SSL_WANT_READ == ret)
    {
        return 0;
    }
    else
    {
        Log_e("colinkTcpSslRead ret = 0X%X",-ret);
        return COLINK_TCP_READ_ERR;
    }
}

#endif /**< #if defined(COLINK_SSL) */

int32_t colinkTcpRead(int32_t fd, uint8_t* buf, uint16_t len)
{
    int ret = -1;

    if (buf == NULL)
    {
        return COLINK_ARG_INVALID;
    }

    ret = (int)(recv(fd, buf, len, MSG_DONTWAIT));

    if (ret <= 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            return COLINK_NO_ERROR;
        }
        else
        {
            mbedtls_printf("ret=%d errno=%d\n", ret, errno);
            return COLINK_TCP_READ_ERR;
        }
    }
    return ret;
}

int32_t colinkTcpSend(int32_t fd, const uint8_t* buf, uint16_t len)
{
    int ret = -1;

    if (buf == NULL)
    {
        return COLINK_ARG_INVALID;
    }

    ret = (int)(send(fd, buf, len, MSG_DONTWAIT));

    if (ret < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR)
        {
            return COLINK_NO_ERROR;
        }
        else
        {
            mbedtls_printf("ret=%d errno=%d\n", ret, errno);
            return COLINK_TCP_SEND_ERR;
        }
    }
    return ret;
}

void colinkTcpDisconnect(int32_t fd)
{
    close(fd);
}

int32_t colinkTcpState(int32_t fd)
{
    int errcode = 0;
    int tcp_fd = fd;
    
    if (tcp_fd < 0)
    {
        return COLINK_TCP_CONNECT_ERR;
    }

    fd_set rset, wset;
    int ready_n;

    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_CLR(tcp_fd, &rset);
    FD_CLR(tcp_fd, &wset);
    FD_SET(tcp_fd, &rset);
    FD_SET(tcp_fd, &wset);

    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    ready_n = select(tcp_fd + 1, &rset, &wset, NULL, &timeout);

    if (0 == ready_n)
    {
        errcode = COLINK_TCP_CONNECTING;
    }
    else if (ready_n < 0)
    {
        Log_e("select error\n");
        errcode = COLINK_TCP_CONNECT_ERR;
    }
    else
    {
        if (FD_ISSET(tcp_fd, &wset) != 0)
        {
            errcode = COLINK_TCP_NO_ERROR;
        }
        else
        {
            int ret;
            socklen_t len = (socklen_t) sizeof(int);;

            if (0 != getsockopt(tcp_fd, SOL_SOCKET, SO_ERROR, &ret, &len))
            {
                Log_e("getsocketopt failed\r\n");
                errcode = COLINK_TCP_CONNECT_ERR;
            }

            if (0 != ret)
            {
                errcode = COLINK_TCP_CONNECT_ERR;
            }

            errcode = COLINK_TCP_CONNECT_ERR;
        }
    }

    return errcode;
}

int32_t colinkTcpConnect(const char* dst, uint16_t port)
{
    struct sockaddr_in servaddr;
    int fd;
    int flags;
    int reuse;

    if (NULL == dst)
    {
        return COLINK_TCP_ARG_INVALID;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0);


    if (fd < 0) {
        Log_e("creat socket fd failed");
        return COLINK_TCP_CONNECT_ERR;
    }

    flags = fcntl(fd, F_GETFL, 0);

    if (flags < 0 || fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
        Log_e("fcntl: %s", strerror(errno));
        close(fd);
        return COLINK_TCP_CONNECT_ERR;
    }

    reuse = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                   (const char*) &reuse, sizeof(reuse)) != 0)
    {
        close(fd);
        Log_e("set SO_REUSEADDR failed");
        return COLINK_TCP_CONNECT_ERR;
    } 

    memset(&servaddr, 0, sizeof(struct sockaddr_in));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(dst);
    servaddr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr*)&servaddr, sizeof(struct sockaddr_in)) == 0)
    {
        Log_i("dst %s errno %d", dst, errno);
        return fd;
    }
    else
    {
        Log_i("dst %s errno %d", dst, errno);

        if (errno == EINPROGRESS)
        {
            Log_i("tcp conncet noblock");
            return fd;
        }
        else
        {
            close(fd);
            return COLINK_TCP_CONNECT_ERR;
        }
    }
}


