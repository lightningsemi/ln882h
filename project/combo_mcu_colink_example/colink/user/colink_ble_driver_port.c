#include "colink_ble_driver_port.h"
#include "colink_wifi_driver_port.h"

#include "usr_app.h"
#include "ble_port.h"
#include "usr_ble_app.h"

#include "lwip/netif.h"
#include "lwip/netifapi.h"
#include "netif/ethernetif.h"

#include "rwip_config.h"    // RW SW configuration TASK_APP
#include "ke_task.h"        // Kernel Task

#include "osal/osal.h"
#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "ln_utils.h"
#include "ln_misc.h"
#include "gapm_task.h"
#include "gapc_task.h"
#include "gattc_task.h"
#include "gattm_task.h"
#include "gap.h"

#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "ln_ble_advertising.h"
#include "ln_ble_rw_app_task.h"
#include "ln_ble_app_kv.h"
#include "ln_ble_connection_manager.h"
#include "ln_ble_event_manager.h"
#include "ln_ble_smp.h"
#include "usr_ble_app.h"
#include "ln_ble_app_default_cfg.h"
#include "usr_ctrl.h"

#include "wifi.h"



static void colink_ble_auth_response(void *pDta);
static void colink_ble_token_response(void *pDta);
static void colink_ble_scan_start(void *pDta);
static void colink_ble_scan_response(void *pDta);
static void colink_ble_scan_response_end(void *pDta);
static void colink_ble_connect_response(void *pDta);
static void colink_ble_connect_hidden_ap_response(void *pDta);
static void colink_ble_get_ip_response(void *pDta);
static void colink_ble_device_info_response(void *pDta);
static void colink_ble_server_info_response(void *pDta);

static usr_stat_transf s_stateTran[] = {
	{COLINK_BLE_READY,			COLINK_BLE_EVENT_REQ_AUTH,		
				COLINK_BLE_AUTH_RESP,				colink_ble_auth_response},
	{COLINK_BLE_SCAN_RESP_END,			COLINK_BLE_EVENT_REQ_AUTH,		
				COLINK_BLE_AUTH_RESP,				colink_ble_auth_response},
	{COLINK_BLE_AUTH_RESP,		COLINK_BLE_EVENT_AUTH_TOKEN,	
				COLINK_BLE_TOKEN_RESP,				colink_ble_token_response},
	{COLINK_BLE_TOKEN_RESP,		COLINK_BLE_EVENT_REQ_SCAN,
				COLINK_BLE_SCAN_RESP,				colink_ble_scan_start},
	{COLINK_BLE_SCAN_RESP,		COLINK_BLE_EVENT_REQ_SCAN,
				COLINK_BLE_SCAN_RESP,				colink_ble_scan_response},
	{COLINK_BLE_SCAN_RESP,		COLINK_BLE_EVENT_REQ_SCAN_END,
				COLINK_BLE_SCAN_RESP_END,			colink_ble_scan_response_end},
	{COLINK_BLE_SCAN_RESP_END,	COLINK_BLE_EVENT_REQ_SCAN,
				COLINK_BLE_SCAN_RESP,				colink_ble_scan_start},
	{COLINK_BLE_SCAN_RESP,		COLINK_BLE_EVENT_REQ_CONNECT,
				COLINK_BLE_CONNECT_RESP,				colink_ble_connect_response},
	{COLINK_BLE_SCAN_RESP_END,	COLINK_BLE_EVENT_REQ_CONNECT,
				COLINK_BLE_CONNECT_RESP,			colink_ble_connect_response},
	{COLINK_BLE_SCAN_RESP_END,	COLINK_BLE_EVENT_REQ_HIDDEN_AP_CONNECT,
				COLINK_BLE_CONNECT_RESP,			colink_ble_connect_hidden_ap_response},
	{COLINK_BLE_SCAN_RESP,		COLINK_BLE_EVENT_REQ_HIDDEN_AP_CONNECT,
				COLINK_BLE_CONNECT_RESP,				NULL},
	{COLINK_BLE_TOKEN_RESP,		COLINK_BLE_EVENT_REQ_HIDDEN_AP_CONNECT,
				COLINK_BLE_CONNECT_RESP,			colink_ble_connect_hidden_ap_response},
	{COLINK_BLE_CONNECT_RESP,		COLINK_BLE_EVENT_REQ_HIDDEN_AP_CONNECT,
				COLINK_BLE_CONNECT_RESP,			colink_ble_connect_hidden_ap_response},
	{COLINK_BLE_CONNECT_RESP,	COLINK_BLE_EVENT_REQ_CONNECT,
				COLINK_BLE_CONNECT_RESP,				colink_ble_connect_response},
	{COLINK_BLE_CONNECT_RESP,	COLINK_BLE_EVENT_GET_IP,
				COLINK_BLE_GET_IP_RESP,				colink_ble_get_ip_response},
	{COLINK_BLE_CONNECT_RESP,	COLINK_BLE_EVENT_REQ_SCAN,
				COLINK_BLE_CONNECT_RESP,				NULL},
	{COLINK_BLE_CONNECT_RESP,	COLINK_BLE_EVENT_REQ_SCAN_END,
				COLINK_BLE_CONNECT_RESP,				NULL},
	{COLINK_BLE_GET_IP_RESP,	COLINK_BLE_EVENT_REQ_CONNECT,
				COLINK_BLE_DEVICE_INFO_RESPONSE,	colink_ble_device_info_response},
	{COLINK_BLE_GET_IP_RESP,	COLINK_BLE_EVENT_REQ_HIDDEN_AP_CONNECT,
				COLINK_BLE_DEVICE_INFO_RESPONSE,	colink_ble_device_info_response},
	{COLINK_BLE_GET_IP_RESP,	COLINK_BLE_EVENT_DEVICE_INFO_REQUEST,
				COLINK_BLE_DEVICE_INFO_RESPONSE,	colink_ble_device_info_response},
	{COLINK_BLE_DEVICE_INFO_RESPONSE, COLINK_BLE_EVENT_SERVER_INFO_REQUEST,
				COLINK_BLE_SERVER_INFO_RESPONSE,	colink_ble_server_info_response}
};

