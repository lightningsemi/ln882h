#include "colink_wifi_driver_port.h"
#include "usr_ctrl.h"

#define SOFTAPCFG_TASK_STACK_SIZE   (3 * 512)

static int ln_softapcfg_start(void);
static void ln_cfg_softap(const char* _ssid, const char* _pwd);
static void ln_wifi_stop(void);
static void ln_reset_wifi_connect_info(void);
static void ln_wifi_init_sta(void);
static void ln_scan_ap(void);
static void ln_set_scan_cfg(uint8_t *_cfg);
static void ln_scan_node_add(const char* _ssid, const uint8_t* _bssid);
static void ln_scan_list_release(void);
static bool ln_scan_list_find_ssid(char* _ssid, const uint8_t* _bssid);
static void ln_connect_to_ap(void);
static int ln_wait_for_connect_Semaphore(int time_ms);
static int ln_wait_for_ip_Semaphore(int time_ms);
static int ln_get_sta_stat(void);
static char* ln_get_sta_ssid_buf(void);
static char* ln_get_sta_pwd_buf(void);
static uint8_t* ln_get_sta_bssid_buf(void);
static void ln_mdns_init(void);

static uint8_t 		psk_value[40]      = {0x0};
static uint8_t 		mac_addr[6]        = {0x00, 0x50, 0xC2, 0x5E, 0x88, 0x99};

static wifi_scan_cfg_t s_scanCfg = {
        .channel   = 0,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = 20,
};

static COLINK_WIFI_DRIVER_ADAPTER s_colinkWifiDrvAdapter = {

	.bWifiInited = false,
	.bGetIpSuc = false,

	.init = ln_reset_wifi_connect_info,

	.init_sta = ln_wifi_init_sta,
	.sta_connect = ln_connect_to_ap,
	.wait_for_connect = ln_wait_for_connect_Semaphore,
	.wait_for_ip = ln_wait_for_ip_Semaphore,
	.get_sta_stat = ln_get_sta_stat,
	
	.scan = ln_scan_ap,
	.scan_cfg = ln_set_scan_cfg,
	.scan_node_add = ln_scan_node_add,
	.scan_list_release = ln_scan_list_release,
	.scan_list_find_ssid = ln_scan_list_find_ssid,
	
	.start_softap = ln_softapcfg_start,
	.cfg_softap = ln_cfg_softap,
	.stop = ln_wifi_stop,
	
	.mdns_init = ln_mdns_init,
	
	.ssid_buf = ln_get_sta_ssid_buf,
	.pwd_buf = ln_get_sta_pwd_buf,
	.bssid_buf = ln_get_sta_bssid_buf,
};

COLINK_WIFI_DRIVER_ADAPTER* get_colink_wifi_handle()
{
	return &s_colinkWifiDrvAdapter;
}

static void ln_reset_wifi_connect_info()
{
	COLINK_WIFI_DRIVER_ADAPTER *pWifi = get_colink_wifi_handle();
	
	pWifi->bWifiInited = false;
	memset(&pWifi->sta, 0, sizeof(COLINK_WIFI_CFG));
	memset(&pWifi->sap, 0, sizeof(COLINK_WIFI_CFG));
	
	pWifi->apListCnt = 0;
	memset(pWifi->apList, NULL, sizeof(pWifi->apList));
	
	//init semaphore
	if (OS_SemaphoreCreateBinary(&pWifi->GetIpSemaphore) != OS_OK
		|| OS_SemaphoreCreateBinary(&pWifi->ConnectApSemaphore) != OS_OK) {
        Log_e("GetIpSemaphore or ConnectApSemaphore create failed...");
        while(1);
    }
}

static char* ln_get_sta_ssid_buf()
{
	return get_colink_wifi_handle()->sta.ssid;
}

static char* ln_get_sta_pwd_buf()
{
	return get_colink_wifi_handle()->sta.pwd;
}

static uint8_t* ln_get_sta_bssid_buf()
{
	return get_colink_wifi_handle()->sta.bssid;
}

