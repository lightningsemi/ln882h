#include <stdio.h>
#include <string.h>
#include "iot_import.h"
#include "iotx_log.h"
#include "dev_bind/os/platform.h"
#include "proj_config.h"

#define DEVINFO_DN  "devinfo_dn"
#define DEVINFO_DS  "devinfo_ds"
#define DEVINFO_PK  "devinfo_pk"
#define DEVINFO_PS  "devinfo_ps"

char sg_product_key     [PRODUCT_KEY_MAXLEN]    = { 0 };
char sg_product_secret  [PRODUCT_SECRET_MAXLEN] = { 0 };
char sg_device_name     [DEVICE_NAME_MAXLEN]    = { 0 };
char sg_device_secret   [DEVICE_SECRET_MAXLEN]  = { 0 };

static const char *demo_iv = "f165u329c054k637";

#define LOG_TAG     "PDT"

/**
 * @brief Get device name from user's system persistent storage
 *
 * @param [ou] device_name: array to store device name, max length is DEVICE_NAME_MAXLEN
 * @return the actual length of device name
 */
int HAL_GetDeviceName(char device_name[DEVICE_NAME_MAXLEN])
{
    int len = strlen(sg_device_name);

    memset(device_name, 0, DEVICE_NAME_MAXLEN);
    if (0 == len) {
        len = DEVICE_NAME_MAXLEN;
        HAL_Kv_Get(DEVINFO_DN, sg_device_name, &len);
    }
    strncpy(device_name, sg_device_name, len);

    return strlen(device_name);
}

/**
 * @brief Get device secret from user's system persistent storage
 *
 * @param [ou] device_secret: array to store device secret, max length is DEVICE_SECRET_MAXLEN
 * @return the actual length of device secret
 */
int HAL_GetDeviceSecret(char device_secret[DEVICE_SECRET_MAXLEN])
{
int res;
    int len = strlen(sg_device_secret);
    memset(device_secret, 0x0, DEVICE_SECRET_MAXLEN);
    if(len == 0) {
        char enc_secret[DEVICE_SECRET_MAXLEN] = {0};
        len = IOTX_DEVICE_SECRET_LEN;
        do {
            res = HAL_Kv_Get(DEVINFO_DS, enc_secret, &len);
            if(res < 0 || len == 0) {
                log_err(LOG_TAG, "kv get ps failed,ret = %d, len = %d",res, len);
                break;
            }
            if(strlen(sg_product_key) == 0) {
                HAL_GetProductKey(sg_product_key);
            }
            p_aes128_t aes_e_h = platform_aes128_init((unsigned char *)sg_product_key,
                                                    (unsigned char *)demo_iv,
                                                    PLATFORM_AES_DECRYPTION);
            if(aes_e_h == NULL ) {
                log_err(LOG_TAG, "aes init failed");
                break;
            }
            res = platform_aes128_cfb_decrypt(aes_e_h, enc_secret, len, sg_device_secret);
            platform_aes128_destroy(aes_e_h);
            if(res < 0) {
                log_err(LOG_TAG, "encrypt ds failed");
                break;
            }
        } while(0);
    }

    strncpy(device_secret, sg_device_secret, len);

    return len;
}

/**
 * @brief Get product key from user's system persistent storage
 *
 * @param [ou] product_key: array to store product key, max length is PRODUCT_KEY_MAXLEN
 * @return  the actual length of product key
 */
int HAL_GetProductKey(char product_key[PRODUCT_KEY_MAXLEN])
{
    int len = strlen(sg_product_key);
    memset(product_key, 0x0, PRODUCT_KEY_MAXLEN);
    if(len == 0) {
        len = PRODUCT_KEY_MAXLEN;
        HAL_Kv_Get(DEVINFO_PK, sg_product_key, &len);
    }

    strncpy(product_key, sg_product_key, len);

    return len;
}

int HAL_GetProductSecret(char product_secret[PRODUCT_SECRET_MAXLEN])
{
    int res;
    int len = strlen(sg_product_secret);
    memset(product_secret, 0x0, PRODUCT_SECRET_MAXLEN);
    if(len == 0) {
        char enc_secret[PRODUCT_SECRET_MAXLEN] = {0};
        len = PRODUCT_SECRET_MAXLEN - 1;
        do {
            res = HAL_Kv_Get(DEVINFO_PS, enc_secret, &len);
            if(res < 0 || len == 0) {
                log_err(LOG_TAG, "kv get ps failed,ret = %d, len = %d",res, len);
                break;
            }
            if(strlen(sg_product_key) == 0) {
                HAL_GetProductKey(sg_product_key);
            }
            p_aes128_t aes_e_h = platform_aes128_init((unsigned char *)sg_product_key, (unsigned char *)demo_iv, PLATFORM_AES_DECRYPTION);
            if(aes_e_h == NULL ) {
                log_err(LOG_TAG, "aes init failed");
                break;
            }
            res = platform_aes128_cfb_decrypt(aes_e_h, enc_secret, len, sg_product_secret);
            platform_aes128_destroy(aes_e_h);
            if(res < 0) {
                log_err(LOG_TAG, "encrypt ds failed");
                break;
            }
        } while(0);
    }

    strncpy(product_secret, sg_product_secret, len);

    return len;
}

/**
 * @brief Get firmware version
 *
 * @param [ou] version: array to store firmware version, max length is IOTX_FIRMWARE_VER_LEN
 * @return the actual length of firmware version
 */
