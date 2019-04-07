// SWU map operations
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "arith.h"
#include "bint.h"
#include "curve.h"
#include "globals.h"
#include "iso.h"
#include "ops.h"

#include <string.h>

// *************************************************************
// S-vdW-Ulas simplified map for curves with nonzero j-invariant
// *************************************************************
// Map to curve with j-invariant != 0, 1728 (i.e., NOT BLS12-381!)
//
// However, we can still use it with BLS12-381 by mapping to an isogenous curve
// and then evaluating the isogeny. In our case, we use an 11-isogenous curve;
// see src/curve/iso.h and src/etc/gen_iso_params.sage.
//
// This version of SWU is based on the description given by
//   Tibouchi, "Elligator Squared: Uniform points on elliptic curves of prime order
//   as uniform random strings." Proc. Financial Crypto 2014. https://eprint.iacr.org/2014/043.
//
// which comes from Section 7 of
//   Brier, Coron, Icart, Madore, Randriam, Tibouchi. "Efficient indifferentiable hashing
//   into ordinary elliptic curves." Proc. CRYPTO 2010. https://eprint.iacr.org/2009/340.
//
// which is itself a simplification of the one given by
//   Ulas, "Rational points on certain hyperelliptic curves over finite fields."
//   Bulletin of the Polish Academy of Science Mathematics, vol 55, no 2, pp 97--104, 2007.
//
// which is based on the map of Shallue and van de Woestijne (see citation above).
//
// We use a couple small tricks to simplify evaluating:
//   1. If g(X0(u)) is nonsquare, then u^3 * g(X0(u))^((p+1)/4) is a sqrt of g(X1(u)).
//      See document in paper/ subdir for justification.
//
//   2. We can compute sqrt(u/v) without inverting v, via a trick given in Section 5 of
//      Berinstein, Duif, Lange, Schwabe, and Yang, "High-speed high-security signatures."
//      J. Crypto. Eng., vol 2, issue 2, pp 77--89, September 2012.
//      http://ed25519.cr.yp.to/ed25519-20110926.pdf
//
// There are two versions: one uses GMP and is not constant-time,
// the other uses bint and is constant time.

