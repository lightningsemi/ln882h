#include "iot_import.h"
#include "flash_partition_table.h"
#include "osal/osal.h"
#include "hal/hal_flash.h"
#include "utils/debug/log.h"

#ifndef FLASH_SECTOR_SIZE
#define FLASH_SECTOR_SIZE    (4 * 1024UL)
#endif

typedef struct {
    uint8_t *sector_buf;        // 动态分配内存
    uint32_t buffer_max_size;   // sector_buf 自身容量大小，默认为 FLASH_SECTOR_SIZE
    uint32_t buffer_cur_len;    // sector_buf 包含的数据长度，有可能是 1KB 也可能是 4KB
    uint32_t flash_offset;      // sector_buf 写入 flash 的偏移量，初始化大小为 OTA_SPACE_OFFSET，步长为 4KB
    int8_t   error_flag;        // 是否产生了错误: 1 -- 产生了错误； 0 -- 没有出错
} storage_cache_t;

static storage_cache_t sg_storage_cache = { 0 };

/**
 * @brief initialize a firmware upgrade.
 *
 * @param None
 * @return None.
 * @see None.
 * @note None.
 */
void HAL_Firmware_Persistence_Start(void)
{
    // 分配缓冲区存储空间
    if (NULL == sg_storage_cache.sector_buf) {
        sg_storage_cache.buffer_max_size = FLASH_SECTOR_SIZE;
        sg_storage_cache.sector_buf = OS_Malloc(sg_storage_cache.buffer_max_size);
    }

    if (NULL == sg_storage_cache.sector_buf) {
        sg_storage_cache.error_flag = 1;
        return;
    } else {
        sg_storage_cache.error_flag = 0;
    }

    memset(sg_storage_cache.sector_buf, 0, sg_storage_cache.buffer_max_size);
    sg_storage_cache.flash_offset = OTA_SPACE_OFFSET;

    // OTA 分区清空
    hal_flash_erase(OTA_SPACE_OFFSET, OTA_SPACE_SIZE);
}

/**
 * @brief indicate firmware upgrade data complete, and trigger data integrity checking,
     and then reboot the system.
 *
 * @param None.
 * @return 0: Success; -1: Failure.
 * @see None.
 * @note None.
 */
int HAL_Firmware_Persistence_Stop(void)
{
    // 把缓冲区剩余不足4K的内容写入 flash
    if (sg_storage_cache.error_flag != 0 || (NULL == sg_storage_cache.sector_buf)) {
        return -1;
    }

    if (sg_storage_cache.buffer_cur_len > 0) {
        hal_flash_program(  sg_storage_cache.flash_offset,
                        sg_storage_cache.buffer_cur_len,
                        sg_storage_cache.sector_buf);
    }

    return 0;
}

/**
 * @brief save firmware upgrade data to flash.
 *
 * @param[in] buffer: @n A pointer to a buffer to save data.
 * @param[in] length: @n The length, in bytes, of the data pointed to by the buffer parameter.
 * @return 0, Save success; -1, Save failure.
 * @see None.
 * @note None.
 */
int HAL_Firmware_Persistence_Write(char *buffer, uint32_t length)
{
    if ( (NULL == buffer) || (0 == length) ) {
        return -1;
    }

    if (sg_storage_cache.error_flag != 0 || (NULL == sg_storage_cache.sector_buf)) {
        return -1;
    }

    // 写入flash，自己统计当前已经写入的长度，4K缓冲区
    if (length + sg_storage_cache.buffer_cur_len <= sg_storage_cache.buffer_max_size) {
        memcpy(sg_storage_cache.sector_buf + sg_storage_cache.buffer_cur_len, buffer, length);
        sg_storage_cache.buffer_cur_len += length;

        // 检查是否有必要写入到 flash
        if (sg_storage_cache.buffer_cur_len >= sg_storage_cache.buffer_max_size) {
            hal_flash_program(  sg_storage_cache.flash_offset,
                            sg_storage_cache.buffer_max_size,
                            sg_storage_cache.sector_buf);
            sg_storage_cache.flash_offset += sg_storage_cache.buffer_max_size;
            sg_storage_cache.buffer_cur_len = 0;
        }
    } else {
        // 拷贝满足4KB，然后写入到 flash，再拷贝剩余的
        int32_t copy_length = sg_storage_cache.buffer_max_size - sg_storage_cache.buffer_cur_len;
        int32_t remain_length = length - copy_length;
        memcpy(sg_storage_cache.sector_buf + sg_storage_cache.buffer_cur_len, buffer, copy_length);
        sg_storage_cache.buffer_cur_len += copy_length;

        hal_flash_program(  sg_storage_cache.flash_offset,
                        sg_storage_cache.buffer_max_size,
                        sg_storage_cache.sector_buf);
        sg_storage_cache.flash_offset += sg_storage_cache.buffer_max_size;
        sg_storage_cache.buffer_cur_len = 0;

        memset(sg_storage_cache.sector_buf, 0, sg_storage_cache.buffer_max_size);
        memcpy(sg_storage_cache.sector_buf, buffer + copy_length, remain_length);
        sg_storage_cache.buffer_cur_len += remain_length;
    }

    return 0;
}
