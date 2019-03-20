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
    mpz_t x, fx, y;
    mpz_init(x);
    mpz_init(fx);
    mpz_init(y);
    mpz_t *p = get_p();

    // load libcrypto error strings and set up SHA and PRNG
    ERR_load_crypto_strings();
    SHA256_CTX hash_ctx;
    CHECK_CRYPTO(SHA256_Init(&hash_ctx));
    EVP_CIPHER_CTX *prng_ctx = EVP_CIPHER_CTX_new();
    CHECK_CRYPTO(prng_ctx != NULL);

    // hash the contents of stdin
    hash_stdin(&hash_ctx);

    const bool first_print = opts.test || (!opts.quiet && !opts.clear_h);
    const bool do_clear = opts.test || opts.clear_h;
    const bool second_print = opts.test || (!opts.quiet && opts.clear_h);

    // loop through different resulting PRNG keys
    for (unsigned i = 0; i < opts.nreps; ++i) {
        unsigned j;
        for (j = 0; j < 256; ++j) {
            next_prng(prng_ctx, &hash_ctx, (i << 8) + j);
            next_modp(prng_ctx, x);

            bls_fx(fx, x);
            if (mpz_legendre(fx, *p) == 1) {
                sqrt_modp(y, fx);
                break;
            }
        }
        if (j == 256) {
            fprintf(stderr, "no point found!\n");
            continue;
        }

        // show results
        //   test:                          (xin, yin, xout, yout)
        //   quiet && !test:                <<nothing>>
        //   !quiet && !test && clear_h:    (xout, yout)
        //   !quiet && !test && !clear_h:   (xin, yin)
        //
        if (first_print || second_print) {
            printf("(");
        }

        // maybe output the input point
        if (first_print) {
            gmp_printf("0x%Zx, 0x%Zx, ", x, y);
        }

        // maybe clear the cofactor
        if (do_clear) {
            clear_h(x, y, x, y);
        }

        // maybe output the result
        if (second_print) {
            gmp_printf("0x%Zx, 0x%Zx, ", x, y);
        }

        if (first_print || second_print) {
            printf(")\n");
        }
    }

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(y);
    mpz_clear(fx);
    mpz_clear(x);
    curve_uninit();

    return 0;
}
