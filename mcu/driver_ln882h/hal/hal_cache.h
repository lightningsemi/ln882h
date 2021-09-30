/**
 * @file     hal_cache.h
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2021-08-17
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */

#ifndef __HAL_CACHE_H__
#define __HAL_CACHE_H__

#include <stdint.h>

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


void  flash_cache_init(uint32_t flash_base_addr);
void  flash_cache_disable(void);
void  flash_cache_flush(uint32_t low_addr,uint32_t high_addr);
void  flash_cache_flush_all(void);


#ifdef __cplusplus
    }
#endif // __cplusplus
    
#endif /* __HAL_CACHE_H__ */


