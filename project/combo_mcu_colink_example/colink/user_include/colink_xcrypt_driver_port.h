#ifndef __COLINK_XCRYPT_DRIVER_PORT_H__
#define __COLINK_XCRYPT_DRIVER_PORT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos_common.h"
#include "usr_app.h"
#include "mbedtls/md5.h"
#include "mbedtls/aes.h"
#include "mbedtls/sha256.h"
#include "mbedtls/cipher.h"
#include "mbedtls/base64.h"

#include "colink_define.h"

#define		AES_CBC_ARRAY_DEFAULT_VALUE					0
#define		AES_CBC_IV_VALUE							48	

#define		AES_CBC_IV_LENGTH							16
#define		AES_CBC_KEY_MD5_LENGTH						48		
#define		AES_CBC_APP_SRC_CODE_LENGTH					48		
#define		AES_CBC_DEVICE_SRC_CODE_LENGTH				48		
#define		AES_CBC_DEVICE_SECRET_CODE_LENGTH			65		
#define		AES_CBC_AUTH_SRC_CODE_LENGTH				128	

#define 	LN_COLINK_MALLOC(addr, l, val)		do{\
													if (addr != NULL){\
														OS_Free(addr);\
													}\
													addr = OS_Malloc(l);\
													memset(addr, val, l);\
												}while(0)
																				
	
typedef struct
{
	uint8_t colinkId[COLINK_ID_LEN];
	uint8_t colinkApikey[COLINK_APIKEY_LEN];
	uint8_t colinkChipid[COLINK_CHIPID_LEN];
	char colinkBleName[20];
	char colinkSoftapName[20];
	
	uint8_t *iv;
	
	void (*init)(void);
	void (*uuid)(char *pBuf);
	void (*md5)(uint8_t *_src, uint8_t *_dst);
	void (*sha256)(uint8_t *_src, uint8_t *_dst);
	void (*encrypt)(uint8_t *_src, uint8_t* _key, uint8_t *_dst);
	void (*decrypt)(uint8_t *_src, uint8_t* _key, uint8_t *_dst);
}COLINK_XCRYPT_DRIVER_ADAPTER;

extern COLINK_XCRYPT_DRIVER_ADAPTER* get_colink_xcrypt_handle(void);

#ifdef __cplusplus
}
#endif

#endif /* __COLINK_XCRYPT_DRIVER_PORT_H__ */