static usr_fsm_t s_colinkBleFsm = {
	.state = COLINK_BLE_READY,
	.transNum = sizeof(s_stateTran) / sizeof(usr_stat_transf),
	.fsmTbl = s_stateTran,
};

static usr_fsm_t* get_colink_ble_fsm()
{
	return &s_colinkBleFsm;
}



static void ln_ble_adapter_init(void);
static void ln_ble_start(void);
static void ln_ble_refresh_handle(uint8_t conidx, uint16_t hdl);
static int ln_ble_receive_colink_msg(const uint8_t*_src, int _len);
static void ln_ble_send_colink_msg(const uint8_t _cmd, const uint8_t _len, const uint8_t *_dta, const uint32_t _dtaLen);

static COLINK_BLE_DRIVER_ADAPTER s_colinkBleDrvAdapter = {
	.stat = WAIT_FOR_CMD,
	.pKeyMd5 = NULL,
	.pAppSrcCode = NULL,
	.pDeviceSrcCode = NULL,
	.pDeviceSecretCode = NULL,
	.pAuthSrcCode = NULL,
	
	.bleConidx = 0,
	.bleHandle = 0,
	
	.init = ln_ble_adapter_init,
	.start = ln_ble_start,
	.refresh = ln_ble_refresh_handle,
	.receive = ln_ble_receive_colink_msg,
	.send = ln_ble_send_colink_msg,
	
	.fsm = get_colink_ble_fsm
};

COLINK_BLE_DRIVER_ADAPTER* get_colink_ble_handle()
{
	return &s_colinkBleDrvAdapter;
}

static void ln_ble_start(void)
{
	//fix bug, so need to start wifi
	get_colink_wifi_handle()->init_sta();	
	
	//start ble usr app
	ble_creat_usr_app_task();
}

static void ln_ble_refresh_handle(uint8_t conidx, uint16_t hdl)
{
	get_colink_ble_handle()->bleConidx = conidx;
	get_colink_ble_handle()->bleHandle = hdl;
}

static unsigned char* ln_get_ble_send_buf()
{
	return get_colink_ble_handle()->sendBuf.cmd;
}

static size_t ln_get_ble_send_buf_len()
{
	short *len = (short *)get_colink_ble_handle()->sendBuf.len;
	return *len + 4;
}

static void ln_calc_colink_apikey_md5()
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	memset(p->pKeyMd5, 0, AES_CBC_KEY_MD5_LENGTH);
	p->pXcrypt->md5(get_colink_xcrypt_handle()->colinkApikey, p->pKeyMd5);
}

