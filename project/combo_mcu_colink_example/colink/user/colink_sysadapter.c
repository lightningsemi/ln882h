//#include "esp_common.h"
#include "colink_sysadapter.h"
#include <stdarg.h>
#include "lwip/def.h"
#include "mbedtls/sha256.h"
#include "hal/hal_trng.h"
#include "netif/ethernetif.h"
#include "utils/debug/log.h"
#include "colink_define.h"
#include "ln_kv_api.h"

#include "usr_ctrl.h"


#define PRINTF_BUFFER_SIZE (1024)
static char printf_buffer[PRINTF_BUFFER_SIZE];

int32_t colinkGettime(uint32_t* ms)
{
    *ms = 1000 * OS_GetTicks() / OS_HZ;
    return COLINK_NO_ERROR;
}

int32_t colinkNetworkState(int32_t* state)
{
    *state = (NETDEV_LINK_UP != netdev_get_link_state(netdev_get_active())) ? 0 : 1;
    return 0;
}

uint32_t colinkRand(void)
{
    uint32_t val = 0;
    trng_init_t_def initStruct;
	memset(&initStruct,0,sizeof(initStruct));
	
	initStruct.trng_fast_mode_en_status = TRNG_FAST_MODE_DIS;
    initStruct.trng_gap = 10;
	
	hal_trng_init(TRNG_BASE,&initStruct);
    
    hal_trng_en(TRNG_BASE,HAL_ENABLE);
    
    while(hal_trng_get_en_status(TRNG_BASE) == 1);
    while(hal_trng_get_it_flag(TRNG_BASE,TRNG_IT_FLAG_TRNG_DONE) == 0)


    val = hal_trng_get_data_0_31(TRNG_BASE);

    hal_trng_en(TRNG_BASE,HAL_DISABLE);

    return val;
}

bool colinkSha256(uint8_t *in, uint32_t len, char *checkout)
{
    uint8_t digest[32];
    int32_t i = 0;
    Log_i("%s", in);
    mbedtls_sha256_context psha256_ctx;
    mbedtls_sha256_starts(&psha256_ctx, 0);
    mbedtls_sha256_update(&psha256_ctx, in, len);
    mbedtls_sha256_finish(&psha256_ctx, digest);
    for (i = 0; i < 64; i += 2)
    {
        sprintf(checkout+i, "%02x", digest[i / 2]);
    }
    Log_i("%s", checkout);

    return 0;
}

uint32_t colinkStrlen(const char* src)
{
    return strlen(src);
}

char* colinkStrcpy(char* dst, const char* src)
{
    return strcpy(dst, src);
}

char* colinkStrncpy(char* dst, const char* src, uint32_t len)
{
    return strncpy(dst, src, len);
}

char* colinkStrcat(char* dst, const char* src)
{
    return strcat(dst, src);
}

char* colinkStrncat(char* dst, const char* src, uint32_t len)
{
    return strncat(dst, src, len);
}

char *colinkStrstr(const char *s1, const char *s2)
{
    return strstr(s1,s2);
}

int32_t colinkStrcmp(const char* str1, const char* str2)
{
    return strcmp(str1, str2);
}

int32_t colinkStrncmp(const char* str1, const char* str2, uint32_t len)
{
    return strncmp(str1, str2, len);
}

char* colinkStrchr(char* str, int32_t ch)
{
    return strchr(str, ch);
}

char* colinkStrrchr(const char* str, char c)
{
    return strrchr(str, c);
}

int32_t colinkAtoi(const char* str)
{
    return atoi(str);
}

int32_t colinkSprintf(char* buf, const char* format, ...)
{
    va_list args;
    int32_t ret;

    va_start(args, format);
    ret = vsprintf(buf, format, args);
    va_end(args);

    return ret;
}

int32_t colinkSnprintf(char* buf, uint32_t size, const char* format, ...)
{
    va_list args;
    int32_t ret;

    va_start(args, format);
    ret = vsnprintf(buf, size, format, args);
    va_end(args);

    return ret;
}

