// simple hash-and-try impl for BLS12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "common.h"

int main(int argc, char **argv) {
    unsigned nreps = 0;
    if (argc > 1) {
        nreps = atoi(argv[1]);
    }
    if (nreps == 0) {
        nreps = 1024;
    }

    // load libcrypto error strings and set up SHA invocation
    ERR_load_crypto_strings();
    SHA256_CTX hash_ctx = {0,};
    CHECK_CRYPTO(SHA256_Init(&hash_ctx));

    EVP_CIPHER_CTX *prng_ctx = EVP_CIPHER_CTX_new();
    CHECK_CRYPTO(prng_ctx != NULL);

    // hash the contents of stdin
    hash_stdin(&hash_ctx);

    // loop through different resulting PRNG keys
    for (unsigned i = 0; i < nreps; ++i) {
        next_prng(prng_ctx, &hash_ctx, i);
        unsigned char *t = next_p(prng_ctx);
        show_buf(stderr, t, P_LEN);
    }

    EVP_CIPHER_CTX_free(prng_ctx);

    return 0;
}
