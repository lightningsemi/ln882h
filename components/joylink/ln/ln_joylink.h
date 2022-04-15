#ifndef _JOYLINK_LN_H_
#define _JOYLINK_LN_H_

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#include "joylink.h"


typedef enum jl_net_config_state{
    JL_NET_CONFIG_INIT = 0,
    JL_NET_CONFIG_ING  = 1,
    JL_NET_CONFIG_END  = 2,
}_jl_net_config_state_E;



#define LN_VERSON                       JLP_VERSION

#define JL_NET_CONFIG_BLE               0
#define JL_NET_CONFIG_SOFTAP            1


#define LN_NET_CONFIG_MODE              JL_NET_CONFIG_BLE
#define LN_NET_CONFIG_MAX_TIMEOUT		(5*60*1000)
#define LN_UUID                         "AA1AE9" 
#define LN_CLOUD_PUB_KEY                "03C240A8CFFABAD097D1AEDE8BC33134B159CEC9307C5A45FADCABF8019731A684"
#define LN_CLOUD_PRIVATE_KEY            "872DE378AA94F6EDC85A534F128FE6AC37342D1B253C8D6B54BF7AF0C417D44D"
#define LN_MAC                          0x00, 0x50, 0xC2, 0x5E, 0x11, 0x22
#define LN_AP_SSID                      "JDDeng3322"
#define LN_FLASH_BLOCK_SIZE             4096

#define DEVICE_NAME                  ("LN_JL")
#define DEVICE_NAME_LEN              (sizeof(DEVICE_NAME))
#define ADV_DATA_MAX_LENGTH          (28)

/*
joylink_dev_parse_ctrl
joylink_dev_package_info
joylink_dev_user_data_get
joylink_dev_user_data_set
*/


#define LN_USER_DATA_POWER   "Power"

typedef struct _user_dev_status_t {
    int Power;
} user_dev_status_t;


void ln_enter_netconfig_mode(void);
int  ln_check_dev_bind(void); //TODO: 返回值说明


#ifdef __cplusplus
}
#endif

#endif

