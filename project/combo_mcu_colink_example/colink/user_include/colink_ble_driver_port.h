#ifndef __COLINK_BLE_DRIVER_PORT_H__
#define __COLINK_BLE_DRIVER_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "json/cJSON.h"
#include "ln_kv_api.h"

#include "usr_app.h"
#include "usr_ctrl.h"

#include "colink_sysadapter.h"
#include "colink_profile.h"
#include "colink_type.h"
#include "colink_global.h"
#include "colink_network.h"
#include "colink_link.h"

#include "colink_xcrypt_driver_port.h"
				
enum{
	COLINK_BLE_EVENT_REQ_AUTH 				= 0x0000,
	COLINK_BLE_EVENT_AUTH_TOKEN 			= 0x0002,
	COLINK_BLE_EVENT_REQ_SCAN 				= 0x0004,
	COLINK_BLE_EVENT_REQ_SCAN_END 			= 0x0005,
	COLINK_BLE_EVENT_REQ_CONNECT			= 0x0007,
	COLINK_BLE_EVENT_DEVICE_INFO_REQUEST	= 0x0009,
	COLINK_BLE_EVENT_SERVER_INFO_REQUEST	= 0x000B,
	COLINK_BLE_EVENT_REQ_HIDDEN_AP_CONNECT	= 0x000D,
	COLINK_BLE_EVENT_GET_IP					= 0x000E
};

enum {
	COLINK_BLE_READY 						= 0x0000,
	COLINK_BLE_AUTH_RESP 					= 0x0001,
	COLINK_BLE_TOKEN_RESP 					= 0x0003,
	COLINK_BLE_SCAN_RESP					= 0x0005,
	COLINK_BLE_SCAN_RESP_END				= 0x0006,
	COLINK_BLE_CONNECT_RESP					= 0x0008,
	COLINK_BLE_DEVICE_INFO_RESPONSE			= 0x000A,
	COLINK_BLE_SERVER_INFO_RESPONSE 		= 0x000C,
	COLINK_BLE_GET_IP_RESP					= 0x000E

};

enum
{
	WAIT_FOR_CMD,
	PARSE_DATA
};
	
typedef struct
{
	uint8_t cmd[2];
	uint8_t len[2];
	uint8_t dta[512];
	uint16_t offset;
}COLINK_BLE_MSG;


typedef struct
{
	COLINK_BLE_MSG	recvBuf;
	COLINK_BLE_MSG  sendBuf;
	
	uint8_t stat;                     //ble rx stat
	uint8_t *pKeyMd5;
	uint8_t *pAppSrcCode;
	uint8_t *pDeviceSrcCode;
	uint8_t *pDeviceSecretCode;
	uint8_t *pAuthSrcCode;
	
	uint8_t bleConidx;
	uint16_t bleHandle;
	
	COLINK_XCRYPT_DRIVER_ADAPTER *pXcrypt;
 
	void (*init)(void);
	void (*start)(void);
	void (*refresh)(uint8_t conidx, uint16_t hdl);
	int (*receive)(const uint8_t *_src, int _len);
	void (*send)(const uint8_t _cmd, const uint8_t _len, const uint8_t *_dta, const uint32_t _dtaLen);
	
	usr_fsm_t* (*fsm)(void);
 
}COLINK_BLE_DRIVER_ADAPTER;

extern COLINK_BLE_DRIVER_ADAPTER* get_colink_ble_handle(void);

	
#ifdef __cplusplus
}
#endif

#endif /* __COLINK_BLE_DRIVER_PORT_H__ */
