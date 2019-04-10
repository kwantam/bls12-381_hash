// consts for bls12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__consts_h__

#include "bint_consts.h"

#include <stdint.h>

// base field characteristic
#define P_LEN 48
extern const uint8_t BLS12_381_p[P_LEN];

// group order
#define Q_LEN 32
extern const uint8_t BLS12_381_q[Q_LEN];

// cofactor
#define H_LEN 16
extern const uint8_t BLS12_381_h[H_LEN];

// base point G' : ./hash_and_check -n 1 <<< "bls12_381 random base point"
extern const bint_ty g_prime_x;
extern const bint_ty g_prime_y;

// 2^128 * the above point
extern const bint_ty g_prime_ll128_x;
extern const bint_ty g_prime_ll128_y;

// constants for the Shallue and van de Woestijne mapping
extern const uint64_t Icx1[6];
extern const uint64_t Icx2[6];
extern const uint64_t IsqrtM27[6];
extern const uint64_t IinvM27[6];

#define __bls_hash__src__curve__consts_h__
#endif  // __bls_hash__src__curve__consts_h__
