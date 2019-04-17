// SvdW single-point hash for BLS12-381 G2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve2.h"
#include "util.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    int retval = 0;
    const struct cmdline_opts opts = get_cmdline_opts(argc, argv);
    const bool do_print = opts.test || !opts.quiet;

    // initialize temp vars for curve2 computations
    curve2_init();

    // get libgmp ready
    mpz_t2 x, y, z, t;
    mpz2_inits(x, y, z, t, NULL);

    // load libcrypto error strings and set up SHA and PRNG
    ERR_load_crypto_strings();
    SHA256_CTX hash_ctx;
    CHECK_CRYPTO(SHA256_Init(&hash_ctx));
    EVP_CIPHER_CTX *prng_ctx = EVP_CIPHER_CTX_new();
    CHECK_CRYPTO(prng_ctx != NULL);

    // hash the contents of stdin
    hash_stdin(&hash_ctx);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    // loop through diffrent resulting PRNG keys
    for (unsigned i = 0; i < opts.nreps; ++i) {
        next_prng(prng_ctx, &hash_ctx, i);
        if (opts.test && i == 0) {
            // in test mode, first test an exceptional input
            mpz_set_ui(t->s, 0);
            mpz_set_ui(t->t, 0);
        } else {
            next_modp(prng_ctx, t->s, opts.constant_time);
            next_modp(prng_ctx, t->t, opts.constant_time);
        }

        if (opts.constant_time) {
            svdw2_map_ct(x, y, z, t);
        } else if (opts.field_only) {
            svdw2_map_fo(x, y, z, t);
        } else {
            svdw2_map(x, y, t);
            mpz_set_ui(z->s, 1);
            mpz_set_ui(z->t, 0);
        }
        clear_h2(x, y, z);

        // show results
        //   test            (t, x, y, z)
        //   quiet && !test: <<nothing>>
        //   otherwise       (x, y, z)
        const bool force = opts.test2 && !check_curve2(x, y, z);
        if (do_print || force) {
            gmp_printf("(");
            if (force) {
                ++retval;
                printf("%u, ", i);
            }
            if (opts.test || force) {
                gmp_printf("0x%Zx, 0x%Zx, ", t->s, t->t);
            }
            gmp_printf("0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, )\n", x->s, x->t, y->s, y->t, z->s, z->t);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    fprintf(opts.quiet ? stdout : stderr, "%ld\n", elapsed);

    // clean up
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz2_clears(x, y, z, t, NULL);
    curve2_uninit();

    return retval;
}
