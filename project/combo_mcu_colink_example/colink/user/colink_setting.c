#include "FreeRTOS.h"
#include "task.h"
#include "lwip/sockets.h"
#include "json/cJSON.h"
#include "colink_define.h"
#include "colink_setting.h"
#include "colink_network.h"
#include "colink_type.h"
#include "colink_global.h"
#include "colink_link.h"
#include "colink_sysadapter.h"
#include "ln_kv_api.h"
#include "colink_wifi_driver_port.h"
#include "colink_xcrypt_driver_port.h"

ColinkLinkInfo colinkInfoSetting;

OS_Thread_t ap_pair_thread;


bool isRecvComplete(char *buffer, uint16_t len)
{
    char *t1 = NULL;
    char *t2 = NULL;
    int32_t content_length = 0;

    t1 = colinkStrstr((const char *)buffer, "Content-Length: ");
    if (NULL == t1)
    {
        return true;
    }

    t1 += colinkStrlen("Content-Length: ");
    content_length = colinkAtoi(t1);

    t2 = colinkStrstr((const char *)buffer, "\r\n\r\n");
    if (NULL == t2)
    {
        return false;
    }

    t2 += 4;
    if (content_length > colinkStrlen(t2))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void colinkSettingTask(void* pData)
{
    char *outBuff = NULL;
    uint16_t outLen = 0;
    ColinkLinkInfo colinkInfo;
    int sockfd, newconn, size, ret;
    struct sockaddr_in address, remotehost;
    char *recv_buffer = NULL;
    uint8_t recv_times = 0;

    os_printf("colinkSettingTask");

    //chipid随便填一个
    colinkLinkInit((char *)get_colink_xcrypt_handle()->colinkId, 
					(char *)get_colink_xcrypt_handle()->colinkApikey, 
					(char *)get_colink_xcrypt_handle()->colinkChipid);
    colinkLinkReset();

    recv_buffer = (char *)OS_Malloc(512);
    outBuff = (char *)OS_Malloc(512);

    if(NULL == recv_buffer)
    {
        os_printf("os_malloc err\r\n");
        OS_ThreadDelete(&ap_pair_thread);
        return;
    }
    /* create a TCP socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        os_printf("can not create socket\r\n");
        OS_Free(recv_buffer);
        OS_ThreadDelete(&ap_pair_thread);
        return;
    }

    /* bind to port 80 at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    address.sin_addr.s_addr = inet_addr(LOCAL_IP);
    if (bind(sockfd, (struct sockaddr *)&address, sizeof (address)) < 0)
    {
        os_printf("can not bind socket\r\n");
        close(sockfd);
        OS_Free(recv_buffer);
        OS_ThreadDelete(&ap_pair_thread);
        return;
    }

    /* listen for connections (TCP listen backlog = 1) */
    listen(sockfd, 1); 
    size = sizeof(remotehost);

    while (1)
    {
        newconn = accept(sockfd, (struct sockaddr *)&remotehost, (socklen_t *)&size);

        if (newconn >= 0)
        {
            vTaskDelay(1000 / portTICK_RATE_MS);
            ret = read(newconn, recv_buffer, 512);
            while (!isRecvComplete(recv_buffer, ret))
            {
                ret += read(newconn, (recv_buffer + ret), 512);
                recv_times++;
                if (10 < recv_times)
                {
                    break;
                }
            }

            ret = colinkLinkParse((uint8_t*)recv_buffer, ret, (uint8_t*)outBuff, 1024, &outLen);
            os_printf("outBuff %d = %s", outLen, outBuff);
            os_printf("ret = %d", ret);

            if(COLINK_LINK_RES_DEV_INFO_OK == ret)
            {
                ret = write(newconn, outBuff, outLen);
                os_printf("write result:%d", ret);
            }
            else if(COLINK_LINK_GET_INFO_OK == ret)
            {
                ret = write(newconn, outBuff, outLen);
                os_printf("write result:%d", ret);

                ret = colinkLinkGetInfo(&colinkInfo);
                if(!ret)
                {
                    os_printf("net info :");
                    os_printf("ssid:%s", colinkInfo.ssid);
                    os_printf("password:%s", colinkInfo.password);
                    os_printf("distor_domain:%s", colinkInfo.distor_domain);
                    os_printf("distor_port:%d", colinkInfo.distor_port);
                    
                    strcpy(colinkInfoSetting.ssid, colinkInfo.ssid);
                    strcpy(colinkInfoSetting.password, colinkInfo.password);
                    strcpy(colinkInfoSetting.distor_domain, colinkInfo.distor_domain);
                    colinkInfoSetting.distor_port =  colinkInfo.distor_port;

                    int kvret = ln_kv_set((const char *)DEVICE_CONFIG_PARA, (void *)(&colinkInfoSetting), sizeof(colinkInfoSetting));
                    if (KV_ERR_NONE != kvret)
                    {
                        Log_e("error ln_kv_set DEVICE_CONFIG_PARA with return %d\r\n", kvret);
                    }

                    close(newconn);
					
					if(DEVICE_MODE_SETTING_SELFAP == coLinkGetDeviceMode())
                    {
                        strcpy(get_colink_wifi_handle()->ssid_buf(), colinkInfo.ssid);
                        strcpy(get_colink_wifi_handle()->pwd_buf(), colinkInfo.password);
                    }
                    break;
                }
                else
                {
                    Log_e("error %d\r\n", ret);
                }
            }
            close(newconn);
        }
    }

    OS_Free(outBuff);
    OS_Free(recv_buffer);
    close(sockfd);

    get_colink_wifi_handle()->stop();
    os_printf("exit colinkSettingTask");

    if(DEVICE_MODE_SETTING_SELFAP == coLinkGetDeviceMode())
    {
		OS_SemaphoreRelease(&get_colink_wifi_handle()->ConnectApSemaphore);
    }

    //colinkProcessStart();
    OS_ThreadDelete(&ap_pair_thread);
}

void colinkSettingStart(void)
{
    if(OS_OK != OS_ThreadCreate(&ap_pair_thread, "colinkSettingTask", colinkSettingTask, NULL, 3, 256 * 4))
    {
        Log_e("create ap pair task fail..\r\n");
        while(1);
    }
}

