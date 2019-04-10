#include "arith2.h"
#include "bint2.h"
#include "curve2.h"
#include "util.h"

#include <gmp.h>
#include <stdio.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    // initialize temp vars for curve2 computations
    curve2_init();

    // get libgmp ready
    mpz_t2 x, y, z;
    mpz2_init(x);
    mpz2_init(y);
    mpz2_init(z);

    // load libcrypto error strings and set up SHA and PRNG
    ERR_load_crypto_strings();
    SHA256_CTX hash_ctx;
    CHECK_CRYPTO(SHA256_Init(&hash_ctx));
    EVP_CIPHER_CTX *prng_ctx = EVP_CIPHER_CTX_new();
    CHECK_CRYPTO(prng_ctx != NULL);

    // hash the contents of stdin
    hash_stdin(&hash_ctx);

    // rng
    next_prng(prng_ctx, &hash_ctx, 0);

    // apply sqrt/divsqrt to random point
    printf("print all([\n");
    for (unsigned i = 0; i < 16384; ++i) {
        next_modp(prng_ctx, x->s);
        next_modp(prng_ctx, x->t);
        next_modp(prng_ctx, y->s);
        next_modp(prng_ctx, y->t);

        const bool square = divsqrt_modp2(z, x, y);

        if (square) {
            gmp_printf("F2(0x%Zx + X * 0x%Zx) / F2(0x%Zx + X * 0x%Zx) == F2(0x%Zx + X * 0x%Zx) ** 2,\n", x->s, x->t,
                       y->s, y->t, z->s, z->t);
        } else {
            gmp_printf("not (F2(0x%Zx + X * 0x%Zx) / F2(0x%Zx + X * 0x%Zx)).is_square(),\n", x->s, x->t, y->s, y->t);
        }
    }
    printf("])\n");

    // clean up
    EVP_CIPHER_CTX_free(prng_ctx);
    mpz2_clear(x);
    mpz2_clear(y);
    mpz2_clear(z);
    curve2_uninit();

    return 0;
}