static void ln_calc_colink_device_secret_code()
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	p->pXcrypt->uuid((char *)p->pDeviceSrcCode);
	//strcpy(p->pDeviceSrcCode, "31b913db-5b9a-4168-b4a3-9ee7714ceef9");
	p->pXcrypt->encrypt(p->pDeviceSrcCode, p->pKeyMd5, p->pDeviceSecretCode);
}

static void ln_ble_send()
{
#if (_LN882H_SDK_V1_8_)
    COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();

    Log_d("ln_get_ble_send_buf_len == %d", ln_get_ble_send_buf_len());
    ln_trans_svr_send_t *svc_send_cmd = blib_malloc(sizeof(ln_trans_svr_send_t));
    svc_send_cmd->data = blib_malloc(ln_get_ble_send_buf_len());
    svc_send_cmd->conn_idx = 0;
    svc_send_cmd->len = ln_get_ble_send_buf_len();
    svc_send_cmd->hdl = ble_usr_svc_get_handle()->desc.start_handle + DATA_TRANS_DECL_CHAR_TX_VAL;
    memcpy(svc_send_cmd->data, ln_get_ble_send_buf(), ln_get_ble_send_buf_len());
    usr_queue_msg_send(BLE_MSG_GATT_NOTIFY, ln_get_ble_send_buf_len(), svc_send_cmd);
#else
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	Log_d("ln_get_ble_send_buf_len == %d", ln_get_ble_send_buf_len());
	struct ln_attc_write_req_ind *data = blib_malloc(sizeof(struct ln_attc_write_req_ind ) + ln_get_ble_send_buf_len());
		data->conidx = p->bleConidx;
		data->handle = p->bleHandle;
		data->length = ln_get_ble_send_buf_len();
		data->offset = 0;//param->offset;
		memcpy(&data->value[0],  ln_get_ble_send_buf(),  ln_get_ble_send_buf_len());
		usr_queue_msg_send(BLE_MSG_WRITE_DATA, ln_get_ble_send_buf_len(), data);
#endif

}

static int ln_ble_parse_colink_cmd(const uint8_t*_src, int _len)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	short *_cmd = (short *)p->recvBuf.cmd;
	Log_d("cmd == %d", *_cmd);
	short *pDtaLen = NULL;
	
	switch(*_cmd)
	{
		case COLINK_BLE_EVENT_REQ_AUTH:
		case COLINK_BLE_EVENT_AUTH_TOKEN:
		case COLINK_BLE_EVENT_REQ_CONNECT:
		case COLINK_BLE_EVENT_REQ_HIDDEN_AP_CONNECT:
		case COLINK_BLE_EVENT_SERVER_INFO_REQUEST:
			p->stat = PARSE_DATA;
			memcpy(p->recvBuf.dta + p->recvBuf.offset, _src, _len);
			p->recvBuf.offset += _len;
		
			pDtaLen = (short *)p->recvBuf.len;
			if (p->recvBuf.offset == *pDtaLen)
			{
				Log_i("ble receive all data, size == %d", *pDtaLen);
				return 1;
			}
			break;
		case COLINK_BLE_EVENT_REQ_SCAN:
			memcpy(p->recvBuf.dta, _src, _len);
			colink_start_fsm(get_colink_ble_fsm(), *_cmd, p->recvBuf.dta);
			break;
		case COLINK_BLE_EVENT_DEVICE_INFO_REQUEST:
			colink_start_fsm(get_colink_ble_fsm(), *_cmd, NULL);
			break;
		default:
			Log_i("Unknow cmd:%d", *_cmd);
			break;
	}
	
	return 0;
}

static int ln_ble_receive_colink_msg(const uint8_t*_src, int _len)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();

	switch(p->stat)
	{
		case WAIT_FOR_CMD:
			if (_len < 4) return 0;
		
			p->recvBuf.offset = 0;
			memcpy(p->recvBuf.cmd, _src, 2);
			memcpy(p->recvBuf.len, _src + 2, 2);
		
			ln_ble_parse_colink_cmd(_src + 4, _len - 4);
			break;
		
		case PARSE_DATA:
			if (ln_ble_parse_colink_cmd(_src, _len))
			{
				p->stat = WAIT_FOR_CMD;
				p->recvBuf.offset = 0;
				
				short *_cmd = (short *)p->recvBuf.cmd;
				colink_start_fsm(get_colink_ble_fsm(), *_cmd, NULL);
				return 1;
			}
			break;
	}
	
	return 0;
}

