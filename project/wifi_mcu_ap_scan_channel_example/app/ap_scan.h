#ifndef _AP_SCAN_H_
#define _AP_SCAN_H_

#include "wifi_manager.h"
#include "utils/debug/log.h"
#include "netif/ethernetif.h"
#include "dhcpd_api.h"
#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "utils/system_parameter.h"
#include <lwip/sockets.h>
#include "lwip/inet.h"
#include "ln_psk_calc.h"
#include "ln_wifi_err.h"
#include "ln_at_adapt.h"
#include "osal/osal.h"

void app_ap_scan(void);
void softap_init(void);
int softap_scan(ap_info_t *ap_info_buf, int list_num);

//func
uint8_t softap_channel_get(ap_info_t **ap_info_buf, int list_num, int scan_times, int *scan_items);

#endif