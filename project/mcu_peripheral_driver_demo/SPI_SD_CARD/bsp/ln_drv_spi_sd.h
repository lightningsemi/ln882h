/**
 * @file     ln_drv_spi_sd.h
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2023-01-05
 * 
 * @copyright Copyright (c) 2023 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

#ifndef __LN_DRV_SPI_SD_H
#define __LN_DRV_SPI_SD_H

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include "hal/hal_common.h"
#include "hal/hal_spi.h"
#include "hal/hal_gpio.h"
#include "hal/hal_dma.h"
#include "ln_sd_type.h"
#include "ln_test_common.h"
#include "stdbool.h"

typedef enum
{
    SPI_GPIO_A = 0,
    SPI_GPIO_B = 1,
}spi_gpio_port_t;


typedef struct
{
    spi_gpio_port_t gpio_clk_port;
    gpio_pin_t      gpio_clk_pin;
    spi_gpio_port_t gpio_mosi_port;
    gpio_pin_t      gpio_mosi_pin;
    spi_gpio_port_t gpio_miso_port;
    gpio_pin_t      gpio_miso_pin;
    spi_gpio_port_t gpio_cs_port;
    gpio_pin_t      gpio_cs_pin;
}spi_pin_cfg_t;


bool ln_drv_spi_sd_init(spi_pin_cfg_t *spi_pin);
bool ln_drv_spi_sd_read(uint32_t addr,uint8_t *data,uint32_t len);
bool ln_drv_spi_sd_write(uint32_t addr,uint8_t *data,uint32_t len);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__LN_DRV_SPI_SD_H
