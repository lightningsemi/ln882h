#include "osal/osal.h"
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "utils/system_parameter.h"
#include "utils/ln_psk_calc.h"
#include "utils/power_mgmt/ln_pm.h"
#include "utils/sysparam_factory_setting.h"
#include "wifi.h"
#include "wifi_port.h"
#include "netif/ethernetif.h"
#include "wifi_manager.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "drv_adc_measure.h"
#include "hal/hal_adc.h"
#include "ln_nvds.h"
#include "ln_wifi_err.h"
#include "ln_misc.h"
#include "ln882h.h"
#include "usr_app.h"

#include "lwip/dns.h"
#include "get_internet_info.h"

#include "hal/hal_gpio.h"
#include "hal/hal_misc.h"
#include "ln_kv_api.h"

#include "rwip_config.h"
#include "llm_int.h"
#include "ln_app_gatt.h"
#include "ln_app_gap.h"
#include "gapm_task.h"
#include "ln_app_gap.h"
#include "ln_app_gatt.h"
#include "ln_app_callback.h"
#include "usr_ble_app.h"
#include "usr_send_data.h"

#define DEVICE_NAME                     ("LN_BLE_NET_CONFIG")
#define DEVICE_NAME_LEN                 (sizeof(DEVICE_NAME))
#define ADV_DATA_MAX_LENGTH             (28)

#define WEB_DISTRIBUTION_NET_KEY_PORT   GPIOB_BASE
#define WEB_DISTRIBUTION_NET_KEY_PIN    GPIO_PIN_3

#define WEB_DISTRIBUTION_NET_LED_PORT   GPIOB_BASE
#define WEB_DISTRIBUTION_NET_LED_PIN    GPIO_PIN_4


#define WIFI_TEMP_CALIBRATE             1

typedef enum 
{
    NET_CONFIG_INIT          = 0,
    NET_CONFIG_GET_SSID      = 1,
    NET_CONFIG_GET_PWD       = 2,

}net_config_state_t;

net_config_state_t net_config_state = NET_CONFIG_INIT;


typedef enum
{
    NET_CONFIG_MODE = 1,
    NORMAL_MODE     = 2,
    ERROR_MODE      = 3,
}app_mode_t;

app_mode_t app_mode = NORMAL_MODE;

uint8_t wifi_sta_connect_flag = 0;

uint8_t adv_actv_idx        = 0;
uint8_t init_actv_idx       = 0;


uint8_t ble_connect_conidx  = 0;
uint8_t ble_rx_handler      = 0;

extern uint8_t svc_uuid[16];
extern uint8_t con_num;

static OS_Thread_t g_usr_app_thread;
#define USR_APP_TASK_STACK_SIZE     6*256 //Byte

#if WIFI_TEMP_CALIBRATE
static OS_Thread_t g_temp_cal_thread;
#define TEMP_APP_TASK_STACK_SIZE    4*256 //Byte
#endif

OS_Thread_t g_get_info_thread; 
#define GET_INFO_TASK_STACK_SIZE    4*256*2 //Byte

OS_Thread_t g_led_thread; 
#define LED_TASK_STACK_SIZE         4*256 //Byte


OS_Thread_t ble_g_usr_app_thread;
#define BLE_USR_APP_TASK_STACK_SIZE  (1024)

/* declaration */
static void wifi_init_ap(void);
static void wifi_init_sta(void);
static void usr_app_task_entry(void *params);
static void ble_app_task_entry(void *params);
static void app_stop_adv(void);
static void temp_cal_app_task_entry(void *params);
static void led_app_task_entry(void *params);
static void sht30_app_task_entry(void *params);

static uint8_t  mac_addr[6]        = {0x08, 0x50, 0xC2, 0x5E, 0xAA, 0xDA};
static uint8_t  psk_value[40]      = {0x0};
char     wifi_ssid[50] = "";
char     wifi_password[50] = "";

wifi_sta_connect_t connect = {
    .ssid    = wifi_ssid,
    .pwd     = wifi_password,
    .bssid   = NULL,
    .psk_value = NULL,
};

