// utilities for bls12-381 hashing
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__util__util_h__

#include <gmp.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <stdbool.h>

#define CHECK_CRYPTO(C)                                                                        \
    do {                                                                                       \
        if ((C) != 1) {                                                                        \
            fprintf(stderr, "libcrypto error: %s\n", ERR_error_string(ERR_get_error(), NULL)); \
            exit(1);                                                                           \
        }                                                                                      \
    } while (0)

// SvdW hashing method:
//   basic: just run svdw_map once
//   add2:  run svdw_map twice, add together, clear cofactor
//   addrG: run svdw_map once, add to rG' while clearing cofactor
enum svdw_method {
    svdw_basic,
    svdw_add2,
    svdw_addrG,
};

// getting commandline options
struct cmdline_opts {
    unsigned nreps;
    bool clear_h;
    bool quiet;
    bool test;
    enum svdw_method method;
};
struct cmdline_opts get_cmdline_opts(int argc, char **argv);

// hashing to Fq and Fp
void hash_stdin(SHA256_CTX *ctx);
void next_prng(EVP_CIPHER_CTX *cctx, const SHA256_CTX *hctx, uint32_t idx);
void next_modp(EVP_CIPHER_CTX *cctx, mpz_t ret);
uint8_t *next_modq(EVP_CIPHER_CTX *cctx, mpz_t *out);

#define __bls_hash__src__util__util_h__
#endif  // __bls_hash__src__util__util_h__
