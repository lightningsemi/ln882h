/**
 * @file      get_internet_info.c
 * @brief     实现了获取天气和时间的网络接口
 * @author    Ju Chao (chao.ju@qq.com)
 * @version   1.0
 * @date      2022-07-09
*/
#include "get_internet_info.h"

#include "osal/osal.h"
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"

#include "lwip/dns.h"
#include "lwip/tcpip.h"
#include "lwip/ip.h"
#include "lwip/api.h"
#include "netif/ethernetif.h"


#include "usr_app.h"
#include "cJSON.h"
#include "ln882h.h"
#include "utils/debug/log.h"

#include "lwip/sockets.h"


static OS_Semaphore_t dns_ok_mutex_Handler          = {NULL};
static OS_Semaphore_t wifi_connect_mutex_Handler    = {NULL};

ip_addr_t weather_host_ip;
weather_t today_weather;

uint32_t system_second = 0;
uint32_t ntp_second = 0;
time_t   sys_time;

void ntp_2_utc(uint32_t second,time_t *time);

/**
 * @brief Get the ntp time object
 * @param  time    时间指针
 * @return true 
 * @return false 
*/
static uint8_t get_ntp_time(uint32_t *second)
{
    struct netconn  *conn;
    struct netbuf   *buf;
    int             ret;
    ip_addr_t       ntp_ip;
    uint32_t        ntp_second;
    uint8_t         ntp_requst_buf[] = {0xdb,0x00,0x11,0xe9,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xe6,0x73,0x55,0x99,0x43,0x01,0x0d,0x46,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe6,0x73,0x96,0xef,0x8a,0xf0,0xb3,0x5b};
    uint8_t         ntp_recv_buf[50];
    struct  netbuf  *send_buf = NULL;
    IP_ADDR4(&ntp_ip,52,231,114,183);
    conn = netconn_new(NETCONN_UDP);
    
    while(1)
    {
        if (conn == NULL)
        {
            LOG(LOG_LVL_ERROR, "NTP create conn failed!\n");
            OS_MsDelay(10);
            continue;
        }

        ret = netconn_connect(conn,&ntp_ip,123);
        if (ret == -1)
        {
            LOG(LOG_LVL_ERROR, "NTP Connect failed!\n");
            netconn_delete(conn);
            OS_MsDelay(10);
            continue;
        }
        LOG(LOG_LVL_ERROR, "Connect NTP server successful! %d\n",ret);
        break;
    }
    
    //构建发送buf
    
    for(int i = 0; i < 6; i++){
        send_buf = netbuf_new();
        if(send_buf != NULL)
            break;
    }
    
    if(send_buf == NULL){
        netbuf_delete(send_buf);
        netbuf_delete(buf);
        netconn_close(conn);
        netconn_delete(conn);
        return 0;
    }
    conn->recv_timeout = 1000;
    netbuf_ref(send_buf,ntp_requst_buf,sizeof(ntp_requst_buf));
    ret = netconn_send(conn,send_buf);
    LOG(LOG_LVL_ERROR, "send_ret:%d\n",ret);
    ret = netconn_recv(conn,&buf);
    LOG(LOG_LVL_ERROR, "recv_ret:%d\n",ret);
    if(ret != 0){
        netbuf_delete(send_buf);
        netbuf_delete(buf);
        netconn_close(conn);
        netconn_delete(conn);
        return 0;
    }

    
    netbuf_copy(buf,ntp_recv_buf,netbuf_len(buf));
    

    ntp_second = ntp_recv_buf[40] * 0x01000000 + ntp_recv_buf[41] * 0x010000 + ntp_recv_buf[42] * 0x0100 + ntp_recv_buf[43] * 0x01 + 60 * 60 * 8;
    
    *second = ntp_second;
    
    netbuf_delete(send_buf);
    netbuf_delete(buf);
    netconn_close(conn);
    netconn_delete(conn);
    return 1;
}

