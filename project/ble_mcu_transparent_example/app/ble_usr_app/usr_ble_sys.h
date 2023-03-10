#ifndef _USR_BLE_SYS_H_
#define _USR_BLE_SYS_H_

#include <stdint.h>
#include <stdbool.h>

#include "ln_kv_api.h"


#define KV_SYSPARAM_BLE		1

#ifdef KV_SYSPARAM_BLE
#define KV_SYSPARAM_BLE_ROLE           			((const char *)"25_ble_role")
#define KV_SYSPARAM_BLE_NAME								((const char *)"26_ble_name")

#endif

#define DEVICE_NAME_DEFAULT                  ("LKBM001")
#define DEVICE_NAME_LEN              (sizeof(DEVICE_NAME_DEFAULT))

#define BLE_ROLE_SLAVE		0
#define BLE_ROLE_MASTER		1

#define BLE_NAME_MAX_LEN	20

#define BLE_MODULE_GPIO_BASE		GPIOB_BASE
#define BLE_MODULE_GPIO_0				GPIO_PIN_3
#define BLE_MODULE_GPIO_1				GPIO_PIN_4
#define BLE_MODULE_GPIO_2				GPIO_PIN_5
#define BLE_MODULE_GPIO_3				GPIO_PIN_6

enum{
    BLE_SYSPARAM_ERR_NONE          = 0,
    BLE_SYSPARAM_ERR_INVALID_PARAM = 1,
    BLE_SYSPARAM_ERR_STORE         = 2,
    BLE_SYSPARAM_ERR_LOAD          = 3,
    BLE_SYSPARAM_ERR_RESET_ALL     = 4,
};

typedef struct {
    uint8_t ble_name_len;
    char  ble_name[BLE_NAME_MAX_LEN];
} ble_name_param_t; 

extern uint8_t gpio_init_flag;
extern uint8_t ble_mac[6];

struct ble_local_phy
{
	uint8_t tx_phy;
	uint8_t rx_phy;
};

extern struct ble_local_phy usr_ble_local_phy;

uint8_t check_master_slave(uint8_t *role);
void hex_to_asciistring(uint8_t* str,uint32_t size,uint8_t* str1);
int check_sysparam_ble_name(ble_name_param_t *p_param);
int ble_param_factory_reset_all(void);

#endif
