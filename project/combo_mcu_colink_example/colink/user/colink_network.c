//#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "colink_profile.h"
#include "colink_define.h"
#include "colink_network.h"
#include "colink_setting.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "json/cJSON.h"
#include "colink_profile.h"
#include "colink_global.h"
#include "colink_upgrade.h"
#include "colink_link.h"
#include "colink_sysadapter.h"
#include "colink_wifi_driver_port.h"
#include "ln_kv_api.h"

#include "usr_app.h"
#include "usr_ctrl.h"


static void colinkSendUpdateCB(int32_t error_code)
{
    os_printf("colinkSendUpdateCB [%d]\r\n", error_code);

    return;
}

static void colinkRecvUpdate(char* data)
{
    os_printf("colinkRecvUpdate [%s]", data);

    cJSON *json_root = NULL;
    cJSON *switch_p = NULL;

    json_root = cJSON_Parse(data);

    if (!json_root)
    {
        Log_e("parse json failed");
        return;
    }

    switch_p = cJSON_GetObjectItem(json_root, "switch");
    if (switch_p)
    {
        if(0 == colinkStrcmp(switch_p->valuestring, "on"))
        {
            Log_i("sever on instruction");
            get_colink_mcu_handle()->switch_on();
        }
        else if(0 == colinkStrcmp(switch_p->valuestring, "off"))
        {
            Log_i("sever off instruction");
            get_colink_mcu_handle()->switch_off();
        }
        else
        {
            Log_e("err net switch info...");
			goto ExitErr1;
        }
    }
    else
    {
        Log_e("can not find switch, parse json failed");
        goto ExitErr1;
    }

ExitErr1:
    cJSON_Delete(json_root);
    return;
}

static void colinkNotifyDevStatus(int32_t status)
{
    os_printf("colinkNotifyDevStatus %d\r\n", status);

    if (status == 3)
    {
        get_colink_mcu_handle()->reset_factory();
    }
    else if(status == 1)
    {
        cJSON* params = NULL;
        cJSON* item = NULL;
        char* raw = NULL;

        params = cJSON_CreateObject();
        item = cJSON_AddStringToObject(params, "fwVersion", VERSION);
        raw = cJSON_PrintUnformatted(params);
				get_colink_mcu_handle()->cloud_indicate_led_on();
        if (!raw)
        {
            os_printf("cJSON_PrintUnformatted failed");
            cJSON_Delete(params);
            return;
        }

        os_printf(raw);

        colinkSendUpdate(raw);
        cJSON_Delete(params);
        cJSON_free(raw);
		
		colinkSendUTCRequest();
    }
		else if(status == DEVICE_OFFLINE)
		{
			get_colink_mcu_handle()->cloud_indicate_led_off();
		}
}

static void colinkSendUTCRequestCB(int32_t error_code, char utc_str[])
{
	if (error_code != COLINK_REQ_RESULT_NO_ERROR)
	{
		Log_e("error code: %d", error_code);
		return;
	}
	
	Log_i("UTC: %s", utc_str);
	get_colink_mcu_handle()->updat_timep(utc_str);
}

static void colinkProcessTask(void* pData)
{
    int ret = 0;
    size_t r_len = 0;
    ColinkDev *dev_data = NULL;
    ColinkEvent ev;
    ColinkLinkInfo colinkInfoCopy;
	
//	uint32_t colink_status=0;

    os_printf("colinkProcessTask\r\n");

    coLinkSetDeviceMode(DEVICE_MODE_WORK_NORMAL);

    dev_data = (ColinkDev *)OS_Malloc(sizeof(ColinkDev));

    if(dev_data == NULL)
    {
        os_printf("os_malloc err\r\n");
		OS_Free(dev_data);
        vTaskDelete(NULL);
        return ;
    }

    ev.colinkRecvUpdateCb = colinkRecvUpdate;
    ev.colinkSendUpdateCb = colinkSendUpdateCB;
    ev.colinkNotifyDevStatusCb = colinkNotifyDevStatus;
    ev.colinkUpgradeRequestCb = colinkUpgradeRequest;/**< 升级通知的回调 */
	ev.colinkSendUTCRequestCb = colinkSendUTCRequestCB;
	
#if 1
    if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_CONFIG_PARA, (void *)(&colinkInfoCopy), sizeof(colinkInfoCopy), &r_len)) {
        os_printf("ln_kv_get DEVICE_CONFIG_PARA error\r\n");
        OS_Free(dev_data);
        vTaskDelete(NULL);
        return ;
    }
#else
	strcpy(colinkInfoCopy.distor_domain, "cn-dispd.coolkit.cn");
	colinkInfoCopy.distor_port = 443;
#endif
    strcpy(dev_data->deviceid, (char *)get_colink_xcrypt_handle()->colinkId);
    strcpy(dev_data->apikey, (char *)get_colink_xcrypt_handle()->colinkApikey);
    strcpy(dev_data->model, MODEL);
    //chipid此处随便填充一个就行了
    strcpy(dev_data->chipid, (char *)get_colink_xcrypt_handle()->colinkChipid);
    strcpy(dev_data->version, VERSION);
    dev_data->dev_type = COLINK_SINGLE;
    dev_data->temporary_server_enable = false;

    dev_data->ssl_enable = true;
    dev_data->distor_port = colinkInfoCopy.distor_port;
    dev_data->server_res_timeout = 8000;

    // 若不加ssl并且在测试环境下，请使用testapiproxy.coolkit.cc这个域名
    // strcpy(dev_data->distor_domain, "testapiproxy.coolkit.cc");
    strcpy(dev_data->distor_domain, colinkInfoCopy.distor_domain);

    os_printf("domain: %s\r\n", dev_data->distor_domain);
    os_printf("port: %d\r\n", dev_data->distor_port);

    colinkInit(dev_data, &ev);

    OS_Free(dev_data);

    while(1)
    {	
        if(ret = colinkProcess())
        {
            os_printf("colinkProcess ret=%d\r\n", ret);
        }
				
        vTaskDelay(50 / portTICK_RATE_MS);
    }

    vTaskDelete(NULL);
}

void colinkProcessStart(void)
{
    xTaskCreate(colinkProcessTask, "colinkProcessTask", 1024*7, NULL, 4, NULL);
}

