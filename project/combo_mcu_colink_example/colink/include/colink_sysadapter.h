/**
 * @file colink_sysadapter.h
 *
 * Show profile of colink system adapter.
 * It shows the detail of colink system adapter interface.
 *
 * @author Wu Jiale
 * @date 2018.09.06
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
#ifndef COLINK_SYSADAPTER_H
#define COLINK_SYSADAPTER_H

#include <stdint.h>
#include "colink_error.h"
#include "colink_typedef.h"

typedef void *  ColinkMutex;

#define COLINK_OUTPUT_DEBUG     (1)
#define COLINK_OUTPUT_INFO     (1)
#define COLINK_OUTPUT_ERROR     (1)
#define COLINK_OUTPUT_DEBUG     (1)

/**
 * @brief 获取系统运行时间。
 *
 * @par 描述:
 * 获取系统运行时间，以毫秒为单位。
 *
 * @param ms     [OUT]   当前时间
 *
 * @retval COLINK_NO_ERROR    获取成功
 * @retval -1                 获取失败
 */
int32_t colinkGettime(uint32_t* ms);

/**
 * @brief 获取网络状态。
 *
 * @par 描述:
 * 获取网络是否可以与外网通信的状态。当设备连上路由器且分配IP，
 * state为1.当未连接路由器或已连接路由器但未分配IP，state为0.
 *
 * @param state  [OUT]    网络状态,取值范围为[0,1]
 *
 * @retval 0    获取成功
 * @retval 非0  获取失败
 */
int32_t colinkNetworkState(int32_t* state);

/**
 * @brief 随机数发生器的初始化函数。
 *
 * @par 描述:
 * 根据系统提供的种子值，产生随机数。
 *
 * @param 无
 *
 * @retval 随机数
 */
uint32_t colinkRand(void);

/**
 * @brief 计算sha256校验值函数
 * 
 * @par 描述：
 * 根据传入的字符串，计算sha256校验值，将结果返回到checkout
 * 
 * @param in  [IN] 传入的数据
 * @param in  [IN] 传入的数据长度
 * @param checkout  [OUT] 计算后的sha256校验值字符串
 * 
 * @retval true  计算校验值成功
 * @retval false  计算校验值失败
 */
bool colinkSha256(uint8_t *in, uint32_t len, char *checkout);

/**
 * @brief 计算给定字符串的长度。
 *
 * @par 描述:
 * 计算给定字符串的（uint32_t型）长度，不包括'\0'在内。
 *
 * @param src   [IN]     字符串的首地址
 *
 * @retval 字符串的长度
 */
uint32_t colinkStrlen(const char* src);

/**
 * @brief 复制字符串中的内容。
 *
 * @par 描述:
 * 把src所指向的字符串中以src地址开始复制到dst所指的数组中，并返回dst。
 *
 * @param dst   [IN]     目标字符数组
 * @param src   [IN]     源字符串的起始位置
 *
 * @retval dst 目标字符数组的首地址
 */
char* colinkStrcpy(char* dst, const char* src);

/**
 * @brief 复制字符串中的内容。
 *
 * @par 描述:
 * 把src所指向的字符串中以src地址开始的前len个字节复制到dst所指的数组中，并返回dst。
 *
 * @param dst   [IN]     目标字符数组
 * @param src   [IN]     源字符串的起始位置
 * @param len   [IN]     要复制的字节数
 *
 * @retval dst 目标字符数组的首地址
 */
char* colinkStrncpy(char* dst, const char* src, uint32_t len);

/**
 * @brief 连接字符串。
 *
 * @par 描述:
 * 把src所指字符串添加到dst结尾处实现字符串的连接，连接过程覆盖dst结尾处的'/0'。
 *
 * @param dst   [IN]     目标字符串的指针
 * @param src   [IN]     源字符串的指针
 *
 * @retval dst  目标字符串的指针
 */
char* colinkStrcat(char* dst, const char* src);

/**
 * @brief 连接字符串。
 *
 * @par 描述:
 * 把src所指字符串添加到dst结尾处实现字符串的连接，连接过程覆盖dst结尾处的'/0'。
 *
 * @param dst   [IN]     目标字符串的指针
 * @param src   [IN]     源字符串的指针
 * @param len   [IN]     字符串的长度
 *
 * @retval dst  目标字符串的指针
 */