/**
 * @brief Get the weather object
 * @return true 
 * @return false 
*/
static uint8_t get_weather(ip_addr_t ip_addr,weather_t *weather)
{
    uint8_t             ret = 0;
    int                 sock = 0,rece;
    struct sockaddr_in  client_addr;
    uint8_t             pbufdata[500];
    const uint8_t       get_buf[] = "GET /v3/weather/daily.json?key=S9Sg1Ymshl8NeI68d&location=shanghai&language=zh-Hans&unit=c&start=0&days=1\n";
    cJSON*  object = NULL;
    cJSON*  item1[1] = {0};
    cJSON*  item2[2] = {0};
    cJSON*  item3 = NULL;


    while(1){
        ret = 1;
    
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0){
            LOG(LOG_LVL_ERROR, "Socket init failed\n");
            OS_MsDelay(100);
            break;
        }
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(80);
        client_addr.sin_addr.s_addr = ip_addr.addr;
        memset(&(client_addr.sin_zero), 0, sizeof(client_addr.sin_zero));
        
        
        LOG(LOG_LVL_ERROR, "addr = %s, port = %d",ip_ntoa(&ip_addr),client_addr.sin_port);

        if (connect(sock,(struct sockaddr *)&client_addr,sizeof(struct sockaddr)) == -1)
        {
            LOG(LOG_LVL_ERROR, "Connect failed!\n");
            closesocket(sock);
            OS_MsDelay(10);
            break;
        }
        LOG(LOG_LVL_ERROR, "Connect to server successful!\n");
        
        write(sock,get_buf,sizeof(get_buf));
        
        OS_MsDelay(500);  
        rece = recv(sock, pbufdata,500,0);
        if(rece <= 0)
            break;
        ret = 0;
        break;
    }
    closesocket(sock);

    if(ret == 0){
        LOG(LOG_LVL_ERROR, "Recv server data succeed!\n");
        
    }else{
        LOG(LOG_LVL_ERROR, "Recv server failed!\n");
    }
    
    object = cJSON_Parse((char *)pbufdata);
    if(object == NULL){
        LOG(LOG_LVL_ERROR, "1cJSON_Parse failed!\n");
        cJSON_Delete(object);
        return 0;
    }

    item3 = cJSON_GetObjectItem(object, "results");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "2cJSON_Parse failed!\n");
        cJSON_Delete(object);
        return 0;
    }
    
    item2[0] = cJSON_GetArrayItem(item3, 0);
    if(item2[0] == NULL){
        LOG(LOG_LVL_ERROR, "3cJSON_Parse failed!\n");
        cJSON_Delete(object);
        return 0;
    }

    
    item3 = cJSON_GetObjectItem(item2[0], "daily");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "4LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    
    item2[0] = cJSON_GetArrayItem(item3, 0);
    if(item2[0] == NULL){
        LOG(LOG_LVL_ERROR, "5cJSON_Parse failed!\n");
        cJSON_Delete(object);
        return 0;
    }
    
    item3 = cJSON_GetObjectItem(item2[0], "code_day");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    weather->code_day = (int)atof(item3->valuestring);
    
    item3 = cJSON_GetObjectItem(item2[0], "code_night");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    weather->code_night = (int)atof(item3->valuestring);
    
    item3 = cJSON_GetObjectItem(item2[0], "high");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    weather->high = (int)atof(item3->valuestring);
    
    item3 = cJSON_GetObjectItem(item2[0], "low");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    weather->low = (int)atof(item3->valuestring);
    
    item3 = cJSON_GetObjectItem(item2[0], "humidity");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    weather->humidity = (int)atof(item3->valuestring);
    
    item3 = cJSON_GetObjectItem(item2[0], "wind_direction_degree");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    weather->wind_dir_degree = (int)atof(item3->valuestring);
    
    item3 = cJSON_GetObjectItem(item2[0], "wind_speed");
    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "LOG(LOG_LVL_ERROR, parse fail");
        cJSON_Delete(object);
        return 0;
    }
    weather->wind_speed = atof(item3->valuestring);
  
    

    cJSON_Delete(object);
    
    LOG(LOG_LVL_ERROR, "code_day:%d\n", today_weather.code_day);
    LOG(LOG_LVL_ERROR, "code_night:%d\n", today_weather.code_night);
    LOG(LOG_LVL_ERROR, "high:%d\n", today_weather.high);
    LOG(LOG_LVL_ERROR, "low:%d\n", today_weather.low);
    LOG(LOG_LVL_ERROR, "humidity:%d\n", today_weather.humidity);
    LOG(LOG_LVL_ERROR, "wind_dir_degree:%d\n", today_weather.wind_dir_degree);
    LOG(LOG_LVL_ERROR, "wind_speed:%f\n", today_weather.wind_speed);
    
    return 1;
}



