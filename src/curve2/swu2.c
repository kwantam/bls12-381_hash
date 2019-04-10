// SWU map ops for G2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "arith2.h"
#include "bint2.h"
#include "curve2.h"
#include "fp2.h"
#include "globals.h"
#include "globals2.h"
#include "ops2.h"

// S-vdW-Ulas simplified map
// see ../curve/swu.c and ../../paper/ for more info

// GMP-based non-ct impl
static inline void swu2_help(const unsigned jp_num, const mpz_t2 u) {
    // compute num and den of X0(u)
    sqr_modp2(mpz2_tmp[6], u);                        // u^2
    mul_modp2(mpz2_tmp[0], mpz2_tmp[6], swu2_xi);     // xi u^2
    sqr_modp2(mpz2_tmp[7], mpz2_tmp[0]);              // xi^2 u^4
    mpz2_add(mpz2_tmp[1], mpz2_tmp[0], mpz2_tmp[7]);  // xi^2 u^4 + xi u^2
    mpz2_add_ui2(mpz2_tmp[2], mpz2_tmp[1], 1, 0);     // xi^2 u^4 + xi u^2 + 1
    mul_modp2(mpz2_tmp[2], mpz2_tmp[2], ell2p_b);     // b * (xi^2 u^4 + xi u^2 + 1)
    if (mpz2_zero_p(mpz2_tmp[1])) {
        // in the exceptional case, set denominator to xi * a
        mul_modp2(mpz2_tmp[1], swu2_xi, ell2p_a);
    } else {
        mul_modp2(mpz2_tmp[1], mpz2_tmp[1], ell2p_a);  // a * (xi^2 u^4 + xi u^2)
        mpz2_neg(mpz2_tmp[1], mpz2_tmp[1]);
    }

    // compute num and den of X0(u)^3 + aX0(u) + b
    // for X0(u) = num/den, this is (num^3 + a num den^2 + b den^3) / den^3
    sqr_modp2(mpz2_tmp[3], mpz2_tmp[1]);               // den^2
    mul_modp2(mpz2_tmp[4], mpz2_tmp[2], mpz2_tmp[3]);  // num den^2
    mul_modp2(mpz2_tmp[4], mpz2_tmp[4], ell2p_a);      // a num den^2

    mul_modp2(mpz2_tmp[3], mpz2_tmp[3], mpz2_tmp[1]);  // den^3
    mul_modp2(mpz2_tmp[5], mpz2_tmp[3], ell2p_b);      // b den^3
    mpz2_add(mpz2_tmp[4], mpz2_tmp[4], mpz2_tmp[5]);   // a num den^2 + b den^3

    sqr_modp2(mpz2_tmp[5], mpz2_tmp[2]);               // num^2
    mul_modp2(mpz2_tmp[5], mpz2_tmp[5], mpz2_tmp[2]);  // num^3
    mpz2_add(mpz2_tmp[4], mpz2_tmp[4], mpz2_tmp[5]);   // num^3 + a num den^2 + b den^3

    // compute sqrt(U/V) where U is mpz2_tmp[4] and V is mpz2_tmp[3]
    if (!divsqrt_modp2(mpz2_tmp[5], mpz2_tmp[4], mpz2_tmp[3])) {
        // g(X0(u)) was nonsquare, so convert X0(u) to X1(u) ...
        mul_modp2(mpz2_tmp[2], mpz2_tmp[2], mpz2_tmp[0]);  // X1(u) = xi u^2 X0(u)

        // ... convert g(X0(u)) to g(X1(u)) ...
        mul_modp2(mpz2_tmp[4], mpz2_tmp[4], mpz2_tmp[0]);  // U = xi u^2 U
        mul_modp2(mpz2_tmp[4], mpz2_tmp[4], mpz2_tmp[7]);  // U = xi^3 u^6 U

        // ... and find sqrt(g(X1(u)))
        mul_modp2(mpz2_tmp[5], mpz2_tmp[5], mpz2_tmp[6]);  // sqrtCand * u^2
        mul_modp2(mpz2_tmp[5], mpz2_tmp[5], u);            // sqrtCand * u^3
        for (unsigned i = 0; i < 4; ++i) {
            mul_modp2(mpz2_tmp[6], mpz2_tmp[5], swu2_eta[i]);  // sqrtCand * u^3 * eta[i]
            sqr_modp2(mpz2_tmp[7], mpz2_tmp[6]);               // (sqrtCand * u^3 * eta[i])^2
            mul_modp2(mpz2_tmp[7], mpz2_tmp[7], mpz2_tmp[3]);  // V * (sqrtCand * u^3 * eta[i])^2
            mpz2_sub(mpz2_tmp[7], mpz2_tmp[7], mpz2_tmp[4]);   // V * (sqrtCand * u^3 * eta[i])^2 - U
            if (mpz2_zero_p(mpz2_tmp[7])) {
                // we found the right one
                mpz2_swap(mpz2_tmp[5], mpz2_tmp[6]);
                break;
            }
        }
    } else if (mpz_cmp(pm1o2, u->s) < 0) {
        // g(X0(u)) was square and u is negative, so negate y
        mpz2_neg(mpz2_tmp[5], mpz2_tmp[5]);
    }

    // now compute X, Y, and Z
    mul_modp2(mpz2_tmp[2], mpz2_tmp[2], mpz2_tmp[1]);  // xnum * xden = X => x = X/Xden^2
    mul_modp2(mpz2_tmp[5], mpz2_tmp[5], mpz2_tmp[3]);  // y * xden^3 = Y

    // export to jacobian point
    to_jac_point2(jp2_tmp + jp_num, mpz2_tmp[2], mpz2_tmp[5], mpz2_tmp[1]);
}

void swu2_map(mpz_t2 x, mpz_t2 y, mpz_t2 z, const mpz_t2 u) {
    swu2_help(0, u);
    from_jac_point2(x, y, z, jp2_tmp);
}
