#include "usr_ctrl.h"
#include <stddef.h>
#include "ln_kv_api.h"

#include "ln_at.h"
#include "ln_at_cmd/ln_at_cmd_version.h"
#include "ln_utils.h"

#include "colink_define.h"

//util
static void str_split(char *_str, char *_split, char **_pp, int _cnt)
{
    int i = 0;
    _pp[0] = strtok(_str, _split);
    Log_i("TOKEN[%d] == %s", 0, _pp[0]);
    for (i = 1; i < _cnt; i++)
    {
        _pp[i] = strtok(NULL, _split);
        if (_pp[i] == NULL)
            return;

        Log_i("TOKEN[%d] == %s", i, _pp[i]);
    }
}

//log
uint8_t s_logLvl = LOG_LVL_CTRL;

static bool set_log_lvl(uint8_t _lvl)
{
	if (_lvl > LOG_LVL_TRACE)
	{
		return false;
	}
	
	int kvret = ln_kv_set((const char *)LN_LOG_CTRL_LVL, (void *)(&_lvl), sizeof(_lvl));
	if (KV_ERR_NONE != kvret)
	{
		Log_e("error ln_kv_set LN_LOG_CTRL_LVL with return %d", kvret);
		return false;
	}
	Log_i("set LN_LOG_CTRL_LVL: %d", _lvl);
	//s_logLvl = _lvl;
	
	return true;
}

uint8_t get_log_lvl(void)
{
	return s_logLvl;
}

void init_log_lvl(void)
{
	size_t r_len = 0;
	uint8_t logLvl = LOG_LVL_CTRL;
	if (!ln_kv_has_key((const char *)LN_LOG_CTRL_LVL))
	{
		Log_e("ln_kv not have LN_LOG_CTRL_LVL, set LOG_LVL_CTRL ON");
		goto DEFAULT_LVL;		
	}
	
	if (KV_ERR_NONE != ln_kv_get((const char *)LN_LOG_CTRL_LVL, (void *)(&logLvl), sizeof(logLvl), &r_len)) {
		Log_e("ln_kv_get LN_LOG_CTRL_LVL error, set LOG_LVL_CTRL ON");
		goto DEFAULT_LVL;
	}
	
	Log_i("read LN_LOG_CTRL_LVL : %d", logLvl);
	if (logLvl > LOG_LVL_TRACE)
	{
		Log_e("unknown log lvl: %d, set LOG_LVL_CTRL ON", logLvl);
		goto DEFAULT_LVL;
	}
	s_logLvl = logLvl;
	return;
	
DEFAULT_LVL:
#ifdef COLINK_DEBUG
	s_logLvl = LOG_LVL_INFO;
#else
	s_logLvl = LOG_LVL_CTRL;
#endif
}


//fsm
static usr_stat_transf* colink_loop_transf(usr_fsm_t* pSM, const int evt)
{
	for (int i = 0; i < pSM->transNum; i++) {
		if ((pSM->fsmTbl[i].curState == pSM->state) 
			&& (pSM->fsmTbl[i].eventId == evt)) {
			return &pSM->fsmTbl[i];
		}
	}
	return NULL;
}

void colink_start_fsm(usr_fsm_t* pSM, int evt, void *pDta) 
{
	usr_stat_transf* pTrans = colink_loop_transf(pSM, evt);
	if (pTrans == NULL)
	{
		USR_FSM_PRINT_ERR( "CurState= %d Do not process enent: %d"
							, pSM->state,evt);
		return;
	}
	
	pSM->state = pTrans->nextState;
	if (pTrans->action == NULL) {
		USR_FSM_PRINT_ERR( "change state to %d. No action",pSM->state);
		return;
	}
	pTrans->action(pDta);
}

//AT plus
static ln_at_err_t ln_at_get_log_lvl(const char *name)
{
    ln_at_printf("%s:%d\r\n", name, (int)s_logLvl);
    ln_at_printf(LN_AT_RET_OK_STR);

    return LN_AT_ERR_NONE;
}

static ln_at_err_t ln_at_set_log_lvl(uint8_t para_num, const char *name)
{
    LN_UNUSED(name);

    int temp = 0;
    bool is_default = false;

    if (para_num != 1 && para_num != 2)
    {
        return LN_AT_ERR_PARAM;
    }
    if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_int_param(1, &is_default, &temp))
    {
        return LN_AT_ERR_PARSE;
    }

	uint8_t logLvl = (uint8_t)(temp);
	if (set_log_lvl(logLvl))
	{
		ln_at_printf(LN_AT_RET_OK_STR);
		return LN_AT_ERR_NONE;
	}
	else
	{
		ln_at_printf(LN_AT_RET_FAIL_STR);
		return LN_AT_ERR_PARAM;
	}
}
LN_AT_CMD_REG(LOGLVL, ln_at_get_log_lvl, ln_at_set_log_lvl, NULL, NULL);

