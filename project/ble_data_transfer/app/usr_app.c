#include "ln_utils.h"  // For LN_UNUSED
#include "osal/osal.h"
#include "ln_kv.h"
#include "ln_kv_api.h"
#include "ln_kv_key_def.h"
#include "ln_misc.h"
#include "proj_config.h"
#include "utils/debug/ln_assert.h"
#include "utils/debug/log.h"
#include "usr_app.h"

static OS_Thread_t g_nim_app_thread;
#define NIM_APP_TASK_STACK_SIZE     (4096)

static uint8_t mac_addr[6] = 
{   
    LN_BLE_MAC_ADDR0, 
    LN_BLE_MAC_ADDR1, 
    LN_BLE_MAC_ADDR2, 
    LN_BLE_MAC_ADDR3, 
    LN_BLE_MAC_ADDR4, 
    LN_BLE_MAC_ADDR5
};
static uint8_t mac_addr_reverse[6] = 
{   
    LN_BLE_MAC_ADDR5, 
    LN_BLE_MAC_ADDR4, 
    LN_BLE_MAC_ADDR3, 
    LN_BLE_MAC_ADDR2, 
    LN_BLE_MAC_ADDR1, 
    LN_BLE_MAC_ADDR0
};

void ln_load_config_info(void)
{
    //1. load mac
    int value_len = 0;
    if(ln_kv_has_key(KV_BLE_MAC)){  
        if (KV_ERR_NONE != ln_kv_get(KV_BLE_MAC, (void *)mac_addr, sizeof(mac_addr), (size_t *)&value_len)) 
        {
            LOG(LOG_LVL_ERROR, "get mac from kv error!\r\n");
        }
    }else{
        LOG(LOG_LVL_INFO, "mac kv never set!\r\n");
        if (mac_addr[0] == LN_BLE_MAC_ADDR0 &&
            mac_addr[1] == LN_BLE_MAC_ADDR1 &&
            mac_addr[2] == LN_BLE_MAC_ADDR2 &&
            mac_addr[3] == LN_BLE_MAC_ADDR3 &&
            mac_addr[4] == LN_BLE_MAC_ADDR4 &&
            mac_addr[5] == LN_BLE_MAC_ADDR5){
            ln_generate_random_mac(mac_addr);
        }
        if (KV_ERR_NONE != ln_kv_set(KV_BLE_MAC, (void *)&mac_addr, sizeof(mac_addr)))
        {
            LOG(LOG_LVL_ERROR, "set mac to kv error!\r\n");
        } 
    }
    mac_addr_reverse[0] = mac_addr[5];
    mac_addr_reverse[1] = mac_addr[4];
    mac_addr_reverse[2] = mac_addr[3];
    mac_addr_reverse[3] = mac_addr[2];
    mac_addr_reverse[4] = mac_addr[1];
    mac_addr_reverse[5] = mac_addr[0];
    LOG(LOG_LVL_INFO, "mac[%02x:%02x:%02x:%02x:%02x:%02x]\r\n", 
                        mac_addr[0], mac_addr[1], mac_addr[2], 
                        mac_addr[3], mac_addr[4], mac_addr[5]);
}

uint8_t *ln_get_mac_array(bool reverse)
{ 
    if (reverse){
        return mac_addr_reverse;
    }else{
        return mac_addr;
    }
}

extern void app_main(void);
static void nim_app_task_entry(void *params)
{
    LN_UNUSED(params);
    app_main();
    while(1)
    {
        LOG(LOG_LVL_INFO, "total: %d, free: %d\r\n", OS_HeapSizeGet(), OS_GetFreeHeapSize());  
        OS_MsDelay(10000); 
              
    }
}

void creat_usr_app_task(void)
{

    if(OS_OK != OS_ThreadCreate(&g_nim_app_thread, "nimApp", nim_app_task_entry, NULL, OS_PRIORITY_NORMAL, NIM_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
    /* print sdk version */
    {
        LOG(LOG_LVL_INFO, "LN882H SDK Ver: %s [build time:%s][0x%08x]\r\n",
                LN882H_SDK_VERSION_STRING, LN882H_SDK_BUILD_DATE_TIME, LN882H_SDK_VERSION);
    }
}
