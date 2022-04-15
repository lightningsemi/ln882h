#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// joylink platform layer header files
#include "joylink_stdio.h"
#include "joylink_string.h"
#include "joylink_memory.h"
#include "joylink_thread.h"

#include "osal/osal.h"


/*********************************** mutex interface ***********************************/

/** @defgroup group_platform_mutex mutex
 *  @{
 */

/**
 * @brief 创建互斥锁
 *
 * @retval NULL : Initialize mutex failed.
 * @retval NOT_NULL : The mutex handle.
 * @see None.
 * @note None.
 */
jl_mutex_t jl_platform_mutex_create(int32_t type)
{
#ifdef __LINUX_PAL__
    pthread_mutexattr_t attr;
    pthread_mutex_t *handle = (pthread_mutex_t *)jl_platform_malloc(sizeof(pthread_mutex_t));

    if(handle == NULL)
    {
        jl_platform_printf("jl_platform_malloc failed\r\n");
        goto ERROR;
    }

    if (0 != pthread_mutexattr_init(&attr))
    {
        jl_platform_printf("pthread_mutexattr_init failed\r\n");
        goto ERROR;
    }

    // 设置递归锁
    if (JL_MUTEX_TYPE_RECURSIVE == type)
    {
        if (0 != pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE))
        {
            jl_platform_printf("pthread_mutexattr_settype (PTHREAD_MUTEX_RECURSIVE) failed\r\n");
            goto ERROR;
        }
    }

    pthread_mutex_init(handle, &attr);

    if (0 != pthread_mutexattr_destroy(&attr))
    {
        jl_platform_printf("pthread_mutexattr_destroy failed\r\n");
    }
    return (jl_mutex_t)handle;
ERROR:
    if(handle)
        jl_platform_free(handle);
    return NULL;
#else
    jl_mutex_t handle = NULL;
    OS_Mutex_t mutex;

    //if(handle == NULL)
    //{
    //    return NULL;
    //}
    mutex.handle = NULL;
    if (JL_MUTEX_TYPE_RECURSIVE == type)
    {
        if (OS_OK != OS_RecursiveMutexCreate(&mutex)) {
            jl_platform_printf("OS_RecursiveMutexCreate failed!!\r\n");
            return NULL;
        }
    }
    else
    {
        if (OS_OK != OS_MutexCreate(&mutex)) {
            jl_platform_printf("OS_MutexCreate failed!\r\n");
            return NULL;
        }
    }
    handle = mutex.handle;
    return handle;
#endif
}

/**
 * @brief 等待指定的互斥锁
 *
 * @param [in] handle @n the specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
int32_t jl_platform_mutex_lock(jl_mutex_t handle)
{  
#ifdef __LINUX_PAL__
    int32_t ret;

    if(handle == NULL)
    {
        jl_platform_printf("handle is NULL\r\n");
        return -1;
    }
    ret = pthread_mutex_lock((pthread_mutex_t *)handle);
    if (ret != 0)
    {
        switch (ret) {
        case EDEADLK:
            jl_platform_printf("the mutex is already locked by the calling thread\r\n");
            break;
        case EINVAL:
            jl_platform_printf("the mutex has not been properly initialized.\r\n");
            break;
        default:
            jl_platform_printf("pthread_mutex_trylock error:%s.\r\n", strerror(ret));
            break;
        }
    }
    return ret;
#else
    int type = ucQueueGetQueueType(handle);
    OS_Mutex_t mutex;
    mutex.handle = handle;

    if(type == queueQUEUE_TYPE_RECURSIVE_MUTEX) {
        OS_RecursiveMutexLock(&mutex,OS_WAIT_FOREVER);
    } else if(type == queueQUEUE_TYPE_MUTEX) {
        OS_MutexLock(&mutex,OS_WAIT_FOREVER);
    }
#endif
}

/**
 * @brief 释放指定的互斥锁
 *
 * @param [in] handle @n the specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
int32_t jl_platform_mutex_unlock(jl_mutex_t handle)
{
#ifdef __LINUX_PAL__
    int32_t ret;

    if(handle == NULL)
    {
        jl_platform_printf("handle is NULL\r\n");
        return -1;
    }
 
    ret = pthread_mutex_unlock((pthread_mutex_t *)handle);
    if (ret != 0)
    {
        switch (ret)
        {
        case EPERM:
            jl_platform_printf("the calling thread does not own the mutex.\r\n");
            break;
        case EINVAL:
            jl_platform_printf("the mutex has not been properly initialized.\r\n");
            break;
        default:
            jl_platform_printf("pthread_mutex_trylock error:%s.\r\n", strerror(ret));
            break;
        }
    }
    return ret;
#else
    int type = ucQueueGetQueueType(handle);
    OS_Mutex_t mutex;
    mutex.handle = handle;

    if (type == queueQUEUE_TYPE_RECURSIVE_MUTEX) {
        OS_RecursiveMutexUnlock(&mutex);
    } else if(type == queueQUEUE_TYPE_MUTEX) {
        OS_MutexUnlock(&mutex);
    }

#endif
}

/**
 * @brief 销毁互斥锁，并回收所占用的资源
 *
 * @param [in] handle @n The specified mutex.
 * @return None.
 * @see None.
 * @note None.
 */
