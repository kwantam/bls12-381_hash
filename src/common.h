#ifndef __bls_hash__src__common_h__

#include <endian.h>
#include <errno.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RDBUF_SIZE 4096
#define P_LEN 96
#define Q_LEN 64

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
unsigned char *next_p(EVP_CIPHER_CTX *cctx);
unsigned char *next_q(EVP_CIPHER_CTX *cctx);

#define __bls_hash__src__common_h__
#endif
