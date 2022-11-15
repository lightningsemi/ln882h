/**
 * @file     ln_drv_sht30.h
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2022-08-04
 * 
 * @copyright Copyright (c) 2022 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */


#ifndef LN_DRV_SHT30_H
#define LN_DRV_SHT30_H

#define TEM_HUM_ADDR        0x88

typedef enum{
      CMD_MEAS_PERI_2_M   = 0x2220, // meas. periodic 2 mps, medium rep.
      CMD_FETCH_DATA      = 0xE000, // readout measurements for periodic mode
}sensor_cmd;


unsigned char ln_tem_hum_init();
unsigned char tem_hum_read_sensor_data(unsigned char *data);


#endif