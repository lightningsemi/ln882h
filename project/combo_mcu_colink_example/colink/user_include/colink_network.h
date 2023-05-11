#ifndef __COLINK_NETWORK_H__
#define __COLINK_NETWORK_H__

#include "colink_mcu_driver_port.h"
#include "colink_type.h"

#define IOTGO_NETWORK_CENTER 0

void colinkProcessStart(void);
void enterSettingSelfAPMode(void);
void colinkSoftOverStart(void);

#endif /* #ifndef __COLINK_NETWORK_H__ */
