// constants for fp2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__consts2_h__

#include <stdint.h>

// a constant used to compute sqrt(sqrt(-1)) and sqrt(-sqrt(-1))
extern const uint64_t IssqrtM1[6];

// constants to compute values of eta for SWU map
extern const uint64_t Ieta1[6];
extern const uint64_t Ieta2[6];

#define __bls_hash__src__curve__consts2_h__
#endif  // __bls_hash__src__curve__consts2_h__
