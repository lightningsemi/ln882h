/**
 * @file colink_user_timer.h
 *
 * Show profile of colink user timer.
 * It shows the detail of colink user timer.
 *
 * @author Huang Xunyan
 * @date 2018.12.01
 * 
 * @copyright 
 * Copyright (C) 2018 Coolkit Technology Co.,Ltd
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#ifndef COLINK_USER_TIMER_H
#define COLINK_USER_TIMER_H

#include "colink_typedef.h"

#ifndef __COLINK_TIMER_DEF__
#define __COLINK_TIMER_DEF__

/**
 * @defgroup ColinkTimerType
 * @{
 */
#define COLINK_SINGLE_TIMER (0)
#define COLINK_REPEAT_TIMER (1)
/** @} */

/**
 * @defgroup ColinkTimerStatus
 * @{
 */
#define COLINK_TIMER_CANCEL    (0)
#define COLINK_TIMER_RUNNING   (1)
#define COLINK_TIMER_TIMEOUT   (2)
#define COLINK_TIMER_NOT_EXIST (3)
/** @} */

/**
 * @defgroup ColinkTimerErrorCode
 * @{
 */
#define COLINK_TIMER_NO_ERR         (0)
#define COLINK_TIMER_OPERATE_FAILED (1)
#define COLINK_TIMER_NOT_FIND       (2)
/** @} */

typedef int32_t COLINK_TIMER;

#endif /* #ifndef __COLINK_TIMER_DEF__ */

/**
 * @brief 初始化colink定时器
 * 
 * @par 描述
 * 初始化colink定时器
 * 
 * @param NULL 无
 * 
 * @retval true  init success
 * @retval false init failed
 */
bool colinkUserTimerInit(void);

/**
 * @brief 新增定时器
 * 
 * @par 描述
 * 新增一个定时器
 * 
 * @param ColinkTimerType 定时器类型
 * 
 * @retval COLINK_TIMER 定时器描述符
 * 
 * @see ColinkTimerType
 */
COLINK_TIMER colinkUserTimerAdd(int32_t timerType);

/**
 * @brief 设置定时器
 * 
 * @par 描述
 * 设置一个定时器
 * 
 * @param fd 定时器描述符
 * @param nms 定时时间（ms）,0代表取消定时器
 * @param timeoutCb 超时回调函数,不需要填NULL
 * 
 * @retval ColinkTimerErrorCode
 * @see ColinkTimerErrorCode
 */
int32_t colinkUserTimerSet(COLINK_TIMER fd, uint32_t nms, void (*colinkTimerTimeoutCb)(COLINK_TIMER));

/**
 * @brief 删除定时器
 * 
 * @par 描述
 * 删除一个定时器
 * 
 * @param COLINK_TIMER 定时器描述符,传入(-1)表示删除所有定时器
 * 
 * @retval ColinkTimerErrorCode
 * @see ColinkTimerErrorCode
 */
int32_t colinkUserTimerDel(COLINK_TIMER fd);

/**
 * @brief 查看定时器
 * 
 * @par 描述
 * 查看一个定时器定时状态
 * 
 * @param COLINK_TIMER 定时器描述符
 * 
 * @retval ColinkTimerStatus
 * @see ColinkTimerStatus
 */
int32_t colinkUserCheckTimer(COLINK_TIMER fd);

/**
 * @brief 定时器进程
 * 
 * @par 描述
 * 定时器进程，如果使用了定时器超时回调，需要一直循环调用该函数。
 * 
 * @retval ColinkTimerErrorCode
 * @see ColinkTimerErrorCode
 */
int32_t colinkUserTimerProcess(void);

#endif /* #ifndef COLINK_USER_TIMER_H */
