/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "os.h"
#include "awss_utils.h"
#include "sha256.h"
#include "passwd.h"
#include "awss_log.h"
#include "awss_wifimgr.h"
#include "awss_crypt.h"

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
extern "C"
{
#endif

// #define VERBOSE_CRYPT_DEBUG

static void _dump_passwd(char *title, uint8_t *passwd, int length)
{
#ifdef VERBOSE_CRYPT_DEBUG
    // 40 + 16 = 56 + 2= 8*7 + 2
    uint8_t *ptr = passwd;

    if (NULL != title) {
        awss_debug("passwd: [%s]\r\n", title);
    }

    if (NULL == passwd) {
        return;
    }

    for (int i = 0; i < length; i += 8) {
        awss_debug("%02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X\r\n",
        *(ptr+0), *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5), *(ptr+6), *(ptr+7));
        ptr += i;
    }
#endif
}

static const char *cal_passwd(void *key, void *random, void *passwd)
{
    uint16_t key_len;
    uint8_t digest[SHA256_DIGEST_SIZE + 1] = {0};
    uint8_t passwd_src[KEY_MAX_LEN + RANDOM_MAX_LEN + 2] = {0};

    if (!passwd || !key || !random)
        return NULL;

    // combine key and random, with split of comma
    key_len = strlen(key);
    if (key_len > KEY_MAX_LEN)
        key_len = KEY_MAX_LEN;
    memcpy(passwd_src, key, key_len);
    passwd_src[key_len ++] = ',';
    memcpy(passwd_src + key_len, random, RANDOM_MAX_LEN);
    key_len += RANDOM_MAX_LEN;

    _dump_passwd("encoded", passwd_src, KEY_MAX_LEN + RANDOM_MAX_LEN + 2);

    // produce digest using combination of key and random
    awss_debug("try to sha256_hash...\r\n");
    SHA256_hash(passwd_src, key_len, digest);

    _dump_passwd("decoded", digest, SHA256_DIGEST_SIZE);

    // use the first 128bits as AES-Key
    memcpy(passwd, digest, AES128_KEY_LEN);

    #ifdef VERBOSE_CRYPT_DEBUG
    // sizeof (passwd) = 16
    uint8_t *ch = passwd;
    awss_debug("get passwd: \r\n%02X, %02X, %02X, %02X, \r\n%02X, %02X, %02X, %02X, \r\n%02X, %02X, %02X, %02X, \r\n%02X, %02X, %02X, %02X\r\n",
                    *(ch+0), *(ch+1), *(ch+2), *(ch+3),
                    *(ch+4), *(ch+5), *(ch+6), *(ch+7),
                    *(ch+8), *(ch+9), *(ch+10), *(ch+11),
                    *(ch+12), *(ch+13), *(ch+14), *(ch+15)
    );
    #endif

    return passwd;
}

int aes_decrypt_string(char *cipher, char *plain, int len, int cipher_hex, int sec_lvl, char cbc, const char *rand)
{
    char res = 0;
    char decrypt = 1;
    uint8_t iv[AES128_KEY_LEN] = {0};
    uint8_t key[AES128_KEY_LEN] = {0};
    uint8_t random[RANDOM_MAX_LEN] = {0};

    uint8_t *decoded = (uint8_t *)os_zalloc(len + 1);
    if (decoded == NULL)
        return -1;

    if (cipher_hex == 0) {
        /*
         * mobile-ap, router, dev-ap
         */
        utils_str_to_hex(cipher, len, decoded, len);
    } else {  // for smartconfig/wps, zconfig,
        /*
         * smartconfig/wps, zconfig
         */
        memcpy(decoded, cipher, len);
    }

    if (rand) {
        /*
         * smartconfig/wps uses zero
         * zconfig/dev-ap/mobile-ap/router uses random
         */
        memcpy(random, rand, sizeof(random));
    }

    awss_debug("security level: %d", sec_lvl);

    switch (sec_lvl) {
        case SEC_LVL_AES128_PRODUCT:
        {
            char product_sec[OS_PRODUCT_SECRET_LEN + 1] = {0};
            os_product_get_secret(product_sec);
            awss_debug("ps --> [%s]\r\n", product_sec);
            cal_passwd(product_sec, random, key);
            memcpy(iv, random, sizeof(random));
            break;
        }
        case SEC_LVL_AES128_DEVICE:
        {
            char dev_sec[OS_DEVICE_SECRET_LEN + 1] = {0};
            os_device_get_secret(dev_sec);
            cal_passwd(dev_sec, random, key);
            memcpy(iv, random, sizeof(random));
            break;
        }
        default:
        {
            decrypt = 0;
            awss_debug("wrong security level: %d\n", sec_lvl);
            res = -2;
            break;
        }
    }

    plain[0] = '\0';

    awss_debug("try to decrypt: %d\r\n", decrypt);
    if (decrypt) {
        p_aes128_t aes = os_aes128_init(key, iv, PLATFORM_AES_DECRYPTION);
        if (cbc) { // AP
        awss_debug("aes128 cbc decrypt -- ap!\r\n");
            /*
             * mobile-ap, dev-ap, router
             */
            res = os_aes128_cbc_decrypt(aes, decoded, len / AES128_KEY_LEN / 2, plain);
        } else {  // smartconfig
            awss_debug("aes128 cfb decrypt -- smartconfig!\r\n");
            /*
             * smartconfig/wps, zconfig
             */
            res = os_aes128_cfb_decrypt(aes, decoded, len, plain);
        }
        os_aes128_destroy(aes);
    }

    os_free(decoded);

    awss_debug("retcode = %d\r\n", res);
    return res;
}

#if defined(__cplusplus)  /* If this is a C++ compiler, use C linkage */
}
#endif
