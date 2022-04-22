/*
 * Shared Dragonfly functionality
 * Copyright (c) 2012-2016, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2019, The Linux Foundation
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

/**
 * @file   ln_wpa_dragonfly.h
 * 
 * Modified by LightningSemi WLAN Team
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-07     MurphyZhao   the first version
 */


#ifndef __LN_WPA_DRAGONFLY_H__
#define __LN_WPA_DRAGONFLY_H__

#include <stdint.h>

/**
 * LN_WPA_SAE_SUPPORTED is defined in the global macro definition.
 * MDK: Options for Target --> C/C++ --> Preprocessor Symbols --> Define
 * GCC: -DLN_WPA_SAE_SUPPORTED
*/
#ifdef LN_WPA_SAE_SUPPORTED
/* wpa_supplicant header files */
#include "crypto/crypto.h"

int dragonfly_generate_scalar(const struct crypto_bignum *order,
        struct crypto_bignum *_rand,
        struct crypto_bignum *_mask,
        struct crypto_bignum *scalar);
#endif /* LN_WPA_SAE_SUPPORTED */

#endif /* __LN_WPA_DRAGONFLY_H__ */
