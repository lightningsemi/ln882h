/**
 * @file style.c
 *
 * Show C source code style.
 * This is detail description of style.c.
 *
 * @author wason.wang
 * @date 2018.06.19
 * @copyright
 * Copyright (C) 2013-2014 ITEAD Intelligent Systems Co., Ltd. \n
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 */

#include "colink_mcu_driver_port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "json/cJSON.h"
#include "colink_sysadapter.h"
#include "colink_profile.h"
#include "colink_type.h"
#include "colink_global.h"
#include "colink_network.h"

#include "usr_ctrl.h"

#include "hal/hal_misc.h"
#include "hal/hal_wdt.h"
#include "ln_kv_api.h"

static void ln_mcu_init(void);
static void ln_switch_on(void);
static void ln_switch_off(void);
static void ln_reset_factory(void);
static void ln_reset(void);
static void ln_update_timep(char* _time);
static uint32_t ln_get_timep(void);
static bool ln_is_poweron_reset_xtimes(const uint8_t _time);

static COLINK_MCU_DRIVER_ADAPTER s_colinkMcuDrvAdapter = {
	.timep = 0,
	
	.init = ln_mcu_init,
	.switch_on = ln_switch_on,
	.switch_off = ln_switch_off,
	
	.cloud_indicate_led_on=ln_cloud_indicate_led_on,
	.cloud_indicate_led_off=ln_cloud_indicate_led_off,
	.ap_indicate_led_on=ln_ap_indicate_led_on,
	.ap_indicate_led_off=ln_ap_indicate_led_off,
	
	.reset = ln_reset,
	.reset_factory = ln_reset_factory,
	.updat_timep = ln_update_timep,
	.get_timep = ln_get_timep,
	
	.check_reset = ln_is_poweron_reset_xtimes,
};

COLINK_MCU_DRIVER_ADAPTER* get_colink_mcu_handle()
{
	return &s_colinkMcuDrvAdapter;
}

/**
 * Key scan task timer.
 */
static OS_Timer_t s_poweronSwitchScanTmr;
static OS_Timer_t s_keyScanTmr;
static OS_Timer_t s_ledScanTmr;
static OS_Timer_t s_timepTmr;

/**
 * Lights off the status flag.
 */
static uint8_t switch_state = SWITCH_OFF;

/*
* Add local ap indicate & cloud link status indicate
*/
static uint8_t cloud_indicate_led_flag_state = SWITCH_OFF;
static uint8_t ap_indicate_led_flag_state = SWITCH_OFF;

/*
*
*/
static void ln_cloud_indicate_led_on(void)
{
	hal_gpio_pin_reset(GPIOB_BASE, CLOUDE_INDICATE_LED);
	cloud_indicate_led_flag_state = SWITCH_ON;
}

static void ln_cloud_indicate_led_off(void)
{
	hal_gpio_pin_set(GPIOB_BASE, CLOUDE_INDICATE_LED);
	cloud_indicate_led_flag_state = SWITCH_OFF;
}

static void ln_ap_indicate_led_on(void)
{
	hal_gpio_pin_reset(GPIOB_BASE, AP_INDICATE_LED);
	cloud_indicate_led_flag_state = SWITCH_ON;
}

static void ln_ap_indicate_led_off(void)
{
	hal_gpio_pin_set(GPIOB_BASE, AP_INDICATE_LED);
	cloud_indicate_led_flag_state = SWITCH_OFF;
}


/**
 * Turn on the small lamp processing function.
 * Turn on the lights and change the variables that represent the status of the lights.
 * @param [in] void.
 * @return void.
 *
 * @author wason.wang
 * @date 2018.06.19
 */
static void ln_switch_on(void)
{
    hal_gpio_pin_reset(GPIOA_BASE, SWITCH_OUTPUT_GPIO);
    switch_state = SWITCH_ON;
}

/**
 * Turn off the small lamp processing function.
 * Turn off the lights and change the variables that represent the status of the lights.
 * @param [in] void.
 * @return void.
 *
 * @author wason.wang
 * @date 2018.06.19
 */
