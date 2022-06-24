#ifndef _SERIAL_H4TL_DRIVER_H_
#define _SERIAL_H4TL_DRIVER_H_


#include <stdbool.h>    // standard boolean definitions
#include <stdint.h>     // standard integer functions


void hci_uart_init(uint32_t baud);
void hci_uart_flow_on(void);
bool hci_uart_flow_off(void);

void hci_uart_read(uint8_t *bufptr, uint32_t size, void (*callback) (void*, uint8_t), void* dummy);
void hci_uart_write(uint8_t *bufptr, uint32_t size, void (*callback) (void*, uint8_t), void* dummy);

#endif
