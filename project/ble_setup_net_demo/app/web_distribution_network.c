/**
 * @file     web_distribution_network.c
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2022-07-21
 * 
 * @copyright Copyright (c) 2022 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

#include "osal/osal.h"
#include "ln882h.h"
#include "usr_app.h"

#include "rwip_config.h"
#include "llm_int.h"

#include "lwip/dns.h"
#include "get_internet_info.h"
#include "lcd_show_info.h"

#include "lwip/sockets.h"

#include "web_distribution_network.h"
#include "ln_kv_api.h"
#include "hal/hal_misc.h"

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

#define DEVICE_NAME                  ("LN_BLE_NET_CONFIG")
#define DEVICE_NAME_LEN              (sizeof(DEVICE_NAME))
#define ADV_DATA_MAX_LENGTH          (28)

extern uint8_t svc_uuid[16];
extern uint8_t con_num;
static OS_Thread_t ble_g_usr_app_thread;
#define BLE_USR_APP_TASK_STACK_SIZE  (1024)

uint8_t adv_actv_idx  =0;
uint8_t init_actv_idx =0;
uint8_t net_config_flag = 0;

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

uint8_t ble_connect_conidx = 0;
uint8_t ble_rx_handler = 0;
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
                        ln_kv_del("wifi_ssid");
                        ln_kv_set("wifi_ssid",p_param->value + strlen("ssid="),p_param->length - strlen("ssid="));
                        if(net_config_flag == 0)net_config_flag = 1;
                        
                        ble_rx_handler = p_param->handle;
                    }
                    ret = memcmp(p_param->value,"pwd=",strlen("pwd="));
                    LOG(LOG_LVL_INFO, "ret:%d\n", ret);
                    if(ret == 0){
                        ln_kv_del("wifi_pwd");
                        ln_kv_set("wifi_pwd",p_param->value + strlen("pwd="),p_param->length - strlen("pwd="));
                        if(net_config_flag == 1)net_config_flag = 2;
                    } 
                    
                    ble_connect_conidx = p_param->conidx;
                    
                    //usr_send_ntf_data("ok",2);
                }
                break;

                case BLE_MSG_CONN_IND:
                {
                    struct ln_gapc_connection_req_info *p_param=(struct ln_gapc_connection_req_info *)usr_msg.msg;
#if (CLIENT)
                    struct ln_gattc_disc_cmd param_ds;
                    param_ds.operation = GATTC_DISC_BY_UUID_SVC;
                    param_ds.start_hdl = 1;
                    param_ds.end_hdl   = 0xFFFF;
                    param_ds.uuid_len  =sizeof(svc_uuid);
                    param_ds.uuid = svc_uuid;
                    ln_app_gatt_discovery(p_param->conidx, &param_ds);
#endif
                    //ln_app_gatt_exc_mtu(p_param->conidx);
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
                    
                    
                }
                break;

                case BLE_MSG_SVR_DIS: 
                {
#if (CLIENT)
                    struct ln_gattc_disc_svc *p_param = (struct ln_gattc_disc_svc *)usr_msg.msg;
                    uint8_t data[] = {0x12,0x78,0x85};
                    struct ln_gattc_write_cmd param_wr;
                    param_wr.operation    = GATTC_WRITE;
                    param_wr.auto_execute = true;
                    param_wr.handle       = p_param->start_hdl + 2;
                    param_wr.length       = sizeof(data);
                    param_wr.offset = 0;
                    param_wr.value = data;
                    ln_app_gatt_write(p_param->conidx,&param_wr);
#endif
                }
                break;

                default:
                    break;
			}
            blib_free(usr_msg.msg);
		}
	}
}