static void ln_ble_adapter_init()
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	//init xcrypt
	p->pXcrypt = get_colink_xcrypt_handle();
	
	//init buf
	LN_COLINK_MALLOC(p->pKeyMd5, AES_CBC_KEY_MD5_LENGTH, AES_CBC_ARRAY_DEFAULT_VALUE);
	LN_COLINK_MALLOC(p->pAppSrcCode, AES_CBC_APP_SRC_CODE_LENGTH, AES_CBC_ARRAY_DEFAULT_VALUE);
	LN_COLINK_MALLOC(p->pDeviceSrcCode, AES_CBC_DEVICE_SRC_CODE_LENGTH, AES_CBC_ARRAY_DEFAULT_VALUE);
	LN_COLINK_MALLOC(p->pDeviceSecretCode, AES_CBC_DEVICE_SECRET_CODE_LENGTH, AES_CBC_ARRAY_DEFAULT_VALUE);
	LN_COLINK_MALLOC(p->pAuthSrcCode, AES_CBC_AUTH_SRC_CODE_LENGTH, AES_CBC_ARRAY_DEFAULT_VALUE);
	
	memset(p->sendBuf.cmd, 0, sizeof(p->sendBuf));
	memset(p->recvBuf.cmd, 0, sizeof(p->recvBuf));
	
	//calc apikeymd5 & device
	ln_calc_colink_apikey_md5();
	ln_calc_colink_device_secret_code();
}

//
static void ln_ble_send_colink_msg(const uint8_t _cmd, const uint8_t _len, const uint8_t *_dta, const uint32_t _dtaLen)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	memset(p->sendBuf.cmd, 0, sizeof(p->sendBuf));
	p->sendBuf.cmd[0] = _cmd;
	
	p->sendBuf.len[0] = _len;
	if (_dta != NULL)
	{
		memcpy(p->sendBuf.dta, _dta, _dtaLen);
	}
	
	ln_ble_send();
}

static void colink_ble_auth_response(void *pDta)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	//decrypt app_src_code
	p->pXcrypt->decrypt(p->recvBuf.dta, p->pKeyMd5, p->pAppSrcCode);
	
	//send msg
	p->send(COLINK_BLE_AUTH_RESP, strlen((char *)p->pDeviceSecretCode), p->pDeviceSecretCode, strlen((char *)p->pDeviceSecretCode));
}

static void colink_ble_token_response(void *pDta)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	uint8_t authStat = 0;
	
	//decrypt auth_src_token
	p->pXcrypt->decrypt(p->recvBuf.dta, p->pKeyMd5, p->pAuthSrcCode);
	
	char *pch = strchr((char*)p->pAuthSrcCode, '_');
	if (pch == NULL)
	{
		Log_e("can not find \'_\' in auth src");
		goto FAIL_STAT;
	}
	
	if (memcmp(p->pAuthSrcCode, p->pAppSrcCode, strlen((char*)p->pAppSrcCode)))
	{
		Log_e("App src code compare fail!");
		goto FAIL_STAT;
	}
	Log_i("App src code compare suc!");
	
	if (memcmp(pch + 1, p->pDeviceSrcCode, strlen((char*)p->pDeviceSrcCode)))
	{
		Log_e("Device src code compare fail!");
		goto FAIL_STAT;
	}
	Log_i("Device src code compare suc!");
	
	//send msg
	p->send(COLINK_BLE_TOKEN_RESP, 0x01, &authStat, 1);
	return;
FAIL_STAT:
	authStat = 1;
	p->send(COLINK_BLE_TOKEN_RESP, 0x01, &authStat, 1);
	get_colink_mcu_handle()->reset_factory();
}

static void colink_ble_scan_start(void *pDta)
{
	//show_hidden  scan_type  scan_time
	get_colink_wifi_handle()->scan_cfg(pDta);
	get_colink_wifi_handle()->scan();
}

static void colink_ble_scan_response(void *pDta)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	uint8_t *pScanDta = pDta;
	
	//send scan msg
	p->send(COLINK_BLE_SCAN_RESP, *pScanDta + 10, pDta, *pScanDta + 10);
}

