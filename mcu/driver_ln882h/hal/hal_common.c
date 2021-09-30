/**
 * @file     hal_common.c
 * @author   BSP Team 
 * @brief 
 * @version  0.0.0.1
 * @date     2021-08-17
 * 
 * @copyright Copyright (c) 2021 Shanghai Lightning Semiconductor Technology Co. Ltd
 * 
 */
#include "utils/debug/log.h"
#include "hal/hal_common.h"

void assert_failed(uint8_t* file, uint32_t line)
{
    uint8_t *file_name = file;
    uint32_t line_num  = line;
//    log_printf("ASSERT Failed-> file: %s @ line %d\r\n", file_name, line_num);
    while(1){
        file_name = file_name;
        line_num  = line_num;
    };
}

