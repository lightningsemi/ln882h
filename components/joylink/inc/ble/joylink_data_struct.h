#ifndef __JOYLINK_DATA_STRUCT_H__
#define __JOYLINK_DATA_STRUCT_H__

#include "list.h"
#include "joylink_ble_sdk.h"

//application work mode
#define JL_WORK_MODE_DEFAULT                  (0)        //standard BLE device,只保留这个
#define JL_WORK_MODE_NETCONFIG                (1)        //wifi net config
#define JL_WORK_MODE_CELLULAR_ACTIVE          (2)        //cellular device active
#define JL_WORK_MODE_LPA                      (3)        //LPA
#define JL_WORK_MODE_NEW_NETCONFIG            (4)        //new wifi net config
#define JL_WORK_MODE_NETCONFIG_SELF_ACTIVATE  (5)        //new wifi net config, dev activate itself

//key type
#define JL_KEY_TYPE_PLAIN                    (0x00)
#define JL_KEY_TYPE_PSK                      (0x01)
#define JL_KEY_TYPE_ECDH                     (0x02)
#define JL_KEY_TYPE_PSK_MIXER                (0x03)
#define JL_KEY_TYPE_PSK_SESSION              (0x04) //with KEY_TYPE_PSK
#define JL_KEY_TYPE_ECDH_SESSION             (0x05) //with KEY_TYPE_ECDH

//security level
#define JL_SECURITY_LEVEL_0                  JL_KEY_TYPE_PLAIN
#define JL_SECURITY_LEVEL_1                  JL_KEY_TYPE_PSK
#define JL_SECURITY_LEVEL_2                  JL_KEY_TYPE_ECDH
#define JL_SECURITY_LEVEL_3                  JL_KEY_TYPE_PSK_MIXER //for WeChat APP

#define JL_WORK_MODE                         JL_WORK_MODE_NETCONFIG_SELF_ACTIVATE

#define JL_SECURITY_LEVEL                    JL_SECURITY_LEVEL_0
//JL_SECURITY_LEVEL说明：数据安全级别，普通设备使用默认值0，如果是门锁、保险箱等设备而且ram、rom资源充足的话使用2，资源不足使用1

//#define JL_SEND_DATA_BY_THREAD
//JL_SEND_DATA_BY_THREAD说明：
//蓝牙芯片发送数据后底层没有send confirm事件，编译时需要放开这个宏，
//放开这个宏之后用户就不需要调用jl_send_confirm接口了，将使用线程函数jl_send_thread发送数据

//auth level
#define JL_APP_AUTH                          (0x00)
#define JL_DEV_AUTH                          (0x01)
#define JL_CLOUD_DEV_AUTH                    (0x02)

#define JL_BLE_SDK_VERSION                    "2.0.8"
#define JL_PROTOCOL_VERSION                  (0)
#define JL_AUTH_MODE                         (JL_CLOUD_DEV_AUTH)

//operate
#define JL_OPERATE_PHONE_READ               (0x01)   //phone read device
#define JL_OPERATE_DEVICE_READ_RESPON       (0x11)   //device return data to phone
#define JL_OPERATE_PHONE_WRITE_NR           (0x02)   //phone write device without response
#define JL_OPERATE_PHONE_WRITE              (0x03)   //phone write device with response
#define JL_OPERATE_DEVICE_WRITE_RESPON      (0x13)   //device return response of write cmd

#define JL_OPERATE_DEVICE_SEND_NR           (0x16)   //device send data to phone without response
#define JL_OPERATE_DEVICE_SEND              (0x17)   //device send data to phone with response
#define JL_OPERATE_PHONE_RESPON             (0x07)   //phone return response to device

#define JL_OPERATE_DEVICE_SEND_CLOUD_NR     (0x36)   //device send data to phone and cloud without response
#define JL_OPERATE_DEVICE_SEND_CLOUD        (0x37)   //device send data to phone and cloud with response

#define JL_OPERATE_DEVICE_ERROR_RESPON      (0xFF)   //error response


#define JL_OFFSET_AD_CHIPINFO1                  (12)
#define JL_OFFSET_AD_CHIPINFO2                  (13)

#define JL_PACKET_HDR_LEN                       (4)
#define JL_PACKET_CRC_LEN                       (2)
#define JL_PROPERTY_HDR_LEN                     (3)
#define JL_FRAME_LEN                            (20)
#define JL_FRAME_HDR_LEN                        (4)
#define JL_BLE_PAYLOAD_LEN                      (16)
#define JL_FEEDID_LEN                           (8)
#define JL_FEEDID_STR_LEN                       (18)
#define JL_ECDH_KEY_LEN                         (16)
#define JL_ECC_SHAREKEY_LEN                     (32)
#define JL_AUTH_RNAD_LEN                        (32)
#define JL_MAX_AUTH_RNAD_LEN                    (33)
#define JL_IDT_C_SIG_BIN_LEN                    (64)
#define JL_AUTH_DATA_LEN                        (16)
#define JL_SHARED_KEY_LEN                       (16)
#define JL_ACCESS_KEY_LEN                       (32)
#define JL_SESSION_KEY_LEN                      (16)
#define JL_EXPRESSED_KEY_LEN                    (176)
#define JL_OTA_FIRMWARE_CODE_LEN                (32)

