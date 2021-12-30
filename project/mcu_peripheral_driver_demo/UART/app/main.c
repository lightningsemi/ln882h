/**
 * @file     main.c
 * @author   BSP Team 
 * @brief    
 * @version  0.0.0.1
 * @date     2021-08-05
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */
#include "hal/hal_common.h"
#include "ln_show_reg.h"
#include "utils/debug/log.h"
#include "ln_test_common.h"

#include "ln_drv_uart.h"

void uart0_callback(uint32_t cur_len);
void uart1_callback(uint32_t cur_len);
void uart2_callback(uint32_t cur_len);

uint8_t send_data[100] = "UART TEST";
uint8_t recv_data[100];

int main (int argc, char* argv[])
{  
    /****************** 1. 系统初始化 ***********************/
    SetSysClock();
    ln_show_reg_init();

    /****************** 2. UART 测试***********************/
    void uart_init(uart_x_t uart_x,uart_pin_cfg_t *uart_pin_cfg,uint32_t baud_rate,void (*uart_it_callback)(uint32_t));
    
    uart_pin_cfg_t  uart_pin_cfg;
    memset(&uart_pin_cfg,0,sizeof(uart_pin_cfg_t));
    
    uart_pin_cfg.uart_tx_port = GPIO_B;
    uart_pin_cfg.uart_tx_pin  = GPIO_PIN_9;
    uart_pin_cfg.uart_rx_port = GPIO_B;
    uart_pin_cfg.uart_rx_pin  = GPIO_PIN_8;
    
    uart_init(UART_0,&uart_pin_cfg,115200,uart0_callback);
    
    uart_recv_data(UART_0,recv_data,100);
    uart_send_data(UART_0,send_data,9);
    
    while(1)
    {
        ln_delay_ms(1000);
    }
}

void uart0_callback(uint32_t cur_len)
{
    if(cur_len == 8)
    {
        uart_send_data(UART_0,recv_data,8);
        uart_recv_data(UART_0,recv_data,100);
    }
}

void uart1_callback(uint32_t cur_len)
{
    if(cur_len == 8)
    {
        uart_send_data(UART_1,recv_data,8);
        uart_recv_data(UART_1,recv_data,100);
    }
}

void uart2_callback(uint32_t cur_len)
{
    if(cur_len == 8)
    {
        uart_send_data(UART_2,recv_data,8);
        uart_recv_data(UART_2,recv_data,100);
    }
}

