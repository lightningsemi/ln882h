/**
 * @file     hal_adc.c
 * @author   BSP Team 
 * @brief    This file provides ADC function.
 * @version  0.0.0.1
 * @date     2021-06-29
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */


/* Includes ------------------------------------------------------------------*/
#include "hal_adc.h"

void hal_adc_init(uint32_t adc_base,adc_init_t_def* adc_init_struct)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));

    hal_assert(IS_AWD_ADC_CH(adc_init_struct->adc_awd_ch));
    hal_assert(IS_ADC_AWD_CH_SEL(adc_init_struct->adc_awd_sgl));
    hal_assert(IS_ADC_AUTO_OFF_MODE(adc_init_struct->adc_auto_off_mode));
    hal_assert(IS_ADC_WAIT_CONV_MODE_EN_STATUS(adc_init_struct->adc_wait_conv_mode_en));
    hal_assert(IS_ADC_CONV_MODE(adc_init_struct->adc_conv_mode));
    hal_assert(IS_ADC_DATA_ALIGN_MODE(adc_init_struct->adc_data_align_mode));
    hal_assert(IS_ADC_SPEC_EXT_TRIG_EN_STATUS(adc_init_struct->adc_spec_ext_trig_en));
    hal_assert(IS_ADC_SPEC_EXT_TRIG_EVENT(adc_init_struct->adc_spec_ext_trig_event_sel));
    hal_assert(IS_ADC_EXT_TRIG_EN_STATUS(adc_init_struct->adc_ext_trig_en));
    hal_assert(IS_ADC_EXT_TRIG_EVENT(adc_init_struct->adc_ext_trig_sel));
    hal_assert(IS_ADC_STAB_TIME(adc_init_struct->adc_stab_time));
    hal_assert(IS_ADC_OVER_SAMPLING_RATIO(adc_init_struct->adc_ov_smp_ratio));
    hal_assert(IS_ADC_OVER_SAMPLING_EN_STATUS(adc_init_struct->adc_ov_smp_ratio_en));
    hal_assert(IS_ADC_HIGH_THOD(adc_init_struct->adc_ht));
    hal_assert(IS_ADC_LOW_THOD(adc_init_struct->adc_lt));
    hal_assert(IS_ADC_SPE_CH(adc_init_struct->adc_spe_ch));
    hal_assert(IS_ADC_CH(adc_init_struct->adc_ch));
    hal_assert(IS_ADC_PRESCALE_VALUE(adc_init_struct->adc_presc));
    hal_assert(IS_ADC_VREF_VALUE(adc_init_struct->adc_vref_set));
    hal_assert(IS_ADC_CH_INGAIN_VALUE(adc_init_struct->adc_ch_ingain));


    if (adc_init_struct->adc_awd_sgl == ADC_AWD_ALL_CH) {
        adc_awdsgl_setf(adc_base,0);
    }
    else if (adc_init_struct->adc_awd_sgl == ADC_AWD_SGL_CH) {
        adc_awdsgl_setf(adc_base,1);
    }

    if (adc_init_struct->adc_auto_off_mode == ADC_AUTO_OFF_MODE_DIS) {
        adc_autoff_setf(adc_base,0);
    }
    else if (adc_init_struct->adc_auto_off_mode == ADC_AUTO_OFF_MODE_EN) {
        adc_autoff_setf(adc_base,1);
    }

    if (adc_init_struct->adc_wait_conv_mode_en == ADC_WAIT_CONV_MODE_DIS) {
        adc_waitm_setf(adc_base,0);
    }
    else if (adc_init_struct->adc_wait_conv_mode_en == ADC_WAIT_CONV_MODE_EN) {
        adc_waitm_setf(adc_base,1);
    }

    if (adc_init_struct->adc_conv_mode == ADC_CONV_MODE_SINGLE) {
        adc_cont_setf(adc_base,0);
    }
    else if (adc_init_struct->adc_conv_mode == ADC_CONV_MODE_CONTINUE) {
        adc_cont_setf(adc_base,1);
    }

    if (adc_init_struct->adc_data_align_mode == ADC_DATA_ALIGN_RIGHT) {
        adc_align_setf(adc_base,0);
    }
    else if (adc_init_struct->adc_data_align_mode == ADC_DATA_ALIGN_LEFT) {
        adc_align_setf(adc_base,1);
    }

    if (adc_init_struct->adc_spec_ext_trig_en == ADC_SPEC_EXT_TRIG_DIS) {
        adc_spec_exten_setf(adc_base,0);
    }
    else if (adc_init_struct->adc_spec_ext_trig_en == ADC_SPEC_EXT_TRIG_EN) {
        adc_spec_exten_setf(adc_base,1);
    }

    switch (adc_init_struct->adc_spec_ext_trig_event_sel)
    {
        case ADC_SPEC_EXT_TRIG_0:
            adc_spec_extsel_setf(adc_base,0);
            break;

        case ADC_SPEC_EXT_TRIG_1:
            adc_spec_extsel_setf(adc_base,1);
            break;

        case ADC_SPEC_EXT_TRIG_2:
            adc_spec_extsel_setf(adc_base,2);
            break;

        case ADC_SPEC_EXT_TRIG_3:
            adc_spec_extsel_setf(adc_base,3);
            break;
        
        default:
            break;
    }

    switch (adc_init_struct->adc_ext_trig_en)
    {
        case ADC_EXT_TRIG_DIS:
            adc_exten_setf(adc_base,0);
            break;

        case ADC_EXT_TRIG_RISING:
            adc_exten_setf(adc_base,1);
            break;

        case ADC_EXT_TRIG_FALLING:
            adc_exten_setf(adc_base,2);
            break;

        case ADC_EXT_TRIG_RISING_AND_FALLING:
            adc_exten_setf(adc_base,3);
            break;
        
        default:
            break;
    }

    switch (adc_init_struct->adc_ext_trig_sel)
    {
        case ADC_EXT_TRIG_0:
            adc_extsel_setf(adc_base,0);
            break;

        case ADC_EXT_TRIG_1:
            adc_extsel_setf(adc_base,1);
            break;

        case ADC_EXT_TRIG_2:
            adc_extsel_setf(adc_base,2);
            break;

        case ADC_EXT_TRIG_3:
            adc_extsel_setf(adc_base,3);
            break;
        
        default:
            break;
    }

    adc_stab_setf(adc_base,adc_init_struct->adc_stab_time);


    switch (adc_init_struct->adc_ov_smp_ratio)
    {
        case ADC_OVER_SAMPLING_RATIO_X2:
            adc_ovsr_setf(adc_base,0);
            break;

        case ADC_OVER_SAMPLING_RATIO_X4:
            adc_ovsr_setf(adc_base,1);
            break;

        case ADC_OVER_SAMPLING_RATIO_X5:
            adc_ovsr_setf(adc_base,2);
            break;

        case ADC_OVER_SAMPLING_RATIO_X16:
            adc_ovsr_setf(adc_base,3);
            break;
            
        case ADC_OVER_SAMPLING_RATIO_X32:
            adc_ovsr_setf(adc_base,4);
            break;

        case ADC_OVER_SAMPLING_RATIO_X64:
            adc_ovsr_setf(adc_base,5);
            break;
        
        default:
            break;
    }

    adc_ovse_setf(adc_base,adc_init_struct->adc_ov_smp_ratio_en);
    adc_ht_setf(adc_base,adc_init_struct->adc_ht);
    adc_lt_setf(adc_base,adc_init_struct->adc_lt);

    switch (adc_init_struct->adc_spe_ch)
    {
        case ADC_SPE_CH0:
            adc_spec_ch_setf(adc_base,0);
            break;

        case ADC_SPE_CH1:
            adc_spec_ch_setf(adc_base,1);
            break;

        case ADC_SPE_CH2:
            adc_spec_ch_setf(adc_base,2);
            break;

        case ADC_SPE_CH3:
            adc_spec_ch_setf(adc_base,3);
            break;
            
        case ADC_SPE_CH4:
            adc_spec_ch_setf(adc_base,4);
            break;

        case ADC_SPE_CH5:
            adc_spec_ch_setf(adc_base,5);
            break;

        case ADC_SPE_CH6:
            adc_spec_ch_setf(adc_base,6);
            break;

        case ADC_SPE_CH7:
            adc_spec_ch_setf(adc_base,7);
            break;
        
        default:
            break;
    }

    switch (adc_init_struct->adc_ch)
    {
        case ADC_CH0:
            adc_chselx_setf(adc_base, 1 << 0);
        break;

        case ADC_CH1:
            adc_chselx_setf(adc_base, 1 << 1);
        break;

        case ADC_CH2:
            adc_chselx_setf(adc_base, 1 << 2);
        break;

        case ADC_CH3:
            adc_chselx_setf(adc_base, 1 << 3);
        break;

        case ADC_CH4:
            adc_chselx_setf(adc_base, 1 << 4);
        break;

        case ADC_CH5:
            adc_chselx_setf(adc_base, 1 << 5);
        break;

        case ADC_CH6:
            adc_chselx_setf(adc_base, 1 << 6);
        break;

        case ADC_CH7:
            adc_chselx_setf(adc_base, 1 << 7);
        break;
    
        default:
            break;
    }
    adc_presc_setf(adc_base,adc_init_struct->adc_presc);



}

