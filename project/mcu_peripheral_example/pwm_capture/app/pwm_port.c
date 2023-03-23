/**
 * @file     ln_drv_pwm.c
 * @author   BSP Team 
 * @brief    PWM驱动，可以快速产生PWM波形，但不支持复杂波形的配置。
 * @version  0.0.0.1
 * @date     2021-11-01
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */
#include "pwm_port.h"


void ADV_TIMER_IRQHandle(void);
void (*timer2_it_handler)(void);
uint8_t pwm_start_flag = 0;
int g_pwm_cnt = 0;

/**
 * @brief PWM初始化
 * 
 * @param freq              设置PWM频率，  单位hz,范围 100hz ~ 20Mhz
 * @param duty              设置PWM占空比，单位%，范围 0% ~ 100%
 * @param pwm_channel_num   设置通道号，0 ~ 11
 * @param gpio_port         设置PWM GPIO端口
 * @param gpio_pin          设置PWM GPIO引脚
 */
void pwm_init(uint32_t freq, float duty,pwm_channel_t pwm_channel_num,gpio_port_t gpio_port,gpio_pin_t gpio_pin)
{
    uint32_t reg_base = 0;
    uint32_t gpio_reg_base = 0;

    /* PWM引脚初始化 */
    switch(gpio_port)
    {
        case GPIO_A: gpio_reg_base = GPIOA_BASE; break;
        case GPIO_B: gpio_reg_base = GPIOB_BASE; break;
    }

    hal_gpio_pin_afio_select(gpio_reg_base,gpio_pin,(afio_function_t)(ADV_TIMER_PWM0 + pwm_channel_num));
    hal_gpio_pin_afio_en(gpio_reg_base,gpio_pin,HAL_ENABLE);


    switch(pwm_channel_num)
    {
        case PWM_CHA_0: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_1: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_2: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_3: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_4: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_5: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_6: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_7: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_8: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_9: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_10: reg_base = ADV_TIMER_5_BASE; break;
        case PWM_CHA_11: reg_base = ADV_TIMER_5_BASE; break;
    }

    /* PWM参数初始化 */
    adv_tim_init_t_def adv_tim_init;
    memset(&adv_tim_init,0,sizeof(adv_tim_init));

    // 设置PWM频率
    if(freq >= 10000){
        adv_tim_init.adv_tim_clk_div = 0;      
        adv_tim_init.adv_tim_load_value =  hal_clock_get_apb0_clk() * 1.0 / freq - 2;       
    }else{
        adv_tim_init.adv_tim_clk_div = (uint32_t)(hal_clock_get_apb0_clk() / 1000000) - 1;      
        adv_tim_init.adv_tim_load_value =  1000000 / freq - 2;            
    }                 
    
    // 设置PWM占空比
    if((pwm_channel_num & 0x01) == 0)
        adv_tim_init.adv_tim_cmp_a_value = (adv_tim_init.adv_tim_load_value + 2) * duty / 100.0f;      //设置通道a比较值
    else
        adv_tim_init.adv_tim_cmp_b_value = (adv_tim_init.adv_tim_load_value + 2) * duty / 100.0f;      //设置通道b比较值
        
    adv_tim_init.adv_tim_dead_gap_value = 0;                                //设置死区时间
    adv_tim_init.adv_tim_dead_en        = ADV_TIMER_DEAD_DIS;               //不开启死区
    adv_tim_init.adv_tim_cnt_mode       = ADV_TIMER_CNT_MODE_INC;           //向上计数模式
    adv_tim_init.adv_tim_cha_inv_en     = ADV_TIMER_CHA_INV_EN;
    adv_tim_init.adv_tim_chb_inv_en     = ADV_TIMER_CHB_INV_EN;
    adv_tim_init.adv_tim_cha_en = ADV_TIMER_CHA_EN;
	adv_tim_init.adv_tim_chb_en = ADV_TIMER_CHB_EN;
    
		adv_tim_init.adv_tim_cha_it_mode = ADV_TIMER_CHA_IT_MODE_INC;
		adv_tim_init.adv_tim_chb_it_mode = ADV_TIMER_CHB_IT_MODE_INC;

    hal_adv_tim_init(reg_base,&adv_tim_init);
    hal_adv_tim_it_cfg(reg_base, ADV_TIMER_IT_FLAG_CMPA, HAL_ENABLE);
    hal_adv_tim_it_cfg(reg_base, ADV_TIMER_IT_FLAG_CMPB, HAL_ENABLE);
    hal_adv_tim_it_cfg(reg_base, ADV_TIMER_IT_FLAG_LOAD, HAL_ENABLE);
    
}

