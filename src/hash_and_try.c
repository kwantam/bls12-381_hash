// simple hash-and-try impl for BLS12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve.h"
#include "util.h"

int main(int argc, char **argv) {
    struct cmdline_opts opts = get_cmdline_opts(argc, argv);

    // initialize temp variables for curve computations
    curve_init();

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
    for (unsigned i = 0; i < opts.nreps; ++i) {
        unsigned j;
        for (j = 0; j < 256; ++j) {
            next_prng(prng_ctx, &hash_ctx, (i << 8) + j);
            next_modp(prng_ctx, t);

            // t^3 + 4
            sqr_modp(ft, t);
            mul_modp(ft, ft, t);
            mpz_add_ui(ft, ft, 4);

            if (mpz_legendre(ft, *p) == 1) {
                mpz_powm(y, ft, pp1o4, *p);
                break;
            }
        }
        if (j == 256) {
            fprintf(stderr, "no point found!\n");
        } else {
            if (opts.clear_h) {
                clear_cofactor(t, y, t, y);
            }
            if (!opts.quiet) {
                gmp_printf("(0x%Zx, 0x%Zx)\n", t, y);
            }
        }
    }

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(pp1o4);
    mpz_clear(y);
    mpz_clear(ft);
    mpz_clear(t);
    curve_uninit();

    return 0;
}