void hal_adc_dma_en(uint32_t adc_base,hal_en_t en)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    hal_assert(IS_FUNCTIONAL_STATE(en));

    if (en == HAL_DISABLE) {
        adc_dma_en_setf(adc_base,0);
    }
    else if (en == HAL_ENABLE) {
        adc_dma_en_setf(adc_base,1);
    }
}

void hal_adc_awd_en(uint32_t adc_base,hal_en_t en)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    hal_assert(IS_FUNCTIONAL_STATE(en));

    if (en == HAL_DISABLE) {
        adc_awden_setf(adc_base,0);
    }
    else if (en == HAL_ENABLE) {
        adc_awden_setf(adc_base,1);
    }    
}

void hal_adc_en(uint32_t adc_base,hal_en_t en)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    hal_assert(IS_FUNCTIONAL_STATE(en));

    if (en == HAL_DISABLE) {
        adc_aden_setf(adc_base,0);
    }
    else if (en == HAL_ENABLE) {
        adc_aden_setf(adc_base,1);
    }  
}

void hal_adc_start_conv(uint32_t adc_base)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    adc_adstart_setf(adc_base,1);
}

void hal_adc_stop_conv(uint32_t adc_base)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    adc_adstp_setf(adc_base,1);
}

void hal_adc_spe_sw_start(uint32_t adc_base)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    adc_spec_sw_start_setf(adc_base,1);
}