// GMP-based non-constant-time SWU impl
// This function outputs a point in Jacobian coordinates in jp_tmp[jp_num].
static inline void swu_help(const unsigned jp_num, const mpz_t u) {
    // compute numerator and denominator of X0(u)
    sqr_modp(mpz_tmp[0], u);                      // u^2
    sqr_modp(mpz_tmp[1], mpz_tmp[0]);             // u^4
    mpz_sub(mpz_tmp[1], mpz_tmp[0], mpz_tmp[1]);  // u^2 - u^4
    mpz_ui_sub(mpz_tmp[2], 1, mpz_tmp[1]);        // u^4 - u^2 + 1
    mul_modp(mpz_tmp[2], mpz_tmp[2], ellp_b);     // b * (u^4 - u^2 + 1)                => X0num
    mul_modp(mpz_tmp[1], mpz_tmp[1], ellp_a);     // a * (u^2 - u^4)                    => Xden
    if (mpz_cmp_ui(mpz_tmp[1], 0) == 0) {
        // u was 0, -1, 1, so num is b and den is 0; set den to -a, because -b/a is square in Fp
        mpz_sub(mpz_tmp[1], fld_p, ellp_a);
    }

    // compute numerator and denominator of X0(u)^3 + aX0(u) + b
    // for X0(u) = num/den, this is (num^3 + a * num * den^2 + b den^3) / den^3
    sqr_modp(mpz_tmp[7], mpz_tmp[1]);              // den^2
    mul_modp(mpz_tmp[4], mpz_tmp[2], mpz_tmp[7]);  // num * den^2
    mul_modp(mpz_tmp[4], mpz_tmp[4], ellp_a);      // a * num * den^2
                                                   //
    mul_modp(mpz_tmp[3], mpz_tmp[7], mpz_tmp[1]);  // den^3
    mul_modp(mpz_tmp[5], mpz_tmp[3], ellp_b);      // b * den^3
    mpz_add(mpz_tmp[4], mpz_tmp[4], mpz_tmp[5]);   // a * num * den^2 + b * den^3
                                                   //
    sqr_modp(mpz_tmp[5], mpz_tmp[2]);              // num^2
    mul_modp(mpz_tmp[5], mpz_tmp[5], mpz_tmp[2]);  // num^3
    mpz_add(mpz_tmp[4], mpz_tmp[4], mpz_tmp[5]);   // (num^3 + a * num * den^2 + b * den^3)

    // compute sqrt(U/V) where U = (num^3 + a * num * den^2 + b * den^3), V = den^3
    if (!divsqrt(mpz_tmp[5], mpz_tmp[6], mpz_tmp[4], mpz_tmp[3], false)) {
        // g(X0(u)) was nonsquare, so convert to g(X1(u))
        // NOTE: multiplying by u^3 preserves sign of u, so no need to apply sgn0(u) to y
        mul_modp(mpz_tmp[5], mpz_tmp[5], mpz_tmp[0]);  // u^2 * sqrtCand
        mul_modp(mpz_tmp[5], mpz_tmp[5], u);           // u^3 * sqrtCand
        mul_modp(mpz_tmp[2], mpz_tmp[2], mpz_tmp[0]);  // b * u^2 * (u^4 - u^2 + 1)
        mpz_sub(mpz_tmp[2], fld_p, mpz_tmp[2]);        // - b * u^2 * (u^4 - u^2 + 1)   => X1num
    } else if (mpz_cmp(pm1o2, u) < 0) {
        // g(X0(u)) was square and u is negative, so negate y
        mpz_sub(mpz_tmp[5], fld_p, mpz_tmp[5]);  // negate y because u is negative
    }

    // now compute X, Y, and Z
    mul_modp(mpz_tmp[2], mpz_tmp[2], mpz_tmp[1]);  // Xnum * Xden = X  =>  x = Xnum/Xden = X / Xden^2
    mul_modp(mpz_tmp[5], mpz_tmp[5], mpz_tmp[7]);  // y * Xden^2 = Y/Z
    mul_modp(mpz_tmp[5], mpz_tmp[5], mpz_tmp[1]);  // y * Xden^3 = Y

    // export to jacobian point
    bint_import_mpz(jp_tmp[jp_num].X, mpz_tmp[2]);  // X
    bint_import_mpz(jp_tmp[jp_num].Y, mpz_tmp[5]);  // Y
    bint_import_mpz(jp_tmp[jp_num].Z, mpz_tmp[1]);  // Z
}

// evaluate polynomial via Horner's method
static inline void horner_eval(mpz_t out, const mpz_t x, const int startval) {
    for (int i = startval; i >= 0; --i) {
        mul_modp(out, out, x);
        mpz_add(out, out, mpz_tmp[i]);
    }
}

// compute monomials of the rational map given even powers of Z
static inline void compute_map_terms(mpz_t *inv, mpz_t *zv, const unsigned len) {
    for (unsigned i = 0; i < len; ++i) {
        mul_modp(mpz_tmp[i], inv[i], zv[i]);
    }
}

