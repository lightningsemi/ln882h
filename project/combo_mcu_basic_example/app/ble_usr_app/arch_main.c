#include "rwip_config.h" // RW SW configuration

#include "arch.h"      // architectural platform definitions
#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition

#include "rwip.h"      // RW SW initialization
#include "ke_task.h"   // Kernel Task

#if (BLE_EMB_PRESENT || BT_EMB_PRESENT)
#include "rf.h"        // RF initialization
#endif // BLE_EMB_PRESENT || BT_EMB_PRESENT

#include "reg_assert_mgr.h"
#include "dbg.h"

//LN 
#include "utils/debug/log.h"
#include "osal/osal.h"
#include "ln_def.h"
#include "ln_app_gap.h"
#include "ble_nvds.h"
#include "flash_partition_table.h"
#include "usr_app.h"

OS_Thread_t g_rw_thread;
OS_Queue_t  g_rw_queue;

#if (PLF_DEBUG)
/// Variable to enable infinite loop on assert
volatile int dbg_assert_block = 1;
#endif //PLF_DEBUG

#if (PLF_DEBUG)
void assert_err(const char *condition, const char * file, int line)
{
    TRC_REQ_SW_ASS_ERR(file, line, 0, 0);

    // Trigger assert message
    rwip_assert(file, line, 0, 0, ASSERT_TYPE_ERROR);

    // Let time for the message transfer
    for(int i = 0; i<2000;i++){dbg_assert_block = 1;};

    LOG(LOG_LVL_ERROR, "assert_err:[%s, %d]\r\n", file, line);

    GLOBAL_INT_STOP(); //TODO: maybe we can't disable irq, it will affects wifi stack.

    while(dbg_assert_block);
}

void assert_param2(int param0, int param1, const char * file, int line)
{
    TRC_REQ_SW_ASS_ERR(file, line, param0, param1);

    // Trigger assert message
    rwip_assert(file, line, param0, param1, ASSERT_TYPE_ERROR);
    // Let time for the message transfer
    for(int i = 0; i<2000;i++){dbg_assert_block = 1;};

    LOG(LOG_LVL_INFO, "assert_param:[%s, %d] param0=0x%x, param1=0x%x\r\n", file, line, param0,param1);

    GLOBAL_INT_STOP(); //TODO: maybe we can't disable irq, it will affects wifi stack.
    while(dbg_assert_block);
}

void assert_warn(int param0, int param1, const char * file, int line)
{
    TRC_REQ_SW_ASS_WARN(file, line, param0, param1);

    // Trigger assert message
    rwip_assert(file, line, param0, param1, ASSERT_TYPE_WARNING);

    LOG(LOG_LVL_WARN, "assert_warn:[%s, %d] param0=0x%x, param1=0x%x\r\n", file, line, param0,param1);

    DUMP_DATA(&param0, sizeof(int));
    DUMP_DATA(&param1, sizeof(int));
}

void dump_data(uint8_t* data, uint16_t length)
{
    hexdump(LOG_LVL_INFO, "[BLE hexdump]:", (void *)data, (uint32_t)length);
}
#endif //PLF_DEBUG


static void app_send_gapm_reset_cmd(void)
{
    // Reset the stack
    struct gapm_reset_cmd *p_cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
                                                TASK_GAPM, TASK_APP,
                                                gapm_reset_cmd);

    p_cmd->operation = GAPM_RESET;

    ke_msg_send(p_cmd);

    rw_queue_msg_send(KE_EVENT_KE_MESSAGE, 0, NULL);//first msg to make the main schedule run.
}

void app_init(void)
{
    /// Application Task Descriptor
    extern const struct ke_task_desc TASK_DESC_APP;

    // Create APP task
    ke_task_create(TASK_APP, &TASK_DESC_APP);

    // Initialize Task state
    ke_state_set(TASK_APP, APP_INIT);
    // Reset the stack
    app_send_gapm_reset_cmd();
}