char* colinkStrncat(char* dst, const char* src, uint32_t len);

/**
 * @brief 查找字符串第一次出现的位置。
 *
 * @par 描述:
 * 在字符串s1中查找第一次出现字符串s2的位置，不包含终止符 '\0'。
 *
 * @param s1   [IN]     字符串1的首地址
 * @param s2   [IN]     字符串2的首地址
 *
 * @retval 非NULL   找到第一次出现的s1字符串位置。
 * @retval NULL     未找到。
 */
char *colinkStrstr(const char *s1, const char *s2);

/**
 * @brief 比较两个字符串的大小。
 *
 * @par 描述:
 * 根据用户提供的字符串首地址及长度，比较两个字符串的大小。指针必须指向合法内存。
 *
 * @param str1   [IN]     字符串1的首地址
 * @param str2   [IN]     字符串2的首地址
 *
 * @retval 等于0   str1等于str2
 * @retval 小于0   str1小于str2
 * @retval 大于0   str1大于str2
 */
int32_t colinkStrcmp(const char* str1, const char* str2);

/**
 * @brief 比较两个字符串的大小。
 *
 * @par 描述:
 * 根据用户提供的字符串首地址及长度，比较两个字符串的大小。指针必须指向合法内存。
 *
 * @param str1   [IN]     字符串1的首地址
 * @param str2   [IN]     字符串2的首地址
 * @param len    [IN]     字符串的长度
 *
 * @retval 等于0   str1等于str2
 * @retval 小于0   str1小于str2
 * @retval 大于0   str1大于str2
 */
int32_t colinkStrncmp(const char* str1, const char* str2, uint32_t len);

/**
 * @brief 查找一个字符在一个字符串中首次出现的位置。
 *
 * @par 描述:
 * 查找一个字符ch在另一个字符串str中末次出现的位置，并返回这个位置的地址。
 * 如果未能找到指定字符，那么函数将返回NULL。
 *
 * @param str    [IN]    字符串的首地址
 * @param ch     [IN]    要查找的字符
 *
 * @retval Null,未能找到指定字符
 * @retval 非Null,字符首次出现位置的地址
 */
char* colinkStrchr(char* str, int32_t ch);

/**
 * @brief 查找一个字符在一个字符串中末次出现的位置。
 *
 * @par 描述:
 * 查找一个字符c在另一个字符串str中末次出现的位置，并返回这个位置的地址。
 * 如果未能找到指定字符，那么函数将返回NULL。
 *
 * @param str    [IN]    字符串的首地址
 * @param c      [IN]    要查找的字符
 *
 * @retval Null     未能找到指定字符
 * @retval 非Null   字符末次出现位置的地址
 */
char* colinkStrrchr(const char* str, char c);

/**
 * @brief 把字符串转换成整型数。
 *
 * @par 描述:
 * 如果第一个非空格字符存在，是数字或者正负号则开始做类型转换，
 * 之后检测到非数字(包括结束符 \0) 字符时停止转换。
 *
 * @param str    [IN]    字符串的首地址
 *
 * @retval 整型数
 */
int32_t colinkAtoi(const char* str);

/**
 * @brief 字符串格式化函数。
 *
 * @par 描述:
 * 把格式化的数据写入某个字符串缓冲区。
 *
 * @param buf        [IN/OUT]    指向将要写入的字符串的缓冲区
 * @param format     [IN]        格式控制信息
 * @param ...        [IN]        可选参数，可以是任何类型的数据
 *
 * @retval 小于0       写入失败
 * @retval 大于等于0   写入的字符串长度
 */
int32_t colinkSprintf(char* buf, const char* format, ...);

/**
 * @brief 字符串格式化函数。
 *
 * @par 描述:
 * 把格式化的数据写入某个字符串缓冲区。
 *
 * @param buf        [IN/OUT]    指向将要写入的字符串的缓冲区
 * @param size       [IN]        要写入的字符的最大数目(含'\0')，超过size会被截断,末尾自动补'\0'
 * @param format     [IN]        格式控制信息
 * @param ...        [IN]        可选参数，可以是任何类型的数据
 *
 * @retval 小于0       写入失败
 * @retval 大于等于0   成功则返回欲写入的字符串长度
 */
