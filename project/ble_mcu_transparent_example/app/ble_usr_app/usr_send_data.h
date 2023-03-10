#ifndef __USR_SEND_DATA_H__
#define __USR_SEND_DATA_H__

//#define ENABLE_DATA_TRANS_USE_UUID_16_LEN

#ifdef ENABLE_DATA_TRANS_USE_UUID_16_LEN
#define DATA_TRANS_SVR_UUID 0xff00
#define DATA_TRANS_RX_UUID  0xff01
#define DATA_TRANS_TX_UUID  0xff02

#define UUID_16_DECLARATION(X)  (X)
typedef  struct attm_desc data_trans_attm_desc;
#else
//#define DATA_TRANS_SVR_UUID 0xdd,0xdd,0xb4,0xf8,0xcd,0xf3,0x45,0xe9,0xa3,0x2f,0x2a,0x2a,0xe2,0xdb,0xcc,0xe4   //ccddb4f8-cdf3-11e9-a32f-2a2ae2dbcce4
#define DATA_TRANS_SVR_UUID     {0x85,0x41,0xDC,0x24,0x0E,0xE5,0xA9,0xE0,0x93,0xF3,0xA3,0xB5,0x01,0x00,0x40,0x6E}
#define DATA_TRANS_RX_UUID      {0xff,0xcc,0xdb,0xe2,0x2a,0x2a,0x32,0xa3,0xe9,0x11,0x67,0xcd,0x4a,0x4a,0xbe,0x28} //28be4a4a-cd67-11e9-a32f-2a2ae2dbcce4
#define DATA_TRANS_TX_UUID      {0xc1,0xff,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01,0x00}//28be4cb6-cd67-11e9-a32f-2a2ae2dbcce4

#define UUID_16_DECLARATION(X)  {X&0xff, (X>>8)&0xff}
typedef  struct attm_desc_128 data_trans_attm_desc;
#endif

#define DATA_TRANS_MAX_LEN 1024

enum trans_data_svc {
    DATA_TRANS_DECL_SVC = 0,
    DATA_TRANS_DECL_CHAR_RX,
    DATA_TRANS_CHAR_VAL_RX,
    DATA_TRANS_DECL_CHAR_TX,
    DATA_TRANS_CHAR_VAL_TX,
    DATA_TRANS_CLIENT_CHAR_CFG_TX,

    DATA_TRANS_IDX_MAX,
};

enum {
    SVC_PREM_TYPE_UUID16_MASK = 0x00,
    SVC_PREM_TYPE_UUID32_MASK = 0x01,
    SVC_PREM_TYPE_UUID128_MASK = 0x02,
};

#define HANDLE_DATA_TRANS_SERVER_START          0x0000
#define HANDLE_DATA_TRANS_DECL_CHAR_RX          0x0001
#define HANDLE_DATA_TRANS_CHAR_VAL_RX           0x0002
#define HANDLE_DATA_TRANS_DECL_CHAR_TX          0x0003
#define HANDLE_DATA_TRANS_CHAR_VAL_TX           0x0004
#define HANDLE_DATA_TRANS_CLIENT_CHAR_CFG_TX    0x0005
#define HANDLE_DATA_TRANS_SERVER_END            0xFFFF

/*
 * Configuration Flag, each bit matches with an attribute of
 * att_db (Max: 32 attributes); if the bit is set to 1, the
 * attribute will be added in the service
*/
#define DATA_TRANS_CONFIG_MASK         (0x1FFF)

void  data_trans_svc_add(void);


#endif /* __USR_APP_H__ */