// evaluate the 11-isogeny map from EllP to Ell
static void eval_iso11(void) {
    bint_export_mpz(mpz_tmp[32], jp_tmp[1].Z);  // extract Jacobian coord Z
    // precompute even powers of Z up to Z^30
    sqr_modp(mpz_tmp[31], mpz_tmp[32]);               // Z^2
    mul_modp(mpz_tmp[32], mpz_tmp[32], mpz_tmp[31]);  // Z^3
    sqr_modp(mpz_tmp[30], mpz_tmp[31]);               // Z^4
    mul_modp(mpz_tmp[29], mpz_tmp[30], mpz_tmp[31]);  // Z^6
    sqr_modp(mpz_tmp[28], mpz_tmp[30]);               // Z^8
    for (unsigned i = 0; i < 3; ++i) {
        mul_modp(mpz_tmp[27 - i], mpz_tmp[28 - i], mpz_tmp[31]);  // Z^10, Z^12, Z^14
    }
    sqr_modp(mpz_tmp[24], mpz_tmp[28]);  // Z^16
    for (unsigned i = 0; i < 7; ++i) {
        mul_modp(mpz_tmp[23 - i], mpz_tmp[24 - i], mpz_tmp[31]);  // Z^18, ..., Z^30
    }
    // last unused tmp at this point is mpz_tmp[16]

    // Denominator of Y map
    //
    //    x^15       + k14 x^14       + k13 x^13       + k12 x^12       + ...
    // =  (X/Z^2)^15 + k14 (X/Z^2)^14 + k13 (X/Z^2)^13 + k12 (X/Z^2)^12 + ...
    // = (X^15       + k14 Z^2 X^14   + k13 Z^4 X^13   + k12 Z^6 X^12   + ...) / Z^30
    //
    // Rewriting numerator in similar way causes Z^30 terms to cancel.
    //
    // We do essentially the same for the X rational map below.

    // Ymap denominator
    compute_map_terms(ymap_den, mpz_tmp + 17, ELLP_YMAP_DEN_LEN);       // Compute k_(15-i) Z^(2i) terms for ymap den
    bint_export_mpz(mpz_tmp[15], jp_tmp[1].X);                          // extract Jacobian coord X
    mpz_add(mpz_tmp[16], mpz_tmp[15], mpz_tmp[ELLP_YMAP_DEN_LEN - 1]);  // X + k_14 Z^2 (denominator is monic)
    horner_eval(mpz_tmp[16], mpz_tmp[15], ELLP_YMAP_DEN_LEN - 2);       // Horner for rest
    mul_modp(mpz_tmp[32], mpz_tmp[32], mpz_tmp[16]);                    // Z^3 * denom because y = Y/Z^3 is in numer

    // Ymap numerator
    compute_map_terms(ymap_num, mpz_tmp + 17, ELLP_YMAP_NUM_LEN - 1);     // Compute k_(15-i) Z^(2i) terms for ymap num
    mul_modp(mpz_tmp[16], mpz_tmp[15], ymap_num[ELLP_YMAP_NUM_LEN - 1]);  // k_15 * X
    mpz_add(mpz_tmp[16], mpz_tmp[16], mpz_tmp[ELLP_YMAP_NUM_LEN - 2]);    // k_15 * X + k_14 Z^2
    horner_eval(mpz_tmp[16], mpz_tmp[15], ELLP_YMAP_NUM_LEN - 3);         // Horner for rest
    bint_export_mpz(mpz_tmp[14], jp_tmp[1].Y);                            // extract Jacobian coord Y
    mul_modp(mpz_tmp[16], mpz_tmp[16], mpz_tmp[14]);                      // numerator * Y
    // at this point: numerator of Ymap is in mpz_tmp[16], denominator in mpz_tmp[32]

    // Xmap denominator
    compute_map_terms(xmap_den, mpz_tmp + 22, ELLP_XMAP_DEN_LEN);       // Compute k_(10-i) Z^(2i) terms for xmap den
    mpz_add(mpz_tmp[14], mpz_tmp[15], mpz_tmp[ELLP_XMAP_DEN_LEN - 1]);  // X + k_9 Z^2 (denominator is monic)
    horner_eval(mpz_tmp[14], mpz_tmp[15], ELLP_XMAP_DEN_LEN - 2);       // Horner for rest
    // multiply by Z^2 because numerator has degree 11 while denominator has degree 10 => Z^20/Z^22 = 1/Z^2
    mul_modp(mpz_tmp[14], mpz_tmp[14], mpz_tmp[31]);

    // Xmap numerator
    compute_map_terms(xmap_num, mpz_tmp + 21, ELLP_XMAP_NUM_LEN - 1);     // compute k_(11-i) Z^(2i) terms for xmap num
    mul_modp(mpz_tmp[13], mpz_tmp[15], xmap_num[ELLP_XMAP_NUM_LEN - 1]);  // k_11 * X
    mpz_add(mpz_tmp[13], mpz_tmp[13], mpz_tmp[ELLP_XMAP_NUM_LEN - 2]);    // k_11 * X + k_10 Z^2
    horner_eval(mpz_tmp[13], mpz_tmp[15], ELLP_XMAP_NUM_LEN - 3);         // Horner for rest
    // at this point: Ymap num/den in 16/32, Xmap num/den in 13/14

    // compute Jacobian projective coordinates
    mul_modp(mpz_tmp[12], mpz_tmp[14], mpz_tmp[32]);  // Xden * Yden = Zout
    mul_modp(mpz_tmp[10], mpz_tmp[16], mpz_tmp[14]);  // Ynum * Xden = Yout' = y * Zout
    mul_modp(mpz_tmp[11], mpz_tmp[13], mpz_tmp[32]);  // Xnum * Yden = Xout' = x * Zout
    mul_modp(mpz_tmp[11], mpz_tmp[11], mpz_tmp[12]);  // Xout = Xout' * Zout = x * Zout^2, so x = Xout / Zout^2
    sqr_modp(mpz_tmp[9], mpz_tmp[12]);                // Zout^2
    mul_modp(mpz_tmp[10], mpz_tmp[10], mpz_tmp[9]);   // Yout = Yout' * Zout^2 = y * Zout^3, so y = Yout / Zout^3

    // write to the jacobian point representation
    bint_import_mpz(jp_tmp[1].X, mpz_tmp[11]);
    bint_import_mpz(jp_tmp[1].Y, mpz_tmp[10]);
    bint_import_mpz(jp_tmp[1].Z, mpz_tmp[12]);
}