//deviceid,apikey,chipid
static ln_at_err_t ln_at_set_colink_info(uint8_t para_num, const char *name)
{
    LN_UNUSED(name);

	char *pBuf = NULL;
    bool is_default = false;

//    if (para_num != 1 && para_num != 2)
//    {
//        return LN_AT_ERR_PARAM;
//    }
	
	//device id
    if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(1, &is_default, &pBuf))
    {
        return LN_AT_ERR_PARSE;
    }
	Log_d("%s", pBuf);
	int kvret = ln_kv_set((const char *)DEVICE_COLINK_ID, (void *)(pBuf), strlen(pBuf) + 1);
	if (KV_ERR_NONE != kvret)
	{
		Log_e("error ln_kv_set DEVICE_COLINK_ID with return %d", kvret);
		ln_at_printf(LN_AT_RET_FAIL_STR);
		return LN_AT_ERR_PARSE;
	}
	
	//apikey
    if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(2, &is_default, &pBuf))
    {
        return LN_AT_ERR_PARSE;
    }
	Log_d("%s", pBuf);
	kvret = ln_kv_set((const char *)DEVICE_COLINK_APIKEY, (void *)(pBuf), strlen(pBuf) + 1);
	if (KV_ERR_NONE != kvret)
	{
		Log_e("error ln_kv_set DEVICE_COLINK_APIKEY with return %d", kvret);
		ln_at_printf(LN_AT_RET_FAIL_STR);
		return LN_AT_ERR_PARSE;
	}
	
	//chipid
    if(LN_AT_PSR_ERR_NONE != ln_at_parser_get_str_param(3, &is_default, &pBuf))
    {
        return LN_AT_ERR_PARSE;
    }
	Log_d("%s", pBuf);
	kvret = ln_kv_set((const char *)DEVICE_COLINK_CHIPID, (void *)(pBuf), strlen(pBuf) + 1);
	if (KV_ERR_NONE != kvret)
	{
		Log_e("error ln_kv_set DEVICE_COLINK_CHIPID with return %d", kvret);
		ln_at_printf(LN_AT_RET_FAIL_STR);
		return LN_AT_ERR_PARSE;
	}

	ln_at_printf(LN_AT_RET_OK_STR);
	return LN_AT_ERR_NONE;
}
LN_AT_CMD_REG(COLINKINFO, NULL, ln_at_set_colink_info, NULL, NULL);

#if STATICTICS_EN
//
#include "osal/osal.h"
#include "utils/debug/ln_assert.h"
#include "task.h"

static OS_Semaphore_t g_get_static;
static ln_at_err_t ln_at_task_list_get(const char *name)
{
    OS_SemaphoreRelease(&g_get_static);
    ln_at_printf(LN_AT_RET_OK_STR);
    return LN_AT_ERR_NONE;
}
LN_AT_CMD_REG(TASKLIST, ln_at_task_list_get, NULL, NULL, NULL);

static OS_Thread_t g_task_static_thread;
#define TASK_STATIC_STACK_SIZE 4 * 256 //Byte
volatile unsigned long ulHightFreqTimerTicks;
static void task_static(void *param)
{
    char *CPU_RunInfo = NULL;//保存任务运行时间信息
    char *pTask[32];
    int taskCnt = 0;
    int taskMaxId = 0;
    char id_buf[3] = {0};
    if (OS_SemaphoreCreate(&g_get_static, 0, 1024) != OS_OK)
    {
        Log_e("OS_SemaphoreCreate sem fail.");
        OS_ThreadDelete(&g_task_static_thread);
        return;
    }

    CPU_RunInfo = OS_Malloc(512);
    while (1)
    {
        memset(pTask, NULL, sizeof(char *) * 32);
        if (OS_OK != OS_SemaphoreWait(&g_get_static, OS_WAIT_FOREVER))
        {
            continue;
        }

        memset(CPU_RunInfo, 0, 512); 
        vTaskList((char *)CPU_RunInfo); 

        str_split(CPU_RunInfo, "\r\n", pTask, 32);
        for (size_t i = 0; i < 32; i++)
        {
            if (pTask[i] == NULL)
            {
                taskCnt = i;
                break;
            }
        }

        for (size_t i = 0; i < taskCnt; i++)
        {
            memset(id_buf, 0, 3);
            id_buf[0] = pTask[i][strlen(pTask[i]) - 2];
            id_buf[1] = pTask[i][strlen(pTask[i]) - 1];

            if (atoi(id_buf) > taskMaxId)
            {
                taskMaxId = atoi(id_buf);
            }
        }

        Log_i("taskCnt=%d    taskMaxId=%d", taskCnt, taskMaxId);
        ln_at_printf("---------------------------------------------\r\n");
        ln_at_printf("TASK\t\tSTAT\tPRI\tSTACK\tID\r\n");
        ln_at_printf("---------------------------------------------\r\n");
        for (size_t i = 1; i <= taskMaxId; i++)
        {
            for (size_t j = 0; j < taskCnt; j++)
            {
                memset(id_buf, 0, 3);
                id_buf[0] = pTask[j][strlen(pTask[j]) - 2];
                id_buf[1] = pTask[j][strlen(pTask[j]) - 1];
                //Log_i("current id: %s", id_buf);
                if (atoi(id_buf) == i)
                {
                    ln_at_printf("%s\r\n", pTask[j]);
                    break;
                }
            }
        }
        ln_at_printf("---------------------------------------------\r\n");

        // memset(CPU_RunInfo, 0, 400); //信息缓冲区清零
        // vTaskGetRunTimeStats((char *)&CPU_RunInfo);
        // at_printf("TASK  STATICS   USED\r\n");
        // at_printf("%s", CPU_RunInfo);
        // at_printf("---------------------------------------------\r\n\n");
        vTaskDelay(1000); /* 延时 500 个 tick */
    }
}

void create_statistics_list_task()
{
    if (OS_OK != OS_ThreadCreate(&g_task_static_thread, "task_static_thread", task_static, NULL, OS_PRIORITY_BELOW_NORMAL, TASK_STATIC_STACK_SIZE))
    {
        LN_ASSERT(1);
    }
}
#endif
