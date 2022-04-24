#ifndef _LIVING_PLATFORM_OTA_PORT_H_
#define _LIVING_PLATFORM_OTA_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

int  ota_part_verify(void);
void ota_reboot_start(void);

#ifdef __cplusplus
}
#endif
#endif // !_LIVING_PLATFORM_OTA_PORT_H_