// bint-based constant-time SWU impl
static inline void swu_help_ct(const unsigned jp_num, const mpz_t u) {
    // mpz to bint
    bint_import_mpz(bint_tmp[10], u);

    // compute numerator and denominator of X0(u)
    bint_sqr(bint_tmp[0], bint_tmp[10]);                 // u^2                                 v = 2   w = 1
    bint_sqr(bint_tmp[1], bint_tmp[0]);                  // u^4                                 v = 2   w = 1
    bint_sub(bint_tmp[1], bint_tmp[0], bint_tmp[1], 2);  // u^2 - u^4                           v = 6   w = 5
    bint_sub(bint_tmp[2], bint_one, bint_tmp[1], 3);     // u^4 - u^2 + 1                       v = 9   w = 9
    bint_mul(bint_tmp[2], bint_tmp[2], bint_ellp_b);     // b * (u^4 - u^2 + 1)    X0_num       v = 2   w = 1
    bint_mul(bint_tmp[1], bint_tmp[1], bint_ellp_a);     // a * (u^2 - u^4)        X0_den       v = 2   w = 1
    bint_neg(bint_tmp[3], bint_ellp_a, 1);               // -a                                  v = 4   w = 3

    // check if denominator is zero and set to -a if so
    const bool den0 = bint_eq0(bint_tmp[1]);                       //                           v = 1   w = 1
    bint_condassign(bint_tmp[1], den0, bint_tmp[3], bint_tmp[1]);  // -a or a(u^2-u^4)          v = 4   w = 3

    // compute numerator and denominator of X0(u)^3 + aX0(u) + b
    // i.e., (num^3 + a num den^2 + b den^3) / den^3
    bint_sqr(bint_tmp[9], bint_tmp[1]);               // den^2                                  v = 2   w = 1
    bint_mul(bint_tmp[4], bint_tmp[2], bint_tmp[9]);  // num den^2                              v = 2   w = 1
    bint_mul(bint_tmp[4], bint_tmp[4], bint_ellp_a);  // a num den^2                            v = 2   w = 1
                                                      //
    bint_mul(bint_tmp[3], bint_tmp[9], bint_tmp[1]);  // den^3                                  v = 2   w = 1
    bint_mul(bint_tmp[5], bint_tmp[3], bint_ellp_b);  // b den^3                                v = 2   w = 1
    bint_add(bint_tmp[4], bint_tmp[4], bint_tmp[5]);  // a num den^2 + b den^3                  v = 4   w = 2
                                                      //
    bint_sqr(bint_tmp[5], bint_tmp[2]);               // num^2                                  v = 2   w = 1
    bint_mul(bint_tmp[5], bint_tmp[5], bint_tmp[2]);  // num^3                                  v = 2   w = 1
    bint_add(bint_tmp[4], bint_tmp[4], bint_tmp[5]);  // num^3 + a num den^2 + b den^3          v = 6   w = 3

    // compute sqrt(bint_tmp[4] / bint_tmp[3])
    const bool x0_good = bint_divsqrt(bint_tmp[5], bint_tmp[4], bint_tmp[3], false);  //        v = 2   w = 1

    // compute value for the case that x0 was good, y needs to be negative
    const bool u_neg = bint_is_neg(bint_tmp[10]);
    bint_neg(bint_tmp[8], bint_tmp[5], 1);  // -sqrtCand                                        v = 2   w = 2

    // compute values for the case that x0 was bad
    bint_mul(bint_tmp[6], bint_tmp[5], bint_tmp[0]);   // u^2 * sqrtCand                        v = 2   w = 1
    bint_mul(bint_tmp[6], bint_tmp[6], bint_tmp[10]);  // u^3 * sqrtCand                        v = 2   w = 1
    bint_mul(bint_tmp[7], bint_tmp[2], bint_tmp[0]);   // b u^2 (u^4 - u^2 + 1)                 v = 2   w = 1
    bint_neg(bint_tmp[7], bint_tmp[7], 1);             // -b u^2 (u^4 - u^2 + 1)                v = 2   w = 2

    // now choose the right values for x and y
    bint_condassign(bint_tmp[5], u_neg, bint_tmp[8], bint_tmp[5]);    // Sgn0(u) * sqrtCand     v = 2   w = 2
    bint_condassign(bint_tmp[5], x0_good, bint_tmp[5], bint_tmp[6]);  // y =  yu^3 if !x0_good  v = 2   w = 2
    bint_condassign(bint_tmp[2], x0_good, bint_tmp[2], bint_tmp[7]);  // x = -xu^2 if !x0_good  v = 2   w = 2

    // compute X, Y, Z
    bint_mul(jp_tmp[jp_num].X, bint_tmp[2], bint_tmp[1]);  // x = X / Z^2                       v = 2   w = 1
    bint_mul(bint_tmp[5], bint_tmp[5], bint_tmp[9]);       // y * Z^2                           v = 2   w = 1
    bint_mul(jp_tmp[jp_num].Y, bint_tmp[5], bint_tmp[1]);  // y = Y / Z^3                       v = 2   w = 1
    bint_redc(jp_tmp[jp_num].Z, bint_tmp[1]);              // Z
}