void platform_reset(uint32_t error)
{
    if (error == RESET_AND_LOAD_FW || error == RESET_TO_ROM) {
        // Not yet supported
    } else {
        LOG(LOG_LVL_INFO,"platform reset (error code = 0x%08X)!!!\r\n", error); 
        //TODO: free xxx_env, rwip_heap_xxx, etc.
        //TODO: restart BLE stack
    }

    // Disable interrupts
    GLOBAL_INT_STOP(); //TODO: maybe we can't disable irq, it will affects wifi stack.
}

// BLE device address 
#define BT_DEFAULT_BDADDR  ("\x12\x34\x56\x77\x77\x77")


void ble_nvds_set_mac(void)
{
    struct ln_bd_addr bd_addr = { .addr = BT_DEFAULT_BDADDR,};
    uint8_t nvds_addr[LN_GAP_BD_ADDR_LEN] = {0};
    nvds_tag_len_t tag_len = LN_GAP_BD_ADDR_LEN;
    
    nvds_get(PARAM_ID_BD_ADDRESS, &tag_len, nvds_addr);
    
    if (memcmp(bd_addr.addr, nvds_addr, LN_GAP_BD_ADDR_LEN) != 0) {
        nvds_put(PARAM_ID_BD_ADDRESS, LN_GAP_BD_ADDR_LEN, bd_addr.addr);
    }
    
    memset(nvds_addr,0,LN_GAP_BD_ADDR_LEN);
    nvds_get(PARAM_ID_BD_ADDRESS, &tag_len, nvds_addr);
    
    LOG(LOG_LVL_INFO, "BLE device address:  %02x:%02x:%02x:%02x:%02x:%02x\r\n", \
        nvds_addr[0],nvds_addr[1],nvds_addr[2],nvds_addr[3],nvds_addr[4],nvds_addr[5]);
}

void rw_main(void *params)
{
    rw_queue_msg_t rw_msg = {0};

    // 1. Platform initialization
    // Initialize random process
    srand(1);

    #if (PLF_NVDS)
    // 2. Initialize NVDS modules
	nvds_init((uint8_t *)USER_SPACE_OFFSET, USER_SPACE_SIZE);
    ble_nvds_set_mac();
    #endif

    // 3. RW SW stack initialization
    // Initialize RW SW stack
    rwip_init(RESET_NO_ERROR);

    // finally start interrupt handling
    GLOBAL_INT_START();//TODO: 
    

    // 3. Main loop
    LOG(LOG_LVL_INFO, "rw_main loop start!!\r\n");
    while(1)
    {
        // schedule all pending events
        if(OS_OK == OS_QueueReceive(&g_rw_queue, &rw_msg, OS_WAIT_FOREVER))
        {
        #if (BLE_HOST_PRESENT)
            switch(rw_msg.id)
            {
                case KE_EVENT_AT_CMD:
                {
                    //app_le_at_msg_handler((void*)rw_msg.msg);
                }
                break;

                default:
                break;    
            }
        #endif
            rwip_schedule();
        }
    }
}

void rw_init(void)
{
#define RW_TASK_STACK_SIZE         8*256
#define RQ_MSG_QUEUE_SIZE          64

    OS_Status ret = OS_OK;
    usr_creat_queue();
    usr_creat_semaphore();
    ret = OS_QueueCreate(&g_rw_queue, RQ_MSG_QUEUE_SIZE, sizeof(rw_queue_msg_t));
    if (ret != OS_OK) {
		LOG(LOG_LVL_ERROR, "OS_QueueCreate rw_queue failed, ret=%d\r\n", ret);
        return;
    }

    ret = OS_ThreadCreate(&g_rw_thread, "rw_task", rw_main, NULL, OS_PRIORITY_REAL_TIME, RW_TASK_STACK_SIZE);
    if(ret != OS_OK) {
        LOG(LOG_LVL_ERROR, "OS_ThreadCreate g_rw_thread fail!ret=%d\n", ret);
        return;
    }
}

OS_Queue_t *rw_get_queue_handle(void)
{
    return &g_rw_queue;
}

void rw_queue_msg_send(uint8_t event_type, uint16_t len, void *msg)
{
    rw_queue_msg_t rw_msg;
    
    rw_msg.id = event_type;
    rw_msg.len = len;
    rw_msg.msg = msg;
    OS_QueueSend(&g_rw_queue, &rw_msg, OS_WAIT_FOREVER);
}

