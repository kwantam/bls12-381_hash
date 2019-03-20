// curve ops for bls12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__curve_h__

#include <gmp.h>
#include <stdint.h>

// static data (un)initialization and access
mpz_t *get_p(void);
void curve_init(void);
void precomp_init(void);
void curve_uninit(void);

// field ops for GMP
void mul_modp(mpz_t out, const mpz_t in1, const mpz_t in2);
void sqr_modp(mpz_t out, const mpz_t in);
void sqrt_modp(mpz_t out, const mpz_t in);

// useful curve functions
void bls_fx(mpz_t out, const mpz_t in);
void svdw_map(mpz_t x, mpz_t y, const mpz_t t);

// addition chain for clearing cofactor
void clear_cofactor(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY);

// clear cofactor, add random subgroup element via 3-point multi-multiplication
void clear_and_add_rG(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY, const uint8_t *r);

#define __bls_hash__src__curve__curve_h__
#endif  // __bls_hash__src__curve__curve_h__
