#ifndef __USR_APP_H__
#define __USR_APP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void creat_usr_app_task(void);
uint32_t usr_reboot_cnt_get(void);
int usr_reboot_cause_get(void);

#ifdef __cplusplus
}
#endif

#endif /* __USR_APP_H__ */