wifi_scan_cfg_t scan_cfg = {
    .channel   = 0,
    .scan_type = WIFI_SCAN_TYPE_ACTIVE,
    .scan_time = 20,
};

static void wifi_scan_complete_cb(void * arg)
{
    LN_UNUSED(arg);

    ln_list_t *list;
    uint8_t node_count = 0;
    ap_info_node_t *pnode;

    wifi_manager_ap_list_update_enable(LN_FALSE);

    // 1.get ap info list.
    wifi_manager_get_ap_list(&list, &node_count);

    // 2.print all ap info in the list.
    LN_LIST_FOR_EACH_ENTRY(pnode, ap_info_node_t, list,list)
    {
        uint8_t * mac = (uint8_t*)pnode->info.bssid;
        ap_info_t *ap_info = &pnode->info;

        LOG(LOG_LVL_INFO, "\tCH=%2d,RSSI= %3d,", ap_info->channel, ap_info->rssi);
        LOG(LOG_LVL_INFO, "BSSID:[%02X:%02X:%02X:%02X:%02X:%02X],SSID:\"%s\"\r\n", \
                           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], ap_info->ssid);
    }

    wifi_manager_ap_list_update_enable(LN_TRUE);
}

void wifi_connected_callback(struct netif *nif) 
{
    change_wifi_status(1);
    wifi_sta_connect_flag = 1;

    if(app_mode == NET_CONFIG_MODE)
        usr_send_ntf_data("BLE CONFIG NET OK",strlen("BLE CONFIG NET OK"));
}

