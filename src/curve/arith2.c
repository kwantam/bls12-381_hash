// arithmetic opeations in Fp^2 for E2 curve ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "arith2.h"

/*
// sqrt(U/V), and return whether we actually found a sqrt
// out should not be the same as tmp, u, or v
bool divsqrt2(mpz_t2 out, mpz_t2 tmp, const mpz_t2 u, const mpz_t2 v) {
    sqr_modp2(out, v);         // v^2
    mul_modp2(tmp, out, v);    // v^3
    sqr_modp2(out, out);       // v^4
    mul_modp2(tmp, tmp, out);  // v^7
    sqr_modp2(out, out);       // v^8
    mul_modp2(tmp, tmp, u);    // u v^7
    mul_modp2(out, tmp, out);  // u v^15
}
*/