static inline void bint_horner(int64_t *out, const int64_t *x, const int startval) {
    for (int i = startval; i >= 0; --i) {
        bint_mul(out, out, x);            // tot *= x               v = 2   w = 1
        bint_add(out, out, bint_tmp[i]);  // tot += next_val        v = 4   w = 2
    }
}

static inline void compute_map_bint(int64_t inv[][BINT_NWORDS], int64_t zv[][BINT_NWORDS], const unsigned len) {
    for (unsigned i = 0; i < len; ++i) {
        bint_mul(bint_tmp[i], inv[i], zv[i]);
    }
}

// bint-based constant-time 11-isogeny impl
static inline void eval_iso11_ct(void) {
    // precompute even powers of Z up to Z^30
    bint_sqr(bint_tmp[31], jp_tmp[1].Z);                 // Z^2
    bint_sqr(bint_tmp[30], bint_tmp[31]);                // Z^4
    bint_mul(bint_tmp[29], bint_tmp[30], bint_tmp[31]);  // Z^6
    bint_sqr(bint_tmp[28], bint_tmp[30]);                // Z^8
    for (unsigned i = 0; i < 3; ++i) {
        bint_mul(bint_tmp[27 - i], bint_tmp[28 - i], bint_tmp[31]);  // Z^10, Z^12, Z^14
    }
    bint_sqr(bint_tmp[24], bint_tmp[28]);  // Z^16
    for (unsigned i = 0; i < 7; ++i) {
        bint_mul(bint_tmp[23 - i], bint_tmp[24 - i], bint_tmp[31]);  // Z^18, ..., Z^30
    }

    // Ymap denominator
    compute_map_bint(bint_yden, bint_tmp + 17, ELLP_YMAP_DEN_LEN);         // k_(15-i) Z^(2i)
    bint_add(bint_tmp[16], jp_tmp[1].X, bint_tmp[ELLP_YMAP_DEN_LEN - 1]);  // X + k_14 Z^2 (denom is monic)
    bint_horner(bint_tmp[16], jp_tmp[1].X, ELLP_YMAP_DEN_LEN - 2);         // Horner for rest
    bint_mul(bint_tmp[15], bint_tmp[16], bint_tmp[31]);                    // Yden * Z^2
    bint_mul(bint_tmp[15], bint_tmp[15], jp_tmp[1].Z);                     // Yden * Z^3

    // Ymap numerator
    compute_map_bint(bint_ynum, bint_tmp + 17, ELLP_YMAP_NUM_LEN - 1);      // k_(15-i) Z^(2i)
    bint_mul(bint_tmp[16], jp_tmp[1].X, bint_ynum[ELLP_YMAP_NUM_LEN - 1]);  // k_15 * X
    bint_add(bint_tmp[16], bint_tmp[16], bint_tmp[ELLP_YMAP_NUM_LEN - 2]);  // k_15 * X + k_14 Z^2
    bint_horner(bint_tmp[16], jp_tmp[1].X, ELLP_YMAP_NUM_LEN - 3);          // Horner for rest
    bint_mul(bint_tmp[16], bint_tmp[16], jp_tmp[1].Y);                      // Ynum * Y
    // at this point, ymap num/den are in bint_tmp[16]/bint_tmp[15]

    // Xmap denominator
    compute_map_bint(bint_xden, bint_tmp + 22, ELLP_XMAP_DEN_LEN);         // k_(10-i) Z^(2i)
    bint_add(bint_tmp[14], jp_tmp[1].X, bint_tmp[ELLP_XMAP_DEN_LEN - 1]);  // X + k_9 Z^2 (denom is monic)
    bint_horner(bint_tmp[14], jp_tmp[1].X, ELLP_XMAP_DEN_LEN - 2);         // Horner for rest
    // mul by Z^2 because numerator has degree one greater than denominator
    bint_mul(bint_tmp[14], bint_tmp[14], bint_tmp[31]);

    // Xmap numerator
    compute_map_bint(bint_xnum, bint_tmp + 21, ELLP_XMAP_NUM_LEN - 1);      // k_(11-i) Z^(2i)
    bint_mul(bint_tmp[13], jp_tmp[1].X, bint_xnum[ELLP_XMAP_NUM_LEN - 1]);  // k_11 * X
    bint_add(bint_tmp[13], bint_tmp[13], bint_tmp[ELLP_XMAP_NUM_LEN - 2]);  // k_11 * X + k_10 * Z^2
    bint_horner(bint_tmp[13], jp_tmp[1].X, ELLP_XMAP_NUM_LEN - 3);          // Horner for rest

    // at this point, xmap num/den are in bint_tmp[13]/bint_tmp[14]
    // now compute Jacobian projective coordinates
    bint_mul(jp_tmp[1].Z, bint_tmp[14], bint_tmp[15]);  // Zout = Xden Yden
    bint_mul(jp_tmp[1].X, bint_tmp[13], bint_tmp[15]);  // Xnum Yden
    bint_mul(jp_tmp[1].X, jp_tmp[1].X, jp_tmp[1].Z);    // Xnum Xden Yden^2 = Xout => Xout / Zout^2 = Xnum / Xden
    bint_sqr(bint_tmp[12], jp_tmp[1].Z);                // Zout^2
    bint_mul(jp_tmp[1].Y, bint_tmp[16], bint_tmp[14]);  // Ynum Xden
    bint_mul(jp_tmp[1].Y, jp_tmp[1].Y, bint_tmp[12]);   // Ynum Xden Zout^2 = Yout => Yout / Zout^3 = Ynum / Yden
}