static void ln_switch_off(void)
{
    hal_gpio_pin_set(GPIOA_BASE, SWITCH_OUTPUT_GPIO);
    switch_state = SWITCH_OFF;
}

/**
 * Report the status of the small light to the server.
 * Report the status of the small lamp to the server according to the agreement 
 * of the company.The format is using json.
 *
 * @param [in] switch_flag - Small lamp status parameters.
 * @return void.
 *
 * @author wason.wang
 * @date 2018.06.19
 */
static void switch_update(uint8_t switch_flag)
{
    cJSON* params = NULL;
    char* raw = NULL;

	params = cJSON_CreateObject();
    cJSON_AddStringToObject(params, "switch", switch_flag ? "on" : "off");

    raw = cJSON_PrintUnformatted(params);

    if (!raw)
    {
        os_printf("cJSON_PrintUnformatted failed");
        cJSON_Delete(params);
        return;
    }

    colinkSendUpdate(raw); 
    cJSON_Delete(params);
    cJSON_free(raw);
}

static void ln_reset(void)
{
	Log_d("ready to reset!");
	hal_misc_awo_set_srstn_all(0);
}

static void ln_reset_factory(void)
{
    Log_i("LN Reset factory!!");
	ln_kv_del(DEVICE_CONFIG_PARA);

    OS_MsDelay(300);

	//reset 
	ln_reset();
}

static void ln_scan_key_tmr_cb(void *arg)
{
    static int32_t _last_key_state = -1;
    static uint32_t count = 0;
    uint32_t key_state = 0;

    key_state = hal_gpio_pin_read(GPIOB_BASE, SWITCH_INPUT_GPIO); /**< Get the level of the button*/

    if(-1 == _last_key_state && 0 == key_state)
    {
        return ;
    }

    if(count > 50)
    {
        if (1 == key_state && 0 == _last_key_state) 
        {
            Log_i("long press up");
        }
    }
    else if(count == 50)
    {
        Log_i("long press");
        get_colink_mcu_handle()->reset_factory();
    }
    else
    {
        if (1 == key_state && 0 == _last_key_state) 
        {
            Log_i("short press up");

            if(SWITCH_ON == switch_state)
            {
                Log_i("Switch OFF!");
                get_colink_mcu_handle()->switch_off();
            }
            else
            {
                Log_i("Switch ON!");
                get_colink_mcu_handle()->switch_on();
            }
			
			if (WIFI_MODE_STATION == wifi_current_mode_get() 
				&& DEVICE_MODE_WORK_NORMAL == coLinkGetDeviceMode())
            {
			    switch_update(switch_state);
            }
						
        }
    }
		
    if(0 == key_state)
    {
        count++;
    }
    else
    {
        count = 0;
    }
    
    _last_key_state = key_state;
}

static void ln_scan_led_tmr_cb(void *arg)
{
	static uint8_t cnt = 0;
	uint8_t thld = 1;
	switch(coLinkGetDeviceMode())
	{
		case DEVICE_MODE_SETTING:
			thld = 1;
			break;
		case DEVICE_MODE_SETTING_SELFAP:
			thld = 4;
			break;
		case DEVICE_MODE_WORK_NORMAL:
			Log_i("connect cloud successfully, then stop led timer");
			get_colink_mcu_handle()->switch_off();
			OS_TimerStop(&s_ledScanTmr);
			return;
		default:
			return;
	}
	
	if (++cnt >= thld)
	{
		cnt  = 0;
		if (switch_state == SWITCH_ON)
		{
			get_colink_mcu_handle()->switch_off();
		}
		else
		{
			get_colink_mcu_handle()->switch_on();
		}
	}
}

static void ln_scan_timep_tmr_cb(void *arg)
{
	if (get_colink_mcu_handle()->timep == 0)
	{
		Log_i("wait for timestamp init ... ");
		return;
	}
	
	get_colink_mcu_handle()->timep += 5;
}

