/**
 * @file colink_cjson.h
 *
 * Show profile of colink_cJSON.
 * It shows the detail of colink_cJSON struct and API.
 *
 * @author Huang Xiaoming
 * @date 2018.09.13
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
#ifndef COLINK_CJSON_H
#define COLINK_CJSON_H

#include <stddef.h>
#include "stdint.h"

/**
 * @defgroup ColinkCjsonType
 * @{
 */
#define C_NULL    (0)
#define C_FALSE   (1)
#define C_TRUE    (2)
#define C_NUMBER  (3)
#define C_STRING  (4)
#define C_ARRAY   (5)
#define C_OBJECT  (6)
/** @} */

/**
 * @defgroup ColinkCjsonParseErrCode
 * @{
 */
#define C_PARSE_OK                            (0)
#define C_PARSE_NODATA                        (1)
#define C_PARSE_EXPECT_VALUE                  (2)
#define C_PARSE_INVALID_VALUE                 (3)
#define C_PARSE_ROOT_NOT_SINGULAR             (4)
#define C_PARSE_NUMBER_TOO_BIG                (5)
#define C_PARSE_MISS_QUOTATION_MARK           (6)
#define C_PARSE_INVALID_STRING_ESCAPE         (7)
#define C_PARSE_INVALID_STRING_CHAR           (8)
#define C_PARSE_INVALID_UNICODE_HEX           (9)
#define C_PARSE_INVALID_UNICODE_SURROGATE     (10)
#define C_PARSE_MISS_COMMA_OR_SQUARE_BRACKET  (11)
#define C_PARSE_MISS_KEY                      (12)
#define C_PARSE_MISS_COLON                    (13)
#define C_PARSE_MISS_COMMA_OR_CURLY_BRACKET   (14)
/** @} */

/**
 * @code 
 * 
 * json对象结构
 * 
 * This json library encapsulate all data types(including C_NULL,C_FALSE,C_TRUE,C_NUMBER,C_STRING,C_ARRAY,C_OBJECT) int32_to a cjson_value.
 * struct cjson_value:   root_object--------C_NULL
 *                                       |----C_FALSE/C_TRUE
 *                                       |----C_NUMBER
 *                                       |----C_STRING
 *                                       |----C_ARRAY-------C_NULL
 *                                                     |----C_FALSE/C_TRUE
 *                                                     |----C_NUMBER
 *                                                     |----C_STRING
 *                                                     |----C_ARRAY------ ......
 *                                                     |----C_OBJECT----- ......
 *                                                                       
 *        
 *                                       |----C_OBJECT------C_NULL
 *                                                     |----C_FALSE/C_TRUE
 *                                                     |----C_NUMBER
 *                                                     |----C_STRING
 *                                                     |----C_ARRAY------ ......
 *                                                     |----C_OBJECT----- ......
 * @endcode
 */ 