static void colink_ble_scan_response_end(void *pDta)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	get_colink_wifi_handle()->stop();
	//send msg
	p->send(COLINK_BLE_SCAN_RESP_END, 0x00, NULL, 0);
}

static void colink_ble_connect_response(void *pDta)
{
	char* pPwd = NULL;
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	COLINK_WIFI_DRIVER_ADAPTER* pWifi = get_colink_wifi_handle();

	//bssid
	memcpy(pWifi->bssid_buf(), p->recvBuf.dta, 6);
	
	//ssid
	if (!pWifi->scan_list_find_ssid(pWifi->ssid_buf(), pWifi->bssid_buf()))
	{
		Log_e("cannot find BSSID:[%02X:%02X:%02X:%02X:%02X:%02X] in ap list", \
                           pWifi->bssid_buf()[0], pWifi->bssid_buf()[1], pWifi->bssid_buf()[2], 
							pWifi->bssid_buf()[3], pWifi->bssid_buf()[4], pWifi->bssid_buf()[5]);
		//todo cannot find ssid
	}
					
	//password
	uint32_t pwdLen = *(p->recvBuf.dta + 8);
	Log_i("pwd len == %d", pwdLen);
	LN_COLINK_MALLOC(pPwd, pwdLen + 1, 0);
	memcpy(pPwd, p->recvBuf.dta + 9, pwdLen);
	p->pXcrypt->decrypt((uint8_t*)pPwd, p->pKeyMd5, (uint8_t*)pWifi->pwd_buf());
	OS_Free(pPwd);
	
	Log_i("ssid == %s  pwd == %s", pWifi->ssid_buf(), pWifi->pwd_buf());
	
	//release connect
	OS_SemaphoreRelease(&pWifi->ConnectApSemaphore);
	
	//p->send(COLINK_BLE_CONNECT_RESP, 0x01, NULL, 0);
}

static void colink_ble_connect_hidden_ap_response(void *pDta)
{
	char* pPwd = NULL;
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();

	//ssid
	uint32_t ssidLen = p->recvBuf.dta[0];
	memcpy(get_colink_wifi_handle()->ssid_buf(), p->recvBuf.dta + 1, ssidLen);
					
	//password
	uint32_t pwdLen = *(p->recvBuf.dta + 1 + ssidLen + 1 + 1);
	LN_COLINK_MALLOC(pPwd, pwdLen + 1, 0);
	memcpy(pPwd, p->recvBuf.dta + 1 + ssidLen + 3, p->recvBuf.dta[1 + ssidLen + 2]);
	p->pXcrypt->decrypt((uint8_t*)pPwd, p->pKeyMd5, (uint8_t*)get_colink_wifi_handle()->pwd_buf());
	OS_Free(pPwd);
	
	Log_i("ssidLen == %d  pwdLen == %d", ssidLen, pwdLen);
	
	//release connect
	OS_SemaphoreRelease(&get_colink_wifi_handle()->ConnectApSemaphore);
	
	//p->send(COLINK_BLE_CONNECT_RESP, 0x01, NULL, 0);
}

static void colink_ble_get_ip_response(void *pDta)
{
	uint32_t ofst = 0;
	uint32_t totalLen = 0;
	uint8_t* pDtaBuf = NULL;
	int8_t ssid[64 + 1] = {0};
	uint8_t bssid[8 + 1] = {0};
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	struct netif *nif = (struct netif *)pDta;
	
	//malloc
	totalLen = 1 + 1 + strlen(get_colink_wifi_handle()->ssid_buf()) + 6 + 4 + 4 + 4;
	LN_COLINK_MALLOC(pDtaBuf, totalLen, 0);
	
	//todo: dta[0] == status: 0,PASS 1,NG
	pDtaBuf[0] = 0;
	
	//ssid
	pDtaBuf[1] = strlen(get_colink_wifi_handle()->ssid_buf());
	memcpy(pDtaBuf + 2, get_colink_wifi_handle()->ssid_buf(), pDtaBuf[1]);
	ofst += 2 + pDtaBuf[1];
	
	//bssid
	wifi_get_sta_conn_info((const char**)&ssid, (const uint8_t**)&bssid);
	memcpy(pDtaBuf + ofst, bssid, 6);
	ofst += 6;
			
	//ip
	for (int i = 0; i < 4; i++)
	{
		pDtaBuf[ofst++] =   *((uint8_t *) &nif->ip_addr + i);
	}
			
	//mask
	for (int i = 0; i < 4; i++)
	{
		pDtaBuf[ofst++] =   *((uint8_t *) &nif->netmask + i);
	}
			
	//mask
	for (int i = 0; i < 4; i++)
	{
		pDtaBuf[ofst++] =   *((uint8_t *) &nif->gw + i);
	}
	
	get_colink_wifi_handle()->scan_list_release();
	p->send(COLINK_BLE_GET_IP_RESP, totalLen, pDtaBuf, totalLen);
	OS_Free(pDtaBuf);
}

