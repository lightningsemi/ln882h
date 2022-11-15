/**
 * @file      get_internet_info.h
 * @brief     this file show the API
 * @author    Ju Chao (chao.ju@qq.com)
 * @version   1.0
 * @date      2022-07-09
*/

#ifndef GET_INTERNET_INFO_H
#define GET_INTERNET_INFO_H
#include "ln882h.h"


typedef struct
{
    int high;
    int low;

    int tem;
    int code;
    int code_day;
    int code_night;
    int humidity;
    double wind_speed;
    int wind_dir_degree;

    float indoor_tem;
    float indoor_hum;
}weather_t;

typedef struct 
{
    uint32_t   year   : 16;  ///< ??
    uint32_t   month  :  6;  ///< ??
    uint32_t   day    :  6;  ///< ??
    uint32_t   week   :  4;  ///< ???
    uint32_t   hour   :  6;  ///< ?
    uint32_t   minute :  6;  ///< ??
    uint32_t   second :  6;  ///< ??
    uint32_t   msec   : 14;  ///< ????
}time_t;


void change_wifi_status(uint8_t sta);
void get_info_task_entry(void *params);


#endif
