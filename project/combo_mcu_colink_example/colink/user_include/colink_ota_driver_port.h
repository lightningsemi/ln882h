#ifndef __COLINK_OTA_DRIVER_PORT_H__
#define __COLINK_OTA_DRIVER_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "usr_ctrl.h"
#include <stdint.h>
#include <string.h>

typedef struct {
    char path[128];//
    char digest[68];//
    char version[24];
    char ip[24];//
    uint16_t port;//
    uint32_t file_length;
} OTAFileInfo;

extern void http_ota_task_creat(void* para);


#ifdef __cplusplus
}
#endif

#endif /* __COLINK_OTA_DRIVER_PORT_H__ */
