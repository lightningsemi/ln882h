#include "joylink_string.h"
#include "joylink_memory.h"
#include "joylink_softap.h"
#include "ln_joylink.h"
/**
 * @brief:set wifi mode to AP mode.
 *
 * @returns:success 0, others failed
 */
int32_t jl_softap_enter_ap_mode(void)
{
    int ret;
    // open wifi whit AP mode
    
    ret = joylink_softap_start(LN_NET_CONFIG_MAX_TIMEOUT);
    return ret;
}


/**
 * @brief:System is expected to get product information that user regiested in Cloud platform.
 *
 * @param[out]: UUID , max length: 8
 * @param[out]: BRAND, max length: 8 
 * @param[out]: CID,   max length: 8
 * @param[out]: dev_soft_ssid max length: 32
 *
 * @returns:success 0, others failed
 */
int32_t jl_softap_get_product_info(char *uuid, char *brand, char *cid, char *dev_soft_ssid)
{
    char *_uuid = "CDEFC2";
    jl_platform_strcpy(uuid, _uuid);
    
    char* _dev_soft_ssid="martin";
    jl_platform_strcpy(dev_soft_ssid, _dev_soft_ssid);
    
    char *_cid = "25";
    jl_platform_strcpy(cid, _cid);

    char *_brand="45";
    jl_platform_strcpy(brand, _brand);
    
    return 0;
}

/**
 * @brief:System is expected to connect wifi router with the in parameters.
 *
 * @param[in]: ssid of wifi router
 * @param[in]: passwd  of wifi router
 *
 * @returns:success 0, others failed
 */
int32_t jl_softap_connect_wifi_router(char *ssid, char *passwd)
{
    jl_platform_printf("ssid:%s, passwd:%s\r\n", ssid, passwd);

    if (0 != ln_start_connect_ap(ssid, passwd)) {
        return -1;
    }

    joylink_dev_lan_active_switch(1);
    return 0;
}
