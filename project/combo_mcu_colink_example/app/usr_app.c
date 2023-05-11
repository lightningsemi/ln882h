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
#include "drv_adc_measure.h"
#include "hal/hal_adc.h"
#include "ln_nvds.h"
#include "ln_wifi_err.h"
#include "ln_misc.h"
#include "ln882h.h"


#include "rwip_config.h"
#include "llm_int.h"

#include "ln_ble_gap.h"
#include "ln_ble_gatt.h"
#include "usr_app.h"
#include "usr_ble_app.h"

#include "usr_ctrl.h"
#include "colink_port.h"
#include "colink_xcrypt_driver_port.h"


static OS_Thread_t g_usr_app_thread;
#define USR_APP_TASK_STACK_SIZE   6*256 //Byte

#define WIFI_TEMP_CALIBRATE             1//1

#if WIFI_TEMP_CALIBRATE
static OS_Thread_t g_temp_cal_thread;
#define TEMP_APP_TASK_STACK_SIZE   4*256 //Byte
#endif

static void usr_app_task_entry(void *params)
{
    LN_UNUSED(params);

    ln_pm_sleep_mode_set(ACTIVE);

    wifi_manager_init();
	
	ln_pm_always_clk_disable_select(CLK_G_I2S | CLK_G_WS2811 | CLK_G_SDIO | CLK_G_AES | CLK_G_SPI0 | CLK_G_SPI1 | CLK_G_ADV_TIMER);
	
	get_colink_handle()->start();
	

    while(1)
    {
        OS_MsDelay(5000);
		Log_i("Free heap: %d",OS_GetFreeHeapSize());
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

        cnt++;
        if ((cnt % 60) == 0) {
            LOG(LOG_LVL_INFO, "adc raw: %4d, temp_IC: %4d\r\n",
                    curr_adc, (int16_t)(25 + (curr_adc - 770) / 2.54f));
            LOG(LOG_LVL_INFO, "Total:%d; Free:%ld;\r\n", 
                    OS_HeapSizeGet(), OS_GetFreeHeapSize());
        }
    }
}

void creat_usr_app_task(void)
{
    if(OS_OK != OS_ThreadCreate(&g_usr_app_thread, "WifiUsrAPP", usr_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, USR_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }

    //ble_creat_usr_app_task();

#if  WIFI_TEMP_CALIBRATE
    if(OS_OK != OS_ThreadCreate(&g_temp_cal_thread, "TempAPP", temp_cal_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, TEMP_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
#endif

#if STATICTICS_EN
	create_statistics_list_task();
#endif

    /* print sdk version */
    {
        LOG(LOG_LVL_INFO, "LN882H SDK Ver: %s [build time:%s][0x%08x]\r\n",
                LN882H_SDK_VERSION_STRING, LN882H_SDK_BUILD_DATE_TIME, LN882H_SDK_VERSION);
    }
}
