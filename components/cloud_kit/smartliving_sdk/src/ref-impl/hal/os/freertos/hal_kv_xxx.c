#include <stdio.h>
#include "iot_import.h"
#include "ln_utils.h"
#include "ln_kv_api.h"

////////////////////////////////////////////////////////////////////////////////
// NOTE: `ln_kv_port_init()` must be called before any following API is called.
////////////////////////////////////////////////////////////////////////////////

int HAL_Kv_Del(const char *key)
{
    kv_err_t err = ln_kv_del(key);
    if (err != KV_ERR_NONE) {
        return STATE_SYS_DEPEND_KV_DELETE;
    }
    return 0;
}

int HAL_Kv_Get(const char *key, void *val, int *buffer_len)
{
    kv_err_t err = KV_ERR_NONE;
    size_t value_len = 0;

    if ( (!key) || (!val) || (!buffer_len) ) {
        return STATE_SYS_DEPEND_KV_GET;
    }

    err = ln_kv_get(key, val, (size_t)*buffer_len, &value_len);

    if ( (err != KV_ERR_NONE) || ( (int)value_len > *buffer_len)) {
        return STATE_SYS_DEPEND_KV_GET;
    }

    return 0;
}

int HAL_Kv_Set(const char *key, const void *val, int len, int sync)
{
    kv_err_t err = KV_ERR_NONE;

    err = ln_kv_set(key, val, (size_t)len);
    if (err != KV_ERR_NONE) {
        return STATE_SYS_DEPEND_KV_SET;
    }

    return 0;
}

DLL_HAL_API int     HAL_Kv_Del_By_Prefix(const char *prefix)
{
    // TODO: 使用 KV 实现
    return -1;
}
