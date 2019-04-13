// G1 hash for BLS12-381: sum of two evaluations of the Shallue and van de Woestijne map
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve.h"
#include "util.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    struct cmdline_opts opts = get_cmdline_opts(argc, argv);
    const bool first_print = opts.test || (!opts.quiet && !opts.clear_h);
    const bool do_clear = opts.test || opts.clear_h;
    const bool second_print = opts.test || (!opts.quiet && opts.clear_h);

    // initialize static data for curve computations
    curve_init();

    // get libgmp ready
    mpz_t x1, y1, z1, t1, x2, y2, z2, t2;
    mpz_init(x1);
    mpz_init(y1);
    mpz_init(z1);
    mpz_init(t1);
    mpz_init(x2);
    mpz_init(y2);
    mpz_init(z2);
    mpz_init(t2);

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
    // loop through different resulting PRNG keys
    for (unsigned i = 0; i < opts.nreps; ++i) {
        next_prng(prng_ctx, &hash_ctx, i);
        next_modp(prng_ctx, t1, opts.constant_time);
        next_modp(prng_ctx, t2, opts.constant_time);
        if (opts.constant_time) {
            svdw_map_ct(x1, y1, z1, t1);
            svdw_map_ct(x2, y2, z2, t2);
        } else if (opts.field_only) {
            svdw_map_fo(x1, y1, z1, t1);
            svdw_map_fo(x2, y2, z2, t2);
        } else {
            svdw_map2(x1, y1, t1, x2, y2, t2);
            mpz_set_ui(z1, 1);
            mpz_set_ui(z2, 1);
        }

        // show results                     svdw_add2
        //   test:                          (t1, t2, xO, yO, zO)
        //   quiet && !test:                <<nothing>>
        //   !quiet && !test && clear_h:    (xO, yO)
        //   !quiet && !test && !clear_h:   (x1, y1, z1, x2, y2, z2)
        if (first_print || second_print) {
            printf("(");
        }

        // maybe output the points
        if (opts.test) {
            gmp_printf("0x%Zx, 0x%Zx, ", t1, t2);
        } else if (first_print) {
            gmp_printf("0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, ", x1, y1, z1, x2, y2, z2);
        }

        // add the points together, and possibly clear the cofactor
        if (do_clear) {
            add2_clear_h(x1, y1, z1, x2, y2, z2);
        } else {
            add2(x1, y1, z1, x2, y2, z2);
        }

        // maybe output the result
        if (second_print) {
            gmp_printf("0x%Zx, 0x%Zx, 0x%Zx, ", x1, y1, z1);
        }

        if (first_print || second_print) {
            printf(")\n");
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    fprintf(opts.quiet ? stdout : stderr, "%ld\n", elapsed);

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(t2);
    mpz_clear(z2);
    mpz_clear(y2);
    mpz_clear(x2);
    mpz_clear(t1);
    mpz_clear(z1);
    mpz_clear(y1);
    mpz_clear(x1);
    curve_uninit();

    return 0;
}
