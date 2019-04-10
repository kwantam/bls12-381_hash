// SWU map ops for G2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "arith2.h"
#include "bint2.h"
#include "curve2.h"
#include "fp2.h"
#include "globals2.h"
#include "iso2.h"
#include "ops2.h"

// forward decl to avoid including globals.h
extern mpz_t pm1o2;

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

// evaluate polynomial using Horner's rule
static inline void bint2_horner(bint2_ty out, const bint2_ty x, const int startval) {
    for (int i = startval; i >= 0; --i) {
        bint2_mul(out, out, x);             // tot *= x         v4,w3
        bint2_add(out, out, bint2_tmp[i]);  // tot += next_val  v8,w6
    }
}

// precompute for isogeny map
static inline void compute_map_zvals(const bint2_ty inv[], bint2_ty zv[], const unsigned len) {
    for (unsigned i = 0; i < len; ++i) {
        bint2_mul(bint2_tmp[i], inv[i], zv[i]);
    }
}

// 3-isogeny map (i/o in jp2_tmp[1])
static inline void eval_iso3(void) {
    // precompute even powers of Z up to z^6
    bint2_sqr(bint2_tmp[14], jp2_tmp[1].Z);                  // Z^2     v4,w3,i16/9
    bint2_sqr(bint2_tmp[13], bint2_tmp[14]);                 // Z^4     v4,w3,i16/9
    bint2_mul(bint2_tmp[12], bint2_tmp[14], bint2_tmp[13]);  // Z^6     v4,w3,i16/9

    // Ymap denominator
    compute_map_zvals(iso2_yden, bint2_tmp + 12, ELLP2_YMAP_DEN_LEN);           // k_(3-i) Z^(2i)
    bint2_add(bint2_tmp[11], jp2_tmp[1].X, bint2_tmp[ELLP2_YMAP_DEN_LEN - 1]);  // X + k_2 Z^2
    bint2_horner(bint2_tmp[11], jp2_tmp[1].X, ELLP2_YMAP_DEN_LEN - 2);          // Horner
    bint2_mul(bint2_tmp[11], bint2_tmp[11], bint2_tmp[14]);                     // Yden * Z^2
    bint2_mul(bint2_tmp[11], bint2_tmp[11], jp2_tmp[1].Z);                      // Yden * Z^3

    // Ymap numerator
    compute_map_zvals(iso2_ynum, bint2_tmp + 12, ELLP2_YMAP_NUM_LEN - 1);        // k_(3-i) Z^(2i)
    bint2_mul(bint2_tmp[10], jp2_tmp[1].X, iso2_ynum[ELLP2_YMAP_NUM_LEN - 1]);   // k_3 * X
    bint2_add(bint2_tmp[10], bint2_tmp[10], bint2_tmp[ELLP2_YMAP_NUM_LEN - 2]);  // k_3 * X + k_2 Z^2
    bint2_horner(bint2_tmp[10], jp2_tmp[1].X, ELLP2_YMAP_NUM_LEN - 3);           // Horner for rest
    bint2_mul(bint2_tmp[10], bint2_tmp[10], jp2_tmp[1].Y);
    // ymap num/den are in bint2_tmp[10]/bint2_tmp[11]

    // Xmap denominator
    compute_map_zvals(iso2_xden, bint2_tmp + 13, ELLP2_XMAP_DEN_LEN);          // k_(2-i) Z^(2i)
    bint2_add(bint2_tmp[9], jp2_tmp[1].X, bint2_tmp[ELLP2_XMAP_DEN_LEN - 1]);  // X + k_1 Z^2
    bint2_horner(bint2_tmp[9], jp2_tmp[1].X, ELLP2_XMAP_DEN_LEN - 2);          // Horner for rest
    // mul by Z^2 because numerator has degree one greater than denominator
    bint2_mul(bint2_tmp[9], bint2_tmp[9], bint2_tmp[14]);

    // Xmap numerator
    compute_map_zvals(iso2_xnum, bint2_tmp + 12, ELLP2_XMAP_NUM_LEN);          // k_(3-i) Z^(2i)
    bint2_mul(bint2_tmp[8], jp2_tmp[1].X, iso2_xnum[ELLP2_XMAP_NUM_LEN - 1]);  // k_3 * X
    bint2_add(bint2_tmp[8], bint2_tmp[8], bint2_tmp[ELLP2_XMAP_NUM_LEN - 2]);  // k_3 * X + k_2 Z^2
    bint2_horner(bint2_tmp[8], jp2_tmp[1].X, ELLP2_XMAP_NUM_LEN - 3);          // Horner for rest
    // xmap num/den are in bint2_tmp[8]/bint2_tmp[9]

    // Jacobian coords
    bint2_mul(jp2_tmp[1].Z, bint2_tmp[9], bint2_tmp[11]);  // Z = Xden Yden
    bint2_mul(jp2_tmp[1].X, bint2_tmp[8], bint2_tmp[11]);  // Xnum Yden
    bint2_mul(jp2_tmp[1].X, jp2_tmp[1].X, jp2_tmp[1].Z);   // X = Xnum Xden Yden^2 => X / Z^2 = Xnum / Xden
    bint2_sqr(bint2_tmp[7], jp2_tmp[1].Z);                 // Z^2
    bint2_mul(jp2_tmp[1].Y, bint2_tmp[10], bint2_tmp[9]);  // Ynum Xden
    bint2_mul(jp2_tmp[1].Y, jp2_tmp[1].Y, bint2_tmp[7]);   // Y = Ynum Xden^3 Yden^2 => Y / Z^3 = Ynum / Yden
}

void swu2_map(mpz_t2 x, mpz_t2 y, mpz_t2 z, const mpz_t2 u) {
    swu2_help(1, u);
    eval_iso3();
    from_jac_point2(x, y, z, jp2_tmp + 1);
}

void swu2_map2(mpz_t2 x, mpz_t2 y, mpz_t2 z, const mpz_t2 u1, const mpz_t2 u2) {
    swu2_help(0, u1);
    swu2_help(1, u2);
    point2_add(jp2_tmp + 1, jp2_tmp, jp2_tmp + 1);
    eval_iso3();
    from_jac_point2(x, y, z, jp2_tmp + 1);
}
