// arithmetic opeations in Fp^2 for E2 curve ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__arith2_h__

#include "arith.h"
#include "fp2.h"

#include <gmp.h>

// forward declarations to avoid including globals{,2}.h in the header
extern mpz_t fld_p;
extern mpz_t2 mul2tmp[2];

// square in fp2
// out == in is OK
static inline void sqr_modp2(mpz_t2 out, const mpz_t2 in) {
    sqr_modp(mul2tmp[0].s, in.s);  // square into tmp so that out==in is OK
    sqr_modp(mul2tmp[0].t, in.t);  // "

    mul_modp(out.t, in.s, in.t);
    mpz_mul_2exp(out.t, out.t, 1);  // 2 * inS * inT
    condsub_p(out.t);               // reduce mod p

    mpz_sub(out.s, mul2tmp[0].s, mul2tmp[0].t);  // inS^2 - inT^2
    condadd_p(out.s);                            // reduce mod p
}

// multiply in fp2
// out == in1 or out == in2 is OK
static inline void mul_modp2(mpz_t2 out, const mpz_t2 in1, const mpz_t2 in2) {
    mul_modp(mul2tmp[0].s, in1.s, in2.s);  // left- and right-mults
    mul_modp(mul2tmp[0].t, in1.t, in2.t);  // "

    mul_modp(mul2tmp[1].s, in1.s, in2.t);  // cross1
    mul_modp(mul2tmp[1].t, in1.t, in2.s);  // cross2

    mpz_sub(out.s, mul2tmp[0].s, mul2tmp[0].t);  // in1S in2S - in1T in2T
    condadd_p(out.s);                            // reduce mod p

    mpz_add(out.t, mul2tmp[1].s, mul2tmp[1].t);  // in1S in2T + in1T in2S
    condsub_p(out.t);                            // reduce mod p
}

// multiply in fp2 by sqrt(-1)
// out == in is OK
static inline void mul_modp2_i(mpz_t2 out, const mpz_t2 in) {
    mpz_set(out.s, in.s);
    mpz_sub(out.t, fld_p, in.t);  // sqrt(-1) * sqrt(-1) * t = -t
    mpz_swap(out.s, out.t);
}

// multiply in fp2 by a scalar (i.e., a value in fp)
static inline void mul_modp2_scalar(mpz_t2 out, const mpz_t2 in1, const mpz_t in2) {
    mul_modp(out.s, in1.s, in2);
    mul_modp(out.t, in1.t, in2);
}

// multiply in fp2 by sqrt(-1) * scalar
static inline void mul_modp2_i_scalar(mpz_t2 out, const mpz_t2 in1, const mpz_t in2) {
    mul_modp2_scalar(out, in1, in2);
    mpz_sub(out.t, fld_p, out.t);
    mpz_swap(out.s, out.t);
}

#define __bls_hash__src__curve__arith2_h__
#endif  // __bls_hash__src__curve__arith2_h__
