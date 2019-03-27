// curve ops for bls12-381 hashing
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__curve_h__

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

// static data (un)initialization and access
void curve_init(void);
void precomp_init(void);
void curve_uninit(void);

// functions for mapping to curve
void svdw_map(mpz_t x, mpz_t y, const mpz_t t);
void svdw_map2(mpz_t x1, mpz_t y1, const mpz_t t1, mpz_t x2, mpz_t y2, const mpz_t t2);
bool check_fx(mpz_t y, const mpz_t x, bool negate, bool force);
void swu_map(mpz_t x, mpz_t y, mpz_t u);

// addition chain for clearing cofactor
void clear_h(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY);

// clear cofactor, add random subgroup element via 3-point multi-multiplication
void addrG_clear_h(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY, const uint8_t *r);

// add two points together and then clear cofactor
void add2(mpz_t outX, mpz_t outY, const mpz_t inX1, const mpz_t inY1, const mpz_t inX2, const mpz_t inY2);
void add2_clear_h(mpz_t outX, mpz_t outY, const mpz_t inX1, const mpz_t inY1, const mpz_t inX2, const mpz_t inY2);

#define __bls_hash__src__curve__curve_h__
#endif  // __bls_hash__src__curve__curve_h__
