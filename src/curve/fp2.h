// definitions for fp2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__fp2_h__

#include <gmp.h>

// struct for holding a point in Fp2
// the represented point is s + t * sqrt(-1) in Fp[sqrt(-1)] / (x^2 + 1)
typedef struct mpz_t2_s {
    mpz_t s;
    mpz_t t;
} mpz_t2;

#define __bls_hash__src__curve__fp2_h__
#endif  // __bls_hash__src__curve__fp2_h__
