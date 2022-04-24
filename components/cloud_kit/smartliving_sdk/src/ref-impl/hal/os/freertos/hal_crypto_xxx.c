#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iot_import.h"
#include "iotx_log.h"
#include "mbedtls/aes.h"

#define LOG_TAG     "CRYPT"

typedef enum {
    AES_MODE_CBC = 0,
    AES_MODE_CFB = 1
} aes_mode_t;

typedef enum {
    AES_DIR_DECRYPT     = 0,
    AES_DIR_ENCRYPT     = 1,
} aes_dir_t;

#define IV_MAX_LEN          (16)    // IV for CFB
#define KEY_MAX_LEN         (16)    // KEY for CFB
typedef struct {
    mbedtls_aes_context aes_ctx;
    aes_mode_t          aes_mode;
    aes_dir_t           aes_dir;
    uint8_t             aes_iv[IV_MAX_LEN];
    uint8_t             aes_key[KEY_MAX_LEN];
} crypto_ctx_t;

/**
 * @brief   初始化AES加密的结构体
 *
 * @param[in] key:
 * @param[in] iv:
 * @param[in] dir: AES_ENCRYPTION or AES_DECRYPTION
 * @return AES128_t
   @verbatim None
   @endverbatim
 * @see None.
 * @note None.
 */
p_HAL_Aes128_t HAL_Aes128_Init(
    const uint8_t *key,
    const uint8_t *iv,
    AES_DIR_t dir)
{
    crypto_ctx_t *ctx = NULL;
    int ret = -1;

    if ( (NULL == key) || (NULL == iv) ) {
        return (p_HAL_Aes128_t)ctx;
    }

    ctx = (crypto_ctx_t *)HAL_Malloc(sizeof(crypto_ctx_t));
    if (NULL == ctx) {
        log_err(LOG_TAG, "failed to malloc for crypto_ctx_t!!!");
        return NULL;
    }
    memset(ctx, 0, sizeof(crypto_ctx_t));

    mbedtls_aes_init(&ctx->aes_ctx);

    if (HAL_AES_ENCRYPTION == dir) {
        ctx->aes_dir = AES_DIR_ENCRYPT;
        ret = mbedtls_aes_setkey_enc(&ctx->aes_ctx, key, 128);
    } else {
        ctx->aes_dir = AES_DIR_DECRYPT;
        ret = mbedtls_aes_setkey_dec(&ctx->aes_ctx, key, 128);
    }

    if (0 == ret) {
        memcpy(ctx->aes_key, key, KEY_MAX_LEN);
        memcpy(ctx->aes_iv, iv, IV_MAX_LEN);
    } else {
        HAL_Free(ctx);
        ctx = NULL;
    }

    return (p_HAL_Aes128_t)ctx;
}


/**
 * @brief   销毁AES加密的结构体
 *
 * @param[in] aes:
 * @return
   @verbatim
     = 0: succeeded
     = -1: failed
   @endverbatim
 * @see None.
 * @note None.
 */
int HAL_Aes128_Destroy(p_HAL_Aes128_t aes)
{
    if (NULL == aes) {
        log_err(LOG_TAG, "invalid aes handler!!!");
        return -1;
    }

    crypto_ctx_t *ctx = (crypto_ctx_t *)aes;
    HAL_Free(ctx);

    return 0;
}

/**
 * @brief   以`AES-CBC-128`方式, 根据`HAL_Aes128_Init()`时传入的密钥解密指定的密文
 *
 * @param[in] aes: AES handler
 * @param[in] src: cipher data
 * @param[in] blockNum: plain data number of 16 bytes size
 * @param[out] dst: plain data
 * @return
   @verbatim
     = 0: succeeded
     = -1: failed
   @endverbatim
 * @see None.
 * @note None.
 */
int HAL_Aes128_Cbc_Decrypt(
    p_HAL_Aes128_t aes,
    const void *src,
    size_t blockNum,
    void *dst)
{
    int ret = -1;
    crypto_ctx_t *ctx = (crypto_ctx_t *)aes;
    const unsigned char *src_ptr = (const unsigned char *)src;
    unsigned char *dst_ptr = (unsigned char *)dst;

    if ( (NULL == aes) || (NULL == src) || (NULL == dst) ) {
        return ret;
    }

    for (int i = 0; i < blockNum; i++) {
        ret = mbedtls_aes_crypt_cbc(&ctx->aes_ctx, MBEDTLS_AES_DECRYPT,
                        16, (unsigned char *)ctx->aes_iv,
                        (const unsigned char *)src_ptr, (unsigned char *)dst_ptr);
        if (ret != 0) {
            log_err(LOG_TAG, "failed to cbc128 decrypt!!!");
            return -1;
        }
        src_ptr += 16;
        dst_ptr += 16;
    }

    return 0;
}

