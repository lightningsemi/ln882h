/*
 * Simultaneous authentication of equals
 * Copyright (c) 2012-2016, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

/**
 * @file   ln_wpa_sae.c
 * 
 * Modified by LightningSemi WLAN Team
 * 
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-07     MurphyZhao   the first version
 */

#include "ln_wpa_sae.h"

#ifdef LN_WPA_SAE_SUPPORTED
#include "ln_wpa_dragonfly.h"
#include "wifi_port.h"
#include "osal/osal.h" // OS_Rand32()

/* wpa_supplicant header files */
#include "crypto/crypto.h"
#include "crypto/sha256.h"

// #define LN_WPA_SAE_TRACE_ENABLED
// #define LN_WPA_SAE_TEST

#define LN_WPA_SAE_ONLY_SUPP_GROUP (19)
#define ETH_ALEN MAC_ADDRESS_LEN

/**
 * ECC Group 19 --> H() SHA-256, Q = 256
 */
#define SAE_KCK_LEN 32           // 256 bit, KCK=L(kck_and_pmk, 0, Q) . If sha-256 then Q is 256.
#define SAE_PMK_LEN 32           // 256 bit, PMK=L(kck_and_pmk, Q, 256)
#define SAE_PMKID_LEN 16         // 128 bit, PMKID=L(context, 0, 128)
#define SAE_MAX_PRIME_LEN 512    // Group-19 prime len 256bit
#define SAE_MAX_ECC_PRIME_LEN 66
#define DRAGONFLY_MAX_ECC_PRIME_LEN 66
#define SAE_MAX_HASH_LEN 64      // max 512 bit, SHA-512
#define SAE_COMMIT_MAX_LEN (2 + 3 * SAE_MAX_PRIME_LEN + 255)

enum sae_state {
    SAE_NOTHING, SAE_COMMITTED, SAE_CONFIRMED, SAE_ACCEPTED
};

typedef struct {
    struct crypto_bignum *own_commit_scalar;
    struct crypto_ec_point *own_commit_element_ecc;
    struct crypto_bignum *peer_commit_scalar;
    struct crypto_ec_point *peer_commit_element_ecc;
    struct crypto_bignum *peer_commit_scalar_accepted;
    struct crypto_ec_point *pwe_ecc;
    struct crypto_bignum *sae_rand;
    struct crypto_ec *ec;
    const struct crypto_bignum *prime;
    const struct crypto_bignum *order;
    uint8_t kck[SAE_MAX_HASH_LEN];
    size_t kck_len;
    int prime_len;
    int order_len;
#ifdef LN_WPA_SAE_TRACE_ENABLED
    uint8_t local_rand[32];
    uint8_t local_mask[32];
#endif
} ln_sae_crypto_data_t;

typedef struct {
    uint8_t scalar[32];
    uint8_t commit_element[64];
    uint8_t own_confirm[34];  /* send_confirm + confirm */
    uint8_t own_verifier[34]; /* send_confirm + confirm */
} ln_sae_cached_data_t;

typedef struct {
    enum sae_state state;

    /* temp data. when auth finish, this will be cleared */
    ln_sae_crypto_data_t crypto;
    ln_sae_cached_data_t cached_data;
    uint8_t passphrase[PASSWORD_MAX_LEN];
    uint8_t own_mac[MAC_ADDRESS_LEN];

    /* peer mac address */
    uint8_t prev_bssid[MAC_ADDRESS_LEN];
    uint8_t password_len;
    uint8_t pmk[SAE_PMK_LEN];
    uint8_t pmkid[SAE_PMKID_LEN];

    /* counter: sc */
    uint16_t send_confirm;
} ln_sae_data_t;

static ln_sae_data_t sg_sae_data = {SAE_NOTHING, 0};
static bool sg_ln_wpa_sae_enabled = false;

bool ln_wpa_sae_is_enabled(void)
{
    return sg_ln_wpa_sae_enabled;
}

void ln_wpa_sae_enable(void)
{
    sg_ln_wpa_sae_enabled = 1;
}

void ln_wpa_sae_disable(void)
{
    sg_ln_wpa_sae_enabled = 0;
}

static void buf_shift_right(uint8_t *buf, size_t len, size_t bits)
{
    size_t i;

    for (i = len - 1; i > 0; i--)
        buf[i] = (buf[i - 1] << (8 - bits)) | (buf[i] >> bits);
    buf[0] >>= bits;
}

static void sae_clear_data(ln_sae_data_t *sae)
{
    ln_sae_crypto_data_t *tmp;
    if (sae == NULL) {
        return;
    }

    tmp = &sae->crypto;
    crypto_ec_deinit(sae->crypto.ec);
    sae->crypto.ec = NULL;
    tmp->prime = NULL; /* crypto.prime = crypto.ec.prime */
    tmp->order = NULL;

    crypto_bignum_deinit(tmp->sae_rand, 0);
    tmp->sae_rand = NULL;

    crypto_bignum_deinit(tmp->own_commit_scalar, 1);
    tmp->own_commit_scalar = NULL;

    crypto_ec_point_deinit(tmp->pwe_ecc, 1);
    tmp->pwe_ecc = NULL;

    crypto_ec_point_deinit(tmp->own_commit_element_ecc, 1);
    tmp->own_commit_element_ecc = NULL;

    crypto_ec_point_deinit(tmp->peer_commit_element_ecc, 1);
    tmp->peer_commit_element_ecc = NULL;

    crypto_bignum_deinit(sae->crypto.peer_commit_scalar, 1);
    crypto_bignum_deinit(sae->crypto.peer_commit_scalar_accepted, 1);

    sae->crypto.peer_commit_scalar = NULL;
    sae->crypto.peer_commit_scalar_accepted = NULL;
}

static void sae_pwd_seed_key(const uint8_t *addr1, const uint8_t *addr2, uint8_t *key)
{
    if (memcmp(addr1, addr2, ETH_ALEN) > 0) {
        memcpy(key, addr1, ETH_ALEN);
        memcpy(key + ETH_ALEN, addr2, ETH_ALEN);
    } else {
        memcpy(key, addr2, ETH_ALEN);
        memcpy(key + ETH_ALEN, addr1, ETH_ALEN);
    }
}

static int random_get_bytes(void *buf, size_t len)
{
    uint8_t *ptr = buf;
    if (!buf || (len <= 0)) {
        return -1;
    }

    srand(OS_GetTicks());

    for (int i = 0; i < len; i++)
    {
        ptr[i] = (uint8_t)rand();
    }
    return 0;
}

/**
 * const_time_select - Constant time unsigned int selection
 * @mask: 0 (false) or -1 (true) to identify which value to select
 * @true_val: Value to select for the true case
 * @false_val: Value to select for the false case
 * Returns: true_val if mask == -1, false_val if mask == 0
 */
static inline unsigned int const_time_select(unsigned int mask,
                         unsigned int true_val,
                         unsigned int false_val)
{
    return (mask & true_val) | (~mask & false_val);
}

/**
 * const_time_select_int - Constant time int selection
 * @mask: 0 (false) or -1 (true) to identify which value to select
 * @true_val: Value to select for the true case
 * @false_val: Value to select for the false case
 * Returns: true_val if mask == -1, false_val if mask == 0
 */
static inline int const_time_select_int(unsigned int mask, int true_val,
                    int false_val)
{
    return (int) const_time_select(mask, (unsigned int) true_val,
                       (unsigned int) false_val);
}

/**
 * const_time_fill_msb - Fill all bits with MSB value
 * @val: Input value
 * Returns: Value with all the bits set to the MSB of the input val
 */
static inline unsigned int const_time_fill_msb(unsigned int val)
{
    /* Move the MSB to LSB and multiple by -1 to fill in all bits. */
    return (val >> (sizeof(val) * 8 - 1)) * ~0U;
}

/* Returns: -1 if val is zero; 0 if val is not zero */
static inline unsigned int const_time_is_zero(unsigned int val)
{
    /* Set MSB to 1 for 0 and fill rest of bits with the MSB value */
    return const_time_fill_msb(~val & (val - 1));
}

/* Returns: -1 if a == b; 0 if a != b */
static inline unsigned int const_time_eq(unsigned int a, unsigned int b)
{
    return const_time_is_zero(a ^ b);
}

static inline int const_time_memcmp(const void *a, const void *b, size_t len)
{
    const uint8_t *aa = a;
    const uint8_t *bb = b;
    int diff, res = 0;
    unsigned int mask;

    if (len == 0)
        return 0;
    do {
        len--;
        diff = (int) aa[len] - (int) bb[len];
        mask = const_time_is_zero((unsigned int) diff);
        res = const_time_select_int(mask, res, diff);
    } while (len);

    return res;
}

