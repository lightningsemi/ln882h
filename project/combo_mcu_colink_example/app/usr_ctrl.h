#ifndef __USR_CTRL_H__
#define __USR_CTRL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "utils/debug/log.h"

#define COLINK_DEBUG      0
#define _LN882H_SDK_V1_8_ 1

uint8_t get_log_lvl(void);

#define LOG_CTRL(level, ...)                  	\
        do{                              		\
            if(level <= get_log_lvl()){   		\
                log_printf(__VA_ARGS__); 		\
            }                            		\
        }while(0)
	
#define Log_iot(lvl, flag, ...)                    				         	\
	do                                                      				\
	{                                                       				\
		LOG_CTRL(lvl, "%s||%s|%s(%d): ", flag, __FILE__, __func__, __LINE__); 	\
		LOG_CTRL(lvl, __VA_ARGS__);                              				\
		LOG_CTRL(lvl, "\r\n");                                   				\
	} while (0)

#define Log_d(...) Log_iot(LOG_LVL_DEBUG,	"[LN_Dbug]", __VA_ARGS__)
#define Log_i(...) Log_iot(LOG_LVL_INFO,	"[LN_Info]", __VA_ARGS__)
#define Log_w(...) Log_iot(LOG_LVL_WARN,	"[LN_Warn]", __VA_ARGS__)
#define Log_e(...) Log_iot(LOG_LVL_ERROR,	"[LN_Error]", __VA_ARGS__)
	
#define LN_LOG_CTRL_LVL  			("kv_ln_log_ctrl_lvl_parameter")

extern void init_log_lvl(void);
	
//fsm
#define USR_FSM_PRINT_ERR(...)		Log_e(__VA_ARGS__)
typedef	void (*usr_action_ptr)(void *pDta);
  
typedef struct 
{
  int curState;
  int eventId;
  int nextState;
  usr_action_ptr action;
} usr_stat_transf;

typedef struct{
  int state;
  int transNum;
  usr_stat_transf* fsmTbl;
}usr_fsm_t;
	
extern void create_statistics_list_task(void);
extern void colink_start_fsm(usr_fsm_t* pSM, int evt, void *pDta);
	
	
#ifdef __cplusplus
}
#endif

#endif /* __USR_CTRL_H__ */
