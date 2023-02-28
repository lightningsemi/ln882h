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

#ifndef __LN_SD_TYPE_H
#define __LN_SD_TYPE_H

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#include "hal/hal_common.h"
#include "utils/crc16.h"
typedef union
{
    struct
    {
        uint8_t  idle_status :1;
        uint8_t  erase_reset :1;
        uint8_t  invaild_cmd :1;
        uint8_t  crc_err     :1;
        
        uint8_t  erase_num_err:1;
        uint8_t  addr_err    :1;
        uint8_t  param_err   :1;
        
        uint8_t  reserve     :1;
    }bit_filed;
    uint8_t val;
}sd_r1_t;

typedef union
{
    struct
    {
        uint32_t ocr_reg :32;
        sd_r1_t  r1;
    }bit_filed;
    uint64_t val;
}sd_r3_t;

typedef union
{
    struct
    {
        uint8_t check_byte      :8;
        uint8_t voltage_range   :4;
        uint16_t reserved       :16;
        uint8_t cmd_version     :4;
        sd_r1_t  r1;
    }bit_filed;
    uint64_t val;
}sd_r7_t;


#define SD_CMD0     0x40    //R1
#define SD_CMD8     0x48    //R7

#define SD_CMD17    0x51    //R1
#define SD_CMD24    0x58    //R1
#define SD_CMD55    0x77    //R1
#define SD_CMD58    0x7A    //R3
#define SD_ACMD41   0x69    //R3

#define SD_BLOCK_SIZE 512

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__LN_SD_TYPE_H

