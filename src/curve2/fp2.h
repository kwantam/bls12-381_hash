// definitions for elements of fp2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve2__fp2_h__

#include <gmp.h>

// struct for holding a point in Fp2
// the represented point is s + t * sqrt(-1) in Fp[sqrt(-1)] / (x^2 + 1)
struct mpz2_struct {
    mpz_t s;
    mpz_t t;
};
typedef struct mpz2_struct mpz_t2[1];

// mpz_init for mpz_t2
static inline void mpz2_init(mpz_t2 io) {
    mpz_init(io->s);
    mpz_init(io->t);
}

// mpz_clear for mpz_t2
static inline void mpz2_clear(mpz_t2 io) {
    mpz_clear(io->s);
    mpz_clear(io->t);
}

#define __bls_hash__src__curve2__fp2_h__
#endif  // __bls_hash__src__curve2__fp2_h__