/**
 * @code
 * 
 * example
 * 
 * void cjson_test()
 * {
 *     char *buf = NULL; 
 *     char test_buf[512] = {"{\"action\":\"update\",\"deviceid\":\"100016a929\",\"apikey\":\"fbfaa76f-b05d-4298-b918-09e2e30e2373\",\"userAgent\":\"device\",\"ts\":0,\"params\":{\"rssi\":-44,\"power\":\"0\",\"switch\":\"off\"},\"from\":\"device\"}"};
 *
 *     uint32_t len = 0;
 *     cjson_value *test = NULL;
 *     cjson_value *test_array = NULL;
 *     cjson_value *test_object = NULL;
 *
 *     cjson_value *parse = NULL;
 *     cjson_value *val = NULL;
 *     cjson_value *num = NULL;
 *     int32_t ret;
 *
 *     int32_t i = 0,j = 0;
 *
 *     //cjson create data test......................................................
 *
 *     test = cjson_create_object(); 
 *     PLATFORM_DEBUG(DEBUG_JSON,TAG, "cjson_create_object ok"); 
 *     cjson_object_additem(test, (const char *)"1111", cjson_create_string((const char *)"2222", (uint32_t)5)); 
 *     PLATFORM_DEBUG(DEBUG_JSON,TAG, "cjson_object_additem ok"); 
 *     
 *     cjson_object_additem(test, (const char *)"aaaa", cjson_create_string((const char *)"bbbb", (uint32_t)4)); 
 *     PLATFORM_DEBUG(DEBUG_JSON,TAG, "cjson_object_additem ok"); 
 *     
 *     cjson_object_additem(test, (const char *)"5555", cjson_create_string((const char *)"6666", (uint32_t)4)); 
 *     PLATFORM_DEBUG(DEBUG_JSON,TAG, "cjson_object_additem ok"); 
 *
 *     test_array = cjson_create_array(); 
 *     cjson_array_additem(test_array, cjson_create_string((const char *)"7777", (uint32_t)4)); 
 *     cjson_array_additem(test_array, cjson_create_string((const char *)"8888", (uint32_t)4)); 
 *     cjson_array_additem(test_array, cjson_create_number(-23244343.23243232)); 
 *     cjson_array_additem(test_array, cjson_create_boolean(0)); 
 *     cjson_array_additem(test_array, cjson_create_boolean(1)); 
 *
 *     cjson_object_additem(test, (const char *)"array", test_array); 
 *    
 *     test_object = cjson_create_object(); 
 *     cjson_object_additem(test_object, (const char *)"xxxx", cjson_create_string((const char *)"yyyy", (uint32_t)4)); 
 *     PLATFORM_DEBUG(DEBUG_JSON,TAG, "cjson_object_additem ok"); 
 *     
 *     cjson_object_additem(test, (const char *)"object", test_object); 
 *  
 *
 *     buf = cjson_stringify(test, &len); 
 *     PLATFORM_DEBUG(DEBUG_JSON,TAG, "cjson_stringify ok"); 
 *
 *     PLATFORM_DEBUG(DEBUG_JSON,TAG, "buf:%s", buf); 
 *
 *     cFree(buf); 
 *
 *     cjson_free(&test);
 *
 *     //cjson parse data test................................................................
 * 
 *     parse = cjson_create_object();
 *     ret = cjson_parse(parse,test_buf);
 *     if (ret == C_PARSE_OK)
 *     {
 *         PLATFORM_DEBUG(DEBUG_JSON,TAG,"cjson parse ok");
 *         len = cjson_get_object_size(parse);
 *         for (i=0;i<len;i++)
 *         {
 *             //find object info By index
 *             buf = (char*)cjson_get_object_key((const cjson_value*)parse,(uint32_t)i);
 *             PLATFORM_DEBUG(DEBUG_JSON,TAG,"parse key[%d]:%s",i,buf);
 *             val = cjson_get_object_value(parse,i);
 *             if (val->type == C_STRING) 
 *             {
 *                 PLATFORM_DEBUG(DEBUG_JSON,TAG,"parse value[%d]:%s",i,val->u.s.s);
 *             }
 *             else if (val->type == C_ARRAY) 
 *             {
 *                 PLATFORM_DEBUG(DEBUG_JSON,TAG,"parse array value bein");
 *                 for (j=0;j<val->u.a.size;j++)
 *                 {
 *                     test_array = cjson_get_array_element(val,j);
 *                     if (test_array->type == C_STRING)
 *                     {
 *                         ;
 *                     }
 *                     else if (test_array->type == C_NUMBER)
 *                     {
 *                         ;
 *                     }
 *                     else if (test_array->type == C_TRUE || test_array->type == C_FALSE)
 *                     {
 *                         ;
 *                     }
 *                 }
 *             }
 *
 *             //find object info By key
 *             val = cjson_get_value_Bykey(parse,(const char*)"action");
 *             if (val != NULL)
 *             {
 *                 PLATFORM_DEBUG(DEBUG_JSON,TAG,"action:%s",val->u.s.s);
 *             }
 *         }
 *         buf = cjson_stringify(parse,&len);
 *         PLATFORM_DEBUG(DEBUG_JSON,TAG,"buf:%s",buf);
 *         cFree(buf);
 *     }
 *     cjson_free(&parse); 
 *
 * #endif
 * }
 * 
 * @endcode
 */

typedef struct cjson_value cjson_value;
typedef struct cjson_member cjson_member;

/**
 * 表示json数据的结构类型
 */
struct cjson_value{
	union {
        struct { cjson_member* m; uint32_t size; }o;   /**< 表示一个对象 */
        struct { cjson_value* e; uint32_t size; }a;    /**< 表示一个数组 */
        struct { char* s; uint32_t len; }s;            /**< 表示一个字符串 */
        double n;                                      /**< 表示一个整形或浮点行数据 */
    }u;
	int32_t type;                                      /**< 表示true、false、NULL类型 */
    cjson_value *next;                                 /**< 指向子对象的指针 */
};

/**
 * 表示json对象数据的结构类型 
 */
struct cjson_member
{
	char *k;                                           /**< 表示对象中的key */
	uint32_t klen;                                     /**< 表示对象中的key的长度 */
	
	cjson_value v;                                     /**< 表示对象中value */
    cjson_member *next;                                /**< 指向对象中的子对象 */
};

