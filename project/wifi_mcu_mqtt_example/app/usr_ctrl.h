#ifndef __USR_CTRL_H__
#define __USR_CTRL_H__

#include "utils/debug/log.h"

#define EZ_PRINTF(...)          \
	do                          \
	{                           \
		at_printf(__VA_ARGS__); \
		at_printf("\r\n");      \
	} while (0)

#define Log_iot(lvl, flag, ...)                             \
	do                                                      \
	{                                                       \
		LOG(lvl, "%s||%s|%s|%04d|:", flag, __FILE__, __func__, __LINE__); \
		LOG(lvl, __VA_ARGS__);                              \
		LOG(lvl, "\r\n");                                   \
	} while (0)

#define Log_d(...) Log_iot(LOG_LVL_DEBUG, "DBG", __VA_ARGS__)
#define Log_i(...) Log_iot(LOG_LVL_INFO, "IFO", __VA_ARGS__)
#define Log_w(...) Log_iot(LOG_LVL_WARN, "WAR", __VA_ARGS__)
#define Log_e(...) Log_iot(LOG_LVL_ERROR, "ERR", __VA_ARGS__)
	

#endif
