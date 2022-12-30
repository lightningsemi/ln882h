#ifndef MQTT_TASK_H
#define MQTT_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "netif/ethernetif.h"
#include "usr_ctrl.h"
#include "mqtt_example.h"

extern void netdev_get_ip_changed_flag_cb(struct netif *nif);
extern void create_mqtt_task(void);

#ifdef __cplusplus
}
#endif

#endif /* MQTT_TASK_H */