int32_t colinkPrintf(const char* format, ...)
{
    va_list ap;
    int32_t ret;

    va_start(ap, format);
    //ret = vprintf(format, ap);
    ret = vsnprintf(printf_buffer, PRINTF_BUFFER_SIZE, format, ap);
    va_end(ap);
	
	LOG(LOG_LVL_INFO, "%s", printf_buffer);
    
	//Log_i("%s", printf_buffer);
    
	return ret;
}

void* colinkMemset(void* dst, int32_t c, uint32_t len)
{
    return memset(dst, c, len);
}

void* colinkMemcpy(void* dst, const void* src, uint32_t len)
{
    return memcpy(dst, src, len);
}

int32_t colinkMemcmp(const void* buf1, const void* buf2, uint32_t len)
{
    return memcmp(buf1, buf2, len);
}

void* colinkMalloc(uint32_t n)
{
#if 1
    return OS_Malloc(n);
#else
	return malloc(n);
#endif
}

void *colinkRealloc(void *ptr, uint32_t n)
{
	//colinkPrintf("realloc %p %d\r\n", ptr, n);
#if 1	
    //colinkPrintf("ptr == NULL\r\n");
    void* dptr = colinkMalloc(n);
    if (dptr == NULL)
    {
        //colinkPrintf("malloc fail!!\r\n");
        return NULL;
    }
    
	if (ptr != NULL)
    {
		colinkMemcpy(dptr, ptr, n);
        colinkFree(ptr);
    }
	
    return dptr;
#else
	
	return realloc(ptr, n);
#endif
}

void colinkFree(void* pt)
{
#if 1
    OS_Free(pt);
#else
	free(pt);
#endif
}

uint16_t colinkHtons(uint16_t hs)
{
    return htons(hs);
}

uint16_t colinkNtohs(uint16_t ns)
{
    return ntohs(ns);
}

int32_t colinkSscanf(const char *s, const char *format, ...)
{
    va_list args;
    int32_t ret;

    va_start(args, format);
    ret = sscanf(s, format, args);
    va_end(args);

    return ret;
}

double colinkStrtod(const char *nptr,char **endptr)
{
    return strtod(nptr, endptr);
}

int32_t colinkTolower(int32_t c)
{
    return tolower(c);
}

/**
 * @brief ´´½¨»¥³âËø¡£
 *
 * @param mutex   [OUT]  »¥³âËø¾ä±úÖ¸Õë
 *
 * @retval µÈÓÚ0   ´´½¨³É¹¦
 * @retval ·Ç0     ´´½¨Ê§°Ü
 */
int32_t colinkMutexInit(ColinkMutex *mutex)
{
    SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();

    if( xSemaphore != NULL)
    {
        *mutex = xSemaphore;
        return 0;
    }
    else
    {
        return -1;
    }
}

/**
 * @brief »¥³âËø¼ÓËø¡£
 *
 * @param mutex   [IN]  »¥³âËø¾ä±ú
 *
 * @retval µÈÓÚ0   ²Ù×÷³É¹¦
 * @retval ·Ç0     ²Ù×÷Ê§°Ü
 */
int32_t colinkMutexLock(ColinkMutex mutex)
{
    if (!xSemaphoreTake(mutex, portMAX_DELAY))
    {
        return 0;
    }
    else 
    {
        return -1;
    }
}

/**
 * @brief »¥³âËø½âËø¡£
 *
 * @param mutex   [IN]  »¥³âËø¾ä±ú
 *
 * @retval µÈÓÚ0   ²Ù×÷³É¹¦
 * @retval ·Ç0     ²Ù×÷Ê§°Ü
 */
int32_t colinkMutexUnlock(ColinkMutex mutex)
{
    if (!xSemaphoreGive(mutex))
    {
        return 0;
    }
    else 
    {
        return -1;
    }
}

/**
 * @brief É¾³ý»¥³âËø¡£
 *
 * @param mutex   [IN]  »¥³âËø¾ä±ú
 *
 * @retval ÎÞ
 */
void deleteMutexLock(ColinkMutex mutex)
{
    vSemaphoreDelete(mutex);
}
