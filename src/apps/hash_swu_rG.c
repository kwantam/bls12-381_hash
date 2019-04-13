// test: hash to curve 11-isogenous to BLS12-381 using SWU map
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve.h"
#include "util.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    struct cmdline_opts opts = get_cmdline_opts(argc, argv);

    // initialize static data for curve computations
    curve_init();
    precomp_init();

    // get libgmp ready
    mpz_t x1, y1, z1, u, rr;
    mpz_init(x1);
    mpz_init(y1);
    mpz_init(z1);
    mpz_init(u);
    mpz_init(rr);

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
        next_modp(prng_ctx, u, false);
        const uint8_t *r = next_modq(prng_ctx, opts.test ? &rr : NULL);
        swu_map_rG(x1, y1, z1, u, r);

        // show results
        //   test:              (xO, yO, zO, u, r)
        //   quiet && !test:    <<nothing>>
        //   !quiet && !test:   (xO, yO, zO)

        // maybe output the points
        if (opts.test) {
            gmp_printf("(0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, 0x%Zx, )\n", x1, y1, z1, u, rr);
        } else if (!opts.quiet) {
            gmp_printf("(0x%Zx, 0x%Zx, 0x%Zx, )\n", x1, y1, z1);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    fprintf(opts.quiet ? stdout : stderr, "%ld\n", elapsed);

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(rr);
    mpz_clear(u);
    mpz_clear(z1);
    mpz_clear(y1);
    mpz_clear(x1);
    curve_uninit();

    return 0;
}
