#ifndef __USR_APP_H__
#define __USR_APP_H__

#include <stdint.h>
#include "stdbool.h"

#define LN_BLE_MAC_ADDR0    0x00    
#define LN_BLE_MAC_ADDR1    0x50    
#define LN_BLE_MAC_ADDR2    0xC2
#define LN_BLE_MAC_ADDR3    0x5E
#define LN_BLE_MAC_ADDR4    0x11
#define LN_BLE_MAC_ADDR5    0x22

void     creat_usr_app_task  (void);
void     ln_load_config_info (void);
uint8_t* ln_get_mac_array    (bool reverse);

#endif /* __USR_APP_H__ */