uint8_t get_current_weather(ip_addr_t ip_addr,weather_t *weather)
{
    uint8_t             ret = 0;
    int                 sock = 0,rece;
    struct sockaddr_in  client_addr;
    uint8_t             pbufdata[500];
    const uint8_t       get_buf[] = "GET /v3/weather/now.json?key=S9Sg1Ymshl8NeI68d&location=shanghai&language=zh-Hans&unit=c\n";
    cJSON*  object = NULL;
    cJSON*  item1[1] = {0};
    cJSON*  item2[2] = {0};
    cJSON*  item3 = NULL;
    
    while(1){
        ret = 1;
    
        sock = socket(AF_INET,SOCK_STREAM,0);
        if(sock < 0){
            LOG(LOG_LVL_ERROR, "Socket init failed\n");
            OS_MsDelay(100);
            break;
        }
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(80);
        client_addr.sin_addr.s_addr = ip_addr.addr;
        memset(&(client_addr.sin_zero), 0, sizeof(client_addr.sin_zero));
        
        
        LOG(LOG_LVL_ERROR, "addr = %s, port = %d",ip_ntoa(&ip_addr),client_addr.sin_port);

        if (connect(sock,(struct sockaddr *)&client_addr,sizeof(struct sockaddr)) == -1)
        {
            LOG(LOG_LVL_ERROR, "Connect failed!\n");
            closesocket(sock);
            OS_MsDelay(10);
            break;
        }
        LOG(LOG_LVL_ERROR, "Connect to server successful!\n");
        
        write(sock,get_buf,sizeof(get_buf));
        
        OS_MsDelay(500);  
        rece = recv(sock, pbufdata,500,0);
        if(rece <= 0)
            break;
        ret = 0;
        break;
    }
    closesocket(sock);

    if(ret == 0){
        LOG(LOG_LVL_ERROR, "Recv server data succeed!\n");
        
    }else{
        LOG(LOG_LVL_ERROR, "Recv server failed!\n");
    }
    
    object = cJSON_Parse((const char *)pbufdata);
    if(object == NULL){
        LOG(LOG_LVL_ERROR, "1cJSON_Parse failed!\n");
        cJSON_Delete(object);
    }else{
        item3 = cJSON_GetObjectItem(object, "results");
    }

    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "2cJSON_Parse failed!\n");
        cJSON_Delete(object);
    }else{
        item2[0] = cJSON_GetArrayItem(item3, 0);
    } 
    
    if(item2[0] == NULL){
        LOG(LOG_LVL_ERROR, "3cJSON_Parse failed!\n");
        cJSON_Delete(object);
    }else{
        item3 = cJSON_GetObjectItem(item2[0], "now");
    }

    if(item3 == NULL){
        LOG(LOG_LVL_ERROR, "4cJSON_Parse failed!\n");
        cJSON_Delete(object);
    }else{
        item2[0] = cJSON_GetObjectItem(item3, "code");
    }

    if(item2[0] == NULL){
        LOG(LOG_LVL_ERROR, "6cJSON_Parse failed!\n");
        cJSON_Delete(object);
    }else{
        today_weather.code = (int)atof(item2[0]->valuestring);
    }

    item2[0] = cJSON_GetObjectItem(item3, "temperature");
    if(item2[0] == NULL){
        LOG(LOG_LVL_ERROR, "7cJSON_Parse failed!\n");
        cJSON_Delete(object);
    }else{
        today_weather.tem = (int)atof(item2[0]->valuestring);
    }
    
    cJSON_Delete(object);

    return 1;
}