static void ln_scan_poweron_switch_tmr_cb(void *arg)
{
	uint8_t poweronResetFlag = POWERON_SWITCH_ON;
	int kvret = ln_kv_set((const char *)DEVICE_POWERON_RESET_WITHIN_3S, (void *)(&poweronResetFlag), sizeof(poweronResetFlag));
	if (KV_ERR_NONE != kvret)
	{
		Log_e("error ln_kv_set DEVICE_POWERON_RESET_WITHIN_3S with return %d", kvret);
	}
	else{
		Log_i("set DEVICE_POWERON_RESET_WITHIN_3S ON");
	}
}

static void ln_start_tmr(OS_Timer_t *_tmr, OS_TimerType type, OS_TimerCallback_t cb, void *arg, uint32_t periodMS)
{
	if (OS_OK != OS_TimerCreate(_tmr, type, cb, arg, periodMS))
	{
		Log_e("OS_TimerCreate fail!");
		while(1);
	}
	
	if (OS_OK != OS_TimerStart(_tmr))
	{
		Log_e("OS_TimerStart fail!");
		while(1);
	}
}

static void ln_start_key_scan(void)
{
	Log_i("start %s timer ... ", __func__);
	ln_start_tmr(&s_keyScanTmr, OS_TIMER_PERIODIC, ln_scan_key_tmr_cb, NULL, 100);
	Log_i("start %s timer ... suc!", __func__);
}

static void ln_start_led_scan(void)
{
	Log_i("start %s timer ... ", __func__);
	ln_start_tmr(&s_ledScanTmr, OS_TIMER_PERIODIC, ln_scan_led_tmr_cb, NULL, 500);
	Log_i("start %s timer ... suc!", __func__);
}

static void ln_start_timep_scan(void)
{
	if (get_colink_mcu_handle()->get_timep() > 0)
	{
		Log_w("timep tmr has already started!");
		return;
	}
	Log_i("start %s timer ... ", __func__);
	ln_start_tmr(&s_timepTmr, OS_TIMER_PERIODIC, ln_scan_timep_tmr_cb, NULL, 5000);
	Log_i("start %s timer ... suc!", __func__);
}

static void ln_start_poweron_switch_scan(void)
{	
	uint8_t poweronResetFlag = POWERON_SWITCH_RESET;
	int kvret = ln_kv_set((const char *)DEVICE_POWERON_RESET_WITHIN_3S, (void *)(&poweronResetFlag), sizeof(poweronResetFlag));
	if (KV_ERR_NONE != kvret)
	{
		Log_e("error ln_kv_set DEVICE_POWERON_RESET_WITHIN_1S with return %d", kvret);
	}
	else{
		Log_i("set DEVICE_POWERON_RESET_WITHIN_3S RESET");
	}
	
	Log_i("start %s timer ... ", __func__);
	ln_start_tmr(&s_poweronSwitchScanTmr, OS_TIMER_ONCE, ln_scan_poweron_switch_tmr_cb, NULL, 3000);
	Log_i("start %s timer ... suc!", __func__);	
}

