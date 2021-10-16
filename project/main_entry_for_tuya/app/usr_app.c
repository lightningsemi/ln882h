#include "osal/osal.h"
#include "utils/debug/ln_assert.h"
#include "drv_adc_measure.h"
#include "utils/system_parameter.h"
#include "hal/hal_adc.h"
#include "ln_nvds.h"
#include "ln_wifi_err.h"

#define WIFI_TEMP_CALIBRATE             1//1

#if WIFI_TEMP_CALIBRATE
static OS_Thread_t g_temp_cal_thread;
#define TEMP_APP_TASK_STACK_SIZE   4*256 //Byte
#endif

#if  WIFI_TEMP_CALIBRATE
static void temp_cal_app_task_entry(void *params);
void temp_cal_app_task_entry(void *params)
{
    int8_t cap_comp = 0;
    uint16_t adc_val = 0;

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
    }
}
#endif /* WIFI_TEMP_CALIBRATE */

void creat_usr_app_task(void)
{
#if  WIFI_TEMP_CALIBRATE
    if(OS_OK != OS_ThreadCreate(&g_temp_cal_thread, "TempAPP", temp_cal_app_task_entry, NULL, OS_PRIORITY_BELOW_NORMAL, TEMP_APP_TASK_STACK_SIZE)) {
        LN_ASSERT(1);
    }
#endif
}