/**
 * @brief  DNS回调函数
 * @param  name             My Param doc
 * @param  host_ip          My Param doc
 * @param  callback_arg     My Param doc
*/
static void dns_found(const char *name, ip_addr_t *host_ip, void *callback_arg)
{
    LOG(LOG_LVL_ERROR, "%s,%s, %s\r\n", name, callback_arg, ipaddr_ntoa(host_ip));
    weather_host_ip.addr = host_ip->addr;
    OS_SemaphoreRelease(&dns_ok_mutex_Handler);
}

/**
 * @brief  从DNS获取URL IP地址
 * @param  url              My Param doc
 * @param  ip               My Param doc
 * @param  cb               My Param doc
*/
static void lwip_get_host_ip_by_dns(char *url,ip_addr_t *ip,dns_found_callback cb)
{
    err_t ret;
    ret = dns_gethostbyname(url, ip, dns_found,"ok");
    if (ret == ERR_INPROGRESS)
    {//需要通过向dns服务器发送dns请求数据来获取hostname对应的IP地址
        LOG(LOG_LVL_ERROR, "wait...\n");
    }
    else if (ret == ERR_OK)
    {//从dns缓存表寻找hostname对应的IP地址
        LOG(LOG_LVL_ERROR, "find %s\r\n", ipaddr_ntoa(ip));
        OS_SemaphoreRelease(&dns_ok_mutex_Handler);
    }
    else if(ret == ERR_OK)
    {
        LOG(LOG_LVL_ERROR, "err%d\n",ret);
    }
}

/**
 * @brief Get the info task entry object
 * @param  params           My Param doc
*/
void get_info_task_entry(void *params)
{
    uint32_t ntp_cnt = 0xFFFF;
    uint32_t weather_cnt = 61;
    uint32_t ntp_second = 0;
    uint8_t sensor_data[20];

    cJSON_Hooks hooks;
    hooks.malloc_fn = pvPortMalloc;
    hooks.free_fn = vPortFree;
    cJSON_InitHooks(&hooks);

    //ln_tem_hum_init();
    
    // 1. 初始化信号量
    OS_Status ret_sta = OS_OK;
    ret_sta = OS_SemaphoreCreateBinary(&wifi_connect_mutex_Handler);
    if (ret_sta != OS_OK) {
        LOG(LOG_LVL_ERROR, "wifi_connect_mutex_Handler sem creat fail\r\n");
        return;
    }
    ret_sta = OS_SemaphoreCreateBinary(&dns_ok_mutex_Handler);
    if (ret_sta != OS_OK) {
        LOG(LOG_LVL_ERROR, "dns_ok_mutex_Handler sem creat fail\r\n");
        return;
    }
    
    LOG(LOG_LVL_INFO, "get_info_task_entry task start\r\n");

    // 2. 等待WIFI连接完成
    OS_SemaphoreWait(&wifi_connect_mutex_Handler, OS_WAIT_FOREVER);
    LOG(LOG_LVL_ERROR, "wifi connected...\n");

    // 3. 等待通过DNS服务器获取天气HOST IP
    lwip_get_host_ip_by_dns("api.seniverse.com",&weather_host_ip,dns_found);
    OS_SemaphoreWait(&dns_ok_mutex_Handler, OS_WAIT_FOREVER);

    // 4. 根据NTP获取时间
    get_ntp_time(&ntp_second);
		system_second = ntp_second;
    ntp_2_utc(system_second,&sys_time);
		

    // 5. 通过天气API获取天气
    LOG(LOG_LVL_INFO, "weather ip:0x%x\r\n",weather_host_ip.addr);
    get_current_weather(weather_host_ip,&today_weather);
    while (1)
    {
        unsigned portBASE_TYPE uxHighWaterMark;
        uxHighWaterMark=uxTaskGetStackHighWaterMark( NULL );
        LOG(LOG_LVL_INFO, "TASK SIZE:%d\n",uxHighWaterMark);
        
        if(ntp_cnt > 10){
            if(get_ntp_time(&ntp_second) == 1){
								system_second = ntp_second;
								ntp_2_utc(system_second,&sys_time);
							
                //update_from_ntp(ntp_second);
                ntp_cnt = 0;
            }
        }
         
        if(weather_cnt > 60){
							
            if(get_current_weather(weather_host_ip,&today_weather) == 1){
                //update_weather(&today_weather);
                weather_cnt = 0;
            }

            if(get_weather(weather_host_ip,&today_weather) == 1){
                //update_weather(&today_weather);
                weather_cnt = 0;
            }
        }

//        if(tem_hum_read_sensor_data(sensor_data) == 0){
//            today_weather.indoor_tem = -45 + (175 * (sensor_data[0] * 256 + sensor_data[1]) * 1.0 / (65535 - 1)); //转换为摄氏度
//            today_weather.indoor_hum = 100 * ((sensor_data[3] * 256 + sensor_data[4]) * 1.0 / (65535 - 1));       //转换为%
//            
//            today_weather.indoor_tem = today_weather.indoor_tem - 7;//板子发热的温度补偿
//        }
        
        weather_cnt ++;
        ntp_cnt ++;
        OS_MsDelay(1000);
    }
}

