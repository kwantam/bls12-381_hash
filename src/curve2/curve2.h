// definitions for curve hashing for G2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve2__curve2_h__

#include "fp2.h"

#include <stdbool.h>

// initialization functions
void curve2_init(void);
void curve2_uninit(void);

// for use with hash-and-check (and SvdW)
bool check_fx2(mpz_t2 y, const mpz_t2 x, const bool negate, const bool force, const bool field_only);

// SvdW maps
void svdw2_map(mpz_t2 x, mpz_t2 y, const mpz_t2 t);
void svdw2_map2(mpz_t2 x1, mpz_t2 y1, const mpz_t2 t1, mpz_t2 x2, mpz_t2 y2, const mpz_t2 t2);
void svdw2_map_fo(mpz_t2 x, mpz_t2 y, mpz_t2 z, const mpz_t2 t);

// point manipulation
void add2_2(mpz_t2 X1, mpz_t2 Y1, mpz_t2 Z1, const mpz_t2 X2, const mpz_t2 Y2, const mpz_t2 Z2);

#define __bls_hash__src__curve2__curve2_h__
#endif  // __bls_hash__src__curve2__curve2_h__
