/* wolfsslRunTests.c
 *
 * Copyright (C) 2006-2021 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#ifndef WOLFSSL_USER_SETTINGS
    #include <wolfssl/options.h>
#endif
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/wc_port.h>
#include <wolfcrypt/test/test.h>
#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/integer.h>
#include <wolfcrypt/benchmark/benchmark.h>

#include <stdint.h>
#include "debug/log.h"

int wolfssl_test(void) {
    /* Check DATA TYPES */
    if (sizeof(long) != 4 || sizeof(long long) != 8 ||
        sizeof(word16) != 2 || sizeof(word32) != 4 ||
        (2 * sizeof(mp_digit)) != sizeof(mp_word)) {
        LOG(LOG_LVL_DEBUG, "[%s:%d] DATA TYPES Error!!!\r\n",
            __func__, __LINE__);
        return -1;
    }

    LOG(LOG_LVL_DEBUG, "sizeof(mp_digit):%d\r\n", sizeof(mp_digit));
    LOG(LOG_LVL_DEBUG, "sizeof(mp_word):%d\r\n", sizeof(mp_word));

#if !defined(NO_64BIT)
    if (sizeof(word64) != 8) {
        LOG(LOG_LVL_DEBUG, "[%s:%d] DATA TYPES Error!!!\r\n",
            __func__, __LINE__);
        return -1;
    }
#endif

#if !defined(NO_CRYPT_TEST)
    wolfcrypt_test(NULL);
#endif

#if !defined(NO_CRYPT_BENCHMARK)
    benchmark_test(NULL);
#endif
    return 0;
}