/**
 * @brief 自定义pwm次数
 * 
 * @param cnt pwm次数
 */
void pwm_cnt_set(int cnt)
{
    pwm_start_flag = 1;
    g_pwm_cnt = cnt;
    pwm_init(1000,20,PWM_CHA_1,GPIO_B,GPIO_PIN_5);
    pwm_start(PWM_CHA_1);
    NVIC_SetPriority(ADV_TIMER_IRQn,     2);     
    NVIC_EnableIRQ(ADV_TIMER_IRQn);      
}

/**
 * @brief 开始产生PWM
 * 
 * @param pwm_channel_num 设置通道号
 */
void pwm_start(pwm_channel_t pwm_channel_num)
{
    uint32_t reg_base = 0;
    switch(pwm_channel_num)
    {
        case PWM_CHA_0: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_1: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_2: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_3: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_4: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_5: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_6: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_7: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_8: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_9: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_10: reg_base = ADV_TIMER_5_BASE; break;
        case PWM_CHA_11: reg_base = ADV_TIMER_5_BASE; break;
    }
    if((pwm_channel_num & 0x01) == 0)
        hal_adv_tim_a_en(reg_base,HAL_ENABLE);                  //使能通道a
    else
        hal_adv_tim_b_en(reg_base,HAL_ENABLE);                  //使能通道b
}

/**
 * @brief 设置PWM占空比
 * 
 * @param duty              设置PWM占空比，单位%，范围 0% ~ 100%
 * @param pwm_channel_num   设置通道号
 */
void pwm_set_duty(float duty, pwm_channel_t pwm_channel_num)
{
    uint32_t reg_base = 0;
    switch(pwm_channel_num)
    {
        case PWM_CHA_0: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_1: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_2: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_3: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_4: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_5: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_6: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_7: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_8: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_9: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_10: reg_base = ADV_TIMER_5_BASE; break;
        case PWM_CHA_11: reg_base = ADV_TIMER_5_BASE; break;
    }
    if((pwm_channel_num & 0x01) == 0)
        hal_adv_tim_set_comp_a(reg_base,(hal_adv_tim_get_load_value(reg_base) + 2) * duty / 100.0f);    //设置通道a比较值
    else
        hal_adv_tim_set_comp_b(reg_base,(hal_adv_tim_get_load_value(reg_base) + 2) * duty / 100.0f);    //设置通道b比较值
}

/**
 * @brief 获得对应通道的占空比
 * 
 * @param pwm_channel_num 设置通道号
 * @return uint32_t 返回PWM占空比，单位%，范围 0% ~ 100%
 */
float pwm_get_duty(pwm_channel_t pwm_channel_num)
{
    uint32_t reg_base = 0;
    float    ret_val = 0;
    switch(pwm_channel_num)
    {
        case PWM_CHA_0: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_1: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_2: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_3: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_4: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_5: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_6: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_7: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_8: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_9: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_10: reg_base = ADV_TIMER_5_BASE; break;
        case PWM_CHA_11: reg_base = ADV_TIMER_5_BASE; break;
    }
    if((pwm_channel_num & 0x01) == 0)
        ret_val = hal_adv_tim_get_comp_a(reg_base) * 1.0f / (hal_adv_tim_get_load_value(reg_base) + 2) * 100;                 
    else
        ret_val = hal_adv_tim_get_comp_b(reg_base) * 1.0f / (hal_adv_tim_get_load_value(reg_base) + 2) * 100;                
        
    return ret_val;
}

/**
 * @brief 设置PWM频率
 * 
 * @param pwm_channel_num 设置通道号
 * @param freq 设置PWM周期，单位us，范围 0 ~ 1638us(未分频)
 */
