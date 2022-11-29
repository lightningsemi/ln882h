#ifndef __LN_ADAPTER_H__
#define __LN_ADAPTER_H__

#include "stdint.h"

#define BLE_HCI_UART_H4_NONE        0x00
#define BLE_HCI_UART_H4_CMD         0x01
#define BLE_HCI_UART_H4_ACL         0x02
#define BLE_HCI_UART_H4_SCO         0x03
#define BLE_HCI_UART_H4_EVT         0x04


typedef struct ln_vhci_host_callback {
    void (*notify_host_send_available)(void);               /*!< callback used to notify that the host can send packet to controller */
    int (*notify_host_recv)(uint8_t *data, uint16_t len);   /*!< callback used to notify that the controller has a packet to send to the host*/
} ln_vhci_host_callback_t;

struct ln_vhci_cmd_t{
    uint16_t opcode;
    uint8_t length;
    uint8_t data[0];
}__attribute__((packed));

struct ln_vhci_acl_data_t{
    uint16_t hdl_flags;
    uint16_t length;
    uint8_t data[0];
}__attribute__((packed));

struct ln_vhci_evt_t{
    uint8_t opcode;
    uint8_t length;
    uint8_t data[0];
}__attribute__((packed));


int ln_nimble_hci_init(void);
int ln_nimble_hci_and_controller_init(void);
int ln_nimble_hci_deinit(void);
int ln_nimble_hci_and_controller_deinit(void);

#endif 


