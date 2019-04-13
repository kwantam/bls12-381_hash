// SvdW two-point hash for BLS12-381 G2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve2.h"
#include "util.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    struct cmdline_opts opts = get_cmdline_opts(argc, argv);
    const bool do_print = opts.test || !opts.quiet;

    // initialize temp vars for curve2 computations
    curve2_init();

    // get libgmp ready
    mpz_t2 x1, y1, z1, t1, x2, y2, z2, t2;
    mpz2_init(x1);
    mpz2_init(y1);
    mpz2_init(z1);
    mpz2_init(t1);
    mpz2_init(x2);
    mpz2_init(y2);
    mpz2_init(z2);
    mpz2_init(t2);

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
        next_modp(prng_ctx, t1->s, opts.constant_time);
        next_modp(prng_ctx, t1->t, opts.constant_time);
        next_modp(prng_ctx, t2->s, opts.constant_time);
        next_modp(prng_ctx, t2->t, opts.constant_time);

        if (opts.constant_time) {
            svdw2_map_ct(x1, y1, z1, t1);
            svdw2_map_ct(x2, y2, z2, t2);
        } else if (opts.field_only) {
            svdw2_map_fo(x1, y1, z1, t1);
            svdw2_map_fo(x2, y2, z2, t2);
        } else {
            svdw2_map2(x1, y1, t1, x2, y2, t2);
            mpz_set_ui(z1->s, 1);
            mpz_set_ui(z1->t, 0);
            mpz_set_ui(z2->s, 1);
            mpz_set_ui(z2->t, 0);
        }

        // show results
        //   test            (t1, x1, y1, x2, y2)
        //   quiet && !test: <<nothing>>
        //   otherwise       (x1, y1, x2, y2)
        if (do_print) {
            gmp_printf("(");
        }

        add2_clear_h2(x1, y1, z1, x2, y2, z2);

        if (opts.test) {
            gmp_printf("0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, ", t1->s, t1->t, t2->s, t2->t);
        }
        if (do_print) {
            gmp_printf("0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, )\n", x1->s, x1->t, y1->s, y1->t, z1->s, z1->t);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    fprintf(opts.quiet ? stdout : stderr, "%ld\n", elapsed);

    // clean up
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz2_clear(t2);
    mpz2_clear(z2);
    mpz2_clear(y2);
    mpz2_clear(x2);
    mpz2_clear(t1);
    mpz2_clear(z1);
    mpz2_clear(y1);
    mpz2_clear(x1);
    curve2_uninit();

    return 0;
}