void pwm_set_freq(pwm_channel_t pwm_channel_num,uint32_t freq)
{
    uint32_t reg_base = 0;
    switch(pwm_channel_num)
    {
        case PWM_CHA_0: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_1: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_2: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_3: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_4: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_5: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_6: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_7: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_8: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_9: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_10: reg_base = ADV_TIMER_5_BASE; break;
        case PWM_CHA_11: reg_base = ADV_TIMER_5_BASE; break;
    }
		
		
    if((pwm_channel_num & 0x01) == 0)
        hal_adv_tim_a_en(reg_base,HAL_DISABLE);                 //失能通道a
    else
        hal_adv_tim_b_en(reg_base,HAL_DISABLE);                 //失能通道b
        
    // 设置PWM频率
    if(freq >= 10000){
				pwm_div_setf(reg_base, 0);
				hal_adv_tim_set_load_value(reg_base,hal_clock_get_apb0_clk() * 1.0 / freq - 2);
    }else{
				pwm_div_setf(reg_base, (uint32_t)(hal_clock_get_apb0_clk() / 1000000) - 1);
				hal_adv_tim_set_load_value(reg_base,1000000 / freq - 2);           
    }                 
    
    if((pwm_channel_num & 0x01) == 0)
        hal_adv_tim_a_en(reg_base,HAL_ENABLE);                  //使能通道a
    else
        hal_adv_tim_b_en(reg_base,HAL_ENABLE);                  //使能通道b
}

/**
 * @brief 查询PWM频率
 * 
 * @param pwm_channel_num 设置通道号
 * @param freq 设置PWM周期，单位us，范围 0 ~ 1638us(未分频)
 */
uint32_t pwm_get_freq(pwm_channel_t pwm_channel_num)
{
    uint32_t reg_base = 0;
		uint32_t freq = 0;
    switch(pwm_channel_num)
    {
        case PWM_CHA_0: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_1: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_2: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_3: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_4: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_5: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_6: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_7: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_8: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_9: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_10: reg_base = ADV_TIMER_5_BASE; break;
        case PWM_CHA_11: reg_base = ADV_TIMER_5_BASE; break;
    }
		
		freq = hal_adv_tim_get_load_value(reg_base);
		
		if(freq < 3998 )
		{
			freq = 1000000 / (freq + 2);
		}
		else
		{
			freq = (hal_clock_get_apb0_clk()) / (hal_adv_tim_get_load_value(reg_base) + 2);
		}
		
		return freq;
}

/**
 * @brief 关闭pwm通道
 * 
 * @param pwm_channel_num 设置通道号
 */
void pwm_disable(pwm_channel_t pwm_channel_num)
{
    uint32_t reg_base = 0;
		uint32_t freq = 0;
    switch(pwm_channel_num)
    {
        case PWM_CHA_0: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_1: reg_base = ADV_TIMER_0_BASE; break;
        case PWM_CHA_2: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_3: reg_base = ADV_TIMER_1_BASE; break;
        case PWM_CHA_4: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_5: reg_base = ADV_TIMER_2_BASE; break;
        case PWM_CHA_6: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_7: reg_base = ADV_TIMER_3_BASE; break;
        case PWM_CHA_8: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_9: reg_base = ADV_TIMER_4_BASE; break;
        case PWM_CHA_10: reg_base = ADV_TIMER_5_BASE; break;
        case PWM_CHA_11: reg_base = ADV_TIMER_5_BASE; break;
    }
		
    if((pwm_channel_num & 0x01) == 0)
        hal_adv_tim_a_en(reg_base,HAL_DISABLE);                 //失能通道a
    else
        hal_adv_tim_b_en(reg_base,HAL_DISABLE);                 //失能通道b

}

/**
 * @brief 定时器初始化
 * 
 * @param timer_ch 选择定时器通道
 * @param us       0 ~ 16777215 us
 * @param timer_it_callback 定时器中断回调函数
 */
