// globals and initialization functions for E2 ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__globals2_h__

#include "fp2.h"

// temps for basic arithmetic in fp2
extern mpz_t2 mul2tmp[2];

// values for computing sqrts
extern mpz_t2 ssqrtM1[2];

// values for SWU map
extern mpz_t2 swu2_eta[4];

#define __bls_hash__src__curve__globals2_h__
#endif  // __bls_hash__src__curve__globals2_h__
