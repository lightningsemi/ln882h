/* --------------------------------------------------
 * @file: joylink_extern_tool.C
 *
 * @brief: 
 *
 * @version: 2.0
 *
 * @date: 2018/07/26 PM
 *
 * --------------------------------------------------
 */

#include <stdio.h>

#include "ln_joylink.h"
#include "joylink_extern_user.h"
#include "joylink_stdio.h"
#include "joylink_stdint.h"
#include "flash_partition_table.h"
#include "joylink_log.h"

static FILE *file_fp = NULL;
static unsigned char *temp_buf = NULL;
static unsigned int total;
static unsigned int total_write;



// hal_flash_erase(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB);
// hal_flash_program(flash_ota_start_addr + flash_ota_offset, SECTOR_SIZE_4KB, (uint8_t *)temp4K_buf);
int joylink_memory_init(void *index, int flag)
{
    //TODO: ???

#if 1
    log_info("joylink_memory_init\r\n");

    if(temp_buf != NULL)
    {
        jl_platform_free(temp_buf);
        temp_buf = NULL;
    }

    temp_buf = jl_platform_malloc(LN_FLASH_BLOCK_SIZE);
    if(temp_buf != NULL)
    {
        hal_flash_erase(OTA_SPACE_OFFSET, OTA_SPACE_SIZE);
        total = 0;
        total_write = 0;
        return 0;
    }
    
    return -1;
#else
    char *save_path = (char *)index;

    if(file_fp > 0)	
        return -1;

    //jl_platform_printf("save_path: %s\n", save_path);
    
    if(flag == MEMORY_WRITE){
        file_fp = jl_platform_fopen(save_path, "wb+");
        if(file_fp == NULL){
            jl_platform_printf("Open file error!\n");
            return -1;
        }
    }
    else if(flag == MEMORY_READ){
        file_fp = jl_platform_fopen(save_path, "rb");
        if(file_fp == NULL){
            jl_platform_printf("Open file error!\n");
            return -1;
        }
    }
    return 0;
#endif    
}

int joylink_memory_write(int offset, char *data, int len)
{
        //TODO: 为什么这个适配与OTA耦合起来？？

#if 1
    int total_offset = total - total_write ;
    int data_offset = 0;
    
    if(total_offset + len >= LN_FLASH_BLOCK_SIZE)
    {
        memcpy(temp_buf + total_offset, data, LN_FLASH_BLOCK_SIZE - total_offset);
        hal_flash_program(OTA_SPACE_OFFSET + total_write, LN_FLASH_BLOCK_SIZE, (uint8_t *)temp_buf);
        memcpy(temp_buf , data + (LN_FLASH_BLOCK_SIZE - total_offset), len -(LN_FLASH_BLOCK_SIZE - total_offset) );
        total_write+= LN_FLASH_BLOCK_SIZE;
        total = total + len;
    }
    else
    {
        memcpy(temp_buf + total_offset, data, len);
        total = total + len;
    }
    return len;
#else

    if(file_fp == NULL || data == NULL)	
        return -1;

    return jl_platform_fwrite(data, 1, len, file_fp);
#endif    
}

int joylink_memory_read(int offset, char *data, int len)
{
#if 1
    
#else

    if(file_fp == NULL || data == NULL)	
        return -1;

    return jl_platform_fread(data, 1, len, file_fp);
#endif    
}

int joylink_memory_finish(void)
{
#if 1
    int total_offset = total - total_write ;
    log_info("joylink_memory_finish total_offset=%d,%d,%d\r\n",total_offset,total,total_write);

    if(total_offset > 0)
    {
        hal_flash_program(OTA_SPACE_OFFSET + total_write, LN_FLASH_BLOCK_SIZE, (uint8_t *)temp_buf);
    }
    jl_platform_free(temp_buf);
    temp_buf = NULL;
    return 0;
#else

    if(file_fp == NULL)
        return -1;

    jl_platform_fclose(file_fp);
    file_fp = NULL;
    return 0;
#endif    
}