uint16_t hal_adc_get_data(uint32_t adc_base,adc_ch_t ch)
{   
    uint16_t data = 0;
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    hal_assert(IS_ADC_CH(ch));

    switch (ch)
    {
        case ADC_CH0:
            data = adc_data_getf(adc_base);
            break;

        case ADC_CH1:
            data = adc_data1_getf(adc_base);
            break;

        case ADC_CH2:
            data = adc_data2_getf(adc_base);
            break;

        case ADC_CH3:
            data = adc_data3_getf(adc_base);
            break;

        case ADC_CH4:
            data = adc_data4_getf(adc_base);
            break;

        case ADC_CH5:
            data = adc_data5_getf(adc_base);
            break;
    
        case ADC_CH6:
            data = adc_data6_getf(adc_base);
            break;

        case ADC_CH7:
            data = adc_data7_getf(adc_base);
            break;

        default:
            break;
    }
    return data;
}

void hal_adc_it_cfg(uint32_t adc_base,adc_it_flag_t adc_it_flag ,hal_en_t en)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    hal_assert(IS_ADC_IT_FLAG_VALUE(adc_it_flag));
    hal_assert(IS_FUNCTIONAL_STATE(en));
    switch (adc_it_flag)
    {
        case ADC_IT_FLAG_AWD:
        {
            if (en == HAL_DISABLE) {
                adc_awdie_setf(adc_base,0);
            }
            else if (en == HAL_DISABLE) {
                adc_awdie_setf(adc_base,1);
            }   
            break;
        }
        case ADC_IT_FLAG_OVR:
        {
            if (en == HAL_DISABLE) {
                adc_ovrie_setf(adc_base,0);
            }
            else if (en == HAL_DISABLE) {
                adc_ovrie_setf(adc_base,1);
            }   
            break;
        }
        case ADC_IT_FLAG_EOS:
        {
            if (en == HAL_DISABLE) {
                adc_eosie_setf(adc_base,0);
            }
            else if (en == HAL_DISABLE) {
                adc_eosie_setf(adc_base,1);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_7:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 7);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 7);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_6:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 6);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 6);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_5:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 5);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 5);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_4:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 4);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 4);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_3:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 3);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 3);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_2:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 2);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 2);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_1:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 1);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 1);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
        case ADC_IT_FLAG_EOC_0:
        {
            if (en == HAL_DISABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp &= ~(1 << 0);
                adc_eocie_setf(adc_base,reg_tmp);
            }
            else if (en == HAL_ENABLE) {
                uint8_t reg_tmp = 0;
                reg_tmp = adc_eocie_getf(adc_base);
                reg_tmp |= (1 << 0);
                adc_eocie_setf(adc_base,reg_tmp);
            }   
            break;
        }
            
        default:
            break;
    }

}

