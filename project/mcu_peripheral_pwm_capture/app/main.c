/**
 * @file     main.c
 * @author   BSP Team 
 * @brief    
 * @version  0.0.0.1
 * @date     2021-08-05
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

#include "hal/hal_common.h"
#include "hal/hal_gpio.h"
#include "ln_test_common.h"
#include "ln_show_reg.h"
#include "utils/debug/log.h"
#include "hal/hal_timer.h"
#include "hal/hal_clock.h"

#include "pwm_port.h"
#include "pulse_cap.h"

uint32_t freqlist[10]={1000, 900, 800, 700, 600, 500, 400, 300, 200, 100};
float dutylist[10]={90, 80, 70, 60, 50, 40, 30, 20, 10, 5};
volatile extern uint8_t pwm_start_flag;

int main (int argc, char* argv[])
{     
    /****************** 1. 系统初始化 ***********************/
    SetSysClock();
    log_init();   
    LOG(LOG_LVL_INFO,"ln882H init! \n");
    ln_show_reg_init();

    //for pwm cnt test
    for(uint8_t i = 0; i < 10; i++)
    {
      pwm_cnt_set(1,freqlist[i],dutylist[i]);
      while(pwm_start_flag);
    }
    //for pwm capture
    //app_pwm();
	
    while(1)
    {
        ln_delay_ms(100);
    }
}
