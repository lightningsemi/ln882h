/**
 * @file     ln_drv_pwm.c
 * @author   BSP Team 
 * @brief    PWM���������Կ��ٲ���PWM���Σ�����֧�ָ��Ӳ��ε����á�
 * @version  0.0.0.1
 * @date     2021-11-01
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */
#include "pwm_port.h"

void (*timer2_it_handler)(void);

/**
 * @brief PWM��ʼ��
 * 
 * @param freq              ����PWMƵ�ʣ�  ��λhz,��Χ 100hz ~ 20Mhz
 * @param duty              ����PWMռ�ձȣ���λ%����Χ 0% ~ 100%
 * @param pwm_channel_num   ����ͨ���ţ�0 ~ 11
 * @param gpio_port         ����PWM GPIO�˿�
 * @param gpio_pin          ����PWM GPIO����
 */
void pwm_init(uint32_t freq, float duty,pwm_channel_t pwm_channel_num,gpio_port_t gpio_port,gpio_pin_t gpio_pin)
{
    uint32_t reg_base = 0;
    uint32_t gpio_reg_base = 0;

    /* PWM���ų�ʼ�� */
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

    /* PWM������ʼ�� */
    adv_tim_init_t_def adv_tim_init;
    memset(&adv_tim_init,0,sizeof(adv_tim_init));

    // ����PWMƵ��
    if(freq >= 10000){
        adv_tim_init.adv_tim_clk_div = 0;      
        adv_tim_init.adv_tim_load_value =  hal_clock_get_apb0_clk() * 1.0 / freq - 2;       
    }else{
        adv_tim_init.adv_tim_clk_div = (uint32_t)(hal_clock_get_apb0_clk() / 1000000) - 1;      
        adv_tim_init.adv_tim_load_value =  1000000 / freq - 2;            
    }                 
    
    // ����PWMռ�ձ�
    if((pwm_channel_num & 0x01) == 0)
        adv_tim_init.adv_tim_cmp_a_value = (adv_tim_init.adv_tim_load_value + 2) * duty / 100.0f;      //����ͨ��a�Ƚ�ֵ
    else
        adv_tim_init.adv_tim_cmp_b_value = (adv_tim_init.adv_tim_load_value + 2) * duty / 100.0f;      //����ͨ��b�Ƚ�ֵ
        
    adv_tim_init.adv_tim_dead_gap_value = 0;                                //��������ʱ��
    adv_tim_init.adv_tim_dead_en        = ADV_TIMER_DEAD_DIS;               //����������
    adv_tim_init.adv_tim_cnt_mode       = ADV_TIMER_CNT_MODE_INC;           //���ϼ���ģʽ
    adv_tim_init.adv_tim_cha_inv_en     = ADV_TIMER_CHA_INV_EN;
    adv_tim_init.adv_tim_chb_inv_en     = ADV_TIMER_CHB_INV_EN;
		
		adv_tim_init.adv_tim_cha_it_mode = ADV_TIMER_CHA_IT_MODE_INC;
		adv_tim_init.adv_tim_chb_it_mode = ADV_TIMER_CHB_IT_MODE_INC;

    hal_adv_tim_init(reg_base,&adv_tim_init);
}

