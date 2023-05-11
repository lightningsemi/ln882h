#include "colink_port.h"

#include "usr_app.h"
#include "usr_ctrl.h"

#include "lwip/init.h"
#include "json/cJSON.h"
#include "colink_define.h"
#include "colink_profile.h"
#include "colink_setting.h"
#include "colink_sysadapter.h"

#include "colink_mcu_driver_port.h"
#include "colink_wifi_driver_port.h"
#include "colink_ble_driver_port.h"
#include "usr_ble_app.h"

static void colink_init(void *pDta);
static void colink_check_id(void *pDta);
static void colink_check_para(void *pDta);
static void colink_match_mode(void *pDta);
static void colink_connect_ap(void *pDta);

typedef enum{
	COLINK_EVENT_CHECK_ID = 1,
	COLINK_EVENT_CHECK_PARA,
	COLINK_EVENT_NO_PARA,
	COLINK_EVENT_HAS_PARA,
	COLINK_EVENT_INIT,
	COLINK_EVENT_CONNECT_AP
} COLINK_EVENT_TYPE;


static usr_stat_transf s_stateTran[] = {
		{DEVICE_MODE_INVALID,		COLINK_EVENT_INIT,			DEVICE_MODE_INIT,			colink_init},
		{DEVICE_MODE_INIT,			COLINK_EVENT_CHECK_ID,		DEVICE_MODE_GET_DEVICEID,	colink_check_id},
		{DEVICE_MODE_GET_DEVICEID,	COLINK_EVENT_CHECK_PARA,	DEVICE_MODE_START,			colink_check_para},
		{DEVICE_MODE_START,			COLINK_EVENT_NO_PARA,		DEVICE_MODE_SETTING,		colink_match_mode},
		{DEVICE_MODE_START,			COLINK_EVENT_HAS_PARA,		DEVICE_MODE_WORK_NORMAL,	colink_connect_ap},
		{DEVICE_MODE_SETTING,		COLINK_EVENT_CONNECT_AP,	DEVICE_MODE_WORK_NORMAL,	colink_connect_ap}
};

static usr_fsm_t s_colinkFsm = {
	.state = DEVICE_MODE_INVALID,
	.transNum = sizeof(s_stateTran) / sizeof(usr_stat_transf),
	.fsmTbl = s_stateTran,
};

static usr_fsm_t* get_colink_fsm()
{
	return &s_colinkFsm;
}

//colink adapter api
static void colink_start(void);

static COLINK_ADAPTER s_colinkAdapter = {
	.start = colink_start
};

COLINK_ADAPTER* get_colink_handle()
{
	return &s_colinkAdapter;
}

static void print_software_ver()
{
	Log_i("LWIP Ver: %s", LWIP_VERSION_STRING);
	Log_i("LN SDK Ver: %s", SDK_VERSION);
	Log_i("APP Ver: %s", VERSION);
	Log_i("CoLink Lib Ver: %s", colinkGetVersion());
	Log_i("cJson Ver: %s", cJSON_Version());
	Log_i("Free heap: %d",OS_GetFreeHeapSize());
}

static uint8_t colink_read_match_mode_para(void)
{
	size_t r_len = 0;
	uint8_t matchMode = MATCH_BLE;
	if (!ln_kv_has_key(DEVICE_MATCH_MODE_PARA))
	{
		Log_i("cannot find key DEVICE_MATCH_MODE_PARA, so init with MATCH_BLE");
		int kvret = ln_kv_set((const char *)DEVICE_MATCH_MODE_PARA, (void *)(&matchMode), sizeof(matchMode));
		if (KV_ERR_NONE != kvret)
		{
			Log_e("error ln_kv_set DEVICE_MATCH_MODE_PARA with return %d", kvret);
		}
	}
	else
	{
		if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_MATCH_MODE_PARA, (void *)(&matchMode), sizeof(matchMode), &r_len)) {
			Log_e("ln_kv_get DEVICE_MATCH_MODE_PARA error, goto ble match");
			matchMode = MATCH_BLE;
		}
		else
		{
			Log_i("get match mode : %d", matchMode);
		}
	}
	
	return matchMode;
}

static void colink_init(void *pDta)
{
	//print
	print_software_ver();
	
	//init
	Log_i("cJSON_Hooks init");
    cJSON_Hooks hooks = { colinkMalloc, colinkFree };
    cJSON_InitHooks(&hooks);
	
	init_log_lvl();
	get_colink_xcrypt_handle()->init();
	get_colink_mcu_handle()->init();
	get_colink_wifi_handle()->init();
	Log_i("Free heap: %d",OS_GetFreeHeapSize());

	//check if factory test
	colink_start_fsm(get_colink_fsm(), COLINK_EVENT_CHECK_ID, NULL);
}

