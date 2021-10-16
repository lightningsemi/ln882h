#include "hal/hal_adc.h"
#include "drv_adc_measure.h"

void drv_adc_init(void)
{
    adc_init_t_def adc_init;

    memset(&adc_init, 0, sizeof(adc_init_t_def));
    adc_init.adc_ch        = ADC_CH0;
    adc_init.adc_conv_mode = ADC_CONV_MODE_CONTINUE;
    adc_init.adc_presc     = 80;                             
    hal_adc_init(ADC_BASE,&adc_init);
        
    hal_adc_en(ADC_BASE,HAL_ENABLE);
    
    hal_adc_start_conv(ADC_BASE);
}

uint16_t drv_adc_read(uint8_t ch)
{
    uint16_t read_adc = 0;

    for(volatile uint32_t i = 0; i < 40000; i++){};
    
    read_adc = hal_adc_get_data(ADC_BASE, (adc_ch_t)ch);
    
    return read_adc;
}