int32_t colinkSnprintf(char* buf, uint32_t size, const char* format, ...);

/**
 * @brief 格式化输出函数。
 *
 * @par 描述:
 * 格式化输出，一般用于向标准输出设备按规定格式输出信息。
 *
 * @param    format  [IN]     格式控制信息
 * @param     ...     [IN]     可选参数，可以是任何类型的数据
 *
 * @retval 小于0       输出失败
 * @retval 大于等于0   输出的长度
 */
int32_t colinkPrintf(const char* format, ...);

/**
 * @brief 在一段内存块中填充某个给定的值。
 *
 * @par 描述:
 * 将dst中前len个字节用c替换并返回dst。
 *
 * @param dst   [IN/OUT]     目标的起始位置
 * @param c     [IN]         要填充的值
 * @param len   [IN]         要填充的值字节数
 *
 */
void* colinkMemset(void* dst, int32_t c, uint32_t len);

/**
 * @brief 复制内存中的内容。
 *
 * @par 描述:
 * 从源src所指的内存地址的起始位置开始复制len个字节到目标dst所指的内存地址的起始位置中。
 *
 * @param dst   [IN/OUT]     目标内存的起始位置
 * @param src   [IN]         源内存的起始位置
 * @param len   [IN]         要复制的字节数
 *
 */
void* colinkMemcpy(void* dst, const void* src, uint32_t len);

/**
 * @brief 比较两块内存区域。
 *
 * @par 描述:
 * 根据用户提供的内存首地址及长度，比较两块内存的前len个字节。指针必须指向合法内存。
 *
 * @param buf1   [IN]     内存1的首地址
 * @param buf2   [IN]     内存2的首地址
 * @param len    [IN]     要比较的字节数
 *
 * @retval 等于0   buf1等于buf2
 * @retval 小于0   buf1小于buf2
 * @retval 大于0   buf1大于buf2
 */
int32_t colinkMemcmp(const void* buf1, const void* buf2, uint32_t len);

/**
 * @brief 获取所需的内存空间。
 *
 * @par 描述:
 * 分配所需的内存空间，并返回一个指向它的指针。指针必须指向合法内存。
 *
 * @param n   [IN]     需要分配内存大小。
 *
 * @retval NULL    分配成功
 * @retval 非NULL  分配失败
 *
 */
void* colinkMalloc(uint32_t n);

/**
 * @brief  调整一块内存空间大小。
 *
 * @par 描述:
 * 尝试重新调整之前调用 colinkMalloc 所分配的 ptr 所指向的内存块的大小。
 *
 * @param n   [IN]     需要分配内存大小。
 *
 * @retval NULL    重新分配成功。
 * @retval 非NULL  返回指针指向重新分配大小的内存。
 *
 */
void *colinkRealloc(void *ptr, uint32_t n);

/**
 * @brief 释放指针pt所占用的内存空间。
 *
 * @par 描述:
 * 根据用户提供的指针pt，释放其所占用的空间。指针必须指向合法内存。
 *
 * @param pt   [IN]     指针
 *
 * @retval 无
 *
 */
void colinkFree(void* pt);

/**
 * @brief 将主机字节顺序转换为网络字节顺序。
 *
 * @par 描述:
 * 把用户提供的主机字节顺序的16位数转换为网络字节顺序表示的16位数。
 *
 * @param ns   [IN]     主机字节顺序表示的16位数
 *
 * @retval 网络字节顺序表示的16位数
 */
unsigned short colinkHtons(unsigned short hs);

/**
 * @brief 将网络字节顺序转换为主机字节顺序。
 *
 * @par 描述:
 * 把用户提供的网络字节顺序的16位数转换为主机字节顺序表示的16位数。
 *
 * @param ns   [IN]     网络字节顺序表示的16位数
 *
 * @retval 主机字节顺序表示的16位数
 */
unsigned short colinkNtohs(unsigned short ns);

/**
 * @brief 从一个字符串中读进与指定格式相符的数据。
 *
 * @par 描述:
 * 从一个字符串中读进与指定格式相符的数据。
 *
 * @param buf        [IN]    检索的字符串
 * @param format     [IN]    格式化的字符串
 * @param ...        [IN]    可选参数，可以是任何类型的数据
 *
 * @retval 小于0   失败
 * @retval 大于0   成功填充的参数列表中的项数
 */
