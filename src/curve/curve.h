// curve ops for bls12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__curve_h__

#include <gmp.h>

// static data initialization and access
mpz_t *get_p(void);
mpz_t *get_q(void);
void curve_init(void);
void curve_uninit(void);

// shorthand for GMP
void mul_modp(mpz_t out, const mpz_t in1, const mpz_t in2);
void sqr_modp(mpz_t out, const mpz_t in);

// addition chain for clearing cofactor
void clear_cofactor(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY);

#define __bls_hash__src__curve__curve_h__
#endif  // __bls_hash__src__curve__curve_h__
