// simple hash-and-try impl for BLS12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "common.h"

int main(int argc, char **argv) {
    unsigned nreps = 0;
    if (argc > 1) {
        nreps = atoi(argv[1]);  // NOLINT(cert-err34-c)
    }
    if (nreps == 0) {
        nreps = 16;
    }

    // get libgmp ready
    mpz_t t, ft, y, pp1o4;
    mpz_init(t);
    mpz_init(ft);
    mpz_init(y);
    mpz_init(pp1o4);
    mpz_t *p = get_p();
    mpz_add_ui(pp1o4, *p, 1);
    mpz_fdiv_q_2exp(pp1o4, pp1o4, 2);

    // load libcrypto error strings and set up SHA and PRNG
    ERR_load_crypto_strings();
    SHA256_CTX hash_ctx;
    CHECK_CRYPTO(SHA256_Init(&hash_ctx));
    EVP_CIPHER_CTX *prng_ctx = EVP_CIPHER_CTX_new();
    CHECK_CRYPTO(prng_ctx != NULL);

    // hash the contents of stdin
    hash_stdin(&hash_ctx);

    // loop through different resulting PRNG keys
    for (unsigned i = 0; i < nreps; ++i) {
        unsigned j = 0;
        for (; j < 256; ++j) {
            next_prng(prng_ctx, &hash_ctx, (i << 8) + j);
            next_modp(prng_ctx, t);

            // t^3 + 4
            mpz_mul(ft, t, t);
            mpz_mod(ft, ft, *p);
            mpz_mul(ft, ft, t);
            mpz_add_ui(ft, ft, 4);
            mpz_mod(ft, ft, *p);

            if (mpz_legendre(ft, *p) == 1) {
                mpz_powm(y, ft, pp1o4, *p);
                break;
            }
        }
        if (j == 256) {
            gmp_fprintf(stderr, "no point found!\n");
        } else {
            gmp_fprintf(stderr, "(%Zx, %Zx)\n", t, y);
        }
    }

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    common_uninit();
    mpz_clear(pp1o4);
    mpz_clear(y);
    mpz_clear(ft);
    mpz_clear(t);

    return 0;
}
