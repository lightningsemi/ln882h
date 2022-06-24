#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "rwip_config.h"
#include "rwprf_config.h"
#include "llm_int.h"
#include "ln_app_gatt.h"
#include "ln_app_gap.h"
#include "usr_app.h"
#include "gapm_task.h"
#include "ln_app_gap.h"
#include "ln_app_gatt.h"
#include "ln_app_bas.h"
#include "ln_app_callback.h"
#include "osal/osal.h"

#include "usr_ble_app.h"


#define BLE_USR_APP_TASK_STACK_SIZE  (1024)
#define DEVICE_NAME                  ("LN_battery")
#define DEVICE_NAME_LEN              (sizeof(DEVICE_NAME))
#define ADV_DATA_MAX_LENGTH          (28)

extern uint8_t svc_uuid[16];
extern uint8_t con_num;
static OS_Thread_t ble_g_usr_app_thread;
OS_Timer_t bas_timer;
uint8_t adv_actv_idx  = 0;
uint8_t init_actv_idx = 0;


void app_create_advertising(void)
{
    #define APP_ADV_CHMAP                (0x07)  // Advertising channel map - 37, 38, 39
    #define APP_ADV_INT_MIN              (256)   // Advertising minimum interval - 120ms (256*0.625ms)
    #define APP_ADV_INT_MAX              (256)   // Advertising maximum interval - 120ms (256*0.625ms)

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

void app_set_adv_data(void)
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

void app_start_advertising(void)
{
    struct ln_gapm_activity_start_cmd  adv_start_param;
    adv_start_param.actv_idx = adv_actv_idx;
    adv_start_param.u_param.adv_add_param.duration = 0;
    adv_start_param.u_param.adv_add_param.max_adv_evt = 0;
    ln_app_advertise_start(&adv_start_param);
}

void app_create_init(void)
{
	struct ln_gapm_activity_create_cmd init_creat_param;
	init_creat_param.own_addr_type = GAPM_STATIC_ADDR;
	ln_app_init_creat(&init_creat_param);
}

void app_start_init(void)
{
	uint8_t peer_addr[6]= {0x00,0x50,0xC2,0x64,0x61,0xEA};
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



void app_restart_adv(void)
{
    app_start_advertising();
}

void start_adv(void)
{
    app_create_advertising();
    app_set_adv_data();
    app_start_advertising();
}
 
void start_init(void)
{
	app_create_init();
	app_start_init();
}
void app_restart_init(void)
{
	app_start_init();
}

#if (BLE_BATT_SERVER)
uint8_t g_conidx = 0;
void bass_add(void)
{
	struct ln_gapm_profile_add_bas cmd = {0};
	cmd.sec_lvl=0;
	cmd.start_hdl=0;
	// Add a BAS instance
	cmd.bas_nb=1;
	// Sending of notifications is supported
	cmd.features[0]=BAS_BATT_LVL_NTF_SUP;
	cmd.batt_level_pres_format[0].unit = ATT_UNIT_PERCENTAGE;
	cmd.batt_level_pres_format[0].description = 1;
	cmd.batt_level_pres_format[0].format = ATT_FORMAT_UINT8;
	cmd.batt_level_pres_format[0].exponent = 0;
	cmd.batt_level_pres_format[0].name_space = 1;
	ln_app_bass_add(&cmd);
}

void bass_enable(uint8_t conidx)
{
	struct bass_enable_req  req= {0};
	req.conidx = conidx;
	req.ntf_cfg = PRF_CLI_START_NTF;
	req.old_batt_lvl[0]   = 100;
	ln_app_bass_enable_prf(&req);
}

void batt_ntf_timer_callback (void *arg)
{
	static int level=0;
	struct bass_batt_level_upd_req req = {0};
	req.batt_level = level;
	req.bas_instance = 0;
	ln_app_bass_send_lvl(g_conidx,&req);
	level++;
	if(level > 100)
		level = 0;
}

int creat_batt_ntf_timer(void)
{
     if(OS_OK != OS_TimerCreate(&bas_timer, OS_TIMER_PERIODIC, batt_ntf_timer_callback, NULL, 1000))
     {
          LOG(LOG_LVL_TRACE,"battery timer create fail\r\n");
          return 1;
     }
	return 0;
}

int start_batt_ntf_timer(void *timer)
{
    if (OS_OK != OS_TimerStart((OS_Timer_t *)timer))
    {
        LOG(LOG_LVL_TRACE,"battery timer start fail\r\n");
        return 1;
    }

    return 0;
}

int stop_batt_ntf_timer(void *timer)
{
    if (OS_OK != OS_TimerStop((OS_Timer_t *)timer))
    {
        LOG(LOG_LVL_TRACE,"battery timer stop fail\r\n");
        return 1;
    }

    return 0;
}

int delete_batt_ntf_timer(void *timer)
{
    if (OS_OK != OS_TimerDelete((OS_Timer_t *)timer))
    {
        LOG(LOG_LVL_TRACE,"battery timer delete fail\r\n");
        return 1;
    }
    return 0;
}
#endif
#if (BLE_BATT_CLIENT)
void basc_add(void)
{
	struct ln_gapm_profile_add_bas cmd = {0};
	cmd.sec_lvl=0;
	cmd.start_hdl=0;
	ln_app_basc_add(&cmd);
}

void basc_enable(uint8_t conidx)
{
	struct basc_enable_req  req= {0};
	req.con_type = PRF_CON_DISCOVERY;
	ln_app_basc_enable_prf(conidx,&req);
}
#endif

static OS_Queue_t ble_usr_queue;
static OS_Semaphore_t usr_semaphore;

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

void ble_app_task_entry(void *params)
{
    ble_usr_msg_t usr_msg;

    usr_creat_queue();

    extern void ble_app_init(void);
    ble_app_init();

#if (BLE_BATT_SERVER)
     creat_batt_ntf_timer();
     bass_add();
#endif
#if (BLE_BATT_CLIENT)
        basc_add();
#endif
#if (SLAVE)
    start_adv();
#endif
#if (MASTER)
    start_init();
#endif
    while(1)
    {
        if(OS_OK == usr_queue_msg_recv((void *)&usr_msg, OS_WAIT_FOREVER))
        {
            LOG(LOG_LVL_TRACE, "connect device number :%d \r\n",con_num);
            switch(usr_msg.id)
            {
                

                case BLE_MSG_CONN_IND:
                {
                    struct ln_gapc_connection_req_info *p_param=(struct ln_gapc_connection_req_info *)usr_msg.msg;
                    #if (BLE_BATT_SERVER)
                    g_conidx = p_param->conidx;
                    bass_enable(p_param->conidx);
                    OS_MsDelay(5000);
                    start_batt_ntf_timer(&bas_timer);
                    #endif
                    #if (BLE_BATT_CLIENT)
                    basc_enable(p_param->conidx);
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

void ble_creat_usr_app_task(void)
{
    if(OS_OK != OS_ThreadCreate(&ble_g_usr_app_thread, "BleUsrAPP", ble_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, BLE_USR_APP_TASK_STACK_SIZE)) 
    {
        LN_ASSERT(1);
    }

}