/**
 * @brief ��ʼ����PWM
 * 
 * @param pwm_channel_num ����ͨ����
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
        hal_adv_tim_a_en(reg_base,HAL_ENABLE);                  //ʹ��ͨ��a
    else
        hal_adv_tim_b_en(reg_base,HAL_ENABLE);                  //ʹ��ͨ��b
}

/**
 * @brief ����PWMռ�ձ�
 * 
 * @param duty              ����PWMռ�ձȣ���λ%����Χ 0% ~ 100%
 * @param pwm_channel_num   ����ͨ����
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
        hal_adv_tim_set_comp_a(reg_base,(hal_adv_tim_get_load_value(reg_base) + 2) * duty / 100.0f);    //����ͨ��a�Ƚ�ֵ
    else
        hal_adv_tim_set_comp_b(reg_base,(hal_adv_tim_get_load_value(reg_base) + 2) * duty / 100.0f);    //����ͨ��b�Ƚ�ֵ
}

/**
 * @brief ��ö�Ӧͨ����ռ�ձ�
 * 
 * @param pwm_channel_num ����ͨ����
 * @return uint32_t ����PWMռ�ձȣ���λ%����Χ 0% ~ 100%
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
 * @brief ����PWMƵ��
 * 
 * @param pwm_channel_num ����ͨ����
 * @param freq ����PWM���ڣ���λus����Χ 0 ~ 1638us(δ��Ƶ)
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
        hal_adv_tim_a_en(reg_base,HAL_DISABLE);                 //ʧ��ͨ��a
    else
        hal_adv_tim_b_en(reg_base,HAL_DISABLE);                 //ʧ��ͨ��b
        
    // ����PWMƵ��
    if(freq >= 10000){
				pwm_div_setf(reg_base, 0);
				hal_adv_tim_set_load_value(reg_base,hal_clock_get_apb0_clk() * 1.0 / freq - 2);
    }else{
				pwm_div_setf(reg_base, (uint32_t)(hal_clock_get_apb0_clk() / 1000000) - 1);
				hal_adv_tim_set_load_value(reg_base,1000000 / freq - 2);           
    }                 
    
    if((pwm_channel_num & 0x01) == 0)
        hal_adv_tim_a_en(reg_base,HAL_ENABLE);                  //ʹ��ͨ��a
    else
        hal_adv_tim_b_en(reg_base,HAL_ENABLE);                  //ʹ��ͨ��b
}

/**
 * @brief ��ѯPWMƵ��
 * 
 * @param pwm_channel_num ����ͨ����
 * @param freq ����PWM���ڣ���λus����Χ 0 ~ 1638us(δ��Ƶ)
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
 * @brief �ر�pwmͨ��
 * 
 * @param pwm_channel_num ����ͨ����
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
        hal_adv_tim_a_en(reg_base,HAL_DISABLE);                 //ʧ��ͨ��a
    else
        hal_adv_tim_b_en(reg_base,HAL_DISABLE);                 //ʧ��ͨ��b

}

/**
 * @brief ��ʱ����ʼ��
 * 
 * @param timer_ch ѡ��ʱ��ͨ��
 * @param us       0 ~ 16777215 us
 * @param timer_it_callback ��ʱ���жϻص�����
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

    //Timer��ʼ��
    tim_init_t_def tim_init;
    memset(&tim_init,0,sizeof(tim_init));
    tim_init.tim_mode = TIM_USER_DEF_CNT_MODE;                      //���ö�ʱ��ģʽ������Ϊ�û�����ģʽ
    
    if(us < 1000){
        tim_init.tim_div = 0;                                      
        tim_init.tim_load_value = us * (uint32_t)(hal_clock_get_apb0_clk() / 1000000)  - 1;                       
    }else{
        tim_init.tim_div = (uint32_t)(hal_clock_get_apb0_clk() / 1000000) - 1;                                
        tim_init.tim_load_value = us - 1;                           
    }
    hal_tim_init(timer_base,&tim_init);                             //��ʼ����ʱ��
    hal_tim_en(timer_base,HAL_ENABLE);                              //ʹ�ܶ�ʱ��ģ��
    hal_tim_it_cfg(timer_base,TIM_IT_FLAG_ACTIVE,HAL_ENABLE);       //�����ж�

    switch(timer_ch)
    {
        case TIMER_CH_0:
        {
            NVIC_SetPriority(TIMER0_IRQn,     4);           //���ö�ʱ�����ȼ�
            NVIC_EnableIRQ(TIMER0_IRQn);                    //ʹ�ܶ�ʱ���ж�
            if(timer_it_callback != NULL)
            break;
        }
        case TIMER_CH_1:
        {
            NVIC_SetPriority(TIMER1_IRQn,     4);           //���ö�ʱ�����ȼ�
            NVIC_EnableIRQ(TIMER1_IRQn);                    //ʹ�ܶ�ʱ���ж�
            if(timer_it_callback != NULL)

            break;
        }
        case TIMER_CH_2:
        {
            NVIC_SetPriority(TIMER2_IRQn,     4);           //���ö�ʱ�����ȼ�
            NVIC_EnableIRQ(TIMER2_IRQn);                    //ʹ�ܶ�ʱ���ж�
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
 * @brief ��ȡ��ǰ��ʱ���м�����������ʱ�䣬��λus
 * 
 * @param timer_ch      ѡ��ʱ��ͨ��
 * @return uint32_t     ���ص�ǰ��ʱ���м�����������ʱ�䣬��λus
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
 * @brief TIMER2�жϴ�������
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
