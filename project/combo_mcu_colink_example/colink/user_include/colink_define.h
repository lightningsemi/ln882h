#ifndef __COLINK_DEFINE_H__
#define __COLINK_DEFINE_H__

#include "utils/debug/log.h"
//#include "usr_ctrl.h"

#define os_printf(...)		Log_i(__VA_ARGS__)

typedef enum
{
    SIG_NETWORK_INIT                      = 1,
    SIG_NETWORK_COLINK_INIT                    = 2,
    SIG_NETWORK_JOIN_AP_ERR                  = 3,
    SIG_NETWORK_ESPTOUCH_START  = 4,
    SIG_NETWORK_ESPTOUCH_STOP   = 5,
    SIG_NETWORK_SETTING_START   = 6,
    SIG_NETWORK_SETTING_STOP   = 7,
    SIG_NETWORK_SOCKET_TEST   = 8,
    SIG_NETWORK_COLINK_PROCESS = 9,
}ColinkSignalSet;

#define DEVICE_CONFIG_START_SEC (0x37C)

#define MODEL			"CK-BL602-5PWM-01"
#define VERSION			"3.4.0"

#define SDK_VERSION		"git-a26d881cb35261b001ed86b1fec75731796e6933"

#ifndef COLINK_MDNS_SUPPORT
#define COLINK_MDNS_SUPPORT			0
#endif

#define LOCAL_IP		"10.10.7.1" //192.168.1.1   10.10.7.1
#define DHCP_IP_START	"10.10.7.100"
#define DHCP_IP_END		"10.10.7.150"

#define BLE_NAME_PRE					("ck_")

#define SOFTAP_NAME_PRE		  			("ITEAD-")
#define SOFTAP_PWD		 				("12345678")

#define	COLINK_ID_LEN					(10 + 1)
#define COLINK_APIKEY_LEN				(36 + 1)
#define COLINK_CHIPID_LEN				(24 + 1)

#define DEVICE_COLINK_ID				{"kv_device_colink_id"}
#define DEVICE_COLINK_APIKEY			{"kv_device_colink_apikey"}
#define DEVICE_COLINK_CHIPID			{"kv_device_colink_chipid"}

#define DEVICE_CONFIG_PARA  			("kv_device_config_parameter")
#define DEVICE_MATCH_MODE_PARA  		("kv_device_match_mode_parameter")
#define	DEVICE_POWERON_SWITCH_COUNT		("kv_device_poweron_switch_count")
#define DEVICE_POWERON_RESET_WITHIN_3S	("kv_device_poweron_reset_within_3s")

#endif /* #ifndef __COLINK_DEFINE_H__ */
