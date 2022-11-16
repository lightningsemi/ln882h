/**
 * @file      lcd_show_info.h
 * @brief 
 * @author    Ju Chao (chao.ju@qq.com)
 * @version   1.0
 * @date      2022-07-10
*/

#ifndef LCD_SHOW_INFO_H
#define LCD_SHOW_INFO_H

#include "ln882h.h"
#include "get_internet_info.h"

typedef enum{
    wifi_sta_init,
    wifi_sta_init_success,
    wifi_sta_init_failed,
    wifi_sta_running,
    wifi_sta_connecting,
    wifi_sta_connect_timeout,

    wifi_get_info_success,
    wifi_ble_net_config,

    wifi_ap_init,
    wifi_ap_connect,
    wifi_ap_connected,
    wifi_ap_recv_info_sucessed,
    wifi_ap_recv_info_failed,
}wifi_state_machine;

void lcd_show_info_task_entry(void *params);
void update_weather(weather_t *weather);
void update_from_ntp(uint32_t second);

wifi_state_machine  lvgl_get_current_state_machine();
void                lvgl_set_next_state_machine(wifi_state_machine state_machine);
#endif