static void wifi_scan_complete_cb(void * arg)
{
    LN_UNUSED(arg);

    ln_list_t *list;
    uint8_t node_count = 0;
    ap_info_node_t *pnode;

    wifi_manager_ap_list_update_enable(LN_FALSE);

    // 1.get ap info list.
    wifi_manager_get_ap_list(&list, &node_count);
	get_colink_wifi_handle()->scan_list_release();

    // 2.print all ap info in the list.
    LN_LIST_FOR_EACH_ENTRY(pnode, ap_info_node_t, list,list)
    {
        uint8_t * mac = (uint8_t*)pnode->info.bssid;
        ap_info_t *ap_info = &pnode->info;

        Log_i("\tCH=%2d,RSSI= %3d,SSID:\"%s\", BSSID:[%02X:%02X:%02X:%02X:%02X:%02X]", 
					ap_info->channel, ap_info->rssi, ap_info->ssid,
					mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		
		uint8_t respDta[43] = {0};
		respDta[0] = strlen(ap_info->ssid); // ssid len
		if (respDta[0] > 0 && respDta[0] < 33)
		{
			memcpy(respDta + 1, ap_info->ssid, respDta[0]); // ssid
			memcpy(respDta + 1 + respDta[0], mac, 6); // bssid
			respDta[1 + respDta[0] + 6] = ap_info->authmode; // authmode 
			respDta[1 + respDta[0] + 6 + 1] = (uint8_t)(-ap_info->rssi); //rssi
			respDta[1 + respDta[0] + 6 + 1 + 1] = 0; // unconnected
			
			get_colink_wifi_handle()->scan_node_add(ap_info->ssid, mac);
			
			if (0)
			{
				goto EXIT_SCAN;
			}
			//send ble response
			colink_start_fsm(get_colink_ble_handle()->fsm(), COLINK_BLE_EVENT_REQ_SCAN, (void *)respDta);
		}
    }
	
	wifi_manager_ap_list_update_enable(LN_TRUE);
	
EXIT_SCAN:
	//end send ble response
	wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, NULL);
	
	//send end response
	colink_start_fsm(get_colink_ble_handle()->fsm(), COLINK_BLE_EVENT_REQ_SCAN_END, NULL);
}

static void ln_wifi_get_ip_cb(struct netif *nif)
{
	get_colink_wifi_handle()->bGetIpSuc = true;
	if(DEVICE_MODE_SETTING == coLinkGetDeviceMode())
	{
		colink_start_fsm(get_colink_ble_handle()->fsm(), COLINK_BLE_EVENT_GET_IP, (void *)nif);
	}
	else
	{
		Log_i("Got IP success!");
		OS_SemaphoreRelease(&get_colink_wifi_handle()->GetIpSemaphore);
	}
}

static int ln_wait_for_connect_Semaphore(int time_ms)
{
	if ( OS_SemaphoreWait(&get_colink_wifi_handle()->ConnectApSemaphore, time_ms) == OS_OK ) {
		return 1;
    }
	
	return 0;
}

static int ln_wait_for_ip_Semaphore(int time_ms)
{
	if ( OS_SemaphoreWait(&get_colink_wifi_handle()->GetIpSemaphore, time_ms) == OS_OK ) {
		return 1;
    }
	
	return 0;
}

static int ln_get_sta_stat(void)
{
	return get_colink_wifi_handle()->bGetIpSuc;
}

static void ln_wifi_disconnected_cb(void * arg)
{
	Log_i("disconnect ap!!");
	get_colink_wifi_handle()->bGetIpSuc = false;
}

static void ln_wifi_init_sta(void)
{
	if (get_colink_wifi_handle()->bWifiInited)
	{
		return;
	}
	
	sta_ps_mode_t ps_mode = PM_WIFI_DEFAULT_PS_MODE;
	
    //1. sta mac get
    if (SYSPARAM_ERR_NONE != sysparam_sta_mac_get(mac_addr)) {
        Log_e("sta mac get filed!!!");
        return;
    }
    if (mac_addr[0] == STA_MAC_ADDR0 &&
        mac_addr[1] == STA_MAC_ADDR1 &&
        mac_addr[2] == STA_MAC_ADDR2 &&
        mac_addr[3] == STA_MAC_ADDR3 &&
        mac_addr[4] == STA_MAC_ADDR4 &&
        mac_addr[5] == STA_MAC_ADDR5) {
        ln_generate_random_mac(mac_addr);
        sysparam_sta_mac_update((const uint8_t *)mac_addr);
    }

    //2. net device(lwip)
    netdev_set_mac_addr(NETIF_IDX_STA, mac_addr);
    netdev_set_active(NETIF_IDX_STA);

    //3. wifi start
    //wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);
	wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_DISCONNECTED, &ln_wifi_disconnected_cb);
	netdev_get_ip_cb_set(ln_wifi_get_ip_cb);

    if(WIFI_ERR_NONE != wifi_sta_start(mac_addr, ps_mode)){
        Log_e("wifi sta start filed!!!");
    }
	
	get_colink_wifi_handle()->bWifiInited = true;
}

static void ln_scan_ap()
{
	ln_wifi_init_sta();
	wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);

	if ( WIFI_ERR_NONE !=  wifi_sta_scan(&s_scanCfg))
	{
		Log_e("start wifi scan failed!");
	}	
}

