/* user_setting.h
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

#ifndef WOLFSSL_USER_SETTINGS_H_
#define WOLFSSL_USER_SETTINGS_H_

/**
 * If use this user config file, please define WOLFSSL_USER_SETTINGS .
 * When gcc compiler, use as "CFLAGS = -DWOLFSSL_USER_SETTINGS".
 * When MDK5, define it
 *      in "Options for Target --> C/C++ --> Preprocessor Symbols --> define", 
 *      eg: WOLFSSL_USER_SETTINGS .
*/

#ifdef __cplusplus
    extern "C" {
#endif

// #define ENABLE_WOLFSSL_USER_TEST

#define FREERTOS

/**
 * Disable WOLFSSL_LWIP configuration when using wolfssl other APIs only
 * without using wolfssl lwip.
*/
// #define WOLFSSL_LWIP
#ifndef WOLFSSL_LWIP
    #define WOLFSSL_USER_IO
    #define WOLFSSL_NO_SOCK
#endif /* WOLFSSL_LWIP */

#define WOLFSSL_SMALL_STACK
/* CMAC need AES */
#define WOLFSSL_CMAC

#define WOLFCRYPT_ONLY
#define NO_DES3
#define NO_PWDBASED
#define NO_CODING
#define NO_ASN
#define NO_MD4
#define NO_SHA
#define NO_RSA
// #define NO_AES
// #define NO_HMAC

// #define WOLFSSL_IGNORE_FILE_WARN

#ifdef ENABLE_WOLFSSL_USER_TEST
/* You can select one or all of the following tests */
#define WOLFSSL_WOLFCRYPT_TEST
#define WOLFSSL_BENCHMARK_TEST
#define WOLFSSL_CLIENT_TEST
#define WOLFSSL_SERVER_TEST

#ifdef WOLFSSL_BENCHMARK_TEST
/* Only output benchmark test results in English */
#define NO_MULTIBYTE_PRINT
#endif
#else
#define NO_CRYPT_TEST
#define NO_CRYPT_BENCHMARK
#endif /* ENABLE_WOLFSSL_USER_TEST */

/* Only for test */
#define USE_TEST_GENSEED

#define NO_DEV_RANDOM
// #define HAVE_PKCS7
#define HAVE_AES_KEYWRAP
// #define HAVE_X963_KDF
#define WOLFSSL_AES_DIRECT

#define NO_FILESYSTEM

/* only for test */
#if 0
#define HAVE_VALGRIND
#define WOLFSSL_PUBLIC_MP
#define WOLFSSL_KEY_GEN
#endif
/* for test end */

#define USER_TIME
#ifdef USER_TIME
    // extern unsigned long ln_wolfssl_xtime(unsigned long* timer);
    #define XTIME my_time
    // #define XGMTIME
    // #define XVALIDATE_DATE
#endif /* USER_TIME */

/* Override Current Time for benchmark */
#if 0
    // TIME_OVERRIDES
    /* Allows custom "custom_time()" function to be used for benchmark */
    #define WOLFSSL_USER_CURRTIME
    #define WOLFSSL_GMTIME
    #define USER_TICKS
#endif

/* ln882h: arm cortex-m4f core, 32bit cpu */
#define SIZEOF_LONG      4
#define SIZEOF_LONG_LONG 8

/**
 * prevents from including multiple definition of main()
 * Please call wolfssl_test() in user task.
*/
#define NO_MAIN_DRIVER
#define NO_TESTSUITE_MAIN_DRIVER

/* includes certificate test buffers via header files */
#define USE_CERT_BUFFERS_2048
/*use kB instead of mB for embedded benchmarking*/
#define BENCH_EMBEDDED

#define NO_WRITE_TEMP_FILES

// #define XSNPRINTF snprintf
#define NO_WRITEV

#define HAVE_AESGCM
// #define WOLFSSL_SHA512
#define HAVE_ECC
// #define HAVE_CURVE25519
// #define CURVE25519_SMALL
// #define HAVE_ED25519
// #define ED25519_SMALL

/* ------------------------------------------------------------------------- */
/* Crypto */
/* ------------------------------------------------------------------------- */
/* RSA */
#undef NO_RSA
#if 1
    #ifdef USE_FAST_MATH
        /* Maximum math bits (Max RSA key bits * 2) */
        #undef  FP_MAX_BITS
        #define FP_MAX_BITS     4096
    #endif

    /* half as much memory but twice as slow */
    #undef  RSA_LOW_MEM
    //#define RSA_LOW_MEM

    /* Enables blinding mode, to prevent timing attacks */
    #if 0
        #undef  WC_RSA_BLINDING
        #define WC_RSA_BLINDING
    #else
        #undef  WC_NO_HARDEN
        #define WC_NO_HARDEN
    #endif

    /* RSA PSS Support */
    #if 0
        #define WC_RSA_PSS
    #endif

    #if 0
        #define WC_RSA_NO_PADDING
    #endif
#else
    #define NO_RSA
#endif

#ifdef __cplusplus
    }   /* extern "C" */
#endif

#endif
