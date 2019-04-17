// utilities for bls12-381 hashing
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "util.h"

#include "consts.h"

#include <endian.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// set supplied mpz_t to p-1, i.e., -1 mod p
void mpz_set_pm1(mpz_t out) {
    mpz_import(out, P_LEN, 1, 1, 1, 0, BLS12_381_p);
    mpz_sub_ui(out, out, 1);
}

// hash stdin into an OpenSSL SHA256_CTX
#define RDBUF_SIZE 4096
void hash_stdin(SHA256_CTX *ctx) {
    char buf[RDBUF_SIZE];

    while (true) {
        ssize_t nread = read(0, buf, RDBUF_SIZE);
        if (nread < 0) {
            perror("Reading stdin");
            exit(1);
        }
        if (nread == 0) {
            break;
        }

        CHECK_CRYPTO(SHA256_Update(ctx, buf, nread));
    }
}
#undef RDBUF_SIZE

// given a SHA256_CTX that contains a hash of stdin:
//   1. append little endian representation of 32-bit idx and finalize the hash
//   2. set the key and IV of the supplied AES-CTR cipher context from the hash output
void next_prng(EVP_CIPHER_CTX *cctx, const SHA256_CTX *hctx, uint32_t idx) {
    // make a copy of the context
    SHA256_CTX lctx;
    memcpy(&lctx, hctx, sizeof(lctx));

    // append the index and compute the hash
    uint32_t idx_bytes = htole32(idx);  // make sure counter is little endian
    CHECK_CRYPTO(SHA256_Update(&lctx, &idx_bytes, sizeof(idx_bytes)));

    uint8_t key_iv[SHA256_DIGEST_LENGTH];
    CHECK_CRYPTO(SHA256_Final(key_iv, &lctx));  // hash to key and IV

    // initialize cipher context with new key and IV
    CHECK_CRYPTO(EVP_CIPHER_CTX_reset(cctx));
    CHECK_CRYPTO(EVP_EncryptInit(cctx, EVP_aes_128_ctr(), key_iv, key_iv + 16));
}

// compare buffer representing bigint in big endian format
static inline bool lt_be(const uint8_t *a, const uint8_t *b, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
    }
    return false;
}

// output the required number of bytes into the output buffer, mask, and compare to the max value
static uint8_t ZEROS[P_LEN] = {0};
static inline int next_com(EVP_CIPHER_CTX *cctx, uint8_t *out, int len, const uint8_t *max, uint8_t mask) {
    int outl = len;
    CHECK_CRYPTO(EVP_EncryptUpdate(cctx, out, &outl, ZEROS, len));
    CHECK_CRYPTO(outl == len);
    int ret = (out[0] & 0x80) ? 1 : 0;  // grab most significant bit to return
    out[0] &= mask;
    if (lt_be(out, max, len)) {
        return ret;
    }
    return -1;
}

// return the next value mod p from the PRNG represented by the supplied cipher context
static inline bool next_modp_nct(EVP_CIPHER_CTX *cctx, mpz_t ret) {
    uint8_t p_out[P_LEN];
    int b;
    while ((b = next_com(cctx, p_out, P_LEN, BLS12_381_p, 0x1f)) < 0) {
    }
    mpz_import(ret, P_LEN, 1, 1, 1, 0, p_out);
    return b != 0;
}

// constant-time rejection sampling for modp
// p is ~0.8 * 2^381, so with 20% probability we do not find a residue.
// 0.2^56 is ~2^-130, i.e., sufficiently low failure probability
#define NUM_SAMP_P 56
static inline bool next_modp_ct(EVP_CIPHER_CTX *cctx, mpz_t ret) {
    uint64_t p_out[P_LEN / 8] = {0};
    int b = -1;
    for (unsigned i = 0; i < NUM_SAMP_P; ++i) {
        uint64_t p_tmp[P_LEN / 8];
        const int b_tmp = next_com(cctx, (uint8_t *)p_tmp, P_LEN, BLS12_381_p, 0x1f);
        const bool b_good = b_tmp >= 0;

        // constant-time assign b and p_out if b_good
        b = b_good ? b_tmp : b;
        const uint64_t mask1 = 0LL - ((uint64_t)b_good);
        const uint64_t mask2 = ~mask1;
        for (unsigned j = 0; j < P_LEN / 8; ++j) {
            p_out[j] = (p_tmp[j] & mask1) | (p_out[j] & mask2);
        }
    }
    if (b == -1) {
        fprintf(stderr, "no residue mod p found!\n");
        exit(1);
    }
    mpz_import(ret, P_LEN, 1, 1, 1, 0, p_out);
    return b != 0;
}
#undef NUM_SAMP_P

// just dispatch to constant-time or non--constant-time impl
bool next_modp(EVP_CIPHER_CTX *cctx, mpz_t ret, const bool constant_time) {
    return constant_time ? next_modp_ct(cctx, ret) : next_modp_nct(cctx, ret);
}

// return the next value mod q from the PRNG represented by the supplied cipher context
// return value is pointer to static buffer containing bytes of value mod q
// (this is because the multiexp routine in curve/curve.c expects this format)
// Also, if out is not NULL, convert byte buffer to mpz_t (used for testing)
uint8_t *next_modq(EVP_CIPHER_CTX *cctx, mpz_t *out) {
    static uint8_t ret[Q_LEN];
    while (next_com(cctx, ret, Q_LEN, BLS12_381_q, 0x7f) < 0) {
    }
    if (out != NULL) {
        mpz_import(*out, Q_LEN, 1, 1, 1, 0, ret);
    }
    return ret;
}

// process commandline options into a struct cmdline_opts
struct cmdline_opts get_cmdline_opts(int argc, char **argv) {
    struct cmdline_opts ret = {0, false, false, false, false, false};
    int opt_ret;
    bool found_err = false;
    while ((opt_ret = getopt(argc, argv, "n:qtTfc")) >= 0) {
        switch (opt_ret) {
            case 'n':
                ret.nreps = atoi(optarg);  // NOLINT(cert-err34-c)
                break;

            case 'q':
                ret.quiet = true;
                break;

            case 't':
                ret.test = true;
                break;

            case 'T':
                ret.test2 = true;
                break;

            case 'f':
                ret.field_only = true;
                break;

            case 'c':
                ret.constant_time = true;
                break;

            default:
                found_err = true;
        }
        if (found_err) {
            break;
        }
    }
    if (found_err || optind != argc) {
        printf("Usage: %s [-n <npoints>] [-q] [-C] [-t] [-f] [-c]\n", argv[0]);
        exit(1);
    }
    if (ret.nreps == 0) {
        ret.nreps = 16;
    }

    return ret;
}
