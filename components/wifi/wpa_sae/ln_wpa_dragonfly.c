/*
 * Shared Dragonfly functionality
 * Copyright (c) 2012-2016, Jouni Malinen <j@w1.fi>
 * Copyright (c) 2019, The Linux Foundation
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

/**
 * @file   ln_wpa_dragonfly.c
 * 
 * Modified by LightningSemi WLAN Team
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-07     MurphyZhao   the first version
 */

#include "ln_wpa_dragonfly.h"

#ifdef LN_WPA_SAE_SUPPORTED
#include "wifi.h"
#include "wifi_port.h"

static int dragonfly_get_rand_2_to_r_1(struct crypto_bignum *val,
                       const struct crypto_bignum *order)
{
    return crypto_bignum_rand(val, order) == 0 &&
        !crypto_bignum_is_zero(val) &&
        !crypto_bignum_is_one(val);
}

int dragonfly_generate_scalar(const struct crypto_bignum *order,
                  struct crypto_bignum *_rand,
                  struct crypto_bignum *_mask,
                  struct crypto_bignum *scalar)
{
    int count;

    /* Select two random values rand,mask such that 1 < rand,mask < r and
     * rand + mask mod r > 1. */
    for (count = 0; count < 100; count++) {
        if (dragonfly_get_rand_2_to_r_1(_rand, order) &&
            dragonfly_get_rand_2_to_r_1(_mask, order) &&
            crypto_bignum_add(_rand, _mask, scalar) == 0 &&
            crypto_bignum_mod(scalar, order, scalar) == 0 &&
            !crypto_bignum_is_zero(scalar) &&
            !crypto_bignum_is_one(scalar))
            return 0;
    }

    /* This should not be reachable in practice if the random number
     * generation is working. */
    WLIB_LOG_I("dragonfly: Unable to get randomness for own scalar\r\n");
    return -1;
}
#endif /* LN_WPA_SAE_SUPPORTED */
