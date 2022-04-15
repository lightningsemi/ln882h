/* --------------------------------------------------
 * @brief: 
 *
 * @version: 1.0
 *
 * @date: 08/01/2018
 * 
 * @desc: Functions in this file must be implemented by the device developers when porting the Joylink SDK.
 *
 * --------------------------------------------------
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "joylink.h"
#include "joylink_extern.h"
#include "joylink_extern_json.h"

#include "joylink_memory.h"
#include "joylink_socket.h"
#include "joylink_string.h"
#include "joylink_stdio.h"
#include "joylink_stdint.h"
#include "joylink_log.h"
#include "joylink_time.h"
#include "joylink_thread.h"
#include "joylink_extern_ota.h"
#include "joylink_ble_sdk.h"

#include "ln_kv_key_def.h"
#include "ln_kv_err.h"
#include "ln_joylink.h"
#include "wifi.h"
#include "netif/ethernetif.h"

const jl2_d_idt_t user_idt = 
{
    .type = 0,
    .cloud_pub_key = IDT_CLOUD_PUB_KEY,

    .sig = "01234567890123456789012345678901",
    .pub_key = "01234567890123456789012345678901",

    .f_sig = "01234567890123456789012345678901",
    .f_pub_key = "01234567890123456789012345678901",
};

user_dev_status_t user_dev = 
{
    .Power = 1,
};

user_dev_status_t virtual_user_dev = 
{
    .Power = 1,
};


/**
 * @brief: 用以返回一个整型随机数
 *
 * @param: 无
 *
 * @returns: 整型随机数
 */
int
joylink_dev_get_random()
{
    //TODO: 适配硬件真随机数接口(TRNG)
    static unsigned long int next = 1;
    next = next *1103515245 + 12345;
    return (int)(next/65536) % (1134);
}

/**
 * @brief: 返回是否可以访问互联网
 *
 * @returns: E_JL_TRUE 可以访问, E_JL_FALSE 不可访问
 */

E_JLBOOL_t joylink_dev_is_net_ok()
{
    int ret = 0;

    if (wifi_current_mode_get() == WIFI_MODE_STATION) {
        ret = netdev_get_link_state(0);
        if (NETDEV_LINK_UP == ret) {
            return E_JL_TRUE;
        }
    }

    return E_JL_FALSE;
}

/**
 * @brief: 此函数用作通知应用层设备与云端的连接状态.
 *
 * @param: st - 当前连接状态  0-Socket init, 1-Authentication, 2-Heartbeat
 *
 * @returns: 
 */
E_JLRetCode_t
joylink_dev_set_connect_st(int st)
{
    static int8_t iot_st = 0;
    JLPInfo_t *jlp = NULL;

    if (iot_st == 0) {
        iot_st = 1;

        if (NULL == (jlp = OS_Malloc(sizeof(JLPInfo_t)))) {
            log_info("jlp OS_Malloc failed!\r\n");
            return E_RET_ERROR;
        }

        joylink_dev_get_jlp_info(jlp);
        ln_send_net_config_state(E_JL_NET_CONF_ST_IOT_CONNECT_SUCCEED, jlp->feedid, jl_platform_strlen(jlp->feedid));
        ln_send_net_config_state(E_JL_NET_CONF_ST_CLOUD_CONNECT_SUCCEED, NULL, 0); 
        ln_stop_ble_netconfig();       //关闭蓝牙连接并关闭广播
        OS_Free(jlp);
    }

    log_info("connect status:%d(0-Socket init, 1-Authentication, 2-Heartbeat)\r\n", st);

    return E_RET_OK;
}


/**
 * @brief: 通知设备已经连上端并认证通过，已经可以上报数据。
 *
 * @returns:
 */
int joylink_server_notify_auth_ok(void)
{
    //TODO:
    //joylink_lock_event_report_unlock_info(NULL, 1, 1, NULL);
    return 1;
}

/**
 * @brief: 传出激活信息
 *
 * @param[in] message: 激活信息
 * 
 * @returns: 0:设置成功
 */
E_JLRetCode_t joylink_dev_active_message(char *message)
{
    //TODO:
    log_info("message = %s\r\n");
    return 0;
}

