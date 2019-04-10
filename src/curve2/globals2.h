// globals and initialization functions for E2 ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve2__globals2_h__

#include "bint2_consts.h"
#include "curve2.h"

// temps for basic arithmetic in fp2
#define NUM_MPZ2_TMP 16
extern mpz_t2 mpz2_tmp[NUM_MPZ2_TMP];
extern mpz_t2 mpz2mul[2];  // private temps for multiplication and squaring

#define NUM_BINT2_TMP 15
extern bint2_ty bint2_tmp[NUM_BINT2_TMP];

// values for SvdW map
extern mpz_t cx1_2, cx2_2, sqrtM3, inv3;
extern bint2_ty bint2_3p4i, bint2_cx1_2, bint2_one;
extern bint_ty bint_cx2_2, bint_sqrtM3;

// values for SWU map
extern mpz_t2 swu2_eta[4];
extern mpz_t2 swu2_xi, ell2p_a, ell2p_b;

#define __bls_hash__src__curve2__globals2_h__
#endif  // __bls_hash__src__curve2__globals2_h__
