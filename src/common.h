#ifndef __bls_hash__src__common_h__

#include <endian.h>
#include <errno.h>
#include <gmp.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RDBUF_SIZE 4096
#define P_LEN 48
#define Q_LEN 32

#define CHECK_CRYPTO(C)                                                                        \
    do {                                                                                       \
        if ((C) != 1) {                                                                        \
            fprintf(stderr, "libcrypto error: %s\n", ERR_error_string(ERR_get_error(), NULL)); \
            exit(1);                                                                           \
        }                                                                                      \
    } while (0)

void hash_stdin(SHA256_CTX *ctx);
void next_prng(EVP_CIPHER_CTX *cctx, const SHA256_CTX *hctx, uint32_t idx);
void show_buf(FILE *of, const unsigned char *buf, size_t len);
bool lt_be(const unsigned char *a, const unsigned char *b, size_t len);
void next_modp(EVP_CIPHER_CTX *cctx, mpz_t ret);
void next_modq(EVP_CIPHER_CTX *cctx, mpz_t ret);
mpz_t *get_p(void);
mpz_t *get_q(void);
void common_init(void);
void common_uninit(void);

#define __bls_hash__src__common_h__
#endif
