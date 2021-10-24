/**
 ****************************************************************************************
 *
 * @file ln_def.c
 *
 * @brief  define APIs source code
 *
 *Copyright (C) 2021. Shanghai Lightning Semiconductor Ltd
 *
 *
 ****************************************************************************************
 */


/**
 ****************************************************************************************
 * @addtogroup APP
 * 
 *
 * @brief define  APIs source code
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration
#include <string.h>
#include "co_math.h"                 // Common Maths Definition
#include "co_utils.h"
#include "ke_msg.h"
#include "ln_def.h"
#include "ble_port.h"
#include "osal/osal.h"


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static OS_Queue_t ble_usr_queue;
static OS_Semaphore_t usr_semaphore;


void usr_creat_queue(void)
{
    
    if(OS_OK != OS_QueueCreate(&ble_usr_queue, BLE_USR_MSG_QUEUE_SIZE, sizeof(rw_queue_msg_t)))
    {
        BLIB_LOG(BLIB_LOG_LVL_E, "usr QueueCreate rw_queue failed!!!\r\n");
    }
}

void usr_queue_msg_send(uint16_t id, uint16_t length, void *msg)
{

    ble_usr_msg_t usr_msg;
    usr_msg.id = id;
    usr_msg.len = length;
    usr_msg.msg = msg;
    OS_QueueSend(&ble_usr_queue, &usr_msg, OS_WAIT_FOREVER);
}

int usr_queue_msg_recv(void *msg, uint32_t timeout)
{
    return OS_QueueReceive(&ble_usr_queue, msg, timeout);
}

void usr_creat_semaphore(void)
{
    
    if (OS_SemaphoreCreate(&usr_semaphore, 1, 1) != 0)
    {
        BLIB_LOG(BLIB_LOG_LVL_E, "usr_creat_semaphore Error!\r\n");
    }
}


void usr_acquire_semaphore(void)
{
    if (OS_SemaphoreWait(&usr_semaphore, OS_WAIT_FOREVER) != 0)
    {
        BLIB_LOG(BLIB_LOG_LVL_E, "usr_acquire_semaphore  Error \r\n");
    }
}

void usr_release_semaphore(void)
{
    
    if (OS_SemaphoreRelease(&usr_semaphore) != 0)
    {
        BLIB_LOG(BLIB_LOG_LVL_E, "osSemaphoreRelease  Error \r\n");
    }
}

void usr_delete_semaphore(void)
{
    if(OS_SemaphoreDelete(&usr_semaphore) != 0)
    {
        BLIB_LOG(BLIB_LOG_LVL_E, "osSemaphoreDelete  Error  \r\n");
    }
}

void ln_ke_msg_send(void const *param_ptr)
{
      usr_acquire_semaphore();
        // Send the message
      ke_msg_send(param_ptr);
}


/// @} APP
