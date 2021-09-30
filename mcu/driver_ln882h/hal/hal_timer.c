/**
 * @file     hal_timer.c
 * @author   BSP Team 
 * @brief    This file provides TIMER function.
 * @version  0.0.0.1
 * @date     2020-12-19
 * 
 * @copyright Copyright (c) 2020 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "hal_timer.h"
#include "reg_sysc_cmp.h"

void hal_tim_init(uint32_t tim_x_base,tim_init_t_def *tim_init_struct)
{
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));

    hal_assert(IS_TIM_MODE(tim_init_struct->tim_mode));
    hal_assert(IS_TIM_CNT_VALUE(tim_init_struct->tim_cnt));
    hal_assert(IS_TIM_CNT_VALUE(tim_init_struct->tim_cnt2));
    hal_assert(IS_TIM_DIV_VALUE(tim_init_struct->tim_div));


    if(tim_init_struct->tim_mode == TIM_FREE_RUNNING_MODE) {
        timer_timermode_setf(tim_x_base,0);
    }
    else if(tim_init_struct->tim_mode == TIM_USER_DEF_CNT_MODE) {
        timer_timermode_setf(tim_x_base,1);
    }

    switch (tim_x_base)
    {
        case TIMER0_BASE:
            sysc_cmp_timer1_div_para_m1_setf(tim_init_struct->tim_div);
            sysc_cmp_timer1_div_para_up_setf(1);
            break;
        case TIMER1_BASE:
            sysc_cmp_timer2_div_para_m1_setf(tim_init_struct->tim_div);
            sysc_cmp_timer2_div_para_up_setf(1);
            break;
        case TIMER2_BASE:
            sysc_cmp_timer3_div_para_m1_setf(tim_init_struct->tim_div);
            sysc_cmp_timer3_div_para_up_setf(1);
            break;
        case TIMER3_BASE:
            sysc_cmp_timer4_div_para_m1_setf(tim_init_struct->tim_div);
            sysc_cmp_timer4_div_para_up_setf(1);
            break;
        default:
            break;
    }


    timer_timerloadcountregister_setf(tim_x_base,tim_init_struct->tim_cnt);
    timer_timerloadcount2register_setf(tim_x_base,tim_init_struct->tim_cnt2);
}

void hal_tim_en(uint32_t tim_x_base,hal_en_t en)
{
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    hal_assert(IS_FUNCTIONAL_STATE(en));
    if(en == HAL_DISABLE) {
        timer_timerenable_setf(tim_x_base,0);
    }
    else if(en == HAL_ENABLE) {
        timer_timerenable_setf(tim_x_base,1);
    }
}

void hal_tim_pwm_en(uint32_t tim_x_base,hal_en_t en)
{   
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    hal_assert(IS_FUNCTIONAL_STATE(en));
    if(en == HAL_DISABLE) {
        timer_timerpwm_setf(tim_x_base,0);
    }
    else if(en == HAL_ENABLE) {
        timer_timerpwm_setf(tim_x_base,1);
    }
}

void hal_tim_set_cnt_value(uint32_t tim_x_base,uint32_t value)
{
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    hal_assert(IS_TIM_CNT_VALUE(value));

    timer_timerloadcountregister_setf(tim_x_base,value);
}

void hal_tim_set_cnt2_value(uint32_t tim_x_base,uint32_t value)
{
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    hal_assert(IS_TIM_CNT_VALUE(value));

    timer_timerloadcount2register_setf(tim_x_base,value);
}

uint32_t hal_tim_get_current_cnt_value(uint32_t tim_x_base)
{
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    return timer_timerloadcountregister_getf(tim_x_base);
}

void hal_tim_it_cfg(uint32_t tim_x_base,tim_it_flag_t tim_it_flag,hal_en_t en)
{
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    hal_assert(IS_TIM_IT_FLAG(tim_it_flag));
    hal_assert(IS_FUNCTIONAL_STATE(en));

    switch (tim_it_flag)
    {
        case TIM_IT_FLAG_ACTIVE:
        {
            if(en == HAL_DISABLE)
            {
                timer_timerinterruptmask_setf(tim_x_base,1);
            }
            else if(en == HAL_ENABLE)
            {
                timer_timerinterruptmask_setf(tim_x_base,0);
            }
            
            break;
        }
        
        default:
            break;
    }
}

uint8_t hal_tim_get_it_flag(uint32_t tim_x_base,tim_it_flag_t tim_it_flag)
{
    uint8_t it_flag = 0;
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    hal_assert(IS_TIM_IT_FLAG(tim_it_flag));

    switch (tim_it_flag)
    {
        case TIM_IT_FLAG_ACTIVE:
        {
            it_flag = timer_timerxinterruptstatusregister_getf(tim_x_base);
            break;
        }    
        default:
            break;
    }
    return it_flag;
}

void hal_tim_clr_it_flag(uint32_t tim_x_base,tim_it_flag_t tim_it_flag)
{
    /* check the parameters */
    hal_assert(IS_TIMER_ALL_PERIPH(tim_x_base));
    hal_assert(IS_TIM_IT_FLAG(tim_it_flag));

    switch (tim_it_flag)
    {
        case TIM_IT_FLAG_ACTIVE:
        {
            timer_timerendofinterruptregister_getf(tim_x_base);
            break;
        }    
        default:
            break;
    }
}