/**
 * @brief 解析json字符串。
 *
 * @par 描述:
 * 解析json字符串,解析成功后传入v数据对象中，数据处理完后，v对象需要调用cjson_free释放资源。
 *
 * @param v         [IN] json数据对象指针，NOT NULL。
 * @param json      [IN] json字符串，NOT NULL。
 *
 * @retval ColinkCjsonParseErrCode json解析返回值
 * @see ColinkCjsonParseErrCode
 */
int32_t cjson_parse(cjson_value* v, const char* json);

/**
 * @brief 生成json字符串。
 *
 * @par 描述:
 * 由v数据对象生成对应的json字符串。
 *
 * @param v         [IN]  json数据对象指针，NOT NULL。
 * @param length    [OUT] 输出json字符串的长度，若不需要，可写NULL。
 *
 * @retval NOT NULL         解析成功。
 * @retval NULL             解析失败。
 */
char* cjson_stringify(const cjson_value* v, uint32_t* length);

/**
 * @brief 释放json数据资源。
 *
 * @par 描述:
 * 释放json数据对象资源。
 *
 * @param v         [IN]  json数据对象指针的指针，NOT NULL。
 * 
 * @retval 无。
 */
void cjson_free(cjson_value **v);

/**
 * @brief 创建一个json对象。
 *
 * @par 描述:
 * 创建一个json对象。
 *
 * @param 无。
 * 
 * @retval NOT NULL    创建成功。
 * @retval NULL        创建失败。
 */
cjson_value *cjson_create_object(void);

/**
 * @brief 创建一个json数组对象。
 *
 * @par 描述:
 * 创建一个json数组对象。
 *
 * @param 无。
 * 
 * @retval NOT NULL    创建成功。
 * @retval NULL        创建失败。
 */
cjson_value *cjson_create_array(void);

/**
 * @brief 往对象中添加成员。
 *
 * @par 描述:
 * 往根对象中添加成员。
 *
 * @param object     [IN]  json根对象数据对象指针，NOT NULL。
 * @param key        [IN]  添加成员的key值，NOT NULL。
 * @param val        [IN]  添加成员的value值，NOT NULL。
 * 
 * @retval 无。
 */
void cjson_object_additem(cjson_value *object,const char *key,cjson_value *val);

/**
 * @brief 往数组对象中添加成员。
 *
 * @par 描述:
 * 往根数组对象中添加成员。
 *
 * @param object     [IN]  json根对象数据对象指针，NOT NULL。
 * @param val        [IN]  新的数组成员指针，NOT NULL。
 * 
 * @retval 无。
 */
void cjson_array_additem(cjson_value *object,cjson_value *val);

/**
 * @brief 创建一个json布尔值对象。
 *
 * @par 描述:
 * 创建一个json布尔值对象。
 *
 * @param b         [IN]  json数据布尔值，true or false。
 * 
 * @retval NOT NULL    创建成功。
 * @retval NULL        创建失败。
 */
cjson_value *cjson_create_boolean(int32_t b);

/**
 * @brief 创建一个json整形数或浮点数对象。
 *
 * @par 描述:
 * 创建一个json数据对象。
 *
 * @param b         [IN]  json整形数或浮点数。
 * 
 * @retval NOT NULL    创建成功。
 * @retval NULL        创建失败。
 */
cjson_value *cjson_create_number(double b);

/**
 * @brief 创建一个json字符串对象。
 *
 * @par 描述:
 * 创建一个json字符串对象。
 *
 * @param string    [IN]  json字符串。 
 * @param len       [IN]  json字符串长度。 
 * 
 * @retval NOT NULL    创建成功。
 * @retval NULL        创建失败。
 */
cjson_value *cjson_create_string(const char*string,uint32_t len);

/**
 * @brief 修改布尔值对象数值。
 *
 * @par 描述:
 * 修改对象布尔数值。
 *
 * @param v         [IN]  json对象指针。 
 * @param b         [IN]  ture or false。 
 * 
 * @retval 无。
 */
void cjson_set_boolean(cjson_value* v, int32_t b);   

/**
 * @brief 修改数值对象数值。
 *
 * @par 描述:
 * 修改对象数值。
 *
 * @param v         [IN]  json对象指针。 
 * @param n         [IN]  整形或浮点形。 
 * 
 * @retval 无。
 */
void cjson_set_number(cjson_value* v, double n);    

/**
 * @brief 修改字符串对象数值。
 *
 * @par 描述:
 * 修改对象字符串。
 *
 * @param v         [IN]  json对象指针。 
 * @param s         [IN]  字符串。 
 * @param len       [IN]  字符串长度。
 * 
 * @retval 无。
 */
