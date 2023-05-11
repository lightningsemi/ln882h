#ifndef __COLINK_PORT_H__
#define __COLINK_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "colink_define.h"
#include "colink_type.h"

#define MATCH_MODE_CHANGE_THLD_TIMES		2

enum {
	MATCH_SOFTAP = 0,
	MATCH_BLE = 1
}; 

//colink adapter api
typedef struct
{
	void (*init)(void);
	void (*start)(void);
 
}COLINK_ADAPTER;

extern COLINK_ADAPTER* get_colink_handle(void);

#ifdef __cplusplus
}
#endif

#endif /* __COLINK_PORT_H__ */
