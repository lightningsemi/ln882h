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


#include "hal_adc.h"
#include "hal_timer.h"
#include "hal_clock.h"
#include "hal_adv_timer.h"

static unsigned char tx_data[100];              //测试BUFFER
static unsigned char rx_data[100];
static unsigned char sensor_data[6];
static unsigned int  err_cnt = 0;

static volatile double        tem_data = 0;     //温度数据
static volatile double        hum_data = 0;     //湿度数据


static unsigned int address[10];
static unsigned int address_num = 0;

static unsigned int light_value = 0;

static unsigned char status[2];


int main (int argc, char* argv[])
{  
    /* 1. 系统初始化 */
    SetSysClock();
    
    log_init();
    
    /* 2. I2C测试 */
    
    ln_i2c_init();              
    
    /* 2.1 开始扫描I2C地址 */
    LOG(LOG_LVL_INFO,"ln8620 i2c scan start! \n");
    
    hal_i2c_address_scan(I2C_BASE);
    
    if(address_num == 0)
    {
        LOG(LOG_LVL_INFO,"There is no device on I2C bus! \n");
    }
    else
    {
        for(int i = address_num; i > 0; i--)
        {
            LOG(LOG_LVL_INFO,"Found device %d,address : 0x%x! \n",address_num - i,address[address_num - i]);
        }
        LOG(LOG_LVL_INFO,"Scan end. \n");
    }
    
    for(int i = 0; i < 100; i++)
    {
        tx_data[i] = i;
    }
    
    /* 2.2 开始读写AT24C04 */
    LOG(LOG_LVL_INFO,"ln8620 i2c + 24c04 test start! \n");
    
    hal_24c04_write(256,tx_data,100);
    hal_24c04_read(256,rx_data,100);
    
    for(int i = 0; i < 100; i++)
    {   
        if(tx_data[i] != rx_data[i])
        {
            err_cnt++;
        }
    }
    if(err_cnt != 0)
        LOG(LOG_LVL_INFO,"ln8620 i2c + 24c04 test fail! \n");
    else
        LOG(LOG_LVL_INFO,"ln8620 i2c + 24c04 test success! \n");
    
    LOG(LOG_LVL_INFO,"ln8620 i2c + oled test start! \n");
    
    /* 2.3 初始化OLED显示屏 */
    gpio_init_t gpio_init;
    memset(&gpio_init,0,sizeof(gpio_init));        //清零结构体
    gpio_init.dir = GPIO_OUTPUT;                   //配置GPIO方向，输入或者输出
    gpio_init.pin = GPIO_PIN_4;                    //配置GPIO引脚号
    gpio_init.speed = GPIO_HIGH_SPEED;             //设置GPIO速度
    hal_gpio_init(GPIOB_BASE,&gpio_init);          //初始化GPIO
    hal_gpio_pin_reset(GPIOB_BASE,GPIO_PIN_4);     //RESET OLED屏幕
    ln_delay_ms(100);
    hal_gpio_pin_set(GPIOB_BASE,GPIO_PIN_4);
    
    ln_oled_init(I2C_BASE);
    ln_oled_display_on();

    ln_oled_show_string(0,40,(unsigned char *)"Test success!",12);      //显示测试成功的英文
    LOG(LOG_LVL_INFO,"ln8620 i2c + oled test success! \n");
    
    ln_oled_refresh();                                                  //刷新显示
    ln_delay_ms(1000);
    ln_oled_clear();
    
    /* 2.4 初始化温湿度传感器 */
    LOG(LOG_LVL_INFO,"ln8620 i2c + sht30 test start! \n");
    ln_tem_hum_init();
    
    /* 3. ADC测试 （采集光敏电阻值）*/
    
    adc_init_t_def adc_init;
    
    memset(&adc_init,0,sizeof(adc_init));
    
    adc_init.adc_ch = ADC_CH5;                              //配置通道
    adc_init.adc_conv_mode = ADC_CONV_MODE_CONTINUE;        //配置ADC为连续转换模式
    adc_init.adc_presc = 40;                             
    hal_adc_init(ADC_BASE,&adc_init);                       //初始化ADC
    
    hal_gpio_pin_mode_set(GPIOB_BASE,GPIO_PIN_3,GPIO_MODE_ANALOG);      //配置PB3为模拟引脚
    hal_adc_it_cfg(ADC_BASE,ADC_IT_FLAG_EOC_5,HAL_ENABLE);            
    
    hal_adc_en(ADC_BASE,HAL_ENABLE);
    hal_adc_start_conv(ADC_BASE);
    

    /* 4. SPI Flash测试 */
    ln_spi_master_init();
    
    //初始化数据buffer
    for(int i = 0; i < 100 ; i++)
    {
        tx_data[i] = i + 2;
    }

    LOG(LOG_LVL_INFO,"ln8620 SPI + 25WQ16 test start! \n");

    /********************************SPI直接读写Flash芯片****************************************************************/

    //读取ID
    hal_25wq16_read_id(SPI0_BASE,rx_data);
    LOG(LOG_LVL_INFO,"ln8620 25WQ16 ID: %x %x %x \n",rx_data[0],rx_data[1],rx_data[2]);
    
    //读取状态
    hal_25wq16_read_status(SPI0_BASE,rx_data);

    //写使能
    hal_25wq16_write_enable(SPI0_BASE);

    //读取状态
    hal_25wq16_read_status(SPI0_BASE,rx_data);

    //擦除芯片
    hal_25wq16_erase_flash(SPI0_BASE,0x200);

    //等待擦除完成
    while(1)
    {
        hal_25wq16_read_status(SPI0_BASE,status);
        if((status[0] & 0x01) == 0x00)
            break;
    }

    //向Flash芯片中写入数据
    hal_25wq16_write_flash(SPI0_BASE,0x200,tx_data,100);

    //等待写入完成
    while(1)
    {
        hal_25wq16_read_status(SPI0_BASE,status);
        if((status[0] & 0x01) == 0x00)
            break;
    }

    //从Flash芯片中读出数据
    hal_25wq16_read_flash(SPI0_BASE,0x200,rx_data,100);
    
    //判断写入的数据是否正确
    err_cnt = 0;
    for(int i = 0 ; i < 100; i++)
    {
        if(rx_data[i] != tx_data[i])
        {
            err_cnt++;
        }
    }

    //打印LOG
    if(err_cnt != 0)
    {
        LOG(LOG_LVL_INFO,"ln8620 SPI + 25WQ16 test fail! \n");
    }
    else
    {
        LOG(LOG_LVL_INFO,"ln8620 SPI + 25WQ16 test success! \n");
    }
    
    /* 5. PWM测试 */
    __NVIC_SetPriorityGrouping(4);
    __enable_irq();

    /* pwm 引脚初始化 */
    hal_gpio_afio_select(GPIOA_BASE,GPIO_PIN_8,ADV_TIMER_PWM0);
    hal_gpio_afio_select(GPIOA_BASE,GPIO_PIN_9,ADV_TIMER_PWM1);
    hal_gpio_afio_en(GPIOA_BASE,GPIO_PIN_8,HAL_ENABLE);
    hal_gpio_afio_en(GPIOA_BASE,GPIO_PIN_9,HAL_ENABLE);  

    /* PWM参数初始化 */
    adv_tim_init_t_def adv_tim_init;
    memset(&adv_tim_init,0,sizeof(adv_tim_init));
    adv_tim_init.adv_tim_clk_div = 0;                               //设置时钟分频，0为不分频
    adv_tim_init.adv_tim_load_value =  40000 - 1;                   //设置PWM频率，40000 * 1 / PCLK(80M) / DIV(0) = 2k
    adv_tim_init.adv_tim_cmp_a_value = 40000 ;                      //设置通道a比较值，占空比为 50%
    adv_tim_init.adv_tim_cmp_b_value = 40000 ;                      //设置通道b比较值，占空比为 50%
    adv_tim_init.adv_tim_dead_gap_value = 1000;                     //设置死区时间
    adv_tim_init.adv_tim_dead_en = ADV_TIMER_DEAD_DIS;              //不开启死区
    adv_tim_init.adv_tim_cnt_mode = ADV_TIMER_CNT_MODE_INC;         //向上计数模式
    adv_tim_init.adv_tim_cha_en = ADV_TIMER_CHA_EN;                 //使能通道a
    adv_tim_init.adv_tim_chb_en = ADV_TIMER_CHB_EN;                 //使能通道b
    adv_tim_init.adv_tim_cha_it_mode = ADV_TIMER_CHA_IT_MODE_INC;   //使能通道a向上计数中断
    adv_tim_init.adv_tim_chb_it_mode = ADV_TIMER_CHB_IT_MODE_INC;   //使能通道b向上计数中断
    hal_adv_tim_init(ADV_TIMER_0_BASE,&adv_tim_init);               //初始化ADV_TIMER0

    uint32_t cmp_a_value = 0;       //通道a的比较值
    float    duty_value  = 0;       //占空比
    uint8_t  inc_dir     = 0;       //占空比递增/减方向
    uint32_t pulse_cnt   = 0;       //脉冲计数
    
    while(1)
    {   
        unsigned char display_buffer[16];
        
        if(tem_hum_read_sensor_data(sensor_data) == 0)                     //获得温湿度信息
        {
            
            memset(display_buffer,' ',16);
            tem_data = -45 + (175 * (sensor_data[0] * 256 + sensor_data[1]) * 1.0 / (65535 - 1)) ;      //转换为摄氏度
            hum_data = 100 * ((sensor_data[3] * 256 + sensor_data[4]) * 1.0 / (65535 - 1)) ;            //转换为%

            sprintf((char *)display_buffer,"TEM : %0.2f",tem_data);        //oled显示和串口打印数据
            ln_oled_show_string_with_len(0,32,display_buffer,16,15);      
            memset(display_buffer,' ',16);
            sprintf((char *)display_buffer,"HUM : %0.2f",hum_data);
            ln_oled_show_string_with_len(0,48,display_buffer,16,15);      
        }

        //显示和打印LOG。
        LOG(LOG_LVL_INFO,"Tem = %0.2f  ",tem_data);
        LOG(LOG_LVL_INFO,"Hum = %0.2f%\n",hum_data);
        
        
        
        
        if(hal_adc_get_it_flag(ADC_BASE,ADC_IT_FLAG_EOC_5))
        {
            light_value = hal_adc_get_data(ADC_BASE,ADC_CH5);     
            hal_adc_clr_it_flag(ADC_BASE,ADC_IT_FLAG_EOC_5);
            
            memset(display_buffer,' ',16);
            sprintf((char *)display_buffer,"LIGHT : %d",light_value);
            ln_oled_show_string_with_len(0,0,display_buffer,16,15);  
            LOG(LOG_LVL_INFO,"LIGHT : %d \r\n\r\n",light_value);
        }
        
        //此处代码是光敏电阻和LED灯联动，光照度越低，灯的亮度越高。
        //200是光照最强时候采集的AD值，2000是完全遮挡光敏电阻采集的值
        if(light_value <= 300)
            duty_value = 0.01;
        else if(light_value >= 2000)
            duty_value = 0.99;
        else
            duty_value = (((light_value - 300) * 1.0f) / (2000 - 300));
        hal_adv_tim_set_comp_a(ADV_TIMER_0_BASE, (duty_value) * 40000);
        hal_adv_tim_set_comp_b(ADV_TIMER_0_BASE, (duty_value) * 40000);
       
        
        memset(display_buffer,' ',16);
        sprintf((char *)display_buffer,"PWM : %0.2f",((duty_value)*100));
        ln_oled_show_string_with_len(0,16,display_buffer,16,15);  
        
        
        
        
        ln_oled_refresh();                                                  //刷新OLED显示
        
        ln_delay_ms(30);
    }
    
}


