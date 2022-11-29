#include <string.h>
#include "osal/osal.h"
#include "ln_nimble_mem.h"
#include "utils/debug/log.h"
void *nimble_platform_mem_malloc(size_t size)
{
    if(size == 0){
        return NULL;
    }
    //LOG(LOG_LVL_INFO, "%s %d\r\n", __func__, size);
    void * buf = OS_Malloc(size);
    return buf;
}

void *nimble_platform_mem_calloc(size_t n, size_t size)
{
    if(size && n > (~(size_t)0) / size){
		return NULL;
    }
    //LOG(LOG_LVL_INFO, "%s %d %d\r\n", __func__, n, size);
    void *buf = NULL;
    buf = OS_Malloc(n * size);
    if(buf){
        memset(buf, 0, n * size);
    }else {
        return NULL;
    }
    
	return buf;
}

void nimble_platform_mem_free(void *ptr)
{
    if(ptr == NULL) {
        return;
    }
    OS_Free(ptr);
}
