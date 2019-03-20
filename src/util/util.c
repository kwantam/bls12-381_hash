// utilities for bls12-381 hashing
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "util.h"

#include "bint.h"
#include "bls12_381_consts.h"

#include <endian.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

// clang-format off
static uint8_t ZEROS[P_LEN] = {0,};
// clang-format on
static int next_com(EVP_CIPHER_CTX *cctx, uint8_t *out, int len, const uint8_t *max, uint8_t mask) {
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

bool next_modp(EVP_CIPHER_CTX *cctx, mpz_t ret) {
    uint8_t p_out[P_LEN];
    int b;
    while ((b = next_com(cctx, p_out, P_LEN, BLS12_381_p, 0x1f)) < 0) {
    }
    mpz_import(ret, P_LEN, 1, 1, 1, 0, p_out);
    return b != 0;
}

uint8_t *next_modq(EVP_CIPHER_CTX *cctx, mpz_t *out) {
    static uint8_t ret[Q_LEN];
    while (next_com(cctx, ret, Q_LEN, BLS12_381_q, 0x73) < 0) {
    }
    if (out != NULL) {
        mpz_import(*out, Q_LEN, 1, 1, 1, 0, ret);
    }
    return ret;
}

struct cmdline_opts get_cmdline_opts(int argc, char **argv) {
    struct cmdline_opts ret = {0, true, false, false};
    int opt_ret;
    bool found_err = false;
    while ((opt_ret = getopt(argc, argv, "n:Cqt")) >= 0) {
        switch (opt_ret) {
            case 'n':
                ret.nreps = atoi(optarg);  // NOLINT(cert-err34-c)
                break;

            case 'C':
                ret.clear_h = false;
                break;

            case 'q':
                ret.quiet = true;
                break;

            case 't':
                ret.test = true;
                break;

            default:
                found_err = true;
        }
        if (found_err) {
            break;
        }
    }
    if (found_err || optind != argc) {
        printf("Usage: %s [-n <npoints>] [-q] [-C]\n", argv[0]);
        exit(1);
    }
    if (ret.nreps == 0) {
        ret.nreps = 16;
    }

    return ret;
}