#define JL_CLOUD_PUBKEY_LEN                     (66)

#define JL_ECC_PUBKEY_LEN                       (64)
#define JL_ECC_PRIKEY_LEN                       (32)
#define JL_ECC_PUBKEY_PRESS_LEN                 (33)

#define JL_MAC_STR_LEN                          (12)
#define JL_MAC_LEN                              (6)
#define JL_PRODUCT_UUID_LEN                     (6)
#define JL_LICENSE_LEN                          (32)
#define JL_MANUFACTURE_DATA_LEN                 (14)

#define JL_ERR_CRC_CHECK                        (-201)
#define JL_ERR_UNSUPPORT_KEY                    (-202)
#define JL_ERR_NUM_UNORDER                      (-203)
#define JL_ERR_NUM_OVERFLOW                     (-204)
#define JL_ERR_NUM_SEQ                          (-205)
#define JL_ERR_OVER_TIME                        (-206)
#define JL_ERR_OVER_BUFLEN                      (-207)
#define JL_ERR_FRAMELEN_ERROE                   (-208)
#define JL_ERR_BUF_NULL                         (-209)

#define JL_RECEIVE_WAIT  (0)
#define JL_RECEIVE_END   (1)

#define JL_GET_SSID_FLAG      (1<<0)
#define JL_GET_PASSWORD_FLAG  (1<<1)
#define JL_GET_TOKEN_FLAG     (1<<2)
#define JL_GET_URL_FLAG       (1<<3)

typedef struct{
    uint8_t sdk_init_flag;

    uint8_t product_uuid[JL_PRODUCT_UUID_LEN + 1];
    uint8_t mac[JL_MAC_LEN];
    uint8_t security_level;

#if ((JL_SECURITY_LEVEL == JL_SECURITY_LEVEL_2) || (JL_SECURITY_LEVEL == 1))
    uint8_t access_key[JL_ACCESS_KEY_LEN];
#endif
    uint8_t get_data_flag;
    uint8_t seq_up;

#if (JL_SECURITY_LEVEL == 1)
    uint8_t  shared_key[JL_SHARED_KEY_LEN+1];
#endif

#if (JL_SECURITY_LEVEL == JL_SECURITY_LEVEL_2)
    uint8_t dev_pub_key[JL_ECC_PUBKEY_LEN];
    uint8_t dev_priv_key[JL_ECC_PRIKEY_LEN];
    uint8_t ecdh_key[JL_ECDH_KEY_LEN];
#endif

#if (JL_SECURITY_LEVEL != JL_SECURITY_LEVEL_0)
    uint8_t share_expressed_key[JL_EXPRESSED_KEY_LEN];
#endif
    uint8_t wifi_connect;
}jl_datum_t;

typedef struct{
    uint8_t count;
    uint8_t num;
    uint8_t keytype;
    uint8_t seq;
}jl_frame_hdr_t;

typedef struct{
    jl_frame_hdr_t hdr;
    uint8_t payload[16];
}jl_frame_data_t;

typedef struct{
    uint16_t tag;
    uint8_t  len;
    uint8_t  *value;
}jl_property_t;

typedef  struct
{
    uint8_t seq;
    uint8_t operation;
    uint16_t len;
    uint8_t *content;
    //uint16_t crc;   //because content's length is not solid
}jl_packet_t;

typedef struct{
    uint16_t frame_count;
    uint16_t last_frame;
    uint8_t  key_type;
    uint8_t *buffer;
    struct list_head node;
}jl_packet_node_t;

typedef struct{
    uint8_t *buffer;
    struct list_head node;
}jl_property_node_t;

typedef struct{
    uint8_t  operate;
    uint16_t property_len;
    struct list_head property_list;
    struct list_head node;
}jl_packet_handle_t;

typedef struct{
    struct list_head packet_list;
    struct list_head handle_list;
#ifdef JL_SEND_DATA_BY_THREAD
    jl_semaphore_t     send_sem;
    jl_mutex_t send_mutex;
#endif
    uint8_t *recv_buffer;
    uint8_t recv_frame_count;
    uint8_t recv_last_frame;
}jl_packet_ctrl_t;

#endif