/**
 * const_time_select_u8 - Constant time uint8_t selection
 * @mask: 0 (false) or -1 (true) to identify which value to select
 * @true_val: Value to select for the true case
 * @false_val: Value to select for the false case
 * Returns: true_val if mask == -1, false_val if mask == 0
 */
static inline uint8_t const_time_select_u8(uint8_t mask, uint8_t true_val, uint8_t false_val)
{
    return (uint8_t) const_time_select(mask, true_val, false_val);
}

/**
 * const_time_select_bin - Constant time binary buffer selection copy
 * 恒定时间二进制缓冲区选择拷贝
 * @mask: 0 (false) or -1 (true) to identify which value to copy
 *        0 (false) 或 -1 (true) 标识要复制的值
 * @true_val: Buffer to copy for the true case
 *            为真实情况复制的缓冲区
 * @false_val: Buffer to copy for the false case
 *             用于复制错误情况的缓冲区
 * @len: Number of octets to copy 要复制的八位字节数
 * @dst: Destination buffer for the copy
 * 此函数使用具有相同内存访问模式的操作将指定的缓冲区复制到目标缓冲区，而不管正在复制哪个缓冲区。
 * This function copies the specified buffer into the destination buffer using
 * operations with identical memory access pattern regardless of which buffer
 * is being copied.
 */
static inline void const_time_select_bin(uint8_t mask, const uint8_t *true_val,
                     const uint8_t *false_val, size_t len,
                     uint8_t *dst)
{
    size_t i;

    for (i = 0; i < len; i++)
        dst[i] = const_time_select_u8(mask, true_val[i], false_val[i]);
}

static int dragonfly_get_random_qr_qnr(const struct crypto_bignum *prime,
                struct crypto_bignum **qr,
                struct crypto_bignum **qnr)
{
    uint32_t cnt = 0;
    *qr = *qnr = NULL;

    while (!(*qr) || !(*qnr)) {
        struct crypto_bignum *tmp;
        int res;
        cnt ++;
        if (cnt > 1000) {
            *qr = *qnr = NULL;
            break;
        }

        tmp = crypto_bignum_init();
        if (!tmp || crypto_bignum_rand(tmp, prime) < 0) {
            crypto_bignum_deinit(tmp, 0);
            break;
        }

        res = crypto_bignum_legendre(tmp, prime);
        if (res == 1 && !(*qr))
            *qr = tmp;
        else if (res == -1 && !(*qnr))
            *qnr = tmp;
        else
            crypto_bignum_deinit(tmp, 0);
    }
    WLIB_LOG_D("dragonfly cal cnt:%d\r\n", cnt);

    if (*qr && *qnr)
        return 0;
    crypto_bignum_deinit(*qr, 0);
    crypto_bignum_deinit(*qnr, 0);
    *qr = *qnr = NULL;
    return -1;
}

static struct crypto_bignum *
dragonfly_get_rand_1_to_p_1(const struct crypto_bignum *prime)
{
    struct crypto_bignum *tmp, *pm1, *one;

    tmp = crypto_bignum_init();
    pm1 = crypto_bignum_init();
    one = crypto_bignum_init_set((const uint8_t *) "\x01", 1);
    if (!tmp || !pm1 || !one ||
        crypto_bignum_sub(prime, one, pm1) < 0 ||
        crypto_bignum_rand(tmp, pm1) < 0 ||
        crypto_bignum_add(tmp, one, tmp) < 0) {
        crypto_bignum_deinit(tmp, 0);
        tmp = NULL;
    }

    crypto_bignum_deinit(pm1, 0);
    crypto_bignum_deinit(one, 0);
    return tmp;
}

static int dragonfly_is_quadratic_residue_blind(struct crypto_ec *ec,
                     const uint8_t *qr, const uint8_t *qnr,
                     const struct crypto_bignum *val)
{
    struct crypto_bignum *r, *num, *qr_or_qnr = NULL;
    int check, res = -1;
    uint8_t *qr_or_qnr_bin = NULL; //[DRAGONFLY_MAX_ECC_PRIME_LEN];
    const struct crypto_bignum *prime;
    size_t prime_len;
    unsigned int mask;

    prime = crypto_ec_get_prime(ec);
    prime_len = crypto_ec_prime_len(ec);

    /*
     * Use a blinding technique to mask val while determining whether it is
     * a quadratic residue modulo p to avoid leaking timing information
     * while determining the Legendre symbol.
     *
     * v = val
     * r = a random number between 1 and p-1, inclusive
     * num = (v * r * r) modulo p
     */
    r = dragonfly_get_rand_1_to_p_1(prime);
    if (!r)
        return -1;

    num = crypto_bignum_init();
    if (!num ||
        crypto_bignum_mulmod(val, r, prime, num) < 0 ||
        crypto_bignum_mulmod(num, r, prime, num) < 0)
        goto fail;

    qr_or_qnr_bin = os_malloc(DRAGONFLY_MAX_ECC_PRIME_LEN);
    if (!qr_or_qnr_bin) {
        goto fail;
    }

    /*
     * Need to minimize differences in handling different cases, so try to
     * avoid branches and timing differences.
     *
     * If r is odd:
     * num = (num * qr) module p
     * LGR(num, p) = 1 ==> quadratic residue
     * else:
     * num = (num * qnr) module p
     * LGR(num, p) = -1 ==> quadratic residue
     *
     * mask is set to !odd(r)
     */
    mask = const_time_is_zero(crypto_bignum_is_odd(r));
    const_time_select_bin(mask, qnr, qr, prime_len, qr_or_qnr_bin);
    qr_or_qnr = crypto_bignum_init_set(qr_or_qnr_bin, prime_len);
    if (!qr_or_qnr ||
        crypto_bignum_mulmod(num, qr_or_qnr, prime, num) < 0)
        goto fail;
    /* branchless version of check = odd(r) ? 1 : -1, */
    check = const_time_select_int(mask, -1, 1);

    /* Determine the Legendre symbol on the masked value */
    res = crypto_bignum_legendre(num, prime);
    if (res == -2) {
        res = -1;
        goto fail;
    }
    /* branchless version of res = res == check
     * (res is -1, 0, or 1; check is -1 or 1) */
    mask = const_time_eq(res, check);
    res = const_time_select_int(mask, 1, 0);
fail:
    crypto_bignum_deinit(num, 1);
    crypto_bignum_deinit(r, 1);
    crypto_bignum_deinit(qr_or_qnr, 1);

    if (qr_or_qnr_bin) {os_free(qr_or_qnr_bin);}
    return res;
}

/* res = sqrt(val) */
static int dragonfly_sqrt(struct crypto_ec *ec, const struct crypto_bignum *val,
           struct crypto_bignum *res)
{
    const struct crypto_bignum *prime;
    struct crypto_bignum *tmp, *one;
    int ret = 0;
    uint8_t *prime_bin = NULL; // [DRAGONFLY_MAX_ECC_PRIME_LEN];
    size_t prime_len;

    /* For prime p such that p = 3 mod 4, sqrt(w) = w^((p+1)/4) mod p */

    prime_bin = os_malloc(DRAGONFLY_MAX_ECC_PRIME_LEN);
    if (!prime_bin) {
        return -1;
    }

    prime = crypto_ec_get_prime(ec);
    prime_len = crypto_ec_prime_len(ec);
    tmp = crypto_bignum_init();
    one = crypto_bignum_init_uint(1);

    if (crypto_bignum_to_bin(prime, prime_bin, DRAGONFLY_MAX_ECC_PRIME_LEN,
                 prime_len) < 0 ||
        (prime_bin[prime_len - 1] & 0x03) != 3 ||
        !tmp || !one ||
        /* tmp = (p+1)/4 */
        crypto_bignum_add(prime, one, tmp) < 0 ||
        crypto_bignum_rshift(tmp, 2, tmp) < 0 ||
        /* res = sqrt(val) */
        crypto_bignum_exptmod(val, tmp, prime, res) < 0)
        ret = -1;

    crypto_bignum_deinit(tmp, 0);
    crypto_bignum_deinit(one, 0);
    if (prime_bin) {os_free(prime_bin);}
    return ret;
}

