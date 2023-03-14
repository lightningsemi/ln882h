
#include "pulse_cap.h"
#include "pwm_port.h"

extern void pulse_cap_init(uint32_t gpio_base, gpio_pin_t pin);
extern void pulse_cap_start(uint32_t *time_arr,uint32_t len);
extern void pulse_cap_stop(void);
extern PULSE_CAP_STA pulse_cap_get_result(void);

uint32_t g_pwm_cap_result[PWM_CAP_CNT_MAX]={0};
pwm_cap_t g_pwm_data[PWM_CAP_DATA_LEN] = {0};

//����GPIOB pin5 ΪPWM�����,Ƶ��Ϊ1000Hz,ռ�ձ�Ϊ20%��PWMͨ��Ϊchannel 1
//����PWM�����λΪGPIOA pin9
void app_pwm(void)
{
	pulse_cap_init(GPIOA_BASE,GPIO_PIN_9);
	pulse_cap_start(g_pwm_cap_result,PWM_CAP_CNT_MAX);
	
	//�û�������������Ƶ�ʺ�ռ�ձ�;
	//Ĭ��Ƶ��1000,��ΧΪ1~38KHz,
	//Ĭ��ռ�ձ�20,��ΧΪ1~100,
	pwm_init(1000,20,PWM_CHA_1,GPIO_B,GPIO_PIN_5);
	
	pwm_start(PWM_CHA_1);
	LOG(LOG_LVL_INFO, "pwm start!please connect GPIOB 5 to GPIOA 9!\r\n");
	while(pulse_cap_get_result() != PULSE_CAP_SUCESS);

	pulse_cap_stop();
	
	pwm_cap_data_handle();

}

void pwm_cap_data_handle(void)
{
	uint32_t freq = 0;
	float duty = 0;
	memset(g_pwm_data,0,sizeof(pwm_cap_t)*PWM_CAP_DATA_LEN);
	
	for(uint16_t i = 0;i < PWM_CAP_DATA_LEN ; i++)
	{
		duty = (float)(100 * g_pwm_cap_result[2*(i+1)] / (g_pwm_cap_result[2*(i+1)+1]+g_pwm_cap_result[2*(i+1)]));
		freq = (TIME_T / (g_pwm_cap_result[2*(i+1)] +  g_pwm_cap_result[2*(i+1)+1]));
		g_pwm_data[i].duty = duty;
		g_pwm_data[i].freq = freq;
		LOG(LOG_LVL_ERROR,"PWM_CAP_DATA[%d]::duty = %01f , freq = %d\r\n",i ,g_pwm_data[i].duty , g_pwm_data[i].freq);
	}
	memset(g_pwm_cap_result,0,sizeof(uint32_t)*PWM_CAP_CNT_MAX);
}