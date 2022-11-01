#ifndef _LN_AT_CMD_BLE_H_
#define _LN_AT_CMD_BLE_H_ 


#include <stdint.h>
#include <stdbool.h>

#include "osal/osal.h"
#include "usr_ble_sys.h"
#include "hal/hal_gpio.h"

#define LN_AT_BLE_RET_OK_STR                        ("OK")

#define LN_AT_BLE_CMD_ONLY_SUPPORT_CONNECTED_STR					"\r\n301\r\n"
#define LN_AT_BLE_CMD_ONLY_SUPPORT_DISCONNECTED_STR				"\r\n300\r\n"
#define LN_AT_BLE_PARA_RANGE_INVALID_STR									"\r\n201\r\n"
#define LN_AT_BLE_GENERAL_PARA_INVALID_STR		    				"\r\n202\r\n"
#define LN_AT_BLE_PARA_NUM_ERR_STR												"\r\n200\r\n"
#define LN_AT_BLE_MASTER_RET_ERR_STR   										"\r\n600\r\n"
#define LN_AT_BLE_SLAVE_RET_ERR_STR    										"\r\n601\r\n"

struct gatt_notify_data
{
	uint8_t data_len;
	uint8_t data[200];
};

extern OS_Semaphore_t app_env_info_sem;

void Release_app_env_sem(void);

#endif
