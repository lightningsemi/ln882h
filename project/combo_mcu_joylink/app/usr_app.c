#include "ln_utils.h"  // For LN_UNUSED
#include "utils/debug/log.h"
#include "utils/debug/ln_assert.h"
#include "utils/power_mgmt/ln_pm.h"
#include "netif/ethernetif.h"
#include "wifi_manager.h"
#include "drv_adc_measure.h"
#include "hal/hal_adc.h"
#include "ln_nvds.h"
#include "usr_app.h"

#include "ln_joylink.h"
#include "joylink_thread.h"


static OS_Thread_t g_usr_app_thread;
#define USR_APP_TASK_STACK_SIZE   6*256 //Byte

#define WIFI_TEMP_CALIBRATE             1//1

#if WIFI_TEMP_CALIBRATE
static OS_Thread_t g_temp_cal_thread;
#define TEMP_APP_TASK_STACK_SIZE   4*256 //Byte
#endif

/* declaration */
static void usr_app_task_entry(void *params);
static void temp_cal_app_task_entry(void *params);
static uint8_t mac_addr[6] = {LN_MAC};

void ln_start_joylink(void)
{
    jl_thread_t task_handle = {0};
    task_handle.thread_task = (threadtask) joylink_main_start;
    task_handle.stackSize   = 0x5000; //TODO: 为什么需要20KB的任务栈？
    task_handle.priority    = JL_THREAD_PRI_DEFAULT;
    task_handle.parameter   = NULL;

    ln_pm_sleep_mode_set(ACTIVE);

    wifi_manager_init();

    if (0 != ln_check_dev_bind()) {
        ln_enter_netconfig_mode();
    }

    jl_platform_thread_start(&task_handle);

}

static void usr_app_task_entry(void *params)
{
    LN_UNUSED(params);

    ln_start_joylink();

    while(1)
    {
        OS_MsDelay(30000);
        {
            extern void *heap0_len;
            extern void ln_main_stack_attr_get(uint8_t **start_addr, size_t *size);

            #define HEAP_TOTAL_LEN    (&heap0_len)
            uint8_t *stack_start_addr = NULL;
            size_t   stack_size = 0;
            ln_main_stack_attr_get(&stack_start_addr, &stack_size);

            LOG(LOG_LVL_INFO, "Total:%ld; Free:%ld; addr:%08x; main stack:%ld\r\n", (size_t)HEAP_TOTAL_LEN, OS_GetFreeHeapSize(), stack_start_addr, stack_size);
       }
    }
}

static void temp_cal_app_task_entry(void *params)
{
    LN_UNUSED(params);

    int8_t cap_comp = 0;
    uint16_t adc_val = 0;
    int16_t curr_adc = 0;
    uint8_t cnt = 0;

    if (NVDS_ERR_OK == ln_nvds_get_tx_power_comp((uint8_t *)&cap_comp)) {
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

        cnt++;
        if ((cnt % 60) == 0) {
            LOG(LOG_LVL_INFO, "adc raw: %4d, temp_IC: %4d\r\n",
                    curr_adc, (int16_t)(25 + (curr_adc - 770) / 2.54f));
        }
    }
}


void creat_usr_app_task(void)
{
    if(OS_OK != OS_ThreadCreate(&g_usr_app_thread, "WifiUsrAPP", usr_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, USR_APP_TASK_STACK_SIZE)) {
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

uint8_t * ln_get_mac_array(void)
{
    return mac_addr;
}