// evaluate the SWU map once, apply isogeny map, and clear cofactor
void swu_map(mpz_t x, mpz_t y, mpz_t z, const mpz_t u, const bool constant_time) {
    if (constant_time) {
        swu_help_ct(1, u);
        eval_iso11_ct();
    } else {
        swu_help(1, u);
        eval_iso11();
    }

    clear_h_chain();
    from_jac_point(x, y, z, jp_tmp + 7);
}

// evaluate the SWU map twice, add results together, apply isogeny map, clear cofactor
void swu_map2(mpz_t x, mpz_t y, mpz_t z, const mpz_t u1, const mpz_t u2, const bool constant_time) {
    if (constant_time) {
        swu_help_ct(0, u1);
        swu_help_ct(1, u2);
    } else {
        swu_help(0, u1);
        swu_help(1, u2);
    }

    point_add(jp_tmp + 1, jp_tmp, jp_tmp + 1);

    if (constant_time) {
        eval_iso11_ct();
    } else {
        eval_iso11();
    }

    clear_h_chain();
    from_jac_point(x, y, z, jp_tmp + 7);
}

// evalute the SWU map once, apply isogeny map, and clear cofactor while adding a random point in subgroup
void swu_map_rG(mpz_t x, mpz_t y, mpz_t z, const mpz_t u, const uint8_t *r) {
    swu_help(1, u);  // evaluate SWU map...
    eval_iso11();    // ...and isogeny

    // precompute values for the multi-point mult table
    memcpy(&bint_precomp[1][0][0], jp_tmp + 1, sizeof(jac_point));
    precomp_finish();

    addrG_clear_h_help(r);            // do the multi-point multiplication
    from_jac_point(x, y, z, jp_tmp);  // convert the result
}