static int sae_test_pwd_seed_ecc(ln_sae_data_t *sae, const uint8_t *pwd_seed,
                 const uint8_t *prime, const uint8_t *qr, const uint8_t *qnr,
                 uint8_t *pwd_value)
{
    struct crypto_bignum *y_sqr, *x_cand;
    int res;
    size_t bits;
    int cmp_prime;
    unsigned int in_range;

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: pwd-seed", (void *)pwd_seed, SHA256_MAC_LEN);
#endif

    /* pwd-value = KDF-z(pwd-seed, "SAE Hunting and Pecking", p) */
    bits = crypto_ec_prime_len_bits( sae->crypto.ec);
    if (sha256_prf_bits(pwd_seed, SHA256_MAC_LEN, "SAE Hunting and Pecking",
                prime, sae->crypto.prime_len, pwd_value, bits) < 0)
        return -1;
    if (bits % 8)
        buf_shift_right(pwd_value, sae->crypto.prime_len, 8 - bits % 8);

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: pwd-value",
            pwd_value, sae->crypto.prime_len);
#endif

    cmp_prime = const_time_memcmp(pwd_value, prime, sae->crypto.prime_len);
    /* Create a const_time mask for selection based on prf result
     * being smaller than prime. */
    in_range = const_time_fill_msb((unsigned int) cmp_prime);
    /* The algorithm description would skip the next steps if
     * cmp_prime >= 0 (return 0 here), but go through them regardless to
     * minimize externally observable differences in behavior. */

    x_cand = crypto_bignum_init_set(pwd_value, sae->crypto.prime_len);
    if (!x_cand)
        return -1;

    y_sqr = crypto_ec_point_compute_y_sqr( sae->crypto.ec, x_cand);
    crypto_bignum_deinit(x_cand, 1);
    if (!y_sqr)
        return -1;

    res = dragonfly_is_quadratic_residue_blind( sae->crypto.ec, qr, qnr,
                           y_sqr);
    crypto_bignum_deinit(y_sqr, 1);
    if (res < 0)
        return res;
    return const_time_select_int(in_range, res, 0);
}

static int sae_derive_pwe_ecc(ln_sae_data_t *sae)
{
    int res = -1;

    /*
     * Continue for at least k iterations to protect against side-channel
     * attacks that attempt to determine the number of iterations required
     * in the loop.
     * 
     * if ECC Group, k = 40
     */
    uint8_t k = 40;
    uint8_t counter = 0;

    /* 0 (false) or 0xff (true) to be used as const_time_mask */
    uint8_t found = 0;
    int total_len = (2 * MAC_ADDRESS_LEN) + (7 * SAE_MAX_ECC_PRIME_LEN);

    /* size(tmp_buf) = total_len */
    uint8_t *tmp_buf = NULL;

    uint8_t *addrs = NULL;
    const uint8_t *addr[2];
    size_t len[2];
    uint8_t *stub_password = NULL, *tmp_password = NULL;

    int pwd_seed_odd = 0;

    uint8_t *prime = NULL;
    size_t prime_len = 0;

    struct crypto_bignum *x = NULL, *y = NULL, *qr = NULL, *qnr = NULL;

    uint8_t *x_bin = NULL;
    uint8_t *x_cand_bin = NULL;
    uint8_t *qr_bin = NULL;
    uint8_t *qnr_bin = NULL;
    uint8_t *x_y = NULL;

    unsigned int is_eq;
    uint8_t *pwd_seed = NULL;

    pwd_seed = os_malloc(SHA256_MAC_LEN);
    tmp_buf = os_malloc(total_len);
    if (!tmp_buf || !pwd_seed) {
        goto fail;
    }

    memset(tmp_buf, 0, total_len);
    memset(pwd_seed, 0, SHA256_MAC_LEN);

    addrs      = &tmp_buf[0];                        /* 2 * MAC_ADDRESS_LEN   */
    prime      = &tmp_buf[2 * MAC_ADDRESS_LEN];      /* SAE_MAX_ECC_PRIME_LEN */
    x_bin      = &prime[SAE_MAX_ECC_PRIME_LEN];      /* SAE_MAX_ECC_PRIME_LEN */
    x_cand_bin = &x_bin[SAE_MAX_ECC_PRIME_LEN];      /* SAE_MAX_ECC_PRIME_LEN */
    qr_bin     = &x_cand_bin[SAE_MAX_ECC_PRIME_LEN]; /* SAE_MAX_ECC_PRIME_LEN */
    qnr_bin    = &qr_bin[SAE_MAX_ECC_PRIME_LEN];     /* SAE_MAX_ECC_PRIME_LEN */
    x_y        = &qnr_bin[SAE_MAX_ECC_PRIME_LEN];    /* 2 * SAE_MAX_ECC_PRIME_LEN */

    stub_password = os_malloc(sae->password_len);
    tmp_password = os_malloc(sae->password_len);
    if (!stub_password || !tmp_password ||
        random_get_bytes(stub_password, sae->password_len) < 0) {
        goto fail;
    }

    memset(stub_password, 0, sae->password_len);
    memset(tmp_password, 0, sae->password_len);

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("stub_password", stub_password, sae->password_len);
#endif

    prime_len = sae->crypto.prime_len;
    if (crypto_bignum_to_bin(sae->crypto.prime, prime, SAE_MAX_ECC_PRIME_LEN,
                 prime_len) < 0)
        goto fail;

    /*
     * Create a random quadratic residue (qr) and quadratic non-residue
     * (qnr) modulo p for blinding purposes during the loop.
     */
    WLIB_LOG_D("--> Start qr qnr\r\n");
    if (dragonfly_get_random_qr_qnr(sae->crypto.prime, &qr, &qnr) < 0 ||
        crypto_bignum_to_bin(qr, qr_bin, SAE_MAX_ECC_PRIME_LEN, prime_len) < 0 ||
        crypto_bignum_to_bin(qnr, qnr_bin, SAE_MAX_ECC_PRIME_LEN, prime_len) < 0) {
        WLIB_LOG_D("dragonfly failed!\r\n");
        goto fail;
    }
    WLIB_LOG_D("<-- End qr qnr\r\n");

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: password", sae->passphrase, sae->password_len);
#endif

    /*
     * H(salt, ikm) = HMAC-SHA256(salt, ikm)
     * base = password
     * pwd-seed = H(MAX(STA-A-MAC, STA-B-MAC) || MIN(STA-A-MAC, STA-B-MAC),
     *              base || counter)
     */
    sae_pwd_seed_key(sae->own_mac, sae->prev_bssid, addrs);
    addr[0] = tmp_password;
    len[0]  = sae->password_len;
    addr[1] = &counter;
    len[1]  = sizeof(counter);

    for (counter = 1; counter <= k || !found; counter++) {
        if (counter > 200) {
            /* This should not happen in practice */
            WLIB_LOG_D("SAE: Failed to derive PWE\r\n");
            break;
        }

        const_time_select_bin(found, stub_password, sae->passphrase,
                      sae->password_len, tmp_password);
        if (hmac_sha256_vector(addrs, (2 * MAC_ADDRESS_LEN), 2,
                       addr, len, pwd_seed) < 0)
            break;

        /* pwd-value = KDF-z(pwd-seed, "SAE Hunting and Pecking", p) */
        res = sae_test_pwd_seed_ecc(sae, pwd_seed,
                        prime, qr_bin, qnr_bin, x_cand_bin);

        const_time_select_bin(found, x_bin, x_cand_bin, prime_len,
                      x_bin);

        pwd_seed_odd = const_time_select_u8(
            found, pwd_seed_odd,
            pwd_seed[SHA256_MAC_LEN - 1] & 0x01);
        memset(pwd_seed, 0, SHA256_MAC_LEN);
        if (res < 0) {
            WLIB_LOG_E("sae_test_pwd_seed_ecc failed!\r\n");
            goto fail;
        }

        /* Need to minimize differences in handling res == 0 and 1 here
         * to avoid differences in timing and instruction cache access,
         * so use const_time_select_*() to make local copies of the
         * values based on whether this loop iteration was the one that
         * found the pwd-seed/x. */

        /* found is 0 or 0xff here and res is 0 or 1. Bitwise OR of them
         * (with res converted to 0/0xff) handles this in constant time.
         */
        found |= res * 0xff;
        // WLIB_LOG_D("SAE: pwd-seed result %d found=0x%02x\r\n",
        //        res, found);
    }

    if (!found) {
        WLIB_LOG_D("SAE: Could not generate PWE\r\n");
        res = -1;
        goto fail;
    }
    WLIB_LOG_D("SAE: counter = %03u\r\n", counter);

    x = crypto_bignum_init_set(x_bin, prime_len);
    if (!x) {
        res = -1;
        goto fail;
    }

    WLIB_LOG_D("Start calculate the two possible values for PWE\r\n");

    /* y = sqrt(x^3 + ax + b) mod p
     * if LSB(save) == LSB(y): PWE = (x, y)
     * else: PWE = (x, p - y)
     * Calculate y and the two possible values for PWE and after that,
     * use constant time selection to copy the correct alternative.
     */
    y = crypto_ec_point_compute_y_sqr(sae->crypto.ec, x);
    if (!y ||
        dragonfly_sqrt(sae->crypto.ec, y, y) < 0 ||
        crypto_bignum_to_bin(y, x_y, SAE_MAX_ECC_PRIME_LEN,
                 prime_len) < 0 ||
        crypto_bignum_sub(sae->crypto.prime, y, y) < 0 ||
        crypto_bignum_to_bin(y, x_y + SAE_MAX_ECC_PRIME_LEN,
                 SAE_MAX_ECC_PRIME_LEN, prime_len) < 0) {
        WLIB_LOG_D("SAE: Could not solve y\r\n");
        goto fail;
    }

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("x_y before", x_y, 2 * SAE_MAX_ECC_PRIME_LEN);
#endif

    is_eq = const_time_eq(pwd_seed_odd, x_y[prime_len - 1] & 0x01);
    const_time_select_bin(is_eq, x_y, x_y + SAE_MAX_ECC_PRIME_LEN,
                  prime_len, x_y + prime_len);

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("x_y after", x_y, 2 * SAE_MAX_ECC_PRIME_LEN);
#endif

    os_memcpy(x_y, x_bin, prime_len);

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE PWE", x_y, 2 * prime_len); /* x_y is PWE */
#endif

    crypto_ec_point_deinit(sae->crypto.pwe_ecc, 1);
    /* pwe_ecc is a point of Elliptic Curve */
    sae->crypto.pwe_ecc = crypto_ec_point_from_bin(sae->crypto.ec, x_y);
    if (!sae->crypto.pwe_ecc) {
        WLIB_LOG_E("SAE: Could not generate PWE\r\n");
        res = -1;
    } else {
        WLIB_LOG_I("SAE: Generate PWE Success!\r\n\r\n");
    }

fail:
    crypto_bignum_deinit(qr, 0);
    crypto_bignum_deinit(qnr, 0);
    crypto_bignum_deinit(y, 1);
    crypto_bignum_deinit(x, 1);

    if (stub_password) {
        os_free(stub_password);
    }
    if (tmp_password) {
        os_free(tmp_password);
    }
    if (tmp_buf) {
        os_free(tmp_buf);
    }
    if (pwd_seed) {
        os_free(pwd_seed);
    }
    return res;
}