int32_t colinkSscanf(const char *s, const char *format, ...);

/**
 * @brief 把字符串转换成双精度浮点数。
 *
 * @par 描述:
 * 扫描参数nptr字符串，跳过前面的空白符，直到遇上数字或正负符号才开始做转换，
 * 到出现非数字或字符串结束时('\0')才结束转换，并将结果返回。
 *
 * @param nptr       [IN]    字符串的首地址
 * @param endptr    [OUT]    可以为NULL，若不为NULL则会将遇到不合条件而终止的nptr中的字符指针由endptr传回
 *
 * @retval 双精度浮点数
 */
double colinkStrtod(const char *nptr,char **endptr);

/**
 * @brief 将字母转换为小写字母。
 *
 * @par 描述:
 * 将字符转换成小写字母,非字母字符不做出处理。
 *
 * @param c       [IN]    需要转换的字符
 *
 * @retval 当c是大写字母字符返回对应小写字母字符，其它值不处理返回c。
 */
int32_t colinkTolower(int32_t c);

/**
 * @brief 分解字符串为一组字符串
 *
 * @par 描述:
 * 分解字符串为一组字符串,详细参考strtok函数。
 *
 * @param s   [IN]  要分解的字符
 * @param delim  [IN]  分隔符字符
 *
 * @retval 从s开头开始的一个被分割的串。
 */
char *colinkStrtok(char *s, const char *delim);

/**
 * @brief 创建互斥锁。
 *
 * @param mutex   [OUT]  互斥锁句柄指针
 *
 * @retval 等于0   创建成功
 * @retval 非0     创建失败
 */
int32_t colinkMutexInit(ColinkMutex *mutex);

/**
 * @brief 互斥锁加锁。
 *
 * @param mutex   [IN]  互斥锁句柄
 *
 * @retval 等于0   操作成功
 * @retval 非0     操作失败
 */
int32_t colinkMutexLock(ColinkMutex mutex);

/**
 * @brief 互斥锁解锁。
 *
 * @param mutex   [IN]  互斥锁句柄
 *
 * @retval 等于0   操作成功
 * @retval 非0     操作失败
 */
int32_t colinkMutexUnlock(ColinkMutex mutex);

/**
 * @brief 删除互斥锁。
 *
 * @param mutex   [IN]  互斥锁句柄
 *
 * @retval 无
 */
void deleteMutexLock(ColinkMutex mutex);

/**
 * @code
 * 
 * //colinkMutex adapt example for freertos.
 * 
 * int32_t colinkMutexInit(ColinkMutex *mutex)
 * {
 *    SemaphoreHandle_t xSemaphore = xSemaphoreCreateMutex();
 *
 *    if( xSemaphore != NULL)
 *    {
 *        *mutex = xSemaphore;
 *        return 0;
 *    }
 *    else
 *    {
 *        return -1;
 *    }
 * }
 *
 * int32_t colinkMutexLock(ColinkMutex mutex)
 * {
 *    if (!xSemaphoreTake(mutex, portMAX_DELAY))
 *    {
 *        return 0;
 *    }
 *    else 
 *    {
 *        return -1;
 *    }
 * }
 *
 * int32_t colinkMutexUnlock(ColinkMutex mutex)
 * {
 *    if (!xSemaphoreGive(mutex))
 *    {
 *        return 0;
 *    }
 *    else 
 *    {
 *        return -1;
 *    }
 * }
 *
 * void deleteMutexLock(ColinkMutex mutex)
 * {
 *     vSemaphoreDelete(mutex);
 * }
 *
 * void mutexUsecase(void)
 * {
 *      int32_t ret = -1;
 *      ColinkMutex mutex = NULL;
 *
 *      ret = colinkMutexInit(&mutex);
 *      if (ret == 0)
 *      {
 *          colinkMutexLock(mutex);
 *          // ... user code...
 *          colinkMutexUnlock(mutex);
 *      }
 *      deleteMutexLock(mutex);
 * }
 * 
 * @endcode
 */

#endif
