// hash-and-check for BLS12-381 G1
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve.h"
#include "util.h"

#include <stdio.h>
#include <time.h>

int main(int argc, char **argv) {
    struct cmdline_opts opts = get_cmdline_opts(argc, argv);

    // initialize temp variables for curve computations
    curve_init();

    // get libgmp ready
    mpz_t x, y, z;
    mpz_init(x);
    mpz_init(y);
    mpz_init(z);

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
        unsigned j;
        for (j = 0; j < 256; ++j) {
            next_prng(prng_ctx, &hash_ctx, (i << 8) + j);
            const bool negate = next_modp(prng_ctx, x, false);
            if (check_fx(y, x, negate, false, opts.field_only)) {
                break;
            }
        }
        if (j == 256) {
            fprintf(stderr, "no point found!\n");
            exit(1);
        }
        mpz_set_ui(z, 1);
        clear_h(x, y, z);

        // show results
        //   quiet:     <<nothing>>
        //   !quiet:    (xout, yout, zout)
        if (!opts.quiet) {
            gmp_printf("(0x%Zx, 0x%Zx, 0x%Zx, )\n", x, y, z);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    fprintf(opts.quiet ? stdout : stderr, "%ld\n", elapsed);

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(z);
    mpz_clear(y);
    mpz_clear(x);
    curve_uninit();

    return 0;
}