uint8_t hal_adc_get_it_flag(uint32_t adc_base,adc_it_flag_t adc_it_flag)
{
    uint8_t it_flag = 0;
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    hal_assert(IS_ADC_IT_FLAG_VALUE(adc_it_flag));

    switch (adc_it_flag)
    {
        case ADC_IT_FLAG_AWD:
            it_flag = adc_awd_getf(adc_base);
            break;

        case ADC_IT_FLAG_OVR:
            it_flag = adc_ovr_getf(adc_base);
            break;

        case ADC_IT_FLAG_EOS:
            it_flag = adc_eos_getf(adc_base);
            break;

        case ADC_IT_FLAG_EOC_7:
            it_flag = adc_eoc_7_getf(adc_base);
            break;
            
        case ADC_IT_FLAG_EOC_6:
            it_flag = adc_eoc_6_getf(adc_base);
            break;
            
        case ADC_IT_FLAG_EOC_5:
            it_flag = adc_eoc_5_getf(adc_base);
            break;
            
        case ADC_IT_FLAG_EOC_4:
            it_flag = adc_eoc_4_getf(adc_base);
            break;
            
        case ADC_IT_FLAG_EOC_3:
            it_flag = adc_eoc_3_getf(adc_base);
            break;
            
        case ADC_IT_FLAG_EOC_2:
            it_flag = adc_eoc_2_getf(adc_base);
            break;
            
        case ADC_IT_FLAG_EOC_1:
            it_flag = adc_eoc_1_getf(adc_base);
            break;
            
        case ADC_IT_FLAG_EOC_0:
            it_flag = adc_eoc_0_getf(adc_base);
            break;
            
        default:
            break;
    }
    return it_flag;
}

void hal_adc_clr_it_flag(uint32_t adc_base,adc_it_flag_t adc_it_flag)
{
    /* check the parameters */
    hal_assert(IS_ADC_ALL_PERIPH(adc_base));
    hal_assert(IS_ADC_IT_FLAG_VALUE(adc_it_flag));

    switch (adc_it_flag)
    {
        case ADC_IT_FLAG_AWD:
            adc_awd_setf(adc_base,1);
            break;

        case ADC_IT_FLAG_OVR:
            adc_ovr_setf(adc_base,1);
            break;

        case ADC_IT_FLAG_EOS:
            adc_eos_setf(adc_base,1);
            break;

        case ADC_IT_FLAG_EOC_7:
            adc_eoc_7_setf(adc_base,1);
            break;
            
        case ADC_IT_FLAG_EOC_6:
            adc_eoc_6_setf(adc_base,1);
            break;
            
        case ADC_IT_FLAG_EOC_5:
            adc_eoc_5_setf(adc_base,1);
            break;
            
        case ADC_IT_FLAG_EOC_4:
            adc_eoc_4_setf(adc_base,1);
            break;
            
        case ADC_IT_FLAG_EOC_3:
            adc_eoc_3_setf(adc_base,1);
            break;
            
        case ADC_IT_FLAG_EOC_2:
            adc_eoc_2_setf(adc_base,1);
            break;
            
        case ADC_IT_FLAG_EOC_1:
            adc_eoc_1_setf(adc_base,1);
            break;
            
        case ADC_IT_FLAG_EOC_0:
            adc_eoc_0_setf(adc_base,1);
            break;
            
        default:
            break;
    }
}
