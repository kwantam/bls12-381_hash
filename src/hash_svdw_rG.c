// G1 hash for BLS12-381: one evaluation of Shallue and van de Woestijne map plus random subgroup point
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
    mpz_t x1, y1, t, rr;
    mpz_init(x1);
    mpz_init(y1);
    mpz_init(t);
    mpz_init(rr);

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
        next_prng(prng_ctx, &hash_ctx, i);
        next_modp(prng_ctx, t);
        svdw_map(x1, y1, t);
        const uint8_t *r = next_modq(prng_ctx, opts.test ? &rr : NULL);

        // show results                     svdw_addrG
        //   test:                          (xI, yI, r, xO, yO)
        //   quiet && !test:                <<nothing>>
        //   !quiet && !test && clear_h:    (xO, yO)
        //   !quiet && !test && !clear_h:   (xI, yI, r)
        if (first_print || second_print) {
            printf("(");
        }

        // maybe output points and r
        if (opts.test) {
            gmp_printf("0x%Zx, 0x%Zx, ", t, rr);
        } else if (first_print) {
            gmp_printf("0x%Zx, 0x%Zx, ", x1, y1);
        }

        // maybe do the addition
        if (do_clear) {
            addrG_clear_h(x1, y1, x1, y1, r);
        }

        // maybe output the result
        if (second_print) {
            gmp_printf("0x%Zx, 0x%Zx, ", x1, y1);
        }

        if (first_print || second_print) {
            printf(")\n");
        }
    }

    // free
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz_clear(rr);
    mpz_clear(t);
    mpz_clear(y1);
    mpz_clear(x1);
    curve_uninit();

    return 0;
}