static void ln_set_scan_cfg(uint8_t *_cfg)
{
	//cfg 0:show_hidden, 1:scan_type(0:active,1:passive,2:mixed), 2:scan_time(s) 
	s_scanCfg.scan_type = (wifi_scan_type_t)_cfg[1];
	s_scanCfg.scan_time = _cfg[2] * 1000 / 13;
}

static void ln_scan_list_release(void)
{
	COLINK_WIFI_DRIVER_ADAPTER* pWifi = get_colink_wifi_handle();
	
	pWifi->apListCnt = 0;
	for (int i = 0; i < SCAN_AP_LIST_CNT; i++)
	{
		if (pWifi->apList[i] == NULL)
		{
			return;
		}
		OS_Free(pWifi->apList[i]);
		pWifi->apList[i] = NULL;
	}
}

static void ln_scan_node_add(const char* _ssid, const uint8_t* _bssid)
{
	COLINK_WIFI_DRIVER_ADAPTER* pWifi = get_colink_wifi_handle();
	
	if (pWifi->apListCnt >= SCAN_AP_LIST_CNT)
	{
		Log_e("count of scan ap list exceed the limit(%d)!", SCAN_AP_LIST_CNT);
		return;
	}
	
	LN_COLINK_MALLOC(pWifi->apList[pWifi->apListCnt], (sizeof(COLINK_WIFI_CFG)), 0);
	strcpy(pWifi->apList[pWifi->apListCnt]->ssid, _ssid);
	memcpy(pWifi->apList[pWifi->apListCnt]->bssid, _bssid, 6);
	
	pWifi->apListCnt++;
}

static bool ln_scan_list_find_ssid(char* _ssid, const uint8_t* _bssid)
{
	bool fRes = true;
	COLINK_WIFI_DRIVER_ADAPTER* pWifi = get_colink_wifi_handle();
	
	for (int i = 0; i < pWifi->apListCnt; i++)
	{
		Log_i("search list[%d], bssid:[%02X:%02X:%02X:%02X:%02X:%02X]...", i,
					pWifi->apList[i]->bssid[0], pWifi->apList[i]->bssid[1], pWifi->apList[i]->bssid[2], 
					pWifi->apList[i]->bssid[3], pWifi->apList[i]->bssid[4], pWifi->apList[i]->bssid[5]);
		fRes = true;
		for (int j = 0; j < 6; j ++)
		{
			if (_bssid[j] != pWifi->apList[i]->bssid[j])
			{
				fRes = false;
				break;
			}
		}
		
		if (!fRes)
		{
			continue;
		}
		
		strcpy(_ssid, pWifi->apList[i]->ssid);
		Log_i("find ssid: \"%s\" from bssid:[%02X:%02X:%02X:%02X:%02X:%02X]", _ssid, 
					_bssid[0], _bssid[1], _bssid[2], _bssid[3], _bssid[4], _bssid[5]);
		return true;
	}
	
	return false;
}