void jl_platform_mutex_delete(jl_mutex_t handle)
{
#ifdef __LINUX_PAL__
    if(handle == NULL)
    {
        jl_platform_printf("handle is NULL\r\n");
        return;
    }

    pthread_mutex_destroy((pthread_mutex_t *)handle);
    jl_platform_free((void *)handle);
#else
    OS_Mutex_t mutex;
    mutex.handle = handle;

    OS_MutexDelete(&mutex);
    //OS_Free(handle);
#endif
}

/**
 * @brief   创建一个计数信号量
 *
 * @return semaphore handle.
 * @see None.
 * @note The recommended value of maximum count of the semaphore is 255.
 */
jl_semaphore_t jl_platform_semaphore_create(void)
{
#ifdef __LINUX_PAL__
    jl_semaphore_t semaphore_t = NULL;

    sem_t *sem = NULL;
    sem = (sem_t *)jl_platform_malloc(sizeof(sem_t));
    sem_init(sem, 0, 0);
    semaphore_t = (jl_semaphore_t)sem;
    return semaphore_t;
#else
    jl_semaphore_t handle = NULL;
    OS_Semaphore_t sem;

    sem.handle = NULL;

    if (OS_OK != OS_SemaphoreCreate(&sem, 0, 255)) {
        jl_platform_printf("Create semaphore failed.\r\n");
        return NULL;
    }
    handle = sem.handle;
    return handle;

#endif
}

/**
 * @brief   销毁一个计数信号量, 回收其所占用的资源
 *
 * @param[in] sem @n the specified sem.
 * @return None.
 * @see None.
 * @note None.
 */
void jl_platform_semaphore_destroy(jl_semaphore_t handle)
{
#ifdef __LINUX_PAL__
    if(handle == NULL)
    {
        jl_platform_printf("[semaphore] handle is NULL\r\n");
        return ;
    }
    sem_destroy((sem_t *)handle);
#else
    OS_Semaphore_t sem;

    if(handle != NULL) {
            sem.handle = handle;
            OS_SemaphoreDelete(&sem);
            //OS_Free(handle);
        }

#endif
}

/**
 * @brief   在指定的计数信号量上做自减操作并等待
 *
 * @param[in] sem @n the specified semaphore.
 * @param[in] timeout_ms @n timeout interval in millisecond.
     If timeout_ms is PLATFORM_WAIT_INFINITE, the function will return only when the semaphore is signaled.
 * @return
   @verbatim
   =  0: The state of the specified object is signaled.
   =  -1: The time-out interval elapsed, and the object's state is nonsignaled.
   @endverbatim
 * @see None.
 * @note None.
 */
void jl_platform_semaphore_wait(jl_semaphore_t handle, uint32_t timeout_ms)
{
#ifdef __LINUX_PAL__
    if(handle == NULL)
    {
        jl_platform_printf("[semaphore] handle is NULL\r\n");
        return ;
    }
    sem_wait((sem_t *)handle);
#else
    OS_Semaphore_t sem;
    if(handle != NULL) {
        sem.handle = handle;
        OS_SemaphoreWait(&sem, timeout_ms);
    }

#endif
}

/**
 * @brief   在指定的计数信号量上做自增操作, 解除其它线程的等待
 *
 * @param[in] sem @n the specified semaphore.
 * @return None.
 * @see None.
 * @note None.
 */
void jl_platform_semaphore_post(jl_semaphore_t handle)
{
#ifdef __LINUX_PAL__
    if(handle == NULL)
    {
        jl_platform_printf("[semaphore] handle is NULL\r\n");
        return ;
    }
    sem_post((sem_t *)handle);
#else
    OS_Semaphore_t sem;
    sem.handle = handle;

    OS_SemaphoreRelease(&sem);

#endif
}

/**
 * @brief   按照指定入参创建一个线程
 *
 * @param[out] thread_handle @n The new thread handle, memory allocated before thread created and return it, free it after thread joined or exit.
 * @param[in] pri @n thread priority
 * @param[in] stacksize @n stack size requirements in bytes
 * @return
   @verbatim
     = 0 : on success.
     = -1: error occur.
   @endverbatim
 * @see None.
 * @note None.
 */
int32_t  jl_platform_thread_create(jl_thread_t* thread_handle, JL_THREAD_PRI_T pri, size_t stacksize)
{
    int32_t ret = 0;
    jl_platform_printf("thread create\r\n");
    return ret;
}

