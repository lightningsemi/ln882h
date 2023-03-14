#include "ap_scan.h"

static void ap_startup_cb(void * arg);
static void ap_scan_cpl_cb(void *arg);

static uint8_t mac_addr[6]        = {0x00, 0x50, 0xC2, 0x5E, 0xAA, 0xDA};
static uint8_t psk_value[40]      = {0x0};
static wifi_softap_cfg_t ap_cfg = {
    .ssid            = "LN_SOFTAP",
    .pwd             = "12345678",
    .bssid           = mac_addr,
    .ext_cfg = {
        .channel         = 6,
        .authmode        = WIFI_AUTH_WPA_WPA2_PSK,//WIFI_AUTH_OPEN,
        .ssid_hidden     = 0,
        .beacon_interval = 100,
        .psk_value = NULL,
    }
};

static OS_Semaphore_t *g_soft_ap_scan_sem = NULL;

/*
*    ap scan start.
*/
void app_ap_scan(void)
{
	int i = 0;
	
	wifi_mode_t curr_mode = 0;
	
	wifi_scan_cfg_t scan_cfg = {0};
	
	//usr can define scan result times,default 10
	int scan_times = 10;
	
	//usr can define ap_info_list number,limit 0 < list_number <= 100 ,default 20
	int list_number = 10; 
	
	//choose thee best channel ap
	uint8_t channel = 0;
	
	//refer to scan_times
	int scan_items[50] = {0};
	
	//init
	ap_info_t **ap_scan_result_temp = (ap_info_t **)OS_Malloc(scan_times);
	
	LOG(LOG_LVL_INFO, "sizeof(ap_info_t)=%d\r\n",sizeof(ap_info_t));
	if(ap_scan_result_temp == NULL)
	{
		LOG(LOG_LVL_ERROR, "ap_scan_result malloc fail..\r\n");
	}
	
	for(i = 0; i < scan_times; i++)
	{
		ap_scan_result_temp[i] = (ap_info_t *)OS_Malloc(sizeof(ap_info_t)*list_number);
		if(ap_scan_result_temp[i] == NULL)
		{
			LOG(LOG_LVL_ERROR, "ap_scan malloc fail at index[%d]..\r\n",i);
			continue;
		}
		memset(ap_scan_result_temp[i], 0, sizeof(ap_info_t)*list_number);
	}
	
	OS_MsDelay(3000);
	
	curr_mode = wifi_current_mode_get();
	//wifi stop
	wifi_stop();
	if (curr_mode == WIFI_MODE_STATION)
	{
			netdev_set_state(NETIF_IDX_STA, NETDEV_DOWN);
	}
	else
	{
			netdev_set_state(NETIF_IDX_AP, NETDEV_DOWN);
	}
	//ap init
	softap_init();
	
	//ap_scan
	for(int j = 0;j < scan_times; j++)
	{
		scan_items[j] = softap_scan(ap_scan_result_temp[j], list_number);
		if(scan_items[j] < 0)
		{
			LOG(LOG_LVL_ERROR, "ap_scan result fail at scan_items index[%d]..\r\n", j);
			continue;
		}
	}
	
	//func --> choose the min channenl
	channel = softap_channel_get(ap_scan_result_temp, list_number, scan_times, scan_items);
	
	LOG(LOG_LVL_INFO, "\r\n## the best channel is %d  ##\r\n", channel);
	
	for(i = 0; i < scan_times; i++)
	{
		OS_Free(ap_scan_result_temp[i]);
		ap_scan_result_temp = NULL;
	}
	OS_Free(ap_scan_result_temp);
	ap_scan_result_temp = NULL;
}

/*
* ap init
*/
void softap_init(void)
{
	tcpip_ip_info_t  ip_info;
	server_config_t  server_config;

	ip_info.ip.addr      = ipaddr_addr((const char *)"192.168.4.1");
	ip_info.gw.addr      = ipaddr_addr((const char *)"192.168.4.1");
	ip_info.netmask.addr = ipaddr_addr((const char *)"255.255.255.0");

	server_config.server.addr   = ip_info.ip.addr;
	server_config.port          = 67;
	server_config.lease         = 2880;
	server_config.renew         = 2880;
	server_config.ip_start.addr = ipaddr_addr((const char *)"192.168.4.100");
	server_config.ip_end.addr   = ipaddr_addr((const char *)"192.168.4.150");
	server_config.client_max    = 3;
	dhcpd_curr_config_set(&server_config);

	//1. net device(lwip).
	netdev_set_mac_addr(NETIF_IDX_AP, mac_addr);
	netdev_set_ip_info(NETIF_IDX_AP, &ip_info);
	netdev_set_active(NETIF_IDX_AP);
	wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_STARTUP, &ap_startup_cb);

	sysparam_softap_mac_update((const uint8_t *)mac_addr);

	ap_cfg.ext_cfg.psk_value = NULL;
	if ((strlen(ap_cfg.pwd) != 0) &&
			(ap_cfg.ext_cfg.authmode != WIFI_AUTH_OPEN) &&
			(ap_cfg.ext_cfg.authmode != WIFI_AUTH_WEP)) {
			memset(psk_value, 0, sizeof(psk_value));
			if (0 == ln_psk_calc(ap_cfg.ssid, ap_cfg.pwd, psk_value, sizeof (psk_value))) {
					ap_cfg.ext_cfg.psk_value = psk_value;
					hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
			}
	}

	//2. wifi
	if(WIFI_ERR_NONE !=  wifi_softap_start(&ap_cfg)){
			LOG(LOG_LVL_ERROR, "[%s, %d]wifi_start() fail.\r\n", __func__, __LINE__);
	}
}