static void ln_connect_to_ap()
{
	COLINK_WIFI_DRIVER_ADAPTER* pWifi = get_colink_wifi_handle();
	wifi_sta_connect_t staCfg = {0};
	
	ln_wifi_init_sta();
	
	staCfg.ssid = pWifi->sta.ssid;
	staCfg.pwd = pWifi->sta.pwd;
	staCfg.bssid = pWifi->sta.bssid;
	
	Log_d("ssid: %s", staCfg.ssid);
	Log_d("pwd: %s", staCfg.pwd);
	
	staCfg.psk_value = NULL;
    if (strlen(staCfg.pwd) != 0) {
        if (0 == ln_psk_calc(staCfg.ssid, staCfg.pwd, psk_value, sizeof (psk_value))) {
            staCfg.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }

	s_scanCfg.scan_time = 20;
	if ( WIFI_ERR_NONE !=  wifi_sta_connect(&staCfg, &s_scanCfg))
	{
		Log_e("Join failed! ssid:%s, pwd:%s", staCfg.ssid, staCfg.pwd);
	}
}

static void ln_ap_startup_cb(void * arg)
{
    netdev_set_state(NETIF_IDX_AP, NETDEV_UP);
}

static int ln_wifi_softap_startup(const char * ssid, const char * pwd, const char * local_ip)
{
	//1. macaddr
	char apssid[64 + 1] = {0};
	char appwd[64 + 1] = {0};
    uint8_t macaddr[6] = {0};
	ln_generate_random_mac(macaddr);
	
	//2. ap cfg
	wifi_softap_cfg_t ap_cfg = {
		.ssid            = apssid,
		.pwd             = appwd,
		.bssid           = macaddr,
		.ext_cfg = {
			.channel         = 7,
			.authmode        = WIFI_AUTH_WPA_WPA2_PSK,//WIFI_AUTH_OPEN,
			.ssid_hidden     = 0,
			.beacon_interval = 100,
			.psk_value = NULL,
		}
	};

    if ((ssid == NULL) || ((ssid != NULL) && (strlen(ssid) > WLAN_SSID_MAX_LEN))) {
        Log_e("wifi softap ssid invalid!");
        return LN_FALSE;
    }
    strcpy((char *)ap_cfg.ssid, ssid);

    if (pwd == NULL) {
        ap_cfg.ext_cfg.authmode = WIFI_AUTH_OPEN;
    }
    else {
        uint16_t pwdlen = strlen(pwd);
        if (pwdlen < 8) {
            Log_e("wifi softap pwd len < 8!");
            return LN_FALSE;
        }
        else if (pwdlen >= WLAN_PWD_MAX_LEN) {
            Log_e("wifi softap pwd len >= %d!", WLAN_PWD_MAX_LEN);
            return LN_FALSE;
        }
        else{
            strcpy((char *)ap_cfg.pwd, pwd);
            ap_cfg.ext_cfg.authmode = WIFI_AUTH_WPA2_PSK;
        }
    }

	//3. tcpip info
    tcpip_ip_info_t ip_info;
    ip_info.ip.addr = ipaddr_addr(local_ip);
	ip_info.gw.addr = ipaddr_addr(local_ip);
	ip_info.netmask.addr = ipaddr_addr((const char *)"255.255.255.0");

	//4. dhcp server
	server_config_t  server_config;
    server_config.server.addr   = ip_info.ip.addr;
    server_config.port          = 67;
    server_config.lease         = 2880;
    server_config.renew         = 2880;
    server_config.ip_start.addr = ipaddr_addr((const char *)DHCP_IP_START);
    server_config.ip_end.addr   = ipaddr_addr((const char *)DHCP_IP_END);
    server_config.client_max    = 3;
    dhcpd_curr_config_set(&server_config);
  
    //1. net device(lwip).
	uint8_t psk_value[40]      = {0x0};
    netdev_set_mac_addr(NETIF_IDX_AP, macaddr);
    netdev_set_ip_info(NETIF_IDX_AP, &ip_info);
    netdev_set_active(NETIF_IDX_AP);
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_SOFTAP_STARTUP, &ln_ap_startup_cb);

    sysparam_softap_mac_update((const uint8_t *)macaddr);

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
        Log_e("wifi softap start failed!");
		return LN_FALSE;
    }

    return LN_TRUE;
}

static int ln_softapcfg_start(void)
{
	COLINK_WIFI_DRIVER_ADAPTER* pWifi = get_colink_wifi_handle();
    Log_i("[APCFG] ssid:[%s],pwd:[%s],local ip:[%s]",pWifi->sap.ssid, pWifi->sap.pwd, LOCAL_IP);

    if (LN_TRUE != ln_wifi_softap_startup(pWifi->sap.ssid, pWifi->sap.pwd, LOCAL_IP)) {
        return LN_FALSE;
    }
	
	OS_MsDelay(500);//wait for ap start

    return LN_TRUE;
}

static void ln_cfg_softap(const char* _ssid, const char* _pwd)
{
	strcpy(get_colink_wifi_handle()->sap.ssid, _ssid);
    strcpy(get_colink_wifi_handle()->sap.pwd, _pwd);
}

static void ln_wifi_stop(void)
{
    OS_MsDelay(250);
    wifi_stop();
	get_colink_wifi_handle()->bWifiInited = false;
}

#if LWIP_MDNS_RESPONDER
static void
srv_txt(struct mdns_service *service, void *txt_userdata)
{
  err_t res;
  LWIP_UNUSED_ARG(txt_userdata);
  
  res = mdns_resp_add_service_txtitem(service, "path=/", 6);
  LWIP_ERROR("mdns add service txt failed\n", (res == ERR_OK), return);
}
#endif

#if LWIP_MDNS_RESPONDER
static void
mdns_example_report(struct netif* netif, u8_t result)
{
  LWIP_PLATFORM_DIAG(("mdns status[netif %d]: %d\n", netif->num, result));
}
#endif

void ln_mdns_init(void)
{
#if LWIP_MDNS_RESPONDER
  mdns_resp_register_name_result_cb(mdns_example_report);
  mdns_resp_init();
  mdns_resp_add_netif(netif_default, "lwip", 3600);
  mdns_resp_add_service(netif_default, "myweb", "_http", DNSSD_PROTO_TCP, 8081, 3600, srv_txt, NULL);
  mdns_resp_announce(netif_default);
#endif
}

