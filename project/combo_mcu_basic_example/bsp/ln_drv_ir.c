/**
 * @file      drv_ir.c
 * @brief 
 * @author    Ju Chao (chao.ju@qq.com)
 * @version   1.0
 * @date      2022-08-22
*/

#include "ln882h.h"
#include "proj_config.h"
#include "serial.h"

#include "ln_drv_ir.h"

Serial_t m_irSerial;

void ln_drv_ir_init()
{
    serial_init(&m_irSerial, SER_PORT_UART1, 9600, NULL);
}

void ln_drv_ir_send_cmd()
{
    uint8_t kongtiao_open_cmd[] = {0xFA,0xFD,0x02,0x00,0xDF};
    serial_write(&m_irSerial,kongtiao_open_cmd,sizeof(kongtiao_open_cmd));
}