/*
* ap scan 
*/
int softap_scan(ap_info_t *ap_info_buf, int list_num)
{
	wifi_scan_cfg_t scan_cfg = {
		.scan_time = 20,
	};
	ap_info_t *ap_info = NULL;
	ap_info_t *ap_info_rst = NULL;
	int ap_info_rst_items = 0;
	
	g_soft_ap_scan_sem = OS_Malloc(sizeof(OS_Semaphore_t));
	
	if(OS_OK != OS_SemaphoreCreate(g_soft_ap_scan_sem, 0, 1))
	{
		LOG(LOG_LVL_ERROR,"g_soft_ap_sem create fail..\r\n");
		return -1;
	}
	
	ap_info = OS_Malloc(list_num * sizeof(ap_info_t));
	if (!ap_info) 
	{
		LOG(LOG_LVL_ERROR,"g_soft_ap_sem create fail..\r\n");
		return -1;
	}
	
	wifi_softap_scan(&scan_cfg, ap_info, list_num, ap_scan_cpl_cb);
	
	//wait ap_info get
	OS_SemaphoreWait(g_soft_ap_scan_sem, OS_WAIT_FOREVER);
	OS_SemaphoreDelete(g_soft_ap_scan_sem);
	OS_Free(g_soft_ap_scan_sem);
	g_soft_ap_scan_sem = NULL;
	
	//get scan result
	wifi_softap_scan_results_get(&ap_info_rst, &ap_info_rst_items);
	
	if(!ap_info_rst)
	{
		LOG(LOG_LVL_WARN, "get ap info result none..\r\n");
		OS_Free(ap_info);
		ap_info = NULL;
		return -1;
	}
	else
	{
		for(int i = 0; i < ap_info_rst_items; i++)
		{
			memcpy(&ap_info_buf[i], &ap_info_rst[i], sizeof(ap_info_t));
		}
	}
	
	LOG(LOG_LVL_INFO, "ap scan results:%d\r\n", ap_info_rst_items);
	
	if(ap_info)
	{
		OS_Free(ap_info);
		ap_info = NULL;		
	}
	return ap_info_rst_items;
}

static void ap_startup_cb(void *arg)
{
    netdev_set_state(NETIF_IDX_AP, NETDEV_UP);
}

/*
* scan finish cb
*/
static void ap_scan_cpl_cb(void *arg)
{
	OS_SemaphoreRelease(g_soft_ap_scan_sem);
}

/*
* return best channel
*/
uint8_t softap_channel_get(ap_info_t **ap_info_buf, int list_num, int scan_times, int *scan_items)
{
	uint8_t channel = 0;
	int32_t rssi_sum[13] = {0};			//13 channel
	uint16_t scan_channel_cnt[13] = {0};
	float average_rssi[13] = {0};
	
	for(uint8_t i = 0; i < scan_times; i++)		//all scan times
	{
		for(uint8_t j = 0; j < scan_items[i]; j++)
		{
			switch(ap_info_buf[i][j].channel)
			{
				case 1:
					rssi_sum[0] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[0]++;
					break;
				case 2:
					rssi_sum[1] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[1]++;
					break;
				case 3:
					rssi_sum[2] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[2]++;
					break;
				case 4:
					rssi_sum[3] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[3]++;
					break;
				case 5:
					rssi_sum[4] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[4]++;
					break;
				case 6:
					rssi_sum[5] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[5]++;
					break;
				case 7:
					rssi_sum[6] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[6]++;
					break;
				case 8:
					rssi_sum[7] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[7]++;
					break;
				case 9:
					rssi_sum[8] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[8]++;
					break;
				case 10:
					rssi_sum[9] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[9]++;
					break;
				case 11:
					rssi_sum[10] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[10]++;
					break;
				case 12:
					rssi_sum[11] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[11]++;
					break;
				case 13:
					rssi_sum[12] += ap_info_buf[i][j].rssi;
					scan_channel_cnt[12]++;
					break;
				
				default:
					break;
			}
		}
	}
	//sort
	int32_t temp = 0;
	for(uint8_t x = 0; x < sizeof(rssi_sum)/sizeof(rssi_sum[0]); x++)
	{
		if(rssi_sum[x] == 0)
		{
			LOG(LOG_LVL_INFO, "##Not scan out channel[%d]..\r\n", x+1);		//channel + 1
		}
		else
		{
			average_rssi[x] = (rssi_sum[x] / scan_channel_cnt[x]) * 1.0 ;
			LOG(LOG_LVL_INFO, "##channel[%d] scan cnt = %d , average rssi = %01f \r\n", x+1, scan_channel_cnt[x], average_rssi[x]);
			if(average_rssi[x] < average_rssi[temp])
			{
				temp = x;
			}
		}
	}
	
	return (temp + 1);		//return channel
}