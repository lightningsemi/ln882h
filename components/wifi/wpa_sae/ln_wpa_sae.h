/**
 * @file   ln_wpa_sae.h
 * 
 * Implemented by LightningSemi WLAN Team
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-07     MurphyZhao   the first version
 */

#ifndef __LN_WPA_SAE_H__
#define __LN_WPA_SAE_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "wifi.h"

/* Status codes (IEEE Std 802.11-2016, 9.4.1.9, Table 9-46) */
#define WLAN_STATUS_SUCCESS                     0
#define WLAN_STATUS_UNSPECIFIED_FAILURE         1
#define WLAN_STATUS_SECURITY_DISABLED           5
#define WLAN_STATUS_NOT_SUPPORTED_AUTH_ALG      13
#define WLAN_STATUS_UNKNOWN_AUTH_TRANSACTION    14
#define WLAN_STATUS_CHALLENGE_FAIL              15
#define WLAN_STATUS_AUTH_TIMEOUT                16

#define SAE_SILENTLY_DISCARD 65535

/**
 * @return true is required if defined LN_WPA_SAE_SUPPORTED as 1 and 
 *         ln_wpa_sae_enable() is called, else false is returned.
*/
bool ln_wpa_sae_is_enabled(void);
void ln_wpa_sae_enable(void);
void ln_wpa_sae_disable(void);

/**
 * @param sae_in_param in param
 *     own_mac  : own mac address
 *     peer_mac : peer mac address
 *     password : passphrase(ascii format password)
 *     password_len : len(password), 0 < len < 63
 *     group_id : only supported ecc group-19
 * @param commit_info out param
 * @return 0 success, otherwise failture
*/
int ln_wpa_sae_start(ln_wpa_sae_in_param_t *sae_in_param,
        ln_wpa_sae_commit_info_t *commit_info);

int ln_wpa_sae_auth_frame_process(ln_wpa_sae_commit_info_t *info,
        ln_wpa_sae_confirm_info_t *confirm_info);
int ln_wpa_sae_pmk_get(uint8_t **in_pmk_ptr);
int ln_wpa_sae_kdf_hash(const uint8_t *key, size_t key_len, const char *label,
        const uint8_t *data, size_t data_len, uint8_t *buf, size_t buf_len);
int ln_wpa_sae_omac1_aes_128(const uint8_t *key, const uint8_t *data, int data_len, uint8_t * mac);
#endif /* __LN_WPA_SAE_H__ */
