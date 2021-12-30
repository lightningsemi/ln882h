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
#include "ln_show_reg.h"
#include "utils/debug/log.h"
#include "ln_test_common.h"

#include "ln_drv_timer.h"

void timer0_callback(void);
void timer1_callback(void);
void timer2_callback(void);
void timer3_callback(void);

int main (int argc, char* argv[])
{  
    /****************** 1. 系统初始化 ***********************/
    SetSysClock();
    log_init();   
    LOG(LOG_LVL_INFO,"ln882H init! \n");
    ln_show_reg_init();

    /****************** 2. RTC 测试***********************/
    timer_init(TIMER_CH_0,500000 ,timer0_callback);
    timer_init(TIMER_CH_1,1000000,timer1_callback);
    timer_init(TIMER_CH_2,2000000,timer2_callback);
    timer_init(TIMER_CH_3,5000000,timer3_callback);

    while(1)
    {
        LOG(LOG_LVL_INFO,"LN882H is running! \n");
        ln_delay_ms(1000);
    }
}

void timer0_callback()
{
    LOG(LOG_LVL_INFO,"go in timer0 interrupt! \n");
}

void timer1_callback()
{
    LOG(LOG_LVL_INFO,"go in timer1 interrupt! \n");
}

void timer2_callback()
{
    LOG(LOG_LVL_INFO,"go in timer2 interrupt! \n");
}

void timer3_callback()
{
    LOG(LOG_LVL_INFO,"go in timer3 interrupt! \n");
}