void cjson_set_string(cjson_value* v, const char* s, uint32_t len);   

/**
 * @brief 获取json对象类型。
 *
 * @par 描述:
 * 获取json对象类型。
 *
 * @param v         [IN]  json对象指针。
 * 
 * @retval ColinkCjsonType json定义的数据类型
 * @see ColinkCjsonType
 */
int32_t cjson_get_type(const cjson_value* v);

/**
 * @brief 获取json尔布对象数据。
 *
 * @par 描述:
 * 获取json布尔对象数据。
 *
 * @param v         [IN]  json对象指针。
 * 
 * @retval true or false。
 */
int32_t cjson_get_boolean(const cjson_value* v);

/**
 * @brief 获取json数值对象数据。
 *
 * @par 描述:
 * 获取json数值对象数据。
 *
 * @param v         [IN]  json对象指针。
 * 
 * @retval 返回实际数值。
 */
double cjson_get_number(const cjson_value* v);

/**
 * @brief 获取json字符串对象数据。
 *
 * @par 描述:
 * 获取json字符串对象数据。
 *
 * @param v         [IN]  json对象指针。
 * 
 * @retval 返回实际字符串。
 */
const char* cjson_get_string(const cjson_value* v);

/**
 * @brief 获取json字符串对象中字符串的长度。
 *
 * @par 描述:
 * 获取json字符串对象中字符串的长度。
 *
 * @param v         [IN]  json对象指针。
 * 
 * @retval 返回长度值。
 */
uint32_t cjson_get_string_length(const cjson_value* v);

/**
 * @brief 获取json数组对象中数组的长度。
 *
 * @par 描述:
 * 获取json数组对象中数组的长度。
 *
 * @param v         [IN]  json对象指针。
 * 
 * @retval 返回长度值。
 */
uint32_t cjson_get_array_size(const cjson_value* v);

/**
 * @brief 根据索引值，获取json数组对象中某个成员的对象。
 *
 * @par 描述:
 * 获取某个json数组成员的对象。
 *
 * @param v         [IN]  json对象指针。
 * @param index     [IN]  索引值，从0开始。
 * 
 * @retval 返回成员对象。
 */
cjson_value* cjson_get_array_element(const cjson_value* v, uint32_t index);

/**
 * @brief 获取json对象中子对象的个数。
 *
 * @par 描述:
 * 获取json对象中子对象的个数。
 *
 * @param v         [IN]  json对象指针。
 * 
 * @retval 返回个数。
 */
uint32_t cjson_get_object_size(const cjson_value* v);

/**
 * @brief 根据索引值，获取json对象中某个子对象的key值。
 *
 * @par 描述:
 * 获取某个json对象成员的key值。
 *
 * @param v         [IN]  json对象指针。
 * @param index     [IN]  索引值，从0开始。
 * 
 * @retval 返回json对象的key值。
 */
const char* cjson_get_object_key(const cjson_value* v, uint32_t index);

/**
 * @brief  根据索引值，获取json对象中某个子对象的key值
 *         的长度。
 *
 * @par 描述:
 * 获取某个json对象成员的key值长度。
 *
 * @param v         [IN]  json对象指针。
 * @param index     [IN]  索引值，从0开始。
 * 
 * @retval 返回json对象的key值长度。
 */
uint32_t cjson_get_object_key_length(const cjson_value* v, uint32_t index);

/**
 * @brief 根据索引值，获取json对象中某个子对象的value值。
 *
 * @par 描述:
 * 获取某个json对象成员的对象。
 *
 * @param v         [IN]  json对象指针。
 * @param index     [IN]  索引值，从0开始。
 * 
 * @retval 返回json对象的对象指针。
 */
cjson_value *cjson_get_object_value(const cjson_value* v, uint32_t index);

/**
 * @brief 根据key值，获取json对象中某个子对象的对象。
 *
 * @par 描述:
 * 获取某个json对象成员的对象。
 *
 * @param v         [IN]  json对象指针。
 * @param index     [IN]  索引值，从0开始。
 * 
 * @retval 返回json对象的对象指针。
 */
cjson_value *cjson_get_value_Bykey(const cjson_value *v,const char *str);

/**
 * @brief 根据key值，删除json对象中某个子对象的对象。
 *
 * @par 描述:
 * 删除某个json对象成员的对象。
 *
 * @param v         [IN]  json对象指针。
 * @param index     [IN]  索引值，从0开始。
 * 
 * @retval 返回json对象的对象指针。
 */
void cjson_delete_value_bykey(cjson_value *v,const char *str);

#endif  /* __COLINK_CJSON_H */
