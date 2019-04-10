// test: hash to curve 11-isogenous to BLS12-381 using SWU map
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve2.h"
#include "util.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    struct cmdline_opts opts = get_cmdline_opts(argc, argv);
    const bool do_print = opts.test || !opts.quiet;

    // initialize static data for curve computations
    curve2_init();

    // get libgmp ready
    mpz_t2 x, y, z, u;
    mpz2_init(x);
    mpz2_init(y);
    mpz2_init(z);
    mpz2_init(u);

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
        if (opts.test && i == 0) {
            // in test mode, make sure exceptional input 0 gives correct result
            mpz_set_ui(u->s, 0);
            mpz_set_ui(u->t, 0);
        } else {
            next_modp(prng_ctx, u->s);
            next_modp(prng_ctx, u->t);
        }
        swu2_map(x, y, z, u);

        // show results
        //   test:              (x, y, z, u)
        //   quiet && !test:    <<nothing>>
        //   !quiet && !test:   (x, y, z)
        if (do_print) {
            gmp_printf("(0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, ", x->s, x->t, y->s, y->t, z->s, z->t);
        }

        if (opts.test) {
            gmp_printf("0x%Zx, 0x%Zx, )\n", u->s, u->t);
        } else if (do_print) {
            gmp_printf(")\n");
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    fprintf(opts.quiet ? stdout : stderr, "%ld\n", elapsed);

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz2_clear(u);
    mpz2_clear(z);
    mpz2_clear(y);
    mpz2_clear(x);
    curve2_uninit();

    return 0;
}