/**
 * @brief: 存储JLP(Joylink Parameters)信息,将入参jlp结构中的信息持久化存储,如文件、设备flash等方式
 *
 * @param [in]: jlp-JLP structure pointer
 *
 * @returns: 
 */
E_JLRetCode_t
joylink_dev_set_attr_jlp(JLPInfo_t *jlp)
{
    if (NULL == jlp) {
        return E_RET_ERROR;
    }

    int ret = E_RET_ERROR;

    ret = ln_kv_set(KV_JOYLINK_INFO_CFG, (void *)jlp, sizeof(JLPInfo_t));
    if (ret != KV_ERR_NONE) {
        return E_RET_ERROR;
    }

    log_info("save JLPInfo_t into kv ok!\r\n");

    ret = E_RET_OK;
    return ret;
}

/**
 * @brief: 设置设备认证信息
 *
 * @param[out]: pidt--设备认证信息结构体指针,需填入必要信息sig,pub_key,f_sig,f_pub_key,cloud_pub_key
 *
 * @returns: 返回设置成功或失败
 */
E_JLRetCode_t
joylink_dev_get_idt(jl2_d_idt_t *pidt)
{
    if (NULL == pidt) {
        return E_RET_ERROR; 
    }

    pidt->type = 0;
    jl_platform_strcpy(pidt->sig,           user_idt.sig);
    jl_platform_strcpy(pidt->pub_key,       user_idt.pub_key);
    //jl_platform_strcpy(pidt->rand,        user_idt.rand); //TODO:
    jl_platform_strcpy(pidt->f_sig,         user_idt.f_sig);
    jl_platform_strcpy(pidt->f_pub_key,     user_idt.f_pub_key);
    jl_platform_strcpy(pidt->cloud_pub_key, user_idt.cloud_pub_key);

    return E_RET_OK;
}

/**
 * @brief: 此函数需返回设备的MAC地址
 *
 * @param[out] out: 将设备MAC地址赋值给此参数
 * 
 * @returns: E_RET_OK 成功, E_RET_ERROR 失败
 */
