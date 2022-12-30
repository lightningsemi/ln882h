#include "tcp_server_echo.h"

#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"

#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/netifapi.h"

#define TCP_SER_ECHO_STK_SIZE     (512*4)
static OS_Thread_t g_tcp_ser_echo_thread;


#define BUFSZ   1500
static char recv_data[BUFSZ];

void tcp_server_echo_entry(void *arg)
{
    socklen_t sin_size;
    int server_sock, conn;
    struct sockaddr_in server_addr, client_addr;
    int ret;

    static uint64_t ser_recv_count = 0;
    
    static int optval = 0;           
    int len = sizeof(int); 
    struct timeval tv;

    uint32_t ser_port = *(uint32_t *)arg;
    
    while (1)
    {
        if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
            LOG(LOG_LVL_ERROR, "[%s, %d]create server_sock failed.\r\n", __func__, __LINE__);
            continue;
        }

        server_addr.sin_family      = AF_INET;
        server_addr.sin_port        = htons(ser_port);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

        if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
            LOG(LOG_LVL_ERROR, "[%s, %d]bind error.\r\n", __func__, __LINE__);
            closesocket(server_sock);
            continue;
        }

        if (listen(server_sock, 5) == -1){
            getsockopt(server_sock, SOL_SOCKET, SO_ERROR, &optval, (socklen_t *)&len);        
            LOG(LOG_LVL_ERROR, "[%s, %d]listen error.optval=%d\r\n", __func__, __LINE__, optval);
            closesocket(server_sock);
            continue;
        }
        
        LOG(LOG_LVL_ERROR, "[%s, %d]listening on port %d\r\n", __func__, __LINE__, ser_port);

        while (1)
        {
            ser_recv_count = 0;
            sin_size = sizeof(struct sockaddr_in);

            if ((conn = accept(server_sock, (struct sockaddr *)&client_addr, &sin_size)) < 0)
            {
                getsockopt(server_sock, SOL_SOCKET, SO_ERROR, &optval, (socklen_t *)&len);        
                LOG(LOG_LVL_ERROR, "[%s, %d]accept failed.optval=%d\r\n", __func__, __LINE__, optval);
                break;
            }

            LOG(LOG_LVL_ERROR, "[%s, %d]got a conn from (%s , %d)\r\n", __func__, __LINE__, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            tv.tv_sec  = 2;
            tv.tv_usec = 10;
            setsockopt(conn, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));
            tv.tv_sec  = 4;
            tv.tv_usec = 10;
            setsockopt(conn, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(struct timeval));

            while (1)
            {
                //---------------recv data-----------------------//
                memset(recv_data,0,sizeof(recv_data));
                
                ret = recv(conn, recv_data, BUFSZ-1, 0);
                getsockopt(conn, SOL_SOCKET, SO_ERROR, &optval, (socklen_t *)&len);        
                if (ret < 0)
                {
                    if(optval == EWOULDBLOCK){
                        LOG(LOG_LVL_ERROR, "[%s, %d] recv < 0 (recv timeout)\r\n", __func__, __LINE__ );
                        continue;
                    } else if(optval == ECONNABORTED) {
                        LOG(LOG_LVL_ERROR, "[%s, %d] recv < 0 (conn abort)\r\n", __func__, __LINE__ );
                        closesocket(conn);
                        break;
                    } else {
                        LOG(LOG_LVL_ERROR, "[%s, %d] recv < 0 (err optval = %d)\r\n", __func__, __LINE__,optval);
                        closesocket(conn);
                        break;
                    }
                }
                else if(ret == 0)
                {
                    if(optval == ERR_OK){
                        LOG(LOG_LVL_ERROR, "[%s, %d] recv = 0  ERR_OK\r\n", __func__, __LINE__);
                        continue;
                    } else {
                        LOG(LOG_LVL_ERROR, "[%s, %d] recv = 0 (err optval = %d)\r\n", __func__, __LINE__,  optval);
                        closesocket(conn);
                        break;
                    }
                }
                else
                {
                    ser_recv_count += ret;
                    LOG(LOG_LVL_ERROR, "server_recv_count = %u\r\n", ser_recv_count);
                
                    //---------------send data-----------------------//
                    ret = send(conn, recv_data, ret, 0);
                    getsockopt(conn, SOL_SOCKET, SO_ERROR, &optval, (socklen_t *)&len);        
                    if (ret < 0)
                    {
                        if(optval == EWOULDBLOCK){
                            LOG(LOG_LVL_ERROR, "[%s, %d] send < 0 (send timeout)\r\n", __func__, __LINE__);
                            continue;
                        } else if(optval == ECONNABORTED) {
                            LOG(LOG_LVL_ERROR, "[%s, %d] send < 0 (conn abort)\r\n", __func__, __LINE__);
                            closesocket(conn);
                            break;
                        } else {
                            LOG(LOG_LVL_ERROR, "[%s, %d] send < 0 (err optval = %d)\r\n", __func__, __LINE__, optval);
                            closesocket(conn);
                            break;
                        }
                    }
                    else if(ret == 0)
                    {
                        if(optval == ERR_OK){
                            LOG(LOG_LVL_ERROR, "[%s, %d] send = 0  ERR_OK\r\n", __func__, __LINE__);
                            continue;
                        } else {
                            LOG(LOG_LVL_ERROR, "[%s, %d] send = 0 (err optval = %d)\r\n", __func__, __LINE__, optval);
                            closesocket(conn);
                            break;
                        }
                    }
                }
            }
        }

        closesocket(server_sock);
        LOG(LOG_LVL_ERROR, "[%s, %d]server_sock closed.\r\n", __func__, __LINE__);
    }
}

void tcp_server_echo_task_creat(uint32_t port)
{
    static uint32_t server_port = 0;
    server_port = port;
    OS_ThreadCreate(&g_tcp_ser_echo_thread,"tcp_ser_echo", tcp_server_echo_entry, &server_port, OS_PRIORITY_NORMAL,TCP_SER_ECHO_STK_SIZE);
}