/**
 * @brief   以`AES-CBC-128`方式, 根据`HAL_Aes128_Init()`时传入的密钥加密指定的明文
 *
 * @param[in] aes: AES handler
 * @param[in] src: plain data
 * @param[in] blockNum: plain data number of 16 bytes size
 * @param[out] dst: cipher data
 * @return
   @verbatim
     = 0: succeeded
     = -1: failed
   @endverbatim
 * @see None.
 * @note None.
 */
int HAL_Aes128_Cbc_Encrypt(
    p_HAL_Aes128_t aes,
    const void *src,
    size_t blockNum,
    void *dst)
{
    int ret = 0;
    crypto_ctx_t *ctx = (crypto_ctx_t *)aes;
    const unsigned char *src_ptr = (const unsigned char *)src;
    unsigned char *dst_ptr = (unsigned char *)dst;

    if ( (NULL == aes) || (NULL == src) || (NULL == dst) ) {
        return -1;
    }

    for (int i = 0; i < blockNum; i++) {
        ret = mbedtls_aes_crypt_cbc(&ctx->aes_ctx, MBEDTLS_AES_ENCRYPT,
                            16, (unsigned char *)ctx->aes_iv,
                            (const unsigned char *)src_ptr,
                            (unsigned char *)dst_ptr);
        if (ret != 0) {
            log_err(LOG_TAG, "failed to cbc128 encryption!!!");
            return -1;
        }
        src_ptr += 16;
        dst_ptr += 16;
    }

    return 0;
}

/**
 * @brief   以`AES-CFB-128`方式, 根据`HAL_Aes128_Init()`时传入的密钥解密指定的密文
 *
 * @param[in] aes: AES handler
 * @param[in] src: cipher data
 * @param[in] blockNum: plain data number of 16 bytes size
 * @param[out] dst: plain data
 * @return
   @verbatim
     = 0: succeeded
     = -1: failed
   @endverbatim
 * @see None.
 * @note None.
 */
DLL_HAL_API int HAL_Aes128_Cfb_Decrypt(
    _IN_ p_HAL_Aes128_t aes,
    _IN_ const void *src,
    _IN_ size_t length,
    _OU_ void *dst)
{
    int ret = -1;
    crypto_ctx_t *ctx = (crypto_ctx_t *)aes;
    size_t offset = 0;

    if ( (NULL == aes) || (NULL == src) || (NULL == dst) ) {
        return ret;
    }

    ret = mbedtls_aes_setkey_enc(&ctx->aes_ctx, ctx->aes_key, 128);
    if (ret != 0) { // error.
        log_err(LOG_TAG, "failed to set key for cfb128!!!");
        return -1;
    }

    ret = mbedtls_aes_crypt_cfb128(&ctx->aes_ctx, MBEDTLS_AES_DECRYPT,
                    length, &offset, ctx->aes_iv,
                    (const unsigned char *)src,
                    (unsigned char *)dst);
    if (ret != 0) { // error.
        log_err(LOG_TAG, "failed to cfb128 decrypt!!!");
        return -1;
    }

    return 0;
}

/**
 * @brief   以`AES-CFB-128`方式, 根据`HAL_Aes128_Init()`时传入的密钥加密指定的明文
 *
 * @param[in] aes: AES handler
 * @param[in] src: plain data
 * @param[in] blockNum: plain data number of 16 bytes size
 * @param[out] dst: cipher data
 * @return
   @verbatim
     = 0: succeeded
     = -1: failed
   @endverbatim
 * @see None.
 * @note None.
 */
DLL_HAL_API int HAL_Aes128_Cfb_Encrypt(
    _IN_ p_HAL_Aes128_t aes,
    _IN_ const void *src,
    _IN_ size_t length,
    _OU_ void *dst)
{
    int ret = -1;
    crypto_ctx_t *ctx = (crypto_ctx_t *)aes;
    size_t offset = 0;

    if ( (NULL == aes) || (NULL == src) || (NULL == dst) ) {
        return ret;
    }


    ret = mbedtls_aes_crypt_cfb128(&ctx->aes_ctx, MBEDTLS_AES_ENCRYPT, length,
                    &offset, ctx->aes_iv,
                    (const unsigned char *)src,
                    (unsigned char *)dst);
    if (ret != 0) {
        log_err(LOG_TAG, "failed to cfb128 encrypt!!!");
        return -1;
    }

    return 0;
}