static void wifi_init_sta(void)
{
    // ln_generate_random_mac(mac_addr);
    //1. sta mac get
     if (SYSPARAM_ERR_NONE != sysparam_sta_mac_get(mac_addr)) {
        LOG(LOG_LVL_ERROR, "[%s]sta mac get filed!!!\r\n", __func__);
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

    //1. net device(lwip)
    netdev_set_mac_addr(NETIF_IDX_STA, mac_addr);
    netdev_set_active(NETIF_IDX_STA);
    sysparam_sta_mac_update((const uint8_t *)mac_addr);

    //2. wifi start
    wifi_manager_reg_event_callback(WIFI_MGR_EVENT_STA_SCAN_COMPLETE, &wifi_scan_complete_cb);

    if(WIFI_ERR_NONE != wifi_sta_start(mac_addr, WIFI_NO_POWERSAVE)){
        LOG(LOG_LVL_ERROR, "[%s]wifi sta start filed!!!\r\n", __func__);
    }

    connect.psk_value = NULL;
    if (strlen(connect.pwd) != 0) {
        if (0 == ln_psk_calc(connect.ssid, connect.pwd, psk_value, sizeof (psk_value))) {
            connect.psk_value = psk_value;
            hexdump(LOG_LVL_INFO, "psk value ", psk_value, sizeof(psk_value));
        }
    }
    netdev_get_ip_cb_set(wifi_connected_callback);
    wifi_sta_connect(&connect, &scan_cfg);
}

static void usr_app_task_entry(void *params)
{
    LN_UNUSED(params);
    uint32_t timeout_cnt  = 0;
    uint8_t  timeout_flag = 0;
    uint8_t buf[10];
    uint32_t buf_len = 0;
    memset(buf,0,sizeof(buf));
    
    //根据KV的值判断是否需要配网
    if(ln_kv_get("net_config_flag",buf,50,&buf_len) == KV_ERR_NONE){
        //需要配网则开启蓝牙，等待小程序发送WIFI名称和密码
        if(strcmp((const char*)buf,"true") == 0){
            if(OS_OK != OS_ThreadCreate(&ble_g_usr_app_thread, "BleUsrAPP", ble_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, BLE_USR_APP_TASK_STACK_SIZE)) 
            {
                LN_ASSERT(1);
            }
            ln_kv_set("net_config_flag","false",sizeof("false"));
            
            while(net_config_state != NET_CONFIG_GET_PWD)OS_MsDelay(100);
            app_mode = NET_CONFIG_MODE;
        }
    }else{  
        app_mode = NORMAL_MODE;
    }
   
    //读取KV是否保存了WIFI名称和密码
    //lvgl_set_next_state_machine(wifi_sta_init);
    if(ln_kv_has_key("wifi_ssid") == LN_FALSE){
        LOG(LOG_LVL_INFO, "There is no ssid in the flash.\r\n");
        app_mode = ERROR_MODE;
    }else{
        uint32_t len = 0;
        memset(wifi_ssid,0,sizeof(wifi_ssid));
        ln_kv_get("wifi_ssid",wifi_ssid,sizeof(wifi_ssid),&len);
        connect.ssid = wifi_ssid;
        
        if(ln_kv_has_key("wifi_pwd") == LN_TRUE){
            memset(wifi_password,0,sizeof(wifi_password));
            ln_kv_get("wifi_pwd",wifi_password,sizeof(wifi_password),&len);
            connect.pwd = wifi_password;
        }
    }

    OS_MsDelay(10);
    hal_gpio_pin_pull_set(WEB_DISTRIBUTION_NET_KEY_PORT,WEB_DISTRIBUTION_NET_KEY_PIN,GPIO_PULL_UP);
    
    //根据app_mode判断是否需要打开WIFI
    if(app_mode == NET_CONFIG_MODE || app_mode == NORMAL_MODE){
        LOG(LOG_LVL_INFO, "ssid:%s\r\n",wifi_ssid);
        LOG(LOG_LVL_INFO, "password:%s\r\n",wifi_password);
        ln_pm_sleep_mode_set(ACTIVE);
        wifi_manager_init();
        wifi_init_sta();
    }
    
    while(1)
    {
        //判断配网按键是否按下
        if(hal_gpio_pin_read(WEB_DISTRIBUTION_NET_KEY_PORT,WEB_DISTRIBUTION_NET_KEY_PIN) == HAL_RESET){
            uint8_t press_cnt = 0;
            while(hal_gpio_pin_read(WEB_DISTRIBUTION_NET_KEY_PORT,WEB_DISTRIBUTION_NET_KEY_PIN) == HAL_RESET){
                press_cnt++;
                OS_MsDelay(10);
            }
            if(press_cnt > 5){
                ln_kv_set("net_config_flag","true",sizeof("true"));
                OS_MsDelay(10);
                hal_misc_reset_all();
            }           
            
        }

        //判断WIFI连接是否超时
        if(wifi_sta_connect_flag == 0){
            timeout_cnt++;
            if(timeout_cnt > 200 && timeout_flag == 0){
                timeout_flag = 1;
            }
        }else if(wifi_sta_connect_flag == 1){
            wifi_sta_connect_flag = 2;
        }

        //根据app_mode分别执行不同的代码
        switch(app_mode)
        {
            case NORMAL_MODE:
            {
                break;
            }
            
            case NET_CONFIG_MODE:
            {
                //连接上WIFI后，延时一段时间关闭BLE广播,并且切换app_mode为NORMAL_MODE
                if(wifi_sta_connect_flag == 2){
                    static uint32_t cnt = 0;
                    cnt ++;
                    if(cnt > 10){
                        app_mode = NORMAL_MODE;
                        app_stop_adv();
                        hal_misc_reset_ble();
                    }
                }
                break;
            }

            case ERROR_MODE:
            {
							//to do
                //lvgl_set_next_state_machine(wifi_sta_init_failed);
                break;
            }
        }
        OS_MsDelay(100);
    }
}

static void temp_cal_app_task_entry(void *params)
{
    LN_UNUSED(params);

    int8_t cap_comp = 0;
    uint16_t adc_val = 0;
    int16_t curr_adc = 0;
    uint8_t cnt = 0;

    if (NVDS_ERR_OK == ln_nvds_get_xtal_comp_val((uint8_t *)&cap_comp)) {
        if ((uint8_t)cap_comp == 0xFF) {
            cap_comp = 0;
        }
    }
    drv_adc_init();
    wifi_temp_cal_init(drv_adc_read(ADC_CH0), cap_comp);
    while (1)
    {
        OS_MsDelay(1000);
        adc_val = drv_adc_read(ADC_CH0);
        wifi_do_temp_cal_period(adc_val);
        curr_adc = (adc_val & 0xFFF);
        if ((cnt % 300) == 0) {
            LOG(LOG_LVL_INFO, "adc raw: %4d, temp_IC: %4d\r\n",
                    curr_adc, (int16_t)(25 + (curr_adc - 770) / 2.54f));
            LOG(LOG_LVL_INFO, "Total:%d; Free:%ld;\r\n", 
                    OS_HeapSizeGet(), OS_GetFreeHeapSize());
        }
        cnt++;
    }
}


static void app_create_advertising(void)
{
#define APP_ADV_CHMAP                (0x07)  // Advertising channel map - 37, 38, 39
#define APP_ADV_INT_MIN              (160)   // Advertising minimum interval - 40ms (64*0.625ms)
#define APP_ADV_INT_MAX              (160)   // Advertising maximum interval - 40ms (64*0.625ms)

	struct ln_gapm_activity_create_adv_cmd  adv_creat_param = {0};

	adv_creat_param.own_addr_type                     = GAPM_STATIC_ADDR;
	adv_creat_param.adv_param.type                    = GAPM_ADV_TYPE_LEGACY;//GAPM_ADV_TYPE_EXTENDED;//GAPM_ADV_TYPE_LEGACY;
	adv_creat_param.adv_param.filter_pol              = ADV_ALLOW_SCAN_ANY_CON_ANY;
	adv_creat_param.adv_param.prim_cfg.chnl_map       = APP_ADV_CHMAP;
	adv_creat_param.adv_param.prim_cfg.phy            = GAP_PHY_1MBPS;
	adv_creat_param.adv_param.prop                    = GAPM_ADV_PROP_UNDIR_CONN_MASK;//GAPM_ADV_PROP_NON_CONN_SCAN_MASK;//GAPM_ADV_PROP_UNDIR_CONN_MASK;//GAPM_ADV_PROP_UNDIR_CONN_MASK;//GAPM_EXT_ADV_PROP_UNDIR_CONN_MASK;//GAPM_ADV_PROP_UNDIR_CONN_MASK;
	adv_creat_param.adv_param.disc_mode               = GAPM_ADV_MODE_GEN_DISC;
	adv_creat_param.adv_param.prim_cfg.adv_intv_min   = APP_ADV_INT_MIN;
	adv_creat_param.adv_param.prim_cfg.adv_intv_max   = APP_ADV_INT_MAX;
	adv_creat_param.adv_param.max_tx_pwr              = 0;
	//adv_creat_param.adv_param.second_cfg.phy        = GAP_PHY_1MBPS;//GAP_PHY_1MBPS;//GAP_PHY_CODED;
	adv_creat_param.adv_param.second_cfg.max_skip     = 0x00;
	adv_creat_param.adv_param.second_cfg.phy          = 0x01;
	adv_creat_param.adv_param.second_cfg.adv_sid      = 0x00;
	adv_creat_param.adv_param.period_cfg.adv_intv_min = 0x0400;
	adv_creat_param.adv_param.period_cfg.adv_intv_max = 0x0400;
	ln_app_advertise_creat(&adv_creat_param);
}

static void app_set_adv_data(void)
{
    //adv data: adv length--adv type--adv string ASCII
    uint8_t adv_data[ADV_DATA_MAX_LENGTH] = {0};
    adv_data[0] = DEVICE_NAME_LEN + 1;
    adv_data[1] = 0x09;  //adv type :local name
    memcpy(&adv_data[2],DEVICE_NAME,DEVICE_NAME_LEN);
    struct ln_gapm_set_adv_data_cmd adv_data_param;
    adv_data_param.actv_idx = adv_actv_idx;
    adv_data_param.length = sizeof(adv_data);
    adv_data_param.data = adv_data;
    ln_app_set_adv_data(&adv_data_param);
}

static void app_start_advertising(void)
{
	struct ln_gapm_activity_start_cmd  adv_start_param;
	adv_start_param.actv_idx = adv_actv_idx;
	adv_start_param.u_param.adv_add_param.duration = 0;
	adv_start_param.u_param.adv_add_param.max_adv_evt = 0;
	ln_app_advertise_start(&adv_start_param);
}

void app_restart_adv(void)
{
	app_start_advertising();
}

void app_create_init(void)
{
	struct ln_gapm_activity_create_cmd init_creat_param;
	init_creat_param.own_addr_type = GAPM_STATIC_ADDR;
	ln_app_init_creat(&init_creat_param);
}

static void app_start_init(void)
{
	uint8_t peer_addr[6]= {0x12,0x34,0x56,0x78,0x90,0xab};
	struct ln_gapm_activity_start_cmd  init_start_param = {0};
    
	init_start_param.actv_idx                                        = init_actv_idx;
	init_start_param.u_param.init_param.type                         = GAPM_INIT_TYPE_DIRECT_CONN_EST;//GAPM_INIT_TYPE_DIRECT_CONN_EST;
	init_start_param.u_param.init_param.prop                         = GAPM_INIT_PROP_1M_BIT;//GAPM_INIT_PROP_CODED_BIT;
	init_start_param.u_param.init_param.conn_to                      = 0;
	init_start_param.u_param.init_param.scan_param_1m.scan_intv      = 16; //16*0.625 ms=10ms
	init_start_param.u_param.init_param.scan_param_1m.scan_wd        = 16;// 16*0.625ms=10ms
	init_start_param.u_param.init_param.conn_param_1m.conn_intv_min  = 80; // 10x1.25ms  (7.5ms--4s)
	init_start_param.u_param.init_param.conn_param_1m.conn_intv_max  = 80; // 10x1.25ms  (7.5ms--4s)
	init_start_param.u_param.init_param.conn_param_1m.conn_latency   = 0;
	init_start_param.u_param.init_param.conn_param_1m.supervision_to = 500; //100x10ms= 1 s
	init_start_param.u_param.init_param.conn_param_1m.ce_len_min     = 0;
	init_start_param.u_param.init_param.conn_param_1m.ce_len_max     = 0;
	init_start_param.u_param.init_param.peer_addr.addr_type          = 0;
	memcpy(init_start_param.u_param.init_param.peer_addr.addr.addr, peer_addr, GAP_BD_ADDR_LEN);
	ln_app_init_start(&init_start_param);
}

void app_restart_init(void)
{
	app_start_init();
}

static void start_adv(void)
{
	app_create_advertising();
	app_set_adv_data();
	app_start_advertising();
}

void app_stop_adv(void)
{
    if (ke_state_get(TASK_APP) == APP_ADVERTISING ){
        ln_app_activity_stop(adv_actv_idx); 
        ke_state_set(TASK_APP, APP_READY);
    } 
}

static void start_init(void)
{
	app_create_init();
	app_start_init();
}

static OS_Queue_t ble_usr_queue;

void usr_creat_queue(void)
{
    if(OS_OK != OS_QueueCreate(&ble_usr_queue, BLE_USR_MSG_QUEUE_SIZE, sizeof(ble_usr_msg_t)))
    {
        BLIB_LOG(BLIB_LOG_LVL_E, "usr QueueCreate rw_queue failed!!!\r\n");
    }
}

void usr_queue_msg_send(uint16_t id, uint16_t length, void *msg)
{
    ble_usr_msg_t usr_msg;
    usr_msg.id = id;
    usr_msg.len = length;
    usr_msg.msg = msg;
    OS_QueueSend(&ble_usr_queue, &usr_msg, OS_WAIT_FOREVER);
}

int usr_queue_msg_recv(void *msg, uint32_t timeout)
{
    return OS_QueueReceive(&ble_usr_queue, msg, timeout);
}

void usr_send_ntf_data(char *data,uint16_t length)
{
    struct ln_attc_write_req_ind p_param;
    struct ln_gattc_send_evt_cmd send_data;
    
    memset(&p_param,0,sizeof(p_param));
    memset(&send_data,0,sizeof(send_data));
    send_data.handle = ble_rx_handler + 2;
    send_data.length = length;
    send_data.value = (uint8_t*)data;
    ln_app_gatt_send_ntf(ble_connect_conidx,&send_data);
}

static void ble_app_task_entry(void *params)
{
    ble_usr_msg_t usr_msg;
    usr_creat_queue();
    extern void ble_app_init(void);
    ble_app_init();
#if (SLAVE)
	start_adv();
#endif
#if (MASTER)
	start_init();
#endif
#if SERVICE
	data_trans_svc_add();
#endif
	while(1)
	{
        if(OS_OK == usr_queue_msg_recv((void *)&usr_msg, OS_WAIT_FOREVER))
		{
            LOG(LOG_LVL_TRACE, "connect device number :%d \r\n",con_num);
			switch(usr_msg.id)
			{
                case BLE_MSG_WRITE_DATA:
                {
                    struct ln_attc_write_req_ind *p_param = (struct ln_attc_write_req_ind *)usr_msg.msg;  
                    ble_connect_conidx = p_param->conidx;                    
                    hexdump(LOG_LVL_INFO, "[recv data]", (void *)p_param->value, p_param->length);
                    
                    int ret = 0;
                    ret = memcmp(p_param->value,"ssid=",strlen("ssid="));
                    LOG(LOG_LVL_INFO, "ret:%d\n", ret);
                    if(ret == 0){
                        ln_kv_set("wifi_ssid",p_param->value + strlen("ssid="),p_param->length - strlen("ssid="));
                        if(net_config_state == NET_CONFIG_INIT)net_config_state = NET_CONFIG_GET_SSID;
                        
                        ble_rx_handler = p_param->handle;
                    }
                    ret = memcmp(p_param->value,"pwd=",strlen("pwd="));
                    LOG(LOG_LVL_INFO, "ret:%d\n", ret);
                    if(ret == 0){
                        ln_kv_set("wifi_pwd",p_param->value + strlen("pwd="),p_param->length - strlen("pwd="));
                        if(net_config_state == NET_CONFIG_GET_SSID)net_config_state = NET_CONFIG_GET_PWD;
                    } 
                    ble_connect_conidx = p_param->conidx;
                    break;
                }
                case BLE_MSG_CONN_IND:
                {
                    struct ln_gapc_connection_req_info *p_param=(struct ln_gapc_connection_req_info *)usr_msg.msg;
                    ble_connect_conidx = p_param->conidx;
                    struct ln_gapc_set_le_pkt_size_cmd pkt_size;
                    pkt_size.tx_octets = 251;
                    pkt_size.tx_time   = 2120;
                    OS_MsDelay(1000);
                    ln_app_param_set_pkt_size(p_param->conidx,  &pkt_size);

                    struct ln_gapc_conn_param conn_param;
                    conn_param.intv_min = 80;  // 10x1.25ms  (7.5ms--4s)
                    conn_param.intv_max = 90;  // 10x1.25ms  (7.5ms--4s)
                    conn_param.latency  = 10;
                    conn_param.time_out = 3000;  //ms*n
                    ln_app_update_param(p_param->conidx, &conn_param);
                    break;
                }
                
                default:
                    break;
			}
            blib_free(usr_msg.msg);
		}
	}
}

void creat_usr_app_task(void)
{

    if(OS_OK != OS_ThreadCreate(&g_usr_app_thread, "WifiUsrAPP", usr_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, USR_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
       
    if(OS_OK != OS_ThreadCreate(&g_get_info_thread, "GetInternetInfo", get_info_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, GET_INFO_TASK_STACK_SIZE)) 
    {
        LN_ASSERT(1);
    }
    
#if  WIFI_TEMP_CALIBRATE
    if(OS_OK != OS_ThreadCreate(&g_temp_cal_thread, "TempAPP", temp_cal_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, TEMP_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
#endif

    /* print sdk version */
    {
        LOG(LOG_LVL_INFO, "LN882H SDK Ver: %s [build time:%s][0x%08x]\r\n",
                LN882H_SDK_VERSION_STRING, LN882H_SDK_BUILD_DATE_TIME, LN882H_SDK_VERSION);
    }
}