static bool ln_is_poweron_reset_xtimes(const uint8_t _time)
{
	size_t r_len = 0;
	uint8_t poweronCnt = 0;
	uint8_t poweronResetFlag = POWERON_SWITCH_ON;
	
	//get poweronCnt
	if (!ln_kv_has_key(DEVICE_POWERON_SWITCH_COUNT))
	{
		poweronCnt = 0;
		Log_w("cannot find key:DEVICE_POWERON_SWITCH_COUNT, so init with 0");
		int kvret = ln_kv_set((const char *)DEVICE_POWERON_SWITCH_COUNT, (void *)(&poweronCnt), sizeof(poweronCnt));
		if (KV_ERR_NONE != kvret)
		{
			Log_e("error ln_kv_set DEVICE_POWERON_SWITCH_COUNT with return %d", kvret);
		}
	}
	
	if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_POWERON_SWITCH_COUNT, (void *)(&poweronCnt), sizeof(poweronCnt), &r_len)) {
		Log_e("ln_kv_get DEVICE_POWERON_SWITCH_COUNT error, goto default mode");
		goto EXIT_FALSE;
	}
	Log_i("read DEVICE_POWERON_SWITCH_COUNT : %d", poweronCnt);
	
	//check poweron reset flag
	if (!ln_kv_has_key(DEVICE_POWERON_RESET_WITHIN_3S))
	{
		Log_w("cannot find key:DEVICE_POWERON_RESET_WITHIN_3S");
		goto EXIT_FALSE;
	}
	
	poweronResetFlag = POWERON_SWITCH_ON;
	if (KV_ERR_NONE != ln_kv_get((const char *)DEVICE_POWERON_RESET_WITHIN_3S, (void *)(&poweronResetFlag), sizeof(poweronResetFlag), &r_len)) {
		Log_e("ln_kv_get DEVICE_POWERON_RESET_WITHIN_3S error, goto default mode");
		goto EXIT_FALSE;
	}
	
	if (POWERON_SWITCH_RESET == poweronResetFlag)
	{
		Log_i("POWERON_SWITCH_RESET detected!");
		if (++poweronCnt >= _time)
		{
			poweronCnt = 0;
			Log_i("Power on reset exceeds %d times!", _time);
			Log_i("DEVICE_POWERON_SWITCH_COUNT clear!");
			int kvret = ln_kv_set((const char *)DEVICE_POWERON_SWITCH_COUNT, (void *)(&poweronCnt), sizeof(poweronCnt));
			if (KV_ERR_NONE != kvret)
			{
				Log_e("error ln_kv_set DEVICE_POWERON_SWITCH_COUNT with return %d", kvret);
			}
			
			goto EXIT_TRUE;
		}
		else
		{
			int kvret = ln_kv_set((const char *)DEVICE_POWERON_SWITCH_COUNT, (void *)(&poweronCnt), sizeof(poweronCnt));
			if (KV_ERR_NONE != kvret)
			{
				Log_e("error ln_kv_set DEVICE_POWERON_SWITCH_COUNT with return %d", kvret);
			}
		}
	}
	else
	{
		//clear DEVICE_POWERON_SWITCH_COUNT
		if (poweronCnt > 0)
		{
			poweronCnt = 0;
			Log_i("DEVICE_POWERON_SWITCH_COUNT clear!");
			int kvret = ln_kv_set((const char *)DEVICE_POWERON_SWITCH_COUNT, (void *)(&poweronCnt), sizeof(poweronCnt));
			if (KV_ERR_NONE != kvret)
			{
				Log_e("error ln_kv_set DEVICE_POWERON_SWITCH_COUNT with return %d", kvret);
			}	
		}	
	}
	
EXIT_FALSE:
	ln_start_poweron_switch_scan();
	return false;
	
EXIT_TRUE:
	Log_i("del DEVICE_CONFIG_PARA");
	ln_kv_del(DEVICE_CONFIG_PARA);
	ln_start_poweron_switch_scan();
	return true;
}

static uint32_t ln_get_timep(void)
{
	return get_colink_mcu_handle()->timep;
}

static void ln_update_timep(char* _time)
{
	char * pch;
	struct tm p; 
	
	Log_i("UTC: %s", _time);
	
	//parse time
	pch = strtok(_time,"ZT.-:");
	p.tm_year = atoi(pch) - 1900;
	
	pch = strtok(NULL, "ZT.-:");
	p.tm_mon = atoi(pch) - 1;
	
	pch = strtok(NULL, "ZT.-:");
	p.tm_mday = atoi(pch);	
	
	pch = strtok(NULL, "ZT.-:");
	p.tm_hour = atoi(pch);

	pch = strtok(NULL, "ZT.-:");
	p.tm_min = atoi(pch);

	pch = strtok(NULL, "ZT.-:");
	p.tm_sec = atoi(pch);	
	
	//start timep scan
	ln_start_timep_scan();
	
	//update timep
	get_colink_mcu_handle()->timep = (uint32_t)mktime(&p);
	Log_i("update timep: %d",get_colink_mcu_handle()->timep);
}