static int sae_derive_commit_element_ecc(ln_sae_data_t *sae,
                     struct crypto_bignum *mask)
{
    /* COMMIT-ELEMENT = inverse(scalar-op(mask, PWE)) */
    if (!sae->crypto.own_commit_element_ecc) {
        sae->crypto.own_commit_element_ecc =
            crypto_ec_point_init(sae->crypto.ec);
        if (!sae->crypto.own_commit_element_ecc)
            return -1;
    }

    if (crypto_ec_point_mul(sae->crypto.ec, sae->crypto.pwe_ecc, mask,
                sae->crypto.own_commit_element_ecc) < 0 ||
        crypto_ec_point_invert(sae->crypto.ec,
                   sae->crypto.own_commit_element_ecc) < 0) {
        WLIB_LOG_D("SAE: Could not compute commit-element\r\n");
        return -1;
    }
    WLIB_LOG_D("SAE: compute commit-element PASS!\r\n");

    return 0;
}

static int sae_derive_commit(ln_sae_data_t *sae)
{
    struct crypto_bignum *mask;
    int ret;

    mask = crypto_bignum_init();
    if (!sae->crypto.sae_rand)
        sae->crypto.sae_rand = crypto_bignum_init();
    if (!sae->crypto.own_commit_scalar)
        sae->crypto.own_commit_scalar = crypto_bignum_init();

    ret = !mask || !sae->crypto.sae_rand || !sae->crypto.own_commit_scalar ||
        dragonfly_generate_scalar(sae->crypto.order, sae->crypto.sae_rand,
                mask, sae->crypto.own_commit_scalar) < 0 ||
        (sae->crypto.ec && sae_derive_commit_element_ecc(sae, mask) < 0);

#ifdef LN_WPA_SAE_TRACE_ENABLED
    /* sae_rand and mask must be dumped here */
    crypto_bignum_to_bin(sae->crypto.sae_rand, sae->crypto.local_rand, 32, 32);
    wlib_hexdump("local rand", sae->crypto.local_rand, 32);

    crypto_bignum_to_bin(mask, sae->crypto.local_mask, 32, 32);
    wlib_hexdump("local mask", sae->crypto.local_mask, 32);
#endif

    crypto_bignum_deinit(mask, 1);
    return ret ? -1 : 0;
}

static int sae_prepare_commit(ln_sae_data_t *sae)
{
    if (sae->crypto.ec && sae_derive_pwe_ecc(sae) < 0) {
        return -1;
    }

    return sae_derive_commit(sae);
}

static int sme_auth_build_sae_commit(ln_sae_data_t *sae)
{
    sae_clear_data(sae);

    /* First, check if this is an ECC group */
    sae->crypto.ec = crypto_ec_init(LN_WPA_SAE_ONLY_SUPP_GROUP);
    if (sae->crypto.ec == NULL) {
        WLIB_LOG_E("[%s:%d] crypto_ec_init failed!\r\n", __func__, __LINE__);
        return -1;
    }

    sae->crypto.prime_len = crypto_ec_prime_len(sae->crypto.ec);
    sae->crypto.prime = crypto_ec_get_prime(sae->crypto.ec);
    sae->crypto.order_len = crypto_ec_order_len(sae->crypto.ec);
    sae->crypto.order = crypto_ec_get_order(sae->crypto.ec);
    WLIB_LOG_D("prime_len:%d, order_len:%d\r\n", 
            sae->crypto.prime_len, sae->crypto.order_len);

    if (sae_prepare_commit(sae) != 0) {
        WLIB_LOG_D("SAE: Could not pick PWE!\r\n");
        return -1;
    }

    return 0;
}

int ln_wpa_sae_start(ln_wpa_sae_in_param_t *sae_in_param,
        ln_wpa_sae_commit_info_t *commit_info)
{
    int status_code = 0;
    ln_sae_data_t *sae = &sg_sae_data;
    if (!sae_in_param || !commit_info) {
        return -1;
    }

    if (sae_in_param->group_id != LN_WPA_SAE_ONLY_SUPP_GROUP ||
        !sae_in_param->own_mac || !sae_in_param->peer_mac ||
       !sae_in_param->password ||
       (sae_in_param->password_len >= PASSWORD_MAX_LEN)) {
        return -1;
    }

    /* new bss */
    memcpy(sae->passphrase, sae_in_param->password, sae_in_param->password_len);
    memcpy(sae->own_mac, sae_in_param->own_mac, MAC_ADDRESS_LEN);
    memcpy(sae->prev_bssid, sae_in_param->peer_mac, MAC_ADDRESS_LEN);
    sae->password_len = sae_in_param->password_len;

    if (sme_auth_build_sae_commit(sae) != 0) {
        WLIB_LOG_D("Status: commit failed\r\n");
        status_code = WLAN_STATUS_CHALLENGE_FAIL;
        return status_code;
    } else {
        status_code = WLAN_STATUS_SUCCESS;
    }

    if (crypto_bignum_to_bin(sae->crypto.own_commit_scalar,
            &sae->cached_data.scalar[0],
            sizeof(sae->cached_data.scalar), sae->crypto.prime_len) < 0) {
        return -1;
    }

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: own commit-scalar",
            &sae->cached_data.scalar[0], sae->crypto.prime_len);
#endif

    if (crypto_ec_point_to_bin(sae->crypto.ec,
                sae->crypto.own_commit_element_ecc,
                &sae->cached_data.commit_element[0],
                &sae->cached_data.commit_element[sae->crypto.prime_len]) < 0) {
        return -1;
    }

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: own commit-element(x)",
            &sae->cached_data.commit_element[0], sae->crypto.prime_len);
    wlib_hexdump("SAE: own commit-element(y)",
            &sae->cached_data.commit_element[sae->crypto.prime_len], sae->crypto.prime_len);
#endif

    commit_info->scalar = (const uint8_t *)(&sae->cached_data.scalar[0]);
    commit_info->commit_element = (const uint8_t *)(&sae->cached_data.commit_element[0]);

    commit_info->auth_seq_num = 1;
    commit_info->status_code = status_code;
    sae->state = SAE_COMMITTED;
    return 0;
}

