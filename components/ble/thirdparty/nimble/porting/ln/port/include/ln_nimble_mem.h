#ifndef __LN_NIMBLE_MEM_H__
#define __LN_NIMBLE_MEM_H__

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

void *nimble_platform_mem_malloc(size_t size);
void *nimble_platform_mem_calloc(size_t n, size_t size);
void nimble_platform_mem_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif /* __LN_NIMBLE_MEM_H__ */
