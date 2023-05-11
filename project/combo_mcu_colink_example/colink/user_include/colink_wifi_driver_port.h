#ifndef __COLINK_WIFI_DRIVER_PORT_H__
#define __COLINK_WIFI_DRIVER_PORT_H__

#include "ln_types.h"
#include "ln_wifi_err.h"
#include "wifi_manager.h"
#include "wifi.h"
#include "wifi_port.h"
#include "wifi_manager.h"
#include "netif/ethernetif.h"
#include "utils/debug/log.h"
#include "utils/system_parameter.h"
#include "utils/ln_psk_calc.h"
#include "utils/debug/ln_assert.h"
#include "ln_misc.h"
#include "utils/ln_psk_calc.h"
#include "ln_nvds.h"
#include "ln_misc.h"
#include "utils/sysparam_factory_setting.h"

#include "lwip/apps/mdns.h"

#include "colink_define.h"
#include "colink_ble_driver_port.h"

#define PM_DEFAULT_SLEEP_MODE             (ACTIVE)
#define PM_WIFI_DEFAULT_PS_MODE           (WIFI_MAX_POWERSAVE)

#define WLAN_SSID_MAX_LEN           (32)//rang:1-32
#define WLAN_PWD_MAX_LEN            (64)//rang:8-63ascii 8-64hex
#define TOKEN_MAX_LEN               (48)

#define SCAN_AP_LIST_CNT					(32)

typedef struct
{
	char ssid[64 + 1];
	char pwd[64 + 1];
	uint8_t bssid[8 + 1];
} COLINK_WIFI_CFG;

typedef struct
{
	COLINK_WIFI_CFG sta;
	COLINK_WIFI_CFG sap;
	
	COLINK_WIFI_CFG* apList[SCAN_AP_LIST_CNT];
	uint8_t apListCnt;
	
	bool bWifiInited;
	bool bGetIpSuc;
	
	OS_Semaphore_t ConnectApSemaphore;
	OS_Semaphore_t GetIpSemaphore;
	
	void (*init)(void);
	
	void (*init_sta)(void);
	void (*sta_connect)(void);
	int (*wait_for_connect)(int);
	int (*wait_for_ip)(int);
	int (*get_sta_stat)(void);
	
	void (*scan)(void);
	void (*scan_cfg)(uint8_t *pDta);
	void (*scan_node_add)(const char* _ssid, const uint8_t* _bssid);
	void (*scan_list_release)(void);
	bool (*scan_list_find_ssid)(char* _ssid, const uint8_t* _bssid);
	
	int (*start_softap)(void);
	void (*cfg_softap)(const char* _ssid, const char* _pwd);
	void (*stop)(void);
	
	void (*mdns_init)(void);
	
	char* (*ssid_buf)(void);
	char* (*pwd_buf)(void);
	uint8_t* (*bssid_buf)(void);
} COLINK_WIFI_DRIVER_ADAPTER;

extern COLINK_WIFI_DRIVER_ADAPTER* get_colink_wifi_handle(void);

#endif /* #ifndef __COLINK_WIFI_DRIVER_PORT_H__ */