static int sae_derive_k_ecc(ln_sae_data_t *sae, uint8_t *k)
{
    struct crypto_ec_point *K;
    int ret = -1;

    K = crypto_ec_point_init(sae->crypto.ec);
    if (K == NULL)
        goto fail;

    /*
    * K = scalar-op(rand, (elem-op(scalar-op(peer-commit-scalar, PWE),
    *                                        PEER-COMMIT-ELEMENT)))
    * If K is identity element (point-at-infinity), reject
    * k = F(K) (= x coordinate)
    */

    if (crypto_ec_point_mul(sae->crypto.ec, sae->crypto.pwe_ecc,
                sae->crypto.peer_commit_scalar, K) < 0 ||
        crypto_ec_point_add(sae->crypto.ec, K,
                sae->crypto.peer_commit_element_ecc, K) < 0 ||
        crypto_ec_point_mul(sae->crypto.ec, K, sae->crypto.sae_rand, K) < 0 ||
        crypto_ec_point_is_at_infinity(sae->crypto.ec, K) ||
        crypto_ec_point_to_bin(sae->crypto.ec, K, k, NULL) < 0) {
        WLIB_LOG_D("SAE: Failed to calculate K and k\r\n");
        goto fail;
    }

#ifdef LN_WPA_SAE_TRACE_ENABLED
    WLIB_LOG_D("prime len:%d\r\n", sae->crypto.prime_len);
    wlib_hexdump("SAE: k", k, sae->crypto.prime_len);
#endif

    ret = 0;
fail:
    crypto_ec_point_deinit(K, 1);
    return ret;
}

static size_t sae_ecc_prime_len_2_hash_len(size_t prime_len)
{
    if (prime_len <= 256 / 8)
        return 32;
    if (prime_len <= 384 / 8)
        return 48;
    return 64;
}

static int hkdf_extract(size_t hash_len, const uint8_t *salt, size_t salt_len,
            size_t num_elem, const uint8_t *addr[], const size_t len[],
            uint8_t *prk)
{
    if (hash_len == 32) {
        return hmac_sha256_vector(salt, salt_len, num_elem, addr, len,
                    prk);
    } else {
        WLIB_LOG_D("hkdf_extract: unsupported hash_len <%d>\r\n", hash_len);
    }
    return -1;
}


static int sae_kdf_hash(size_t hash_len, const uint8_t *k, const char *label,
            const uint8_t *context, size_t context_len,
            uint8_t *out, size_t out_len)
{
    if (hash_len == 32)
        return sha256_prf(k, hash_len, label,
                context, context_len, out, out_len);
    return -1;
}

int ln_wpa_sae_kdf_hash(const uint8_t *key, size_t key_len, const char *label,
        const uint8_t *data, size_t data_len, uint8_t *buf, size_t buf_len)
{
    return sha256_prf(key, key_len, label, data, data_len, buf, buf_len);
}

int ln_wpa_sae_omac1_aes_128(const uint8_t *key, const uint8_t *data, int data_len, uint8_t * mac)
{
    return omac1_aes_128(key, data, data_len, mac);
}

static int sae_derive_keys(ln_sae_data_t *sae, const uint8_t *k)
{
    uint8_t *zero = NULL, *val = NULL, *keyseed = NULL, *keys = NULL;
    const uint8_t *salt;

    struct crypto_bignum *tmp = NULL;
    int ret = -1;
    size_t hash_len, salt_len, prime_len = sae->crypto.prime_len;
    const uint8_t *addr[1];
    size_t len[1];

    zero = (uint8_t *)os_malloc(SAE_MAX_HASH_LEN);
    val  = (uint8_t *)os_malloc(SAE_MAX_PRIME_LEN);
    keyseed = (uint8_t *)os_malloc(SAE_MAX_HASH_LEN);
    keys = (uint8_t *)os_malloc(2 * SAE_MAX_HASH_LEN + SAE_PMK_LEN);
    if (!zero || !val || !keyseed || !keys) {
        goto fail;
    }

    tmp = crypto_bignum_init();
    if (tmp == NULL)
        goto fail;

    /* keyseed = H(salt, k)
    * KCK || PMK = KDF-Hash-Length(keyseed, "SAE KCK and PMK",
    *                      (commit-scalar + peer-commit-scalar) modulo r)
    * PMKID = L((commit-scalar + peer-commit-scalar) modulo r, 0, 128)
    *
    * When SAE-PK is used,
    * KCK || PMK || KEK = KDF-Hash-Length(keyseed, "SAE-PK keys", context)
    */

    hash_len = sae_ecc_prime_len_2_hash_len(prime_len);

    os_memset(zero, 0, hash_len);
    salt = zero;
    salt_len = hash_len;

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: salt for keyseed derivation", (void *)salt, salt_len);
#endif

    addr[0] = k;
    len[0] = prime_len;
    if (hkdf_extract(hash_len, salt, salt_len, 1, addr, len, keyseed) < 0)
        goto fail;

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: keyseed", keyseed, hash_len);
#endif

    if (crypto_bignum_add(sae->crypto.own_commit_scalar,
                sae->crypto.peer_commit_scalar, tmp) < 0 ||
        crypto_bignum_mod(tmp, sae->crypto.order, tmp) < 0)
        goto fail;
    /* IEEE Std 802.11-2016 is not exactly clear on the encoding of the bit
    * string that is needed for KCK, PMK, and PMKID derivation, but it
    * seems to make most sense to encode the
    * (commit-scalar + peer-commit-scalar) mod r part as a bit string by
    * zero padding it from left to the length of the order (in full
    * octets). */
    crypto_bignum_to_bin(tmp, val, SAE_MAX_PRIME_LEN, sae->crypto.order_len);

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: PMKID", val, SAE_PMKID_LEN);
#endif

    if (sae_kdf_hash(hash_len, keyseed, "SAE KCK and PMK",
            val, sae->crypto.order_len,
            keys, hash_len + SAE_PMK_LEN) < 0)
        goto fail;

    forced_memzero(keyseed, SAE_MAX_HASH_LEN);
    memcpy(sae->crypto.kck, keys, hash_len);
    sae->crypto.kck_len = hash_len;
    memcpy(sae->pmk, keys + hash_len, SAE_PMK_LEN);
    memcpy(sae->pmkid, val, SAE_PMKID_LEN);

    forced_memzero(keys, 2 * SAE_MAX_HASH_LEN + SAE_PMK_LEN);

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: KCK", sae->crypto.kck, sae->crypto.kck_len);
    wlib_hexdump("SAE: PMK", sae->pmk, SAE_PMK_LEN);
#endif

    ret = 0;
fail:
    if (zero) {os_free(zero);}
    if (val)  {os_free(val);}
    if (keyseed) {os_free(keyseed);}
    if (keys) {os_free(keys);}

    if (tmp) {crypto_bignum_deinit(tmp, 0);}
    return ret;
}

static int sae_process_commit(ln_sae_data_t *sae)
{
    uint8_t *k = NULL;
    k = os_malloc(SAE_MAX_PRIME_LEN);
    if (!k) {
        return -1;
    }

    if ((sae->crypto.ec && sae_derive_k_ecc(sae, k) < 0) ||
        sae_derive_keys(sae, k) < 0) {
        os_free(k);
        return -1;
    }
    os_free(k);
    return 0;
}

static uint16_t sae_parse_commit_scalar(ln_sae_data_t *sae, ln_wpa_sae_commit_info_t *info)
{
    /* sae_parse_commit_scalar */
    struct crypto_bignum *peer_scalar = NULL;
    peer_scalar = crypto_bignum_init_set((const u8 *)info->scalar, sae->crypto.prime_len);
    if (peer_scalar == NULL) {
        return WLAN_STATUS_UNSPECIFIED_FAILURE;
    }

    /*
    * IEEE Std 802.11-2012, 11.3.8.6.1: If there is a protocol instance for
    * the peer and it is in Authenticated state, the new Commit Message
    * shall be dropped if the peer-scalar is identical to the one used in
    * the existing protocol instance.
    */
    if (sae->state == SAE_ACCEPTED && sae->crypto.peer_commit_scalar_accepted &&
        crypto_bignum_cmp(sae->crypto.peer_commit_scalar_accepted,
                    peer_scalar) == 0) {
        WLIB_LOG_D("SAE: Do not accept re-use of previous "
                "peer-commit-scalar\r\n");
        crypto_bignum_deinit(peer_scalar, 0);
        return WLAN_STATUS_UNSPECIFIED_FAILURE;
    }

    /* 1 < scalar < r */
    if (crypto_bignum_is_zero(peer_scalar) ||
        crypto_bignum_is_one(peer_scalar) ||
        crypto_bignum_cmp(peer_scalar, sae->crypto.order) >= 0) {
        WLIB_LOG_D("SAE: Invalid peer scalar\r\n");
        crypto_bignum_deinit(peer_scalar, 0);
        return WLAN_STATUS_UNSPECIFIED_FAILURE;
    }

    crypto_bignum_deinit(sae->crypto.peer_commit_scalar, 0);
    sae->crypto.peer_commit_scalar = peer_scalar;
    return WLAN_STATUS_SUCCESS;
}