static void colink_ble_device_info_response(void *pDta)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	memset(p->sendBuf.cmd, 0, sizeof(p->sendBuf));
	p->sendBuf.cmd[0] = COLINK_BLE_DEVICE_INFO_RESPONSE;
			
	//deviceid
	p->sendBuf.dta[0] = strlen((char *)get_colink_xcrypt_handle()->colinkId);
	memcpy(p->sendBuf.dta + 1, (char *)get_colink_xcrypt_handle()->colinkId, p->sendBuf.dta[0]);
	p->sendBuf.offset = p->sendBuf.dta[0] + 1;
		
	//apikey
	p->pXcrypt->encrypt(get_colink_xcrypt_handle()->colinkApikey, p->pKeyMd5, p->pDeviceSecretCode);
	p->sendBuf.dta[p->sendBuf.offset++] = strlen((char *)p->pDeviceSecretCode);
	memcpy(p->sendBuf.dta + p->sendBuf.offset, p->pDeviceSecretCode, strlen((char *)p->pDeviceSecretCode));
	p->sendBuf.offset += strlen((char *)p->pDeviceSecretCode);
		
	//chip id
	p->pXcrypt->encrypt(get_colink_xcrypt_handle()->colinkChipid, p->pKeyMd5, p->pDeviceSecretCode);
	p->sendBuf.dta[p->sendBuf.offset++] = strlen((char *)p->pDeviceSecretCode);
	memcpy(p->sendBuf.dta + p->sendBuf.offset, p->pDeviceSecretCode, strlen((char *)p->pDeviceSecretCode));
	p->sendBuf.offset += strlen((char *)p->pDeviceSecretCode);
		
	//len
	p->sendBuf.len[0] = p->sendBuf.offset;
	
	ln_ble_send();
}

static void colink_ble_server_info_response(void *pDta)
{
	COLINK_BLE_DRIVER_ADAPTER* p = get_colink_ble_handle();
	
	ColinkLinkInfo coLinkInfo;
	memset(&coLinkInfo, 0, sizeof(ColinkLinkInfo));
	
	char *pDomain = NULL;
	char *pPort = NULL;
	const char *strDomainPre = "https://";
	
	//domain
	pDomain = strstr((char *)p->recvBuf.dta + 2, strDomainPre);
	if (pDomain == NULL)
	{
		Log_e("can not find domain pre-index \'%s\'", strDomainPre);
		while(1);
	}
	pDomain += strlen(strDomainPre);
						
	//port
	pPort = strstr(pDomain, ":");
	if (pPort == NULL)
	{
		Log_e("can not find port pre-index \':\'");
		while(1);
	}
	*pPort = 0;
	pPort += 1;
						
	//save kv
	strcpy(coLinkInfo.ssid, get_colink_wifi_handle()->ssid_buf());
	strcpy(coLinkInfo.password, get_colink_wifi_handle()->pwd_buf());
	strcpy(coLinkInfo.distor_domain, pDomain);
	coLinkInfo.distor_port =  colinkAtoi(pPort);
						
	Log_d("ssid: %s", coLinkInfo.ssid);
	Log_d("pwd : %s", coLinkInfo.password);
	Log_d("port: %d", coLinkInfo.distor_port);
	Log_d("domain : %s", coLinkInfo.distor_domain);

	int kvret = ln_kv_set((const char *)DEVICE_CONFIG_PARA, (void *)(&coLinkInfo), sizeof(coLinkInfo));
	if (KV_ERR_NONE != kvret)
	{
		Log_e("error ln_kv_set DEVICE_CONFIG_PARA with return %d\r\n", kvret);
	}
			
	p->send(COLINK_BLE_SERVER_INFO_RESPONSE, 0x01, NULL, 0);
	OS_SemaphoreRelease(&get_colink_wifi_handle()->GetIpSemaphore);
}