static bool colink_get_id_info(void)
{
	size_t r_len = 0;
	//read deviceId
	if (!ln_kv_has_key((const char *)DEVICE_COLINK_ID))
	{
		Log_w("ln kv can not find DEVICE_COLINK_ID");
		return false;
	}	
	if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_COLINK_ID, (void *)(get_colink_xcrypt_handle()->colinkId), COLINK_ID_LEN, &r_len)) {
		Log_e("ln_kv_get DEVICE_COLINK_ID error");
		return false;
	}
	Log_i("DeviceId: %s", get_colink_xcrypt_handle()->colinkId);
	strcpy(get_colink_xcrypt_handle()->colinkBleName, BLE_NAME_PRE);
	strcat(get_colink_xcrypt_handle()->colinkBleName, (char*)get_colink_xcrypt_handle()->colinkId);
	strcpy(get_colink_xcrypt_handle()->colinkSoftapName, SOFTAP_NAME_PRE);
	strcat(get_colink_xcrypt_handle()->colinkSoftapName, (char*)get_colink_xcrypt_handle()->colinkId);
	
	//read 
	if (!ln_kv_has_key((const char *)DEVICE_COLINK_APIKEY))
	{
		Log_w("ln kv can not find DEVICE_COLINK_APIKEY");
		return false;
	}	
	if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_COLINK_APIKEY, (void *)(get_colink_xcrypt_handle()->colinkApikey), COLINK_APIKEY_LEN, &r_len)) {
		Log_e("ln_kv_get DEVICE_COLINK_APIKEY error");
		return false;
	}
	Log_i("Apikey: %s", get_colink_xcrypt_handle()->colinkApikey);
	
	//chipid
	if (!ln_kv_has_key((const char *)DEVICE_COLINK_CHIPID))
	{
		Log_w("ln kv can not find DEVICE_COLINK_CHIPID");
		return false;
	}	
	if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_COLINK_CHIPID, (void *)(get_colink_xcrypt_handle()->colinkChipid), COLINK_CHIPID_LEN, &r_len)) {
		Log_e("ln_kv_get DEVICE_COLINK_CHIPID error");
		return false;
	}
	Log_i("ChipID: %s", get_colink_xcrypt_handle()->colinkChipid);
	
	return true;
}

static void colink_check_id(void *pDta)
{
	while (!colink_get_id_info())
	{
		OS_MsDelay(1000);
		Log_i("wait for get colink id info ... ");
	}
	
	colink_start_fsm(get_colink_fsm(), COLINK_EVENT_CHECK_PARA, NULL);
}

static void colink_check_para(void *pDta)
{
	coLinkSetDeviceMode(DEVICE_MODE_START);
	
	//check power on reset cnt
	if (get_colink_mcu_handle()->check_reset(MATCH_MODE_CHANGE_THLD_TIMES))
	{
		uint8_t matchMode = colink_read_match_mode_para() == MATCH_SOFTAP ? MATCH_BLE : MATCH_SOFTAP;
		int kvret = ln_kv_set((const char *)DEVICE_MATCH_MODE_PARA, (void *)(&matchMode), sizeof(matchMode));
		if (KV_ERR_NONE != kvret)
		{
			Log_e("error ln_kv_set DEVICE_MATCH_MODE_PARA with return %d", kvret);
		}
	}
	
	//check para
	ColinkLinkInfo colinkInfoCopy = {0};
	size_t r_len = 0;
	int evt = COLINK_EVENT_HAS_PARA;
	if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_CONFIG_PARA, (void *)(&colinkInfoCopy), sizeof(colinkInfoCopy), &r_len)) {
        Log_i("ln_kv_get DEVICE_CONFIG_PARA error, goto match mode");
        coLinkSetDeviceMode(DEVICE_MODE_SETTING);
		
		evt = COLINK_EVENT_NO_PARA;
    }
	else{
		strcpy(get_colink_wifi_handle()->ssid_buf(), colinkInfoCopy.ssid);
		strcpy(get_colink_wifi_handle()->pwd_buf(), colinkInfoCopy.password);
		OS_SemaphoreRelease(&get_colink_wifi_handle()->ConnectApSemaphore);
		
		evt = COLINK_EVENT_HAS_PARA;
	}
	
	colink_start_fsm(get_colink_fsm(), evt, NULL);
}

static void colink_softap_match(void *pDta)
{
	Log_i("start softap match ... ");
	coLinkSetDeviceMode(DEVICE_MODE_SETTING_SELFAP);

	get_colink_wifi_handle()->cfg_softap(get_colink_xcrypt_handle()->colinkSoftapName, SOFTAP_PWD);
    if (LN_TRUE != get_colink_wifi_handle()->start_softap()) {
        Log_e("start softapcfg failed!");
    }

    colinkSettingStart();
}

static void colink_ble_match(void *pDta)
{
	Log_i("start ble matching ... ");
	get_colink_ble_handle()->init();
	get_colink_ble_handle()->start();
	
	//ble match
	while(1)
	{
		if (get_colink_wifi_handle()->wait_for_connect(5000))
		{
			Log_i("ble wifi match done!");
			OS_SemaphoreRelease(&get_colink_wifi_handle()->ConnectApSemaphore);
			break;
		}
		Log_i("wait for ble connect ... ");
		Log_d("Free heap: %d",OS_GetFreeHeapSize());
		//Log_i("timep: %d", get_colink_mcu_handle()->get_timep());
	}
}

static void colink_match_mode(void *pDta)
{
	uint8_t matchMode = colink_read_match_mode_para();
	switch(matchMode)
	{
		case MATCH_SOFTAP:
			colink_softap_match(pDta);
			break;
		case MATCH_BLE:
			colink_ble_match(pDta);
			break;
		default:
			Log_e("unknow match mode : %d", matchMode);
	}
	
	colink_start_fsm(get_colink_fsm(), COLINK_EVENT_CONNECT_AP, NULL);
}

static void colink_connect_ap(void *pDta)
{
	//Wait for network link up
	while(1)
	{
		if (get_colink_wifi_handle()->wait_for_connect(1000))
		{
			Log_i("wifi match done!");
			get_colink_wifi_handle()->sta_connect();
		}
		
		if (get_colink_wifi_handle()->wait_for_ip(1000))
		{
			break;
		}
		Log_i("wait for getting ip ... ");
		Log_i("Free heap: %d",OS_GetFreeHeapSize());
#if COLINK_MDNS_SUPPORT
		get_colink_wifi_handle()->mdns_init();
#endif
	}

    colinkProcessStart();
}

static void colink_start()
{	
	colink_start_fsm(get_colink_fsm(), COLINK_EVENT_INIT, NULL);
}
