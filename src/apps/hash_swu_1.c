// test: hash to curve 11-isogenous to BLS12-381 using SWU map
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
    mpz_t x1, y1, u;
    mpz_init(x1);
    mpz_init(y1);
    mpz_init(u);

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
        next_modp(prng_ctx, u);
        swu_map(x1, y1, u);

        // show results
        //   test:              (xO, yO, u1, u2)
        //   quiet && !test:    <<nothing>>
        //   !quiet && !test:   (xO, yO)

        // maybe output the points
        if (opts.test) {
            gmp_printf("(0x%Zx, 0x%Zx, 0x%Zx, )\n", x1, y1, u);
        } else if (!opts.quiet) {
            gmp_printf("(0x%Zx, 0x%Zx, )\n", x1, y1);
        }
    }

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(u);
    mpz_clear(y1);
    mpz_clear(x1);
    curve_uninit();

    return 0;
}