/**
 * @brief   通过线程句柄启动指定得任务
 *
 * @param[in] thread_handle @n the thread handle 
 * @param[in] task @n specify the task to start on thread_handle
 * @param[in] parameter @n user parameter input
 * @return
 * @see None.
 * @note None.
 */
void jl_platform_thread_start(jl_thread_t* thread_handle)
{
#ifdef __LINUX_PAL__
    pthread_t task_id;

    if (0 != pthread_create(&task_id, NULL, (void *) thread_handle->thread_task, thread_handle->parameter))
    {
        jl_platform_printf("pthread_create failed(%d): %s\r\n", errno, strerror(errno));
    }
#else
    int32_t ret = 0;
    static int task_name_offset = 0;
    char name[10] = {0};
    OS_Thread_t rw_thread = {NULL};
    
    if (thread_handle->stackSize == 0) {
        thread_handle->stackSize = 5*1024; //TODO: default stackSize = 5KB
    }

    snprintf(name, sizeof(name), "task%d", task_name_offset);
    task_name_offset++;

    jl_platform_printf("name: %s,size = %d,task = %d\r\n", name, thread_handle->stackSize, thread_handle->thread_task);
    ret = OS_ThreadCreate(&rw_thread, name, thread_handle->thread_task, thread_handle->parameter, OS_PRIORITY_NORMAL, thread_handle->stackSize);
    if(ret == 0)
    {
        thread_handle->handle = rw_thread.handle;
    }
    return ret;

#endif
}

/**
 * @brief   设置指定的线程为`Detach`状态
 *
 * @param[in] thread_handle: pointer to thread handle.
 * @return None.
 * @see None.
 * @note None.
 */
void jl_platform_thread_detach(jl_thread_t* thread_handle)
{
}

/**
 * @brief   线程主动退出
 *
 * @param[in] thread_handle: pointer to thread handle.
 * @return None.
 * @see None.
 * @note None.
 */
void jl_platform_thread_exit(jl_thread_t* thread_handle)
{
    OS_ThreadDelete(NULL);
}

/**
 * @brief   杀死指定的线程
 *
 * @param[in] thread_handle: pointer to thread handle, NULL means itself
 * @return None.
 * @see None.
 * @note None.
 */
void jl_platform_thread_delete(jl_thread_t* thread_handle)
{
    OS_Thread_t thread;

    thread.handle = thread_handle->handle;
    OS_ThreadDelete(&thread);
}

/**
 * @brief   获取线程执行状态
 *
* @param[in] thread_handle: pointer to thread handle.
 * @return
 	0:idel
	1:running
 * @see None.
 * @note None.
 */
int32_t jl_platform_thread_isrunning(jl_thread_t* thread_handle)
{
    eTaskState state;

    state = eTaskGetState( thread_handle );
    if (state == eRunning) {
        return 1;
    }
    return 0;
}

/**
 * @brief 毫秒级休眠
 *
 * @param [in] ms @n the time interval for which execution is to be suspended, in milliseconds.
 * @return None.
 * @see None.
 * @note None.
 */
void  jl_platform_msleep(uint32_t ms)
{
    OS_MsDelay(ms);
}

/**
 * 创建定时器
 *
 * @param htimer:Timer handler
 * @return 0:success, -1:failed.
 *
 */
int32_t  jl_timer_create(jl_timer_t *htimer)
{
    OS_Timer_t t;
    OS_Status ret;

    ret = OS_TimerCreate(&t, OS_TIMER_ONCE, htimer->callback,htimer->userData, htimer->periodicInterval);
    if(ret == 0)
    {
        htimer->handle = t.handle;
        return 0;
    }
    return -1;

}

/**
 * 启动定时器
 *
 * @param htimer:Timer handler
 * @return 0:success, -1:failed.
 */
int32_t  jl_timer_start(jl_timer_t *htimer)
{
    OS_Timer_t t;
    OS_Status ret;
    t.handle = htimer->handle;

    ret = OS_TimerStart(&t);
    if(ret == 0)
    {
        return 0;
    }
    return -1;


}

/**
 * 停止定时器
 *
 * @param htimer:Timer handler
 * @return 0:success, -1:failed.
 *
 */
int32_t jl_timer_stop(jl_timer_t *htimer)
{
    OS_Timer_t t;
    OS_Status ret;
    t.handle = htimer->handle;

    ret = OS_TimerStop(&t);
    if(ret == 0)
    {
        return 0;
    }
    return -1;


}

/**
 * 删除定时器
 *
 * @param htimer:Timer handler
 * @return 0:success, -1:failed.
 *
 */
int32_t  jl_timer_delete(jl_timer_t *htimer)
{
    OS_Timer_t t;
    OS_Status ret;
    t.handle = htimer->handle;

    ret = OS_TimerDelete(&t);
    if(ret == 0)
    {
        return 0;
    }
    return -1;


}


/** @} */ /* end of platform_mutex */