/* sae_parse_commit_element_ecc */
static uint16_t sae_parse_commit_element_ecc(ln_sae_data_t *sae, ln_wpa_sae_commit_info_t *info)
{
    int res = WLAN_STATUS_SUCCESS;
    uint8_t *prime = NULL;
    prime = os_malloc(SAE_MAX_ECC_PRIME_LEN);
    if (!prime) {
        res = WLAN_STATUS_UNSPECIFIED_FAILURE;
        goto fail;
    }

    if (crypto_bignum_to_bin(sae->crypto.prime, prime,
                SAE_MAX_ECC_PRIME_LEN * sizeof(uint8_t),
                sae->crypto.prime_len) < 0) {
        res = WLAN_STATUS_UNSPECIFIED_FAILURE;
        goto fail;
    }

    /* element x and y coordinates < p */
    if (os_memcmp(info->commit_element, prime, sae->crypto.prime_len) >= 0 ||
        os_memcmp(info->commit_element + sae->crypto.prime_len, prime,
            sae->crypto.prime_len) >= 0) {
        WLIB_LOG_D("SAE: Invalid coordinates in peer element\r\n");
        res = WLAN_STATUS_UNSPECIFIED_FAILURE;
        goto fail;
    }

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: Peer commit-element(x)",
            (void *)info->commit_element, sae->crypto.prime_len);
    wlib_hexdump("SAE: Peer commit-element(y)",
            (void *)(info->commit_element + sae->crypto.prime_len),
            sae->crypto.prime_len);
#endif

    crypto_ec_point_deinit(sae->crypto.peer_commit_element_ecc, 0);
    sae->crypto.peer_commit_element_ecc =
        crypto_ec_point_from_bin(sae->crypto.ec, (const u8 *)info->commit_element);
    if (sae->crypto.peer_commit_element_ecc == NULL) {
        res = WLAN_STATUS_UNSPECIFIED_FAILURE;
        goto fail;
    }

    if (!crypto_ec_point_is_on_curve(sae->crypto.ec,
                    sae->crypto.peer_commit_element_ecc)) {
        WLIB_LOG_D("SAE: Peer element is not on curve\r\n");
        res = WLAN_STATUS_UNSPECIFIED_FAILURE;
        goto fail;
    }

fail:
    if (prime) {os_free(prime);}
    return res;
}

static uint16_t sae_parse_commit(ln_sae_data_t *sae, ln_wpa_sae_commit_info_t *info)
{
    uint16_t res = WLAN_STATUS_SUCCESS;
    /* commit-scalar */
    res = sae_parse_commit_scalar(sae, info);
    if (res != WLAN_STATUS_SUCCESS) {
        return res;
    }

    /* commit-element */
    res = sae_parse_commit_element_ecc(sae, info);
    if (res != WLAN_STATUS_SUCCESS) {
        return res;
    }

    /*
    * Check whether peer-commit-scalar and PEER-COMMIT-ELEMENT are same as
    * the values we sent which would be evidence of a reflection attack.
    */
    if (!sae->crypto.own_commit_scalar ||
        crypto_bignum_cmp(sae->crypto.own_commit_scalar,
                    sae->crypto.peer_commit_scalar) != 0 ||
        (sae->crypto.ec &&
            (!sae->crypto.own_commit_element_ecc ||
            crypto_ec_point_cmp(sae->crypto.ec,
                    sae->crypto.own_commit_element_ecc,
                    sae->crypto.peer_commit_element_ecc) != 0))) {
        return WLAN_STATUS_SUCCESS; /* scalars/elements are different */
    }

    /*
    * This is a reflection attack - return special value to trigger caller
    * to silently discard the frame instead of replying with a specific
    * status code.
    */
    return SAE_SILENTLY_DISCARD;
}

static int sae_cn_confirm(ln_sae_data_t *sae, const uint8_t *sc,
                const struct crypto_bignum *scalar1,
                const uint8_t *element1, size_t element1_len,
                const struct crypto_bignum *scalar2,
                const uint8_t *element2, size_t element2_len,
                uint8_t *confirm)
{
    int res = 0;
    const uint8_t *addr[5];
    size_t len[5];
    uint8_t *scalar_b1 = NULL, *scalar_b2 = NULL;
    scalar_b1 = os_malloc(SAE_MAX_PRIME_LEN);
    scalar_b2 = os_malloc(SAE_MAX_PRIME_LEN);
    if (!scalar_b1 || !scalar_b2) {
        return -1;
    }

    /* Confirm
    * CN(key, X, Y, Z, ...) =
    *    HMAC-SHA256(key, D2OS(X) || D2OS(Y) || D2OS(Z) | ...)
    * confirm = CN(KCK, send-confirm, commit-scalar, COMMIT-ELEMENT,
    *              peer-commit-scalar, PEER-COMMIT-ELEMENT)
    * verifier = CN(KCK, peer-send-confirm, peer-commit-scalar,
    *               PEER-COMMIT-ELEMENT, commit-scalar, COMMIT-ELEMENT)
    */
    if (crypto_bignum_to_bin(scalar1, scalar_b1, SAE_MAX_PRIME_LEN,
                sae->crypto.prime_len) < 0 ||
        crypto_bignum_to_bin(scalar2, scalar_b2, SAE_MAX_PRIME_LEN,
                sae->crypto.prime_len) < 0) {
        res = -1;
        goto fail;
    }

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("SAE: scalar_b1",
            scalar_b1, sae->crypto.prime_len);

    wlib_hexdump("SAE: scalar_b2",
            scalar_b2, sae->crypto.prime_len);
#endif

    addr[0] = sc;
    len[0] = 2;
    addr[1] = scalar_b1;
    len[1] = sae->crypto.prime_len;
    addr[2] = element1;
    len[2] = element1_len;
    addr[3] = scalar_b2;
    len[3] = sae->crypto.prime_len;
    addr[4] = element2;
    len[4] = element2_len;
    res = hkdf_extract(sae->crypto.kck_len, sae->crypto.kck, sae->crypto.kck_len,
                5, addr, len, confirm);
fail:
    if (scalar_b1) {os_free(scalar_b1);}
    if (scalar_b2) {os_free(scalar_b2);}
    return res;
}

static int sae_cn_confirm_ecc(ln_sae_data_t *sae, const uint8_t *sc,
                    const struct crypto_bignum *scalar1,
                    const struct crypto_ec_point *element1,
                    const struct crypto_bignum *scalar2,
                    const struct crypto_ec_point *element2,
                    uint8_t *confirm)
{
    int res = 0;
    uint8_t *element_b1 = NULL;
    uint8_t *element_b2 = NULL;
    element_b1 = os_malloc(2 * SAE_MAX_ECC_PRIME_LEN);
    element_b2 = os_malloc(2 * SAE_MAX_ECC_PRIME_LEN);
    if (!element_b1 || !element_b2) {
        return -1;
    }

    if (crypto_ec_point_to_bin(sae->crypto.ec, element1, element_b1,
                    element_b1 + sae->crypto.prime_len) < 0 ||
        crypto_ec_point_to_bin(sae->crypto.ec, element2, element_b2,
                    element_b2 + sae->crypto.prime_len) < 0 ||
        sae_cn_confirm(sae, sc, scalar1, element_b1,
                2 * sae->crypto.prime_len,
                scalar2, element_b2, 2 * sae->crypto.prime_len,
                confirm) < 0) {
        res = -1;
        goto fail;
    }

#ifdef LN_WPA_SAE_TRACE_ENABLED
    WLIB_LOG_D("prime len:%d\r\n", sae->crypto.prime_len);
    wlib_hexdump("confirm", confirm, 32);
    wlib_hexdump("sc", (void *)sc, 34); // sc: send confirm
#endif

fail:
    if (element_b1) {os_free(element_b1);}
    if (element_b2) {os_free(element_b2);}
    return res;
}

static uint16_t sme_auth_build_sae_confirm(ln_sae_data_t *sae)
{
    size_t hash_len;
    int res;

    hash_len = sae->crypto.kck_len;
    WLIB_LOG_D("hash_len:%d\r\n", hash_len);

    /* Send-Confirm */
    if (sae->send_confirm < 0xffff)
        sae->send_confirm++;

    sae->cached_data.own_confirm[0] = (uint8_t)sae->send_confirm;
    sae->cached_data.own_confirm[1] = (uint8_t)(sae->send_confirm >> 8);

    if (sae->crypto.ec) {
        res = sae_cn_confirm_ecc(sae, sae->cached_data.own_confirm, sae->crypto.own_commit_scalar,
                    sae->crypto.own_commit_element_ecc,
                    sae->crypto.peer_commit_scalar,
                    sae->crypto.peer_commit_element_ecc,
                    &sae->cached_data.own_confirm[2]);
    }

    if (res) {
        return res;
    }

    return 0;
}