E_JLRetCode_t
joylink_dev_get_user_mac(char *out)
{    
    char * mac = ln_get_mac_array();
    sprintf(out,"%02X%02X%02X%02X%02X%02X",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    return E_RET_OK;
}

/**
 * @brief: 此函数需返回设备私钥private key,该私钥可从小京鱼后台获取
 *
 * @param[out] out: 将私钥字符串赋值给该参数返回
 * 
 * @returns: E_RET_OK:成功, E_RET_ERROR:发生错误
 */
E_JLRetCode_t
joylink_dev_get_private_key(char *out)
{
    jl_platform_strcpy(out, LN_CLOUD_PRIVATE_KEY);
    return E_RET_OK;
}

/**
 * @brief: 从永久存储介质(文件或flash)中读取jlp信息,并赋值给参数jlp,其中feedid, accesskey,localkey,joylink_server,server_port必须正确赋值
 *
 * @param[out] jlp: 将JLP(Joylink Parameters)读入内存,并赋值给该参数
 *
 * @returns: E_RET_OK:成功, E_RET_ERROR:发生错误
 */
E_JLRetCode_t
joylink_dev_get_jlp_info(JLPInfo_t *jlp)
{
    if (NULL == jlp) {
        return E_RET_ERROR;
    }

    int value_len = 0;
    jl_platform_memset(jlp, 0, sizeof(JLPInfo_t));

    /* get it from KV */
    {
        if (ln_kv_has_key(KV_JOYLINK_INFO_CFG)) {
            if (KV_ERR_NONE != ln_kv_get(KV_JOYLINK_INFO_CFG, (void *)jlp, sizeof(JLPInfo_t), &value_len)) {
                log_error("get JLPInfo_t from kv error!\r\n");
            }
        } else {
            log_error("JLPInfo_t kv never set!\r\n");
        }
        
        /*
         * jlp.feedid
         * jlp.accesskey
         * jlp.localkey
         * jlp.joylink_server
         * jlp.server_port
         * jlp.domain
         * jlp.token
        */
    }

    /* get it from local const data */
    {
        joylink_dev_get_user_mac(jlp->mac);
        joylink_dev_get_private_key(jlp->prikey);

        jl_platform_strcpy(jlp->modelCode, JLP_CHIP_MODEL_CODE);
        jl_platform_strcpy(jlp->uuid, JLP_UUID);
        jlp->is_actived      = E_JL_TRUE;
        jlp->model_code_flag = E_JL_FALSE;
        jlp->version         = JLP_VERSION;
        jlp->devtype         = JLP_DEV_TYPE;
        jlp->lancon          = JLP_LAN_CTRL;
        jlp->cmd_tran_type   = JLP_CMD_TYPE;
        jlp->noSnapshot      = JLP_SNAPSHOT;
    }

    log_info("get JLPInfo_t (mac:%s, prikey:%s)\r\n", jlp->mac, jlp->prikey);
    return E_RET_OK;
}

/**
 * @brief: 返回设备状态,通过填充user_data参数,返回设备当前状态
 *
 * @param[out] user_data: 设备状态结构体指针
 * 
 * @returns: 0
 */
int
joylink_dev_user_data_get(user_dev_status_t *user_data)
{
    /**
    *FIXME:must to do
    */
    user_data->Power = virtual_user_dev.Power;
    return 0;
}

/**
 * @brief: 获取设备快照json结构,结构中包含返回状态码
 *
 * @param[in] ret_code: 返回状态码
 * @param[out] out_snap: 序列化为字符串的设备快照json结构
 * @param[in] out_max: out_snap可写入的最大长度
 *
 * @returns: 实际写入out_snap的数据长度
 */
int
joylink_dev_get_snap_shot_with_retcode(int32_t ret_code, char *out_snap, int32_t out_max)
{
    if (NULL == out_snap || out_max < 0) {
        return 0;
    }
    /**
    *FIXME:must to do
    */
    int len = 0;

    joylink_dev_user_data_get(&user_dev);

    char *packet_data = joylink_dev_package_info(ret_code, &user_dev);
    if (NULL !=  packet_data) {
        len = jl_platform_strlen(packet_data);
        log_info("------>%s:len:%d\r\n", packet_data, len);
        if (len < out_max) {
            jl_platform_memcpy(out_snap, packet_data, len); 
        } else {
            len = 0;
        }
    }

    if (NULL !=  packet_data) {
        jl_platform_free(packet_data);
    }
    return len;
}

/**
 * @brief: 获取设备快照json结构
 *
 * @param[out] out_snap: 序列化为字符串的设备快照json结构
 * @param[in] out_max: out_snap可写入的最大长度
 *
 * @returns: 实际写入out_snap的数据长度
 */
int
joylink_dev_get_snap_shot(char *out_snap, int32_t out_max)
{
    return joylink_dev_get_snap_shot_with_retcode(0, out_snap, out_max); 
}

/**
 * @brief: 获取向App返回的设备快照json结构
 *
 * @param[out] out_snap: 序列化为字符串的设备快照json结构
 * @param[in] out_max: out_snap允许写入的最大长度
 * @param[in] code: 返回状态码
 * @param[in] feedid: 设备的feedid
 *
 * @returns: 
 */
int
joylink_dev_get_json_snap_shot(char *out_snap, int32_t out_max, int code, char *feedid)
{
    /**
     *FIXME:must to do
     */
    jl_platform_sprintf(out_snap, "{\"code\":%d, \"feedid\":\"%s\"}", code, feedid);

    return jl_platform_strlen(out_snap);
}

/**
 * @brief: 通过App控制设备,需要实现此函数,根据传入的json_cmd对设备进行控制
 *
 * @param[in] json_cmd: 设备控制命令
 *
 * @returns: E_RET_OK:控制成功, E_RET_ERROR:发生错误 
 */
E_JLRetCode_t 
joylink_dev_lan_json_ctrl(const char *json_cmd)
{
    /**
     *FIXME:must to do
     */
    log_debug("json ctrl:%s\r\n", json_cmd);

    return E_RET_OK;
}

/**
 * @brief: 根据传入的cmd值,设置对应设备属性
 *
 * @param[in] cmd: 设备属性名称
 * @param[out] user_data: 设备状态结构体
 * 
 * @returns: E_RET_OK 设置成功
 */
E_JLRetCode_t
joylink_dev_user_data_set(char *cmd, user_dev_status_t *user_data)
{
    /**
    *FIXME:must to do
    */
    if (strcmp(cmd,LN_USER_DATA_POWER) == 0) {
        virtual_user_dev.Power = user_data->Power;
    }

    log_info("\r\n###############################################\r\njoylink_dev_user_data_set power = %d\r\n###############################################\r\n",virtual_user_dev.Power);
    return E_RET_OK;
}

/**
 * @brief:根据src参数传入的控制命令数据包对设备进行控制.调用joylink_dev_parse_ctrl进行控制命令解析,并更改设备属性值
 *
 * @param[in] src: 控制指令数据包
 * @param[in] src_len: src长度
 * @param[in] ctr: 控制码
 * @param[in] from_server: 是否来自server控制 0-App,2-Server
 *
 * @returns: E_RET_OK 成功, E_RET_ERROR 失败
 */

E_JLRetCode_t 
joylink_dev_script_ctrl(const char *src, int src_len, JLContrl_t *ctr, int from_server)
{
    if (NULL == src || NULL == ctr) {
        return E_RET_ERROR;
    }

    /**
    *FIXME:must to do
    */
    int ret = -1;
    ctr->biz_code = (int)(*((int *)(src + 4)));
    ctr->serial = (int)(*((int *)(src +8)));

    uint32_t tt = jl_get_time_second(NULL);
    log_info("bcode:%d:server:%d:time:%ld\r\n", ctr->biz_code, from_server,(long)tt);

    if(ctr->biz_code == JL_BZCODE_GET_SNAPSHOT){
        /*
        *Nothing to do!
        */
        ret = E_RET_OK;
    } else if (ctr->biz_code == JL_BZCODE_CTRL) {
        joylink_dev_parse_ctrl(src + 12, &user_dev);
        return E_RET_OK;
    } else if (ctr->biz_code == JL_BZCODE_MENU) {
        joylink_dev_parse_ctrl(src + 12, &user_dev);
        return E_RET_OK;
    } else {
        log_error("Unknown biz_code:%d\r\n", ctr->biz_code);
    }
    return ret;
}

/**
 * @brief: 实现接收到ota命令和相关参数后的动作,可使用otaOrder提供的参数进行具体的OTA操作
 *
 * @param[in] otaOrder: OTA命令结构体
 *
 * @returns: E_RET_OK 成功, E_RET_ERROR 发生错误
 */
E_JLRetCode_t
joylink_dev_ota(JLOtaOrder_t *otaOrder)
{
    jl_thread_t task_id;

    if (NULL == otaOrder) {
        return E_RET_ERROR;
    }

    joylink_set_ota_info(otaOrder);

    task_id.thread_task = (threadtask) joylink_ota_task;
    task_id.stackSize   = 15*1024; //TODO:???为什么需要这么大的任务栈空间？
    task_id.priority    = JL_THREAD_PRI_DEFAULT; //TODO:任务优先级整改
    task_id.parameter   = (void *)otaOrder;
    jl_platform_thread_start(&task_id);

    log_info("creak ota task\r\n");

    return E_RET_OK;
}


/**
 * @brief: OTA执行状态上报,无需返回值
 */
void
joylink_dev_ota_status_upload()
{
    //TODO:
    return;	
}

/**
 * @brief: SDK main loop 运行状态报告,正常情况下此函数每5秒会被调用一次,可以用来判断SDK主任务的运行状态.
 * 
 * @param[in] status: SDK main loop运行状态 0正常, -1异常
 * 
 * @return: reserved 当前此函数仅做通知,调用方不关心返回值.
 */

int joylink_dev_run_status(JLRunStatus_t status)
{
    int ret = -1;
    /**
         *FIXME:must to do
    */
    log_info("dev run status:%d\r\n", status);
    return ret;
}

/**
 * @brief: 每间隔1个main loop周期此函数将在SDK main loop中被调用,让用户有机会将代码逻辑运行在核心任务中.
 * 
 * @note:  正常情况下一个main loop周期为1s(取决于socket等待接收数据的timeout时间),但不保证精度,请勿用作定时器
 * @note:  仅用作关键的非阻塞任务执行,例如OTA状态上报或设备状态上报.
 * @note:  执行阻塞或耗时较多的代码,将会妨碍主task运行.
 */
void joylink_dev_run_user_code()
{
    //You may add some code run in the main loop if necessary.
}
