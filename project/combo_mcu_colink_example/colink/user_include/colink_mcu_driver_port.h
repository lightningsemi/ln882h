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

#ifndef __COLINK_MCU_DRIVER_PORT_H__
#define __COLINK_MCU_DRIVER_PORT_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "ln_types.h"
#include "hal/hal_gpio.h"
#include "colink_define.h"
#include "usr_ctrl.h"

enum {
	POWERON_SWITCH_RESET 	= 0,
	POWERON_SWITCH_ON,
};

typedef struct
{
	uint32_t timep;
	
	void (*init)(void);
	void (*switch_on)(void);
	void (*switch_off)(void);
	
	void (*cloud_indicate_led_on)(void);
	void (*cloud_indicate_led_off)(void);
	void (*ap_indicate_led_on)(void);
	void (*ap_indicate_led_off)(void);
	
	void (*reset)(void);
	void (*reset_factory)(void);
	void (*updat_timep)(char* _time);
	uint32_t (*get_timep)(void);
	
	bool (*check_reset)(const uint8_t _time);
 
}COLINK_MCU_DRIVER_ADAPTER;

/**
 * GPIOA0 related definitions.
 */
#define SWITCH_INPUT_GPIO           (GPIO_PIN_5)

/**
 * Small lamp status macro definition.
 */
#define SWITCH_ON       (0x1)
#define SWITCH_OFF      (0x0)

/**
 * GPIOB5 related definitions.
 */
#define SWITCH_OUTPUT_GPIO          (GPIO_PIN_12)

#define CLOUDE_INDICATE_LED         (GPIO_PIN_3)
#define AP_INDICATE_LED         (GPIO_PIN_4)

extern COLINK_MCU_DRIVER_ADAPTER* get_colink_mcu_handle(void);

static void ln_cloud_indicate_led_on(void);
static void ln_cloud_indicate_led_off(void);
static void ln_ap_indicate_led_on(void);
static void ln_ap_indicate_led_off(void);

#endif