void change_wifi_status(uint8_t sta)
{
    if(sta == 1){
        OS_SemaphoreRelease(&wifi_connect_mutex_Handler);
    }
}

void ntp_2_utc(uint32_t second,time_t *time)
{
    //NTP时间翻译
    #define YEAR400    (365*4*100 + (4*(100/4 - 1) + 1))
    #define YEAR100    (365*100 + (100/4 - 1))
    #define YEAR004    (365*4 + 1)
    #define YEAR001    365

    int day  = second / (60 * 60 * 24);
    int y400 = day/YEAR400;
    int y100 = (day - y400*YEAR400)/YEAR100;
    int y004 = (day - y400*YEAR400 - y100*YEAR100)/YEAR004;
    int y001 = (day - y400*YEAR400 - y100*YEAR100 - y004*YEAR004) / YEAR001;
    int year = y400*4*100 + y100*100 + y004*4 + y001*1 + 1900;
    int date = (day - y400*YEAR400 - y100*YEAR100 - y004*YEAR004) % YEAR001;
    int n    = 0;
    int month= 0;
    int week = 0;

    
    uint8_t month_day[13] = {0,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (!(year%1000)){
        if (!(year/1000%4)){
            month_day[2] = 29;
        }
    }else{
        if (!(year%4)){
            month_day[2] = 29;
        }
    }

    for (n=1; n<=12; n++){
        if (date - month_day[n] <= 0){
            break;
        }else{
            date = date - month_day[n];
        }
    }

    date = date;
    month = n;
    
    time->year  = year;
    time->month = month;
    time->day   = date;
    
    if(month < 3)month += 12;
    
    week = (year-2000) + ((year-2000)/4) + (20 / 4) - 20 * 2 + (26 * (month + 1) / 10) + date - 1;
    week = week % 7;

    time->week   = week;
    time->hour   = second % (60 * 60 * 24) / (60 * 60);
    time->minute = second % (60 * 60) / (60);
    time->second = second % (60);


		LOG(LOG_LVL_ERROR, "utc-time:%d-%d-%d %d %d:%d:%d\r\n",time->year,time->month,time->day,time->week,time->hour,time->minute,time->second);

}

