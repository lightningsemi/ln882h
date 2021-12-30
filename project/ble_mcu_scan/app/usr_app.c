#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "rwip_config.h"
#include "llm_int.h"
#include "ln_app_gatt.h"
#include "ln_app_gap.h"
#include "usr_app.h"
#include "gapm_task.h"
#include "ln_app_gap.h"
#include "ln_app_gatt.h"
#include "ln_app_callback.h"
#include "osal/osal.h"
#include "usr_ble_app.h"



#define BLE_USR_APP_TASK_STACK_SIZE  (1024)

static OS_Thread_t ble_g_usr_app_thread;

uint8_t scan_actv_idx = 0;
uint8_t adv_actv_idx  = 0;
uint8_t init_actv_idx = 0;

static void create_scanning(void)
{
    struct ln_gapm_activity_create_cmd scan_creat_param;
    scan_creat_param.own_addr_type = GAPM_STATIC_ADDR;
    ln_app_scan_creat(&scan_creat_param);
}

static void start_scanning(void)
{
    struct ln_gapm_activity_start_cmd  scan_start_param;

    memset(&scan_start_param,0,sizeof(struct ln_gapm_activity_start_cmd));
    
    scan_start_param.actv_idx                                      = scan_actv_idx;
    scan_start_param.u_param.scan_param.type                       = GAPM_SCAN_TYPE_OBSERVER;
    scan_start_param.u_param.scan_param.prop                       = GAPM_SCAN_PROP_ACTIVE_1M_BIT|GAPM_SCAN_PROP_PHY_1M_BIT;
    scan_start_param.u_param.scan_param.dup_filt_pol               = 1;  //0;
    scan_start_param.u_param.scan_param.scan_param_1m.scan_intv    = 0xa0; //20*0.625 ms
    scan_start_param.u_param.scan_param.scan_param_1m.scan_wd      = 0x50; //15*0.625 ms
    scan_start_param.u_param.scan_param.duration                   = 0;  
    scan_start_param.u_param.scan_param.scan_param_coded.scan_intv = 0xa0;
    scan_start_param.u_param.scan_param.scan_param_coded.scan_wd   = 0x50;
    scan_start_param.u_param.scan_param.period                     = 2;
    ln_app_scan_start(&scan_start_param);
}

static void app_start_scan(void)
{
    create_scanning();
    start_scanning();
}

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
    app_start_scan();
    while(1)
    {
        if(OS_OK == usr_queue_msg_recv((void *)&usr_msg, OS_WAIT_FOREVER))
        {
            switch(usr_msg.id)
            {

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