static int os_memcmp_const(const void *a, const void *b, size_t len)
{
    const uint8_t *aa = a;
    const uint8_t *bb = b;
    size_t i;
    uint8_t res;

    for (res = 0, i = 0; i < len; i++)
        res |= aa[i] ^ bb[i];

    return res;
}

static int sae_check_confirm(ln_sae_data_t *sae,
        ln_wpa_sae_confirm_info_t *confirm_info)
{
    size_t hash_len;

    hash_len = sae->crypto.kck_len;

    if (!sae->crypto.peer_commit_scalar || !sae->crypto.own_commit_scalar) {
        WLIB_LOG_D("SAE: Temporary data not yet available");
        return -1;
    }

    if (sae->crypto.ec) {
        if (!sae->crypto.peer_commit_element_ecc ||
            !sae->crypto.own_commit_element_ecc ||
            sae_cn_confirm_ecc(sae, confirm_info->confirm, sae->crypto.peer_commit_scalar,
                    sae->crypto.peer_commit_element_ecc,
                    sae->crypto.own_commit_scalar,
                    sae->crypto.own_commit_element_ecc,
                    sae->cached_data.own_verifier) < 0) {
            return -1;
        }
    } else {
        return -1;
    }

    if (os_memcmp_const(sae->cached_data.own_verifier, &confirm_info->confirm[2], hash_len) != 0) {
        WLIB_LOG_E("SAE: Confirm mismatch\r\n");
#ifdef LN_WPA_SAE_TRACE_ENABLED
        wlib_hexdump("SAE: Received confirm",
                (void *)(&confirm_info->confirm[2]), hash_len);
        wlib_hexdump("SAE: Calculated verifier", sae->cached_data.own_verifier, hash_len);
#endif
        return -1;
    }

    return 0;
}

int ln_wpa_sae_auth_frame_process(ln_wpa_sae_commit_info_t *info,
        ln_wpa_sae_confirm_info_t *confirm_info)
{
    uint16_t res = WLAN_STATUS_SUCCESS;
    ln_sae_data_t *sae = &sg_sae_data;

    if (info && confirm_info && info->auth_seq_num == 1) {
        if (!info->scalar || !info->commit_element) {
            return -1;
        }

#ifdef LN_WPA_SAE_TRACE_ENABLED
        WLIB_LOG_D("sq_num:%d\r\n", info->auth_seq_num);
        wlib_hexdump("Peer scalar", (void *)info->scalar, 32);
        wlib_hexdump("Peer commit", (void *)info->commit_element, 64);
#endif

        if (sae->state != SAE_COMMITTED) {
            WLIB_LOG_D("Error sae state: %d\r\n", sae->state);
            return -1;
        }

        res = sae_parse_commit(sae, info);
        if (res == SAE_SILENTLY_DISCARD) {
            WLIB_LOG_E("SAE: Drop commit message due to reflection attack\r\n");
            return 0;
        }
        if (res != WLAN_STATUS_SUCCESS) {
            return -1;
        }
        if (sae_process_commit(sae) < 0) {
            WLIB_LOG_E("SAE: Failed to process peer commit\r\n");
            return -1;
        }

        sme_auth_build_sae_confirm(sae);

        confirm_info->send_confirm = sae->send_confirm;
        confirm_info->confirm = &sae->cached_data.own_confirm[2];
        sae->state = SAE_CONFIRMED;
    } else if (confirm_info && confirm_info->auth_seq_num == 2
            && confirm_info->confirm) {
#ifdef LN_WPA_SAE_TRACE_ENABLED
        wlib_hexdump("recv confirm", (void *)confirm_info->confirm, 34);
#endif

        if (sae->state != SAE_CONFIRMED) {
            WLIB_LOG_D("Error sae state: %d\r\n", sae->state);
            return -1;
        }

        if (sae_check_confirm(sae, confirm_info) < 0) {
            WLIB_LOG_E("peer confirm check failed!\r\n");
            return -1;
        }
        sae->state = SAE_ACCEPTED;
        WLIB_LOG_I("SAE Auth Passed!\r\n");
        sae_clear_data(sae);
    }

    return WLAN_STATUS_SUCCESS;
}

int ln_wpa_sae_pmk_get(uint8_t **in_pmk_ptr)
{
    ln_sae_data_t *sae = &sg_sae_data;
    if (in_pmk_ptr == NULL) {
        return -1;
    }
    *in_pmk_ptr = (uint8_t *)sae->pmk;

#ifdef LN_WPA_SAE_TRACE_ENABLED
    wlib_hexdump("sae->pmk", sae->pmk, 32);
#endif
    return 0;
}

/**
 * =============================================================================
 * SAE TEST
 * =============================================================================
*/
#ifdef LN_WPA_SAE_TEST
#define CONFIG_SAE

static int test_sae_write_commit(ln_sae_data_t *sae)
{
    if (crypto_bignum_to_bin(sae->crypto.own_commit_scalar,
            &sae->cached_data.scalar[0],
            sae->crypto.prime_len, sae->crypto.prime_len) < 0) {
        return -1;
    }
    wlib_hexdump("SAE: own commit-scalar",
            &sae->cached_data.scalar[0], sae->crypto.prime_len);

    if (crypto_ec_point_to_bin(sae->crypto.ec,
                sae->crypto.own_commit_element_ecc,
                &sae->cached_data.commit_element[0],
                &sae->cached_data.commit_element[sae->crypto.prime_len]) < 0) {
        return -1;
    }
    wlib_hexdump("SAE: own commit-element(x)",
            &sae->cached_data.commit_element[0], sae->crypto.prime_len);
    wlib_hexdump("SAE: own commit-element(y)",
            &sae->cached_data.commit_element[sae->crypto.prime_len], sae->crypto.prime_len);
    return 0;
}