void timer_init(timer_ch_t timer_ch,uint32_t us,void (*timer_it_callback)(void))
{
    uint32_t timer_base = 0;
    switch(timer_ch)
    {
        case TIMER_CH_0: timer_base = TIMER0_BASE; break;
        case TIMER_CH_1: timer_base = TIMER1_BASE; break;
        case TIMER_CH_2: timer_base = TIMER2_BASE; break;
        case TIMER_CH_3: timer_base = TIMER3_BASE; break;
    }

    //Timer初始化
    tim_init_t_def tim_init;
    memset(&tim_init,0,sizeof(tim_init));
    tim_init.tim_mode = TIM_USER_DEF_CNT_MODE;                      //设置定时器模式，设置为用户定义模式
    
    if(us < 1000){
        tim_init.tim_div = 0;                                      
        tim_init.tim_load_value = us * (uint32_t)(hal_clock_get_apb0_clk() / 1000000)  - 1;                       
    }else{
        tim_init.tim_div = (uint32_t)(hal_clock_get_apb0_clk() / 1000000) - 1;                                
        tim_init.tim_load_value = us - 1;                           
    }
    hal_tim_init(timer_base,&tim_init);                             //初始化定时器
    hal_tim_en(timer_base,HAL_ENABLE);                              //使能定时器模块
    hal_tim_it_cfg(timer_base,TIM_IT_FLAG_ACTIVE,HAL_ENABLE);       //配置中断

    switch(timer_ch)
    {
        case TIMER_CH_0:
        {
            NVIC_SetPriority(TIMER0_IRQn,     4);           //配置定时器优先级
            NVIC_EnableIRQ(TIMER0_IRQn);                    //使能定时器中断
            if(timer_it_callback != NULL)
            break;
        }
        case TIMER_CH_1:
        {
            NVIC_SetPriority(TIMER1_IRQn,     4);           //配置定时器优先级
            NVIC_EnableIRQ(TIMER1_IRQn);                    //使能定时器中断
            if(timer_it_callback != NULL)

            break;
        }
        case TIMER_CH_2:
        {
            NVIC_SetPriority(TIMER2_IRQn,     4);           //配置定时器优先级
            NVIC_EnableIRQ(TIMER2_IRQn);                    //使能定时器中断
            if(timer_it_callback != NULL)
                timer2_it_handler = timer_it_callback;
            break;
        }
        case TIMER_CH_3:
        {
        }
    }           
}

/**
 * @brief 获取当前定时器中计数器的运行时间，单位us
 * 
 * @param timer_ch      选择定时器通道
 * @return uint32_t     返回当前定时器中计数器的运行时间，单位us
 */
uint32_t timer_get_timer_cnt_time(timer_ch_t timer_ch)
{
    uint32_t timer_base = 0;
    switch(timer_ch)
    {
        case TIMER_CH_0: timer_base = TIMER0_BASE; break;
        case TIMER_CH_1: timer_base = TIMER1_BASE; break;
        case TIMER_CH_2: timer_base = TIMER2_BASE; break;
        case TIMER_CH_3: timer_base = TIMER3_BASE; break;
    }
    return (uint32_t)((hal_tim_get_load_value(timer_base) - hal_tim_get_current_cnt_value(timer_base)) 
            / ((hal_clock_get_apb0_clk() / 1000000) * 1.0f / ((hal_tim_get_div(timer_base) + 1))));
}



/**
 * @brief TIMER2中断处理函数
 */
void TIMER2_IRQHandler()
{
    if(hal_tim_get_it_flag(TIMER2_BASE,TIM_IT_FLAG_ACTIVE))
    {
        hal_tim_clr_it_flag(TIMER2_BASE,TIM_IT_FLAG_ACTIVE);
        if(timer2_it_handler != NULL)
            timer2_it_handler();
    }
}


void ADV_TIMER_IRQHandler(void)
{
   uint8_t it_flag = 0;
    
   if(pwm_start_flag)
   {
       it_flag = hal_adv_tim_get_it_flag(ADV_TIMER_0_BASE, ADV_TIMER_IT_FLAG_LOAD);
       if(it_flag)
       {
           hal_adv_tim_clr_it_flag(ADV_TIMER_0_BASE, ADV_TIMER_IT_FLAG_LOAD);
            if(g_pwm_cnt > 0)
            {
                pwm_cmp_a_setf(ADV_TIMER_0_BASE,200);
                g_pwm_cnt --;
            }
            else
            {
                pwm_start_flag = 0;
                pwm_disable(PWM_CHA_1);
                NVIC_DisableIRQ(ADV_TIMER_IRQn);
            }
        }
       else
       {
           it_flag =0;
       }
    }
}