int HAL_GetFirmwareVersion(char *version)
{
    char firmware_ver[FIRMWARE_VERSION_MAXLEN];

    if (!version)
    {
        return 0;
    }

    memset(version, 0, FIRMWARE_VERSION_MAXLEN);
    HAL_Snprintf(firmware_ver, FIRMWARE_VERSION_MAXLEN, "app-%d.%d-%s-%s",
            FLASH_IMAGE_VER_MAJOR, FLASH_IMAGE_VER_MINOR, __DATE__, __TIME__);

    memcpy(version, firmware_ver, strlen(firmware_ver));

    return strlen(firmware_ver);
}

int HAL_SetDeviceName(char *device_name)
{
    int  len;
    if(device_name == NULL) {
        return -1;
    }

    memset(sg_device_name, 0x0, DEVICE_NAME_MAXLEN);
    len = strlen(device_name);
    if (len > DEVICE_NAME_MAXLEN) {
        return -1;
    } else if (len == 0) {
        HAL_Kv_Del(DEVINFO_DN);
        return 0;
    }

    strncpy(sg_device_name, device_name, len);
    HAL_Kv_Set(DEVINFO_DN, device_name, len, 1);
    return len;
}

int HAL_SetDeviceSecret(char *device_secret)
{
    int res, len;
    if(device_secret == NULL) {
        return -1;
    }

    memset(sg_device_secret, 0x0, DEVICE_SECRET_MAXLEN);
    len = strlen(device_secret);
    if (len > DEVICE_SECRET_MAXLEN) {
        return -1;
    } else if (len == 0) {
        HAL_Kv_Del(DEVINFO_DS);
        return 0;
    }

    strncpy(sg_device_secret, device_secret, len);
    do {
        char dec_secret[DEVICE_SECRET_MAXLEN] = {0};
        if(strlen(sg_product_key) == 0) {
                HAL_GetProductKey(sg_product_key);
        }
        p_aes128_t aes_e_h = platform_aes128_init((unsigned char *)sg_product_key, (unsigned char *)demo_iv, PLATFORM_AES_ENCRYPTION);
        if(aes_e_h == NULL ) {
            log_err(LOG_TAG, "aes init failed");
            break;
        }
        res = platform_aes128_cfb_encrypt(aes_e_h, sg_device_secret, len, dec_secret);
        platform_aes128_destroy(aes_e_h);
        if(res < 0) {
            log_err(LOG_TAG, "encrypt ds failed");
            break;
        }
        HAL_Kv_Set(DEVINFO_DS, dec_secret, len, 1);

    } while(0);

    return len;
}

int HAL_SetProductKey(char *product_key)
{
    int len;
    if(product_key == NULL) {
        return -1;
    }

    memset(sg_product_key, 0x0, PRODUCT_KEY_MAXLEN);
    len = strlen(product_key);
    if (len > PRODUCT_KEY_MAXLEN) {
        return -1;
    } else if (len == 0) {
        HAL_Kv_Del(DEVINFO_PK);
        return 0;
    }

    strncpy(sg_product_key, product_key, len);
    HAL_Kv_Set(DEVINFO_PK, product_key, len, 1);

    return len;
}

int HAL_SetProductSecret(char *product_secret)
{
    int res;
    int len;

    if(product_secret == NULL) {
        return -1;
    }

    memset(sg_product_secret, 0x0, PRODUCT_SECRET_MAXLEN);
    len = strlen(product_secret);
    if (len > PRODUCT_SECRET_MAXLEN) {
        return -1;
    } else if (len == 0) {
        HAL_Kv_Del(DEVINFO_PS);
        return 0;
    }

    strncpy(sg_product_secret, product_secret, len);

    do {
        char dec_secret[PRODUCT_SECRET_MAXLEN] = {0};
        if(strlen(sg_product_key) == 0) {
                HAL_GetProductKey(sg_product_key);
        }
        p_aes128_t aes_e_h = platform_aes128_init((unsigned char *)sg_product_key, (unsigned char *)demo_iv, PLATFORM_AES_ENCRYPTION);
        if(aes_e_h == NULL ) {
            log_err(LOG_TAG, "aes init failed");
            break;
        }
        res = platform_aes128_cfb_encrypt(aes_e_h, sg_product_secret, len, dec_secret);
        platform_aes128_destroy(aes_e_h);
        if(res < 0) {
            log_err(LOG_TAG, "encrypt ps failed");
            break;
        }
        HAL_Kv_Set(DEVINFO_PS, dec_secret, len, 1);
    } while(0);

    return len;
}

#define __DEMO__

int HAL_GetPartnerID(char *pid_str)
{
    memset(pid_str, 0x0, PID_STRLEN_MAX);
#ifdef __DEMO__
    strcpy(pid_str, "Your company name");
#endif
    return strlen(pid_str);
}

int HAL_GetModuleID(char *mid_str)
{
    memset(mid_str, 0x0, MID_STRLEN_MAX);
#ifdef __DEMO__
    strcpy(mid_str, "Your module name");
#endif
    return strlen(mid_str);
}

char *HAL_GetChipID(_OU_ char *cid_str)
{
    memset(cid_str, 0x0, HAL_CID_LEN);
#ifdef __DEMO__
    strncpy(cid_str, "12345678", HAL_CID_LEN);
    cid_str[HAL_CID_LEN - 1] = '\0';
#endif
    return cid_str;
}

int HAL_GetDeviceID(_OU_ char *device_id)
{
    char pk[PRODUCT_KEY_MAXLEN];
    char dn[DEVICE_NAME_MAXLEN];

    memset(device_id, 0x0, DEVICE_ID_LEN);
    memset(pk, 0x0, PRODUCT_KEY_MAXLEN);
    memset(dn, 0x0, DEVICE_NAME_MAXLEN);

    HAL_GetProductKey(pk);
    HAL_GetDeviceName(dn);

    HAL_Snprintf(device_id, DEVICE_ID_LEN, "%s.%s", pk, dn);

    return strlen(device_id);
}