static void ln_wdt_init(void)
{
    /* 看门狗初始化 */
    wdt_init_t_def wdt_init;
    memset(&wdt_init,0,sizeof(wdt_init));
    wdt_init.wdt_rmod = WDT_RMOD_1;         //等于0的时候，计数器溢出时直接复位，等于1的时候，先产生中断，如果再次溢出，则产生复位。
    wdt_init.wdt_rpl = WDT_RPL_32_PCLK;     //设置复位延时的时间
    wdt_init.top = WDT_TOP_VALUE_7;         //设置看门狗计数器的值,当TOP=1时，对应计数器的值为0x1FF，而看门狗是用的时钟是一个单独的32k时钟，
                                            //所以此时的喂狗时间必须在 (1/32k) * 0x1FF 内。
    hal_wdt_init(WDT_BASE,&wdt_init);
    
    /* 配置看门狗中断 */
    NVIC_SetPriority(WDT_IRQn,     4);
    NVIC_EnableIRQ(WDT_IRQn);
    
    /* 使能看门狗 */
    hal_wdt_en(WDT_BASE,HAL_ENABLE);
    
    /* 先喂下狗 */
    hal_wdt_cnt_restart(WDT_BASE);
}

void WDT_IRQHandler()
{
    //注：WDT的寄存器时钟使用的32K，而CPU时钟160M，远远大于32K，所以可能出现清完WDT中断标志位后再次进入WDT中断的情形（WDT 32k操作寄存器比较慢）。
    hal_wdt_cnt_restart(WDT_BASE);              //喂狗操作
    //Log_d("feed dog~!");        //LOG打印
}

static void ln_mcu_init(void)
{	
	//init timep
	get_colink_mcu_handle()->timep = 0;
	
	//init gpio input
    gpio_init_t_def InputConfig;
	InputConfig.dir      = GPIO_INPUT;
	InputConfig.pin   = SWITCH_INPUT_GPIO;                  
    InputConfig.speed = GPIO_HIGH_SPEED;             
    InputConfig.mode  = GPIO_MODE_DIGITAL;           
    InputConfig.pull  = GPIO_PULL_UP;               
    hal_gpio_init(GPIOB_BASE,&InputConfig); 
	
	//init gpio output
    gpio_init_t_def OutputConfig;
    OutputConfig.dir = GPIO_OUTPUT;
	OutputConfig.pin   = SWITCH_OUTPUT_GPIO;                  
    OutputConfig.speed = GPIO_HIGH_SPEED;             
    OutputConfig.mode  = GPIO_MODE_DIGITAL;           
    OutputConfig.pull  = GPIO_PULL_DOWN;               
    hal_gpio_init(GPIOA_BASE,&OutputConfig);   

	//B3 --> 0 
	gpio_init_t_def B3Config;
    B3Config.dir = GPIO_OUTPUT;
	B3Config.pin   = GPIO_PIN_3|GPIO_PIN_4;                  
    B3Config.speed = GPIO_HIGH_SPEED;             
    B3Config.mode  = GPIO_MODE_DIGITAL;           
    B3Config.pull  = GPIO_PULL_DOWN;   
    hal_gpio_init(GPIOB_BASE,&B3Config);  
	hal_gpio_pin_set(GPIOB_BASE, GPIO_PIN_3);
	hal_gpio_pin_set(GPIOB_BASE, GPIO_PIN_4);
//	gpio_init_t_def B4Config;
//    B3Config.dir = GPIO_OUTPUT;
//	B3Config.pin   = GPIO_PIN_4;                  
//    B3Config.speed = GPIO_HIGH_SPEED;             
//    B3Config.mode  = GPIO_MODE_DIGITAL;           
//    B3Config.pull  = GPIO_PULL_UP;   
//    hal_gpio_init(GPIOB_BASE,&B4Config);  
//	hal_gpio_pin_set(GPIOB_BASE, GPIO_PIN_4);
	
	//init tmr
    ln_start_key_scan();
	ln_start_led_scan();
	
	//init wdt
	ln_wdt_init();
	
	//set colink mode
	coLinkSetDeviceMode(DEVICE_MODE_START);
} 
