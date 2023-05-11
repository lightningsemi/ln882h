#include "colink_xcrypt_driver_port.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/debug/log.h"
#include "usr_ctrl.h"

static void ln_xcrypt_init(void);
static void ln_random_uuid(char *pBuf);
static void ln_md5_encrypt(uint8_t *_src, uint8_t *_dst);
static void ln_sha256_encrypt(uint8_t *_src, uint8_t *_dst);
static void ln_aes_cbc_pkcs7_128_encrypt(uint8_t *_src, uint8_t* _key, uint8_t *_dst);
static void ln_aes_cbc_pkcs7_128_decrypt(uint8_t *_src, uint8_t* _key, uint8_t *_dst);

static COLINK_XCRYPT_DRIVER_ADAPTER s_colinkXcryptDrvAdapter = {
	.iv = NULL,
	
	.init = ln_xcrypt_init,
	.uuid = ln_random_uuid,
	.md5 = ln_md5_encrypt,
	.sha256 = ln_sha256_encrypt,
	.encrypt = ln_aes_cbc_pkcs7_128_encrypt,
	.decrypt = ln_aes_cbc_pkcs7_128_decrypt
};

COLINK_XCRYPT_DRIVER_ADAPTER* get_colink_xcrypt_handle(void)
{
	return &s_colinkXcryptDrvAdapter;
}

static void ln_xcrypt_init(void)
{
	LN_COLINK_MALLOC(get_colink_xcrypt_handle()->iv, AES_CBC_IV_LENGTH, AES_CBC_IV_VALUE);
	
	memset(get_colink_xcrypt_handle()->colinkId, 0, sizeof(get_colink_xcrypt_handle()->colinkId));
	memset(get_colink_xcrypt_handle()->colinkApikey, 0, sizeof(get_colink_xcrypt_handle()->colinkApikey));
	memset(get_colink_xcrypt_handle()->colinkChipid, 0, sizeof(get_colink_xcrypt_handle()->colinkChipid));
	memset(get_colink_xcrypt_handle()->colinkBleName, 0, sizeof(get_colink_xcrypt_handle()->colinkBleName));
	memset(get_colink_xcrypt_handle()->colinkSoftapName, 0, sizeof(get_colink_xcrypt_handle()->colinkSoftapName));
}

static void ln_random_uuid(char *pBuf)
{
    const char *c = "89ab";
    char *p = pBuf;
    int n;
  
    for( n = 0; n < 16; ++n )
    {
        int b = rand()%255;
  
        switch( n )
        {
            case 6:
                sprintf(
                    p,
                    "4%x",
                    b%15 );
                break;
            case 8:
                sprintf(
                    p,
                    "%c%x",
                    c[rand()%strlen( c )],
                    b%15 );
                break;
            default:
                sprintf(
                    p,
                    "%02x",
                    b );
                break;
        }
  
        p += 2;
  
        switch( n )
        {
            case 3:
            case 5:
            case 7:
            case 9:
                *p++ = '-';
                break;
        }
    }
  
    *p = 0;
}

static void ln_md5_encrypt(uint8_t *_src, uint8_t *_dst)
{
	Log_i("src:%s", _src);
	mbedtls_md5_context md5_ctx;
	mbedtls_md5_init(&md5_ctx);
	mbedtls_md5_starts(&md5_ctx);
	mbedtls_md5_update(&md5_ctx, _src, strlen((char *)_src));
	mbedtls_md5_finish(&md5_ctx, _dst);
	hexdump(LOG_LVL_INFO, "[MD5]", (void *)_dst, 16);
	mbedtls_md5_free(&md5_ctx);
}

static void ln_aes_cbc_pkcs7_128_encrypt(uint8_t *_src, uint8_t* _key, uint8_t *_dst)
{
	unsigned char cipher[128] = { 0 };

	size_t olen = 0;
	mbedtls_cipher_context_t ctx;
	mbedtls_cipher_init(&ctx);
	mbedtls_cipher_set_padding_mode(&ctx, MBEDTLS_PADDING_PKCS7);
	mbedtls_cipher_setup(&ctx, mbedtls_cipher_info_from_values(MBEDTLS_CIPHER_ID_AES, 128,MBEDTLS_MODE_CBC));
	mbedtls_cipher_setkey(&ctx, _key, 128, MBEDTLS_ENCRYPT);
	mbedtls_cipher_set_iv(&ctx, get_colink_xcrypt_handle()->iv, 16);
	mbedtls_cipher_reset(&ctx);
	
	char *pSrc = (char *)_src;
	Log_i("src:%s", _src);
	Log_d("Src len  = %d ",strlen(pSrc));
	mbedtls_cipher_update(&ctx, _src, strlen(pSrc), cipher, &olen);
	Log_d("first olen = %d", olen);
	
	int inlen = olen;
	mbedtls_cipher_finish(&ctx, cipher + olen, &olen);
	inlen += olen;

	Log_d("second olen = %d", olen);
	Log_d("inlen = %d", inlen);
	hexdump(LOG_LVL_INFO, "[CBC_ENCRYPT]", (void *)cipher, inlen);
    mbedtls_cipher_free(&ctx);
	
	mbedtls_base64_encode(_dst, 65, &olen, cipher, inlen);
	Log_i("src_encrypt_base64:%s", _dst);
}

static void ln_aes_cbc_pkcs7_128_decrypt(uint8_t *_src, uint8_t* _key, uint8_t *_dst)
{
	unsigned char cipher[128] = { 0 };

	size_t olen = 0;
	mbedtls_cipher_context_t ctx;
	mbedtls_cipher_init(&ctx);
	mbedtls_cipher_set_padding_mode(&ctx, MBEDTLS_PADDING_PKCS7);
	mbedtls_cipher_setup(&ctx,mbedtls_cipher_info_from_values(MBEDTLS_CIPHER_ID_AES, 128,MBEDTLS_MODE_CBC));
	mbedtls_cipher_setkey(&ctx, _key, 128, MBEDTLS_DECRYPT);
	mbedtls_cipher_set_iv(&ctx, get_colink_xcrypt_handle()->iv, 16);
	mbedtls_cipher_reset(&ctx);
	
	Log_i("src_encrypt_base64:%s", _src);
	mbedtls_base64_decode(cipher, 128, &olen, _src, strlen((char *)_src));
	hexdump(LOG_LVL_INFO, "[BASE64_DECODE]", (void *)cipher, olen);
	
	int cipherLen = olen;
	mbedtls_cipher_update(&ctx, cipher, cipherLen, _dst, &olen);
	size_t total_len = olen;
	mbedtls_cipher_finish(&ctx, _dst + olen, &olen);
	total_len += olen;
	_dst[total_len] = 0;
	hexdump(LOG_LVL_INFO, "[CBC_DECRYPT]", (void *)_dst, total_len);
	mbedtls_cipher_free(&ctx);
}

static void ln_sha256_encrypt(uint8_t *_src, uint8_t *_dst)
{
	mbedtls_sha256_context sha256_ctx;
    uint8_t digest[32];
	
	mbedtls_sha256_init(&sha256_ctx);
    mbedtls_sha256_starts(&sha256_ctx, 0);
    mbedtls_sha256_update(&sha256_ctx, _src, strlen((char*)_src));
    mbedtls_sha256_finish(&sha256_ctx, digest);
	mbedtls_sha256_free(&sha256_ctx);
	
    for (int i = 0; i < 64; i += 2)
    {
        sprintf((char*)_dst+i, "%02x", digest[i / 2]);
    }

    _dst[64] = 0;
    Log_i("digest_hex=[%s]", _dst);
}