int sae_tests(void)
{
#ifdef CONFIG_SAE
    ln_sae_data_t *sae = &sg_sae_data;
    int ret = -1;
    /* IEEE Std 802.11-2020, Annex J.10 */
    const uint8_t addr1[ETH_ALEN] = { 0x4d, 0x3f, 0x2f, 0xff, 0xe3, 0x87 };
    const uint8_t addr2[ETH_ALEN] = { 0xa5, 0xd8, 0xaa, 0x95, 0x8e, 0x3c };
    const char *pw = "mekmitasdigoat";

    const uint8_t local_rand[] = {
        0x99, 0x24, 0x65, 0xfd, 0x3d, 0xaa, 0x3c, 0x60,
        0xaa, 0x65, 0x65, 0xb7, 0xf6, 0x2a, 0x2a, 0x7f,
        0x2e, 0x12, 0xdd, 0x12, 0xf1, 0x98, 0xfa, 0xf4,
        0xfb, 0xed, 0x89, 0xd7, 0xff, 0x1a, 0xce, 0x94
    };
    const uint8_t local_mask[] = {
        0x95, 0x07, 0xa9, 0x0f, 0x77, 0x7a, 0x04, 0x4d,
        0x6a, 0x08, 0x30, 0xb9, 0x1e, 0xa3, 0xd5, 0xdd,
        0x70, 0xbe, 0xce, 0x44, 0xe1, 0xac, 0xff, 0xb8,
        0x69, 0x83, 0xb5, 0xe1, 0xbf, 0x9f, 0xb3, 0x22
    };
    const uint8_t local_commit[] = {
        0x13, 0x00, 0x2e, 0x2c, 0x0f, 0x0d, 0xb5, 0x24,
        0x40, 0xad, 0x14, 0x6d, 0x96, 0x71, 0x14, 0xce,
        0x00, 0x5c, 0xe1, 0xea, 0xb0, 0xaa, 0x2c, 0x2e,
        0x5c, 0x28, 0x71, 0xb7, 0x74, 0xf6, 0xc2, 0x57,
        0x5c, 0x65, 0xd5, 0xad, 0x9e, 0x00, 0x82, 0x97,
        0x07, 0xaa, 0x36, 0xba, 0x8b, 0x85, 0x97, 0x38,
        0xfc, 0x96, 0x1d, 0x08, 0x24, 0x35, 0x05, 0xf4,
        0x7c, 0x03, 0x53, 0x76, 0xd7, 0xac, 0x4b, 0xc8,
        0xd7, 0xb9, 0x50, 0x83, 0xbf, 0x43, 0x82, 0x7d,
        0x0f, 0xc3, 0x1e, 0xd7, 0x78, 0xdd, 0x36, 0x71,
        0xfd, 0x21, 0xa4, 0x6d, 0x10, 0x91, 0xd6, 0x4b,
        0x6f, 0x9a, 0x1e, 0x12, 0x72, 0x62, 0x13, 0x25,
        0xdb, 0xe1
    };
    const uint8_t peer_commit[] = {
        0x13, 0x00, 0x59, 0x1b, 0x96, 0xf3, 0x39, 0x7f,
        0xb9, 0x45, 0x10, 0x08, 0x48, 0xe7, 0xb5, 0x50,
        0x54, 0x3b, 0x67, 0x20, 0xd8, 0x83, 0x37, 0xee,
        0x93, 0xfc, 0x49, 0xfd, 0x6d, 0xf7, 0xe0, 0x8b,
        0x52, 0x23, 0xe7, 0x1b, 0x9b, 0xb0, 0x48, 0xd3,
        0x87, 0x3f, 0x20, 0x55, 0x69, 0x53, 0xa9, 0x6c,
        0x91, 0x53, 0x6f, 0xd8, 0xee, 0x6c, 0xa9, 0xb4,
        0xa6, 0x8a, 0x14, 0x8b, 0x05, 0x6a, 0x90, 0x9b,
        0xe0, 0x3e, 0x83, 0xae, 0x20, 0x8f, 0x60, 0xf8,
        0xef, 0x55, 0x37, 0x85, 0x80, 0x74, 0xdb, 0x06,
        0x68, 0x70, 0x32, 0x39, 0x98, 0x62, 0x99, 0x9b,
        0x51, 0x1e, 0x0a, 0x15, 0x52, 0xa5, 0xfe, 0xa3,
        0x17, 0xc2
    };
    const uint8_t kck[] = {
        0x1e, 0x73, 0x3f, 0x6d, 0x9b, 0xd5, 0x32, 0x56,
        0x28, 0x73, 0x04, 0x33, 0x88, 0x31, 0xb0, 0x9a,
        0x39, 0x40, 0x6d, 0x12, 0x10, 0x17, 0x07, 0x3a,
        0x5c, 0x30, 0xdb, 0x36, 0xf3, 0x6c, 0xb8, 0x1a
    };
    const uint8_t pmk[] = {
        0x4e, 0x4d, 0xfa, 0xb1, 0xa2, 0xdd, 0x8a, 0xc1,
        0xa9, 0x17, 0x90, 0xf9, 0x53, 0xfa, 0xaa, 0x45,
        0x2a, 0xe5, 0xc6, 0x87, 0x3a, 0xb7, 0x5b, 0x63,
        0x60, 0x5b, 0xa6, 0x63, 0xf8, 0xa7, 0xfe, 0x59
    };
    const uint8_t pmkid[] = {
        0x87, 0x47, 0xa6, 0x00, 0xee, 0xa3, 0xf9, 0xf2,
        0x24, 0x75, 0xdf, 0x58, 0xca, 0x1e, 0x54, 0x98
    };

    struct crypto_bignum *mask = NULL;
    ln_wpa_sae_commit_info_t commit_info = {0};

    os_memset(sae, 0, sizeof(*sae));

    memcpy(sae->passphrase, pw, os_strlen(pw));
    memcpy(sae->own_mac, addr1, 6);
    memcpy(sae->prev_bssid, addr2, 6);
    sae->password_len = os_strlen(pw);

    if (sme_auth_build_sae_commit(sae) != 0) {
        WLIB_LOG_E("Test error: [%d]\r\n", __LINE__);
        goto fail;
    }

    /* Override local values based on SAE test vector */
    crypto_bignum_deinit(sae->crypto.sae_rand, 1);
    sae->crypto.sae_rand = crypto_bignum_init_set(local_rand,
                           sizeof(local_rand));
    mask = crypto_bignum_init_set(local_mask, sizeof(local_mask));
    if (!sae->crypto.sae_rand || !mask) {
        WLIB_LOG_E("Test error: [%d]\r\n", __LINE__);
        goto fail;
    }

    if (crypto_bignum_add(sae->crypto.sae_rand, mask,
                  sae->crypto.own_commit_scalar) < 0 ||
        crypto_bignum_mod(sae->crypto.own_commit_scalar, sae->crypto.order,
                  sae->crypto.own_commit_scalar) < 0 ||
        crypto_ec_point_mul(sae->crypto.ec, sae->crypto.pwe_ecc, mask,
                sae->crypto.own_commit_element_ecc) < 0 ||
        crypto_ec_point_invert(sae->crypto.ec,
                   sae->crypto.own_commit_element_ecc) < 0) {
        WLIB_LOG_E("Test error: [%d]\r\n", __LINE__);
        goto fail;
    }

    /* Check that output matches the test vector */
    if (test_sae_write_commit(sae) != 0) {
        WLIB_LOG_E("Test error: [%d]\r\n", __LINE__);
        goto fail;
    }

    if (98 != sizeof(local_commit) ||
        os_memcmp(sae->cached_data.scalar, &local_commit[2], 32) != 0 ||
        os_memcmp(sae->cached_data.commit_element, &local_commit[2+32], 64) != 0) {
        WLIB_LOG_E("SAE: Mismatch in local commit\r\n");
        WLIB_LOG_E("Test error: [%d]\r\n", __LINE__);
        goto fail;
    }

    commit_info.auth_seq_num = 1;
    commit_info.status_code = 0;
    commit_info.scalar = (const uint8_t *)(&peer_commit[2]);
    commit_info.commit_element = (const uint8_t *)(&peer_commit[2+32]);
    if (sae_parse_commit(sae, &commit_info) != 0) {
        WLIB_LOG_E("Test error: [%d]\r\n", __LINE__);
        goto fail;
    }

    if (sae_process_commit(sae) < 0) {
        WLIB_LOG_D("SAE: Failed to process peer commit\r\n");
        return -1;
    }

    wlib_hexdump("own kck", sae->crypto.kck, SAE_KCK_LEN);
    wlib_hexdump("need kck", (void *)kck, SAE_KCK_LEN);
    if (os_memcmp(kck, sae->crypto.kck, SAE_KCK_LEN) != 0) {
        WLIB_LOG_E("SAE: Mismatch in KCK\r\n");
        goto fail;
    }

    if (os_memcmp(pmk, sae->pmk, SAE_PMK_LEN) != 0) {
        WLIB_LOG_E("SAE: Mismatch in PMK\r\n");
        goto fail;
    }

    if (os_memcmp(pmkid, sae->pmkid, SAE_PMKID_LEN) != 0) {
        WLIB_LOG_E("SAE: Mismatch in PMKID\r\n");
        goto fail;
    }

    ret = 0;
fail:
    sae_clear_data(sae);
    crypto_bignum_deinit(mask, 1);
    WLIB_LOG_I("SAE TEST Finished!!! <%d>\r\n", ret);
    return ret;
#else /* CONFIG_SAE */
    return 0;
#endif /* CONFIG_SAE */
}
#endif /* LN_WPA_SAE_TEST */
#else /* Not defined LN_WPA_SAE_SUPPORTED */
int ln_wpa_sae_start(ln_wpa_sae_in_param_t *sae_in_param,
        ln_wpa_sae_commit_info_t *commit_info)
{
    (void)sae_in_param;
    (void)commit_info;
    return 0;
}

int ln_wpa_sae_auth_frame_process(ln_wpa_sae_commit_info_t *info,
        ln_wpa_sae_confirm_info_t *confirm_info)
{
    (void)info;
    (void)confirm_info;
    return 0;
}

int ln_wpa_sae_pmk_get(uint8_t **in_pmk_ptr)
{
    (void)in_pmk_ptr;
    return 0;
}

int ln_wpa_sae_kdf_hash(const uint8_t *key, size_t key_len, const char *label,
        const uint8_t *data, size_t data_len, uint8_t *buf, size_t buf_len)
{
    (void)key;
    (void)key_len;
    (void)label;
    (void)data;
    (void)data_len;
    (void)buf;
    (void)buf_len;
    return 0;
}

int ln_wpa_sae_omac1_aes_128(const uint8_t *key, const uint8_t *data, int data_len, uint8_t * mac)
{
    (void)key;
    (void)data;
    (void)data_len;
    (void)mac;
    return 0;
}

bool ln_wpa_sae_is_enabled(void)
{
    return false;
}

void ln_wpa_sae_enable(void)
{
}

void ln_wpa_sae_disable(void)
{
}
#endif /* LN_WPA_SAE_SUPPORTED */
