// G1 hash for BLS12-381: Shallue and van de Woestijne map plus random subgroup point
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve.h"
#include "util.h"

int main(int argc, char **argv) {
    struct cmdline_opts opts = get_cmdline_opts(argc, argv);

    // initialize static data for curve computations
    curve_init();
    precomp_init();

    // get libgmp ready
    mpz_t x, y, t;
    mpz_init(x);
    mpz_init(y);
    mpz_init(t);

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
        next_prng(prng_ctx, &hash_ctx, i);
        next_modp(prng_ctx, t);
        svdw_map(x, y, t);

        // show results
        //   test:                          (xin, yin, xout, yout)
        //   quiet && !test:                <<nothing>>
        //   !quiet && !test && clear_h:    (xout, yout)
        //   !quiet && !test && !clear_h:   (xin, yin)
        //
        if (opts.test) {
            gmp_printf("0x%Zx, 0x%Zx, ", x, y);
        } else if (!opts.quiet) {
            printf("(");
        }

        if (opts.clear_h || opts.test) {
            clear_cofactor(x, y, x, y);
        }

        if (opts.test || !opts.quiet) {
            gmp_printf("0x%Zx, 0x%Zx)\n", x, y);
        }
    }

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(t);
    mpz_clear(y);
    mpz_clear(x);
    curve_uninit();

    return 0;
}
