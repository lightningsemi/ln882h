/**
 * @file     ln_i2c_test.h
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2021-08-20
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

#include "hal/hal_common.h"


void ln_i2c_test(void);
void hal_write_command_byte(unsigned int base, unsigned char cmd);
void hal_write_data_byte(unsigned int  base, unsigned char data);
int hal_i2c_master_7bit_read_target_address(uint32_t base, uint8_t dev_addr,uint8_t *target_addr,uint8_t target_addr_len,uint8_t * buf, uint16_t buf_len);
int hal_i2c_master_7bit_write(uint32_t base, uint8_t dev_addr, const uint8_t * buf, uint16_t buf_len);
int hal_i2c_master_7bit_write_target_address(uint32_t base, uint8_t dev_addr, uint8_t *target_addr,uint8_t target_addr_len,const uint8_t * buf, uint16_t buf_len);

