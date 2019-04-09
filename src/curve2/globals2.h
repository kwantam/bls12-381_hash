// globals and initialization functions for E2 ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve2__globals2_h__

#include "curve2.h"

// temps for basic arithmetic in fp2
#define NUM_MPZ2_TMP 16
extern mpz_t2 mpz2_tmp[NUM_MPZ2_TMP];
extern mpz_t2 mpz2mul[2];  // private temps for multiplication and squaring

// values for SWU map
extern mpz_t2 swu2_eta[4];

#define __bls_hash__src__curve2__globals2_h__
#endif  // __bls_hash__src__curve2__globals2_h__
