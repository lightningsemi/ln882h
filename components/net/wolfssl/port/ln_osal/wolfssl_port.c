
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "osal/osal.h"

/* This is used by wolfCrypt asn.c for cert time checking */
unsigned long my_time(unsigned long* timer)
{
    (void)timer;
    return OS_GetTime();
}

void *ln_wolf_realloc(void *old, int newlen)
{
    void *res = NULL;
    if (newlen == 0) {
        OS_Free(old);
        return NULL;
    }

    res = (void *)OS_Malloc(newlen);

    if (old == NULL) {
        return res;
    }

    if (res != NULL) {
        memcpy(res, old, newlen);
        OS_Free(old);
    } else {
        // ptr->_errno = ENOMEM;
    }

    return res;
}

void *ln_wolf_calloc(int nmemb, int size)
{
    void *res = NULL;
    res = (void *)OS_Malloc(size *nmemb);
    if (res != NULL) {
        memset(res, 0x0, size * nmemb);
    }
    return res;
}

// #ifndef NO_CRYPT_BENCHMARK
// /* This is used by wolfCrypt benchmark tool only */
// double current_time(int reset)
// {
//     (void)reset;
//     return OS_GetTicks();
// }
// #endif

