#ifndef __USR_APP_H__
#define __USR_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

//usr config  these 4 macros
#define SLAVE       1   //SLAVE  set 1, will start an advertising
#define MASTER      0   //MASTER set 1, will start a direct connectable initiating requst 

#define SERVICE     1   // provide data device ,for example mouse .keyboard ,sensor
#define CLIENT      0	// get data device,for example PC, mobile phone

#define BLE_PAIR_MODE

typedef enum 
{
    NET_CONFIG_INIT          = 0,
    NET_CONFIG_GET_SSID      = 1,
    NET_CONFIG_GET_PWD       = 2,
}net_config_state_t;

typedef enum
{
    NET_CONFIG_MODE = 1,
    NORMAL_MODE     = 2,
    ERROR_MODE      = 3,
}app_mode_t;



void creat_usr_app_task(void);



#ifdef __cplusplus
}
#endif

#endif /* __USR_APP_H__ */


