// arithmetic opeations in Fp^2 for E2 curve ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "arith2.h"

#include "arith2_chains.h"
#include "globals2.h"

// helper for divsqrt: test whether we've found the desired sqrt
static inline bool divsqrt2_test_help(const mpz_t2 in, const mpz_t2 u, const mpz_t2 v) {
    mul_modp2(mpz2_tmp[0], in, v);          // in * v
    mpz2_sub(mpz2_tmp[0], mpz2_tmp[0], u);  // in * v - u
    return mpz2_zero_p(mpz2_tmp[0]);
}

// sqrt(U/V), and return whether we actually found a sqrt
// out should not be the same as u or v
bool divsqrt2(mpz_t2 out, const mpz_t2 u, const mpz_t2 v) {
    sqr_modp2(out, v);                         // v^2
    mul_modp2(mpz2_tmp[0], out, v);            // v^3
    sqr_modp2(out, out);                       // v^4
    mul_modp2(mpz2_tmp[0], mpz2_tmp[0], out);  // v^7
    sqr_modp2(out, out);                       // v^8
    mul_modp2(mpz2_tmp[0], mpz2_tmp[0], u);    // u v^7
    mul_modp2(mpz2_tmp[1], mpz2_tmp[0], out);  // u v^15
    chain_divsqrt2(out, mpz2_tmp[1]);          // (uv^15)^((p^2-9)/16)
    mul_modp2(out, out, mpz2_tmp[0]);          // uv^7(uv^15)^((p^2-9)/16)

    // did we find the sqrt?
    sqr_modp2(mpz2_tmp[1], out);             // out^2
    if (divsqrt2_test_help(mpz2_tmp[1], u, v)) {
        return true;
    }

    // did we find sqrt/sqrt(-1) ?
    mpz2_neg(mpz2_tmp[1], mpz2_tmp[1]);      // - (out^2)
    if (divsqrt2_test_help(mpz2_tmp[1], u, v)) {
        mul_modp2_i(out, out);
        return true;
    }

    // test other two possibilities
    for (unsigned i = 0; i < 2; ++i) {
        mul_modp2(mpz2_tmp[1], out, ssqrtM1[i]);  // sqrt(sqrt(-1)) * out
        sqr_modp2(mpz2_tmp[0], mpz2_tmp[1]);      // (sqrt(sqrt(-1)) * out) ^ 2
        if (divsqrt2_test_help(mpz2_tmp[0], u, v)) {
            mpz2_swap(out, mpz2_tmp[1]);
            return true;
        }
    }

    return false;
}

// inverse in Fp2
void invert_modp2(mpz_t2 out, const mpz_t2 in) {
    sqr_modp(mpz2_tmp[0].s, in.s);                         // in.s ^ 2
    sqr_modp(mpz2_tmp[0].t, in.t);                         // in.t ^ 2
    mpz_add(mpz2_tmp[0].s, mpz2_tmp[0].s, mpz2_tmp[0].t);  // in.s ^ 2 + in.t ^ 2

    mpz_invert(mpz2_tmp[0].t, mpz2_tmp[0].s, fld_p);  // 1 / (in.s ^ 2 + in.t ^ 2)

    mul_modp(out.s, in.s, mpz2_tmp[0].t);  // in.s / (in.s ^ 2 + in.t ^ 2)
    mul_modp(out.t, in.t, mpz2_tmp[0].t);  // in.t / (in.s ^ 2 + in.t ^ 2)
    mpz_sub(out.t, fld_p, out.t);          // -in.t / (in.s ^ 2 + in.t ^ 2)
}
