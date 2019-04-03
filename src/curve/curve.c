// curve ops for bls12-381 hashing
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve.h"

#include "bint.h"
#include "bls12_381_consts.h"
#include "iso_params.h"

#include <string.h>

// ******************************
// static data setup and teardown
// ******************************
// init an mpz_t and set it from a constant defined in bls12_381_consts.h
static inline void mpz_init_import(mpz_t out, const uint64_t *in) {
    mpz_init(out);
    mpz_import(out, 6, -1, 8, 0, 0, in);
}

// temporary bigints
#define NUM_TMP_BINT 32
static int64_t bint_tmp[NUM_TMP_BINT][BINT_NWORDS];

// initialize the temporary variables and constants uesd in this file
#define NUM_TMP_MPZ 33  // NOTE: needs to be at least 3 + ELLP_YMAP_DEN_LEN + ELLP_YMAP_NUM_LEN
static mpz_t cx1, cx2, sqrtM27, invM27, mpz_tmp[NUM_TMP_MPZ], fld_p, pp1o4, pm3o4;
static mpz_t ellp_a, ellp_b, pm2, pm1o2;
static mpz_t xmap_num[ELLP_XMAP_NUM_LEN], xmap_den[ELLP_XMAP_DEN_LEN];
static mpz_t ymap_num[ELLP_YMAP_NUM_LEN], ymap_den[ELLP_YMAP_DEN_LEN];
static int64_t bint_ellp_b[BINT_NWORDS], bint_ellp_a[BINT_NWORDS], bint_one[BINT_NWORDS];
static int64_t bint_xnum[ELLP_XMAP_NUM_LEN][BINT_NWORDS], bint_xden[ELLP_XMAP_DEN_LEN][BINT_NWORDS];
static int64_t bint_ynum[ELLP_YMAP_NUM_LEN][BINT_NWORDS], bint_yden[ELLP_YMAP_DEN_LEN][BINT_NWORDS];
static int64_t bint_cx1[BINT_NWORDS], bint_cx2[BINT_NWORDS], bint_sqrtM27[BINT_NWORDS];
static int64_t bint_23[BINT_NWORDS], bint_M27[BINT_NWORDS], bint_81[BINT_NWORDS];
static bool init_done = false;
void curve_init(void) {
    if (!init_done) {
        init_done = true;

        // p, (p+1)/4, (p+1)/2
        mpz_init(fld_p);
        mpz_init(pp1o4);
        mpz_init(pm3o4);
        mpz_init(pm2);
        mpz_init(pm1o2);
        mpz_import(fld_p, P_LEN, 1, 1, 1, 0, BLS12_381_p);
        mpz_add_ui(pp1o4, fld_p, 1);       // p+1
        mpz_fdiv_q_2exp(pp1o4, pp1o4, 2);  // (p+1)/4  (for square root)
        mpz_sub_ui(pm3o4, fld_p, 3);       // p-3
        mpz_fdiv_q_2exp(pm3o4, pm3o4, 2);  // (p-3)/4  (for simultaneous invert--square root)
        mpz_sub_ui(pm2, fld_p, 2);         // p-2
        mpz_sub_ui(pm1o2, fld_p, 1);       // p-1
        mpz_fdiv_q_2exp(pm1o2, pm1o2, 1);  // (p-1)/2

        // SvdW constants
        mpz_init_import(cx1, Icx1);
        bint_import_mpz(bint_cx1, cx1);
        mpz_init_import(cx2, Icx2);
        bint_import_mpz(bint_cx2, cx2);
        mpz_init_import(sqrtM27, IsqrtM27);
        bint_import_mpz(bint_sqrtM27, sqrtM27);
        mpz_init_import(invM27, IinvM27);

        // 11-isogeny constants
        mpz_init_import(ellp_a, ELLP_a);
        mpz_init_import(ellp_b, ELLP_b);
        for (unsigned i = 0; i < ELLP_XMAP_NUM_LEN; ++i) {
            mpz_init_import(xmap_num[i], ELLP_XMAP_NUM[i]);
            bint_import_mpz(bint_xnum[i], xmap_num[i]);
        }
        for (unsigned i = 0; i < ELLP_XMAP_DEN_LEN; ++i) {
            mpz_init_import(xmap_den[i], ELLP_XMAP_DEN[i]);
            bint_import_mpz(bint_xden[i], xmap_den[i]);
        }
        for (unsigned i = 0; i < ELLP_YMAP_NUM_LEN; ++i) {
            mpz_init_import(ymap_num[i], ELLP_YMAP_NUM[i]);
            bint_import_mpz(bint_ynum[i], ymap_num[i]);
        }
        for (unsigned i = 0; i < ELLP_YMAP_DEN_LEN; ++i) {
            mpz_init_import(ymap_den[i], ELLP_YMAP_DEN[i]);
            bint_import_mpz(bint_yden[i], ymap_den[i]);
        }

        // temp variables
        for (unsigned i = 0; i < NUM_TMP_MPZ; ++i) {
            mpz_init(mpz_tmp[i]);
        }

        // other bint temps
        bint_import_mpz(bint_ellp_a, ellp_a);
        bint_import_mpz(bint_ellp_b, ellp_b);
        bint_set1(bint_one);
        mpz_sub_ui(mpz_tmp[0], fld_p, 27);
        bint_import_mpz(bint_M27, mpz_tmp[0]);
        mpz_set_ui(mpz_tmp[0], 23);
        bint_import_mpz(bint_23, mpz_tmp[0]);
        mpz_set_ui(mpz_tmp[0], 81);
        bint_import_mpz(bint_81, mpz_tmp[0]);
    }
}

// uninitialize temporary variables and constants
void curve_uninit(void) {
    if (init_done) {
        init_done = false;

        // p, (p+1)/4, (p+1)/2
        mpz_clear(fld_p);
        mpz_clear(pp1o4);
        mpz_clear(pm3o4);
        mpz_clear(pm2);
        mpz_clear(pm1o2);

        // SvdW constants
        mpz_clear(cx1);
        mpz_clear(cx2);
        mpz_clear(sqrtM27);
        mpz_clear(invM27);

        // 11-isogeny constants
        mpz_clear(ellp_a);
        mpz_clear(ellp_b);
        for (unsigned i = 0; i < ELLP_XMAP_NUM_LEN; ++i) {
            mpz_clear(xmap_num[i]);
        }
        for (unsigned i = 0; i < ELLP_XMAP_DEN_LEN; ++i) {
            mpz_clear(xmap_den[i]);
        }
        for (unsigned i = 0; i < ELLP_YMAP_NUM_LEN; ++i) {
            mpz_clear(ymap_num[i]);
        }
        for (unsigned i = 0; i < ELLP_YMAP_DEN_LEN; ++i) {
            mpz_clear(ymap_den[i]);
        }

        // temp variables
        for (unsigned i = 0; i < NUM_TMP_MPZ; ++i) {
            mpz_clear(mpz_tmp[i]);
        }
    }
}

// ********************
// arithmetic utilities
// ********************

// in ^ 2 mod p
static inline void sqr_modp(mpz_t out, const mpz_t in) {
    mpz_mul(out, in, in);
    mpz_mod(out, out, fld_p);
}

// in1 * in2 mod p
static inline void mul_modp(mpz_t out, const mpz_t in1, const mpz_t in2) {
    mpz_mul(out, in1, in2);
    mpz_mod(out, out, fld_p);
}

// sqrt(U/V) ; return whether we actually found a sqrt
// out is the result, tmp is garbage
static inline bool divsqrt(mpz_t out, mpz_t tmp, const mpz_t u, const mpz_t v, bool force) {
    sqr_modp(out, v);                  // V^2
    mul_modp(tmp, u, v);               // UV
    mul_modp(out, out, tmp);           // UV^3
    mpz_powm(out, out, pm3o4, fld_p);  // (UV^3)^((p-3)/4)
    mul_modp(out, out, tmp);           // UV(UV^3)^((p-3)/4)

    if (!force) {
        sqr_modp(tmp, out);     // out^2
        mul_modp(tmp, tmp, v);  // out^2 * V
        mpz_sub(tmp, tmp, u);   // out^2 * V - U
        return mpz_divisible_p(tmp, fld_p);
    }
    return true;
}

// check if x is a point on the curve; if so, compute the corresponding y-coord with given sign
// This is used by the SvdW map and by hash_and_check.
bool check_fx(mpz_t y, const mpz_t x, const bool negate, const bool force, const bool field_only) {
    sqr_modp(mpz_tmp[10], x);                 // x^2
    mul_modp(mpz_tmp[10], mpz_tmp[10], x);    // x^3
    mpz_add_ui(mpz_tmp[10], mpz_tmp[10], 4);  // x^3 + 4

    // non-field-only case: if not forcing, check Legendre symbol
    if (!field_only && !force && mpz_legendre(mpz_tmp[10], fld_p) != 1) {
        // f(x) is not a residue
        return false;
    }

    // compute sqrt of f(x)
    mpz_powm(y, mpz_tmp[10], pp1o4, fld_p);

    // field-only case: if not forcing, square and compare
    if (field_only && !force) {
        sqr_modp(mpz_tmp[11], y);
        mpz_sub(mpz_tmp[11], mpz_tmp[11], mpz_tmp[10]);
        if (!mpz_divisible_p(mpz_tmp[11], fld_p)) {  // did we actually find a sqrt?
            return false;
        }
    }

    // fix up sign of y
    if (negate) {
        mpz_sub(y, fld_p, y);
    }
    return true;
}

// modular reduction when we know that -p <= in < p
static inline void condadd_p(mpz_t in) {
    if (mpz_cmp_ui(in, 0) < 0) {
        mpz_add(in, in, fld_p);
    }
}

// modular reduction when we know that 0 <= in < 2p
static inline void condsub_p(mpz_t in) {
    if (mpz_cmp(in, fld_p) >= 0) {
        mpz_sub(in, in, fld_p);
    }
}

// *********************
// SvdW map to BLS12-381
// *********************
// Map to curve given by
//   Shallue and van de Woestijne, "Construction of rational points on elliptic curves over finite fields."
//   Proc. ANTS 2006. https://works.bepress.com/andrew_shallue/1/
//
// Derivation follows the one from
//   Fouque and Tibouchi, "Indifferentiable hashing to Barreto-Naehrig curves."
//   Proc. LATINCRYPT 2012. https://link.springer.com/chapter/10.1007/978-3-642-33481-8_1
//
// See the paper/ subdir for a complete worked derivation.
//
// NOTE: t must be reduced mod p!
// t == x is OK, but x and y must be distinct
// compute the map after precomp and inversion
static inline void svdw_map_help(mpz_t x, mpz_t y, const bool neg_t, const unsigned tmp_offset) {
    // x1
    mpz_add(x, cx1, mpz_tmp[tmp_offset]);  // (3 - sqrt(-27))/2 + t^2 * sqrt(-27) / (23 - t^2)
    if (check_fx(y, x, neg_t, false, false)) {
        condsub_p(x);  // reduce x mod p (mpz_tmp[tmp_offset] and cx1 were both reduced, so x < 2p)
        return;
    }

    // x2
    mpz_sub(x, cx2, mpz_tmp[tmp_offset]);  // (3 - sqrt(-27))/2 - t^2 * sqrt(-27) / (23 - t^2)
    if (check_fx(y, x, neg_t, false, false)) {
        condadd_p(x);  // reduce x mod p (mpz_tmp[tmp_offset] and cx2 were both reduced, so x > -p)
        return;
    }

    // x3
    sqr_modp(x, mpz_tmp[tmp_offset + 1]);     // (23 - t^2)^2
    mul_modp(x, x, mpz_tmp[tmp_offset + 1]);  // (23 - t^2)^3
    mul_modp(x, x, mpz_tmp[tmp_offset + 2]);  // (23 - t^2)^2 / t^2
    mul_modp(x, x, invM27);                   // - (23 - t^2)^2 / (27 * t^2)
    mpz_sub_ui(x, x, 3);                      // -3 - (23 - t^2)^2 / (27 * t^2)
    condadd_p(x);                             // reduce x mod p (subtracted p from a reduced value, so x >= -3)
    check_fx(y, x, neg_t, true, false);
}

// pre-inversion precomp: input to inverse
static inline void svdw_precomp1(const mpz_t t, const unsigned tmp_offset) {
    sqr_modp(mpz_tmp[tmp_offset], t);                                                 // t^2
    mpz_ui_sub(mpz_tmp[tmp_offset + 1], 23, mpz_tmp[tmp_offset]);                     // 23 - t^2
    mul_modp(mpz_tmp[tmp_offset + 2], mpz_tmp[tmp_offset + 1], mpz_tmp[tmp_offset]);  // t^2 * (23 - t^2)
}

// post-inversion precomp: compute one addend of x1 and x2
static inline void svdw_precomp2(const unsigned tmp_offset) {
    sqr_modp(mpz_tmp[tmp_offset], mpz_tmp[tmp_offset]);                           // t^4
    mul_modp(mpz_tmp[tmp_offset], mpz_tmp[tmp_offset], mpz_tmp[tmp_offset + 2]);  // t^2 / (23 - t^2)
    mul_modp(mpz_tmp[tmp_offset], mpz_tmp[tmp_offset], sqrtM27);                  // t^2 sqrt(-27) / (23 - t^2)
}

// apply the SvdW map to input t
void svdw_map(mpz_t x, mpz_t y, const mpz_t t) {
    svdw_precomp1(t, 0);  // compute input to inversion in mpz_tmp[2]
    if (mpz_cmp_ui(mpz_tmp[2], 0) != 0) {
        mpz_invert(mpz_tmp[2], mpz_tmp[2], fld_p);  // invert if nonzero
    }
    svdw_precomp2(0);                          // compute non-constant part of x1 and x2 in tmp0
    const bool neg_t = mpz_cmp(pm1o2, t) < 0;  // true (negative) when t > (p-1)/2
    svdw_map_help(x, y, neg_t, 0);             // finish computing the map
}

// Apply the SvdW map to two points simultaneously
// This saves an inversion vs two calls to svdw_map().
void svdw_map2(mpz_t x1, mpz_t y1, const mpz_t t1, mpz_t x2, mpz_t y2, const mpz_t t2) {
    svdw_precomp1(t1, 0);  // inversion input for t1
    svdw_precomp1(t2, 3);  // inversion input for t2

    // invert one or both of mpz_tmp[2] and mpz_tmp[5]
    const bool p10 = mpz_cmp_ui(mpz_tmp[2], 0) == 0;  // t1^2 * (23 - t1^2) != 0
    const bool p20 = mpz_cmp_ui(mpz_tmp[5], 0) == 0;  // t2^2 * (23 - t2^2) != 0
    if (p10 && !p20) {
        mpz_invert(mpz_tmp[5], mpz_tmp[5], fld_p);  // (t2^2 * (23 - t2^2)) ^ -1
    } else if (!p10 && p20) {
        mpz_invert(mpz_tmp[2], mpz_tmp[2], fld_p);  // (t1^2 * (23 - t1^2)) ^ -1
    } else if (!p10 && !p20) {
        mul_modp(mpz_tmp[6], mpz_tmp[5], mpz_tmp[2]);  // (t1^2 * (23 - t1^2) * t2^2 * (23 - t2^2))
        mpz_invert(mpz_tmp[6], mpz_tmp[6], fld_p);     // (t1^2 * (23 - t1^2) * t2^2 * (23 - t2^2)) ^ -1
        mul_modp(mpz_tmp[5], mpz_tmp[5], mpz_tmp[6]);  // (t1^2 * (23 - t1^2)) ^ -1
        mul_modp(mpz_tmp[2], mpz_tmp[2], mpz_tmp[6]);  // (t2^2 * (23 - t2^2)) ^ -1
        mpz_swap(mpz_tmp[2], mpz_tmp[5]);              // [2] should hold t1 val, [5] should hold t2 val
    }

    svdw_precomp2(0);  // non-constant part of x11 and x12
    svdw_precomp2(3);  // non-constant part of x21 and x22

    const bool neg_t1 = mpz_cmp(pm1o2, t1) < 0;  // t1 negative
    svdw_map_help(x1, y1, neg_t1, 0);            // finish computing the first map
    const bool neg_t2 = mpz_cmp(pm1o2, t2) < 0;  // t2 negative
    svdw_map_help(x2, y2, neg_t2, 3);            // finish computing the second map
}

// **
// ** the following two functions implement the SvdW map using only field operations
// **
// helper for svdw_map_fo: try sqrt of f(x/z), converting to projective if we found one
static inline bool check_fxOverZ(mpz_t x, mpz_t y, mpz_t z, const bool negate, const bool force) {
    sqr_modp(mpz_tmp[2], x);                      // x^2
    mul_modp(mpz_tmp[2], mpz_tmp[2], x);          // x^3
    sqr_modp(mpz_tmp[5], z);                      // z^2
    mul_modp(mpz_tmp[3], mpz_tmp[5], z);          // z^3
    mpz_mul_2exp(mpz_tmp[4], mpz_tmp[3], 2);      // 4 z^3
    mpz_add(mpz_tmp[2], mpz_tmp[2], mpz_tmp[4]);  // x^3 + 4 z^3
    if (divsqrt(y, mpz_tmp[4], mpz_tmp[2], mpz_tmp[3], force)) {
        mul_modp(x, x, z);           // X = x z
        mul_modp(y, y, mpz_tmp[5]);  // Y = y z^2
        mul_modp(y, y, z);           // Y = y z^3
        mpz_mod(z, z, fld_p);        // reduce z
        if (negate) {
            mpz_sub(y, fld_p, y);  // fix sign of y
        }
        return true;
    }
    return false;
}

// svdw map using field ops only
void svdw_map_fo(mpz_t x, mpz_t y, mpz_t z, const mpz_t t) {
    const bool neg_t = mpz_cmp(pm1o2, t) < 0;  // true (negative) when t >= (p+1)/2

    sqr_modp(mpz_tmp[0], t);                    // t^2
    mpz_ui_sub(z, 23, mpz_tmp[0]);              // 23 - t^2               = V
    mul_modp(mpz_tmp[1], mpz_tmp[0], sqrtM27);  // t^2 * sqrt(-27)

    // x1 : (cx1 * (23 - t^2) + t^2 * sqrt(-27)) / (23 - t^2)
    mul_modp(x, cx1, z);        // cx1 * (23 - t^2)
    mpz_add(x, x, mpz_tmp[1]);  // cx1 * (23 - t^2) + t^2 * sqrt(-27)     = U
    if (check_fxOverZ(x, y, z, neg_t, false)) {
        return;
    }

    // x2 : (cx2 * (23 - t^2) - t^2 * sqrt(-27)) / (23 - t^2)
    mul_modp(x, cx2, z);        // cx2 * (23 - t^2)
    mpz_sub(x, x, mpz_tmp[1]);  // cx2 * (23 - t^2) - t^2 * sqrt(-27)     = U
    if (check_fxOverZ(x, y, z, neg_t, false)) {
        return;
    }

    // x3 : ((23 - t^2)^2 + 81t^2) / (-27 t^2)
    sqr_modp(x, z);                      // (23 - t^2)^2
    mpz_mul_si(z, mpz_tmp[0], -27);      // -27 t^2                       = V
    mpz_mul_2exp(mpz_tmp[0], z, 1);      // -54 t^2
    mpz_add(mpz_tmp[0], mpz_tmp[0], z);  // -81 t^2
    mpz_sub(x, x, mpz_tmp[0]);           // (23 - t^2)^2 + 81t^2          = U
    check_fxOverZ(x, y, z, neg_t, true);
}

// f(x) helper for constant-time svdw map
static inline bool check_fxOverZ_ct(const unsigned x, const unsigned y, const unsigned z) {
    bint_sqr(bint_tmp[12], bint_tmp[x]);                 // x^2                                 v = 2   w = 1
    bint_mul(bint_tmp[12], bint_tmp[12], bint_tmp[x]);   // x^3                                 v = 2   w = 1
    bint_sqr(bint_tmp[15], bint_tmp[z]);                 // z^2                                 v = 2   w = 1
    bint_mul(bint_tmp[13], bint_tmp[15], bint_tmp[z]);   // z^3              (DEN)              v = 2   w = 1
    bint_lsh(bint_tmp[14], bint_tmp[13], 2);             // 4 z^3                               v = 8   w = 4
    bint_add(bint_tmp[12], bint_tmp[12], bint_tmp[14]);  // x^3 + 4 z^3      (NUM)              v = 10  w = 5
    return bint_divsqrt(bint_tmp[y], bint_tmp[12], bint_tmp[13], false);  // y = sqrt(NUM/DEN)  v = 2   w = 1
}

// svdw map that runs in constant time
void svdw_map_ct(mpz_t x, mpz_t y, mpz_t z, const mpz_t t) {
    bint_import_mpz(bint_tmp[0], t);
    const bool neg_t = bint_is_neg(bint_tmp[0]);

    bint_sqr(bint_tmp[0], bint_tmp[0]);                // t^2                                   v = 2   w = 1
    bint_sub(bint_tmp[2], bint_23, bint_tmp[0], 1);    // 23 - t^2                              v = 4   w = 3
    bint_mul(bint_tmp[1], bint_tmp[0], bint_sqrtM27);  // t^2 * sqrt(-27)                       v = 2   w = 1

    // x1: (cx1 * (23 - t^2) + t^2 * sqrt(-27)) / (23 - t^2)
    bint_mul(bint_tmp[3], bint_cx1, bint_tmp[2]);     // cx1 * (23 - t^2)                       v = 2   w = 1
    bint_add(bint_tmp[3], bint_tmp[3], bint_tmp[1]);  // cx1 * (23 - t^2) + t^2 * sqrt(-27)     v = 4   w = 2
    const bool x1g = check_fxOverZ_ct(3, 4, 2);       // bint_tmp[4] is y1                      v = 2   w = 1

    // x2: (cx2 * (23 - t^2) - t^2 * sqrt(-27)) / (23 - t^2)
    bint_mul(bint_tmp[5], bint_cx2, bint_tmp[2]);        // cx2 * (23 - t^2)
    bint_sub(bint_tmp[5], bint_tmp[5], bint_tmp[1], 1);  // cx2 * (23 - t^2) - t^2 * sqrt(-27)  v = 4   w = 3
    const bool x2g = check_fxOverZ_ct(5, 6, 2);          // bint_tmp[6] is y2                   v = 2   w = 1

    // select output from either x1 or x2
    bint_condassign(bint_tmp[10], x1g, bint_tmp[3], bint_tmp[5]);  // Xout = x1g ? x1 : x2      v = 4   w = 3
    bint_condassign(bint_tmp[11], x1g, bint_tmp[4], bint_tmp[6]);  // Yout = x1g ? y1 : y2      v = 2   w = 1
    const bool found = x1g | x2g;

    // x3 : ((23 - t^2)^2 + 81t^2) / (-27 t^2)
    bint_sqr(bint_tmp[7], bint_tmp[2]);                  // (23 - t^2)^2                        v = 2   w = 1
    bint_mul(bint_tmp[8], bint_tmp[0], bint_M27);        // -27 t^2                             v = 2   w = 1
    bint_lsh(bint_tmp[9], bint_tmp[8], 1);               // -54 t^2                             v = 4   w = 2
    bint_add(bint_tmp[9], bint_tmp[9], bint_tmp[8]);     // -81 t^2                             v = 6   w = 3
    bint_sub(bint_tmp[7], bint_tmp[7], bint_tmp[9], 3);  // (23 - t^2)^2 + 81 t^2               v = 10  w = 9
    check_fxOverZ_ct(7, 9, 8);                           // bint_tmp[9] is y3                   v = 2   w = 1

    // if we hadn't already found it, now we have
    bint_condassign(bint_tmp[10], found, bint_tmp[10], bint_tmp[7]);  // X'out
    bint_condassign(bint_tmp[11], found, bint_tmp[11], bint_tmp[9]);  // Y'out
    bint_condassign(bint_tmp[12], found, bint_tmp[2], bint_tmp[8]);   // Z'out

    // negate Y if necessary
    bint_neg(bint_tmp[5], bint_tmp[11], 1);                           // -Y                     v = 2   w = 2
    bint_condassign(bint_tmp[11], neg_t, bint_tmp[5], bint_tmp[11]);  // Y = neg_t ? -Y : Y     v = 2   w = 2

    // compute Jacobian coordinates
    bint_mul(bint_tmp[10], bint_tmp[10], bint_tmp[12]);  // Xout = x z                          v = 2   w = 1
    bint_sqr(bint_tmp[5], bint_tmp[12]);                 // Z'^2                                v = 2   w = 1
    bint_mul(bint_tmp[11], bint_tmp[11], bint_tmp[5]);   // y z^2                               v = 2   w = 1
    bint_mul(bint_tmp[11], bint_tmp[11], bint_tmp[12]);  // y z^3                               v = 2   w = 1

    // export to mpz_t
    bint_export_mpz(x, bint_tmp[10]);
    bint_export_mpz(y, bint_tmp[11]);
    bint_export_mpz(z, bint_tmp[12]);
}

// **************************
// BLS12-381 curve operations
// **************************
// Jacobian coordinates: x = X/Z^2, y = Y/Z^3
struct jac_point {
    int64_t X[BINT_NWORDS];
    int64_t Y[BINT_NWORDS];
    int64_t Z[BINT_NWORDS];
};

// double a point in Jacobian coordinates
// out == in is OK
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#doubling-dbl-2009-l
static inline void point_double(struct jac_point *out, const struct jac_point *in) {
    bint_sqr(bint_tmp[0], in->X);                        // A = X^2                      v = 2   w = 1
    bint_sqr(bint_tmp[1], in->Y);                        // B = Y^2                      v = 2   w = 1
    bint_sqr(bint_tmp[2], bint_tmp[1]);                  // C = B^2                      v = 2   w = 1
                                                         //
    bint_add(bint_tmp[3], in->X, bint_tmp[1]);           // X + B                        v = 4   w = 2
    bint_sqr(bint_tmp[3], bint_tmp[3]);                  // (X + B)^2                    v = 2   w = 1
    bint_add(bint_tmp[4], bint_tmp[0], bint_tmp[2]);     // (A + C)                      v = 4   w = 2
    bint_sub(bint_tmp[3], bint_tmp[3], bint_tmp[4], 2);  // (X + B)^2 - A - C            v = 6   w = 5
    bint_lsh(bint_tmp[3], bint_tmp[3], 1);               // D = 2 * ((X + B)^2 - A - C)  v = 12  w = 10
                                                         //
    bint_lsh(bint_tmp[4], bint_tmp[0], 1);               // 2 * A                        v = 4   w = 2
    bint_add(bint_tmp[4], bint_tmp[4], bint_tmp[0]);     // E = 3 * A                    v = 6   w = 3
                                                         //
    bint_sqr(bint_tmp[5], bint_tmp[4]);                  // F = E^2                      v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[6], bint_tmp[3], 1);               // 2 * D                        v = 24  w = 20
    bint_sub(bint_tmp[6], bint_tmp[5], bint_tmp[6], 5);  // F - 2 * D                    v = 34  w = 33
    bint_redc(out->X, bint_tmp[6]);                      // X3 = F - 2 * D               v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[6], in->Z, 1);                     // 2 * Z                        v = 4   w = 2
    bint_mul(out->Z, bint_tmp[6], in->Y);                // 2 * Z * Y                    v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[2], bint_tmp[2], 3);               // 8 * C                        v = 16  w = 8
    bint_sub(bint_tmp[6], bint_tmp[3], out->X, 1);       // D - X3                       v = 16  w = 14
    bint_mul(bint_tmp[6], bint_tmp[6], bint_tmp[4]);     // E * (D - X3)                 v = 2   w = 1
    bint_sub(bint_tmp[6], bint_tmp[6], bint_tmp[2], 4);  // E * (D - X3) - 8 * C         v = 18  w = 17
    bint_redc(out->Y, bint_tmp[6]);                      // Y = E * (D - X3) - 8 * C     v = 2   w = 1
}

// add two points in Jacobian coordinates
// out == in1 or out == in2 is OK
// NOTE: out->Y remains unreduced, but it meets numerical stability criteria
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#addition-add-2007-bl
static inline void point_add(struct jac_point *out, const struct jac_point *in1, const struct jac_point *in2) {
    bint_sqr(bint_tmp[0], in1->Z);                       // Z1Z1 = Z1^2                  v = 2   w = 1
                                                         //
    bint_sqr(bint_tmp[1], in2->Z);                       // Z2Z2 = Z2^2                  v = 2   w = 1
                                                         //
    bint_mul(bint_tmp[2], bint_tmp[1], in1->X);          // U1 = X1 * Z2Z2               v = 2   w = 1
    bint_mul(bint_tmp[3], bint_tmp[0], in2->X);          // U2 = X2 * Z1Z1               v = 2   w = 1
                                                         //
    bint_mul(bint_tmp[4], in1->Y, in2->Z);               // Y1 * Z2                      v = 2   w = 1
    bint_mul(bint_tmp[4], bint_tmp[4], bint_tmp[1]);     // S1 = Y1 * Z2 * Z2Z2          v = 2   w = 1
                                                         //
    bint_mul(bint_tmp[5], in2->Y, in1->Z);               // Y2 * Z1                      v = 2   w = 1
    bint_mul(bint_tmp[5], bint_tmp[5], bint_tmp[0]);     // S2 = Y2 * Z1 * Z1Z1          v = 2   w = 1
                                                         //
    bint_sub(bint_tmp[6], bint_tmp[3], bint_tmp[2], 1);  // H = U2 - U1                  v = 4   w = 3
                                                         //
    bint_lsh(bint_tmp[7], bint_tmp[6], 1);               // 2 * H                        v = 8   w = 6
    bint_sqr(bint_tmp[7], bint_tmp[7]);                  // I = (2 * H)^2                v = 2   w = 1
                                                         //
    bint_mul(bint_tmp[8], bint_tmp[7], bint_tmp[6]);     // J = H * I                    v = 2   w = 1
                                                         //
    bint_sub(bint_tmp[9], bint_tmp[5], bint_tmp[4], 1);  // S2 - S1                      v = 4   w = 3
    bint_lsh(bint_tmp[9], bint_tmp[9], 1);               // r = 2 * (S2 - S1)            v = 8   w = 6
                                                         //
    bint_mul(bint_tmp[10], bint_tmp[2], bint_tmp[7]);    // V = U1 * I                   v = 2   w = 1
                                                         //
    bint_lsh(out->X, bint_tmp[10], 1);                   // 2 * V                        v = 4   w = 2
    bint_add(out->X, out->X, bint_tmp[8]);               // J + 2 * V                    v = 6   w = 3
    bint_sqr(bint_tmp[7], bint_tmp[9]);                  // r^2                          v = 2   w = 1
    bint_sub(out->X, bint_tmp[7], out->X, 3);            // r^2 - J - 2 * V              v = 10  w = 9
    bint_redc(out->X, out->X);                           // X3 = r^2 - J - 2 * V         v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[4], bint_tmp[4], 1);               // 2 * S1                       v = 4   w = 2
    bint_mul(bint_tmp[4], bint_tmp[4], bint_tmp[8]);     // 2 * S1 * J                   v = 2   w = 1
    bint_sub(out->Y, bint_tmp[10], out->X, 1);           // V - X3                       v = 4   w = 3
    bint_mul(out->Y, out->Y, bint_tmp[9]);               // r * (V - X3)                 v = 2   w = 1
    bint_sub(out->Y, out->Y, bint_tmp[4], 1);            // r * (V - X3) - 2 * S1 * J    v = 4   w = 3
                                                         //
    bint_add(out->Z, in1->Z, in2->Z);                    // Z1 + Z2                      v = 4   w = 2
    bint_sqr(out->Z, out->Z);                            // (Z1 + Z2)^2                  v = 2   w = 1
    bint_add(bint_tmp[0], bint_tmp[0], bint_tmp[1]);     // Z1Z1 + Z2Z2                  v = 4   w = 2
    bint_sub(out->Z, out->Z, bint_tmp[0], 2);            // (Z1 + Z2)^2 - Z1Z1 - Z2Z2    v = 6   w = 5
    bint_mul(out->Z, out->Z, bint_tmp[6]);               // Z3 = 2 * Z1 * Z2 * H         v = 2   w = 1
}

// temporary points for intermediate computations (mostly used in clear_h_chain())
#define NUM_TMP_JP 8
static struct jac_point jp_tmp[NUM_TMP_JP];
// Addition chain: Bos-Coster (win=7) : 147 links, 8 variables
// input point is taken from jp_tmp[1], output is in jp_tmp[7]
// TODO(rsw): is there a faster addition-subtraction chain?
static inline void clear_h_chain(void) {
    point_double(jp_tmp + 3, jp_tmp + 1);
    point_add(jp_tmp + 2, jp_tmp + 3, jp_tmp + 1);
    point_double(jp_tmp + 5, jp_tmp + 3);
    point_double(jp_tmp + 4, jp_tmp + 5);
    point_add(jp_tmp + 1, jp_tmp + 4, jp_tmp + 2);
    point_double(jp_tmp + 7, jp_tmp + 4);
    point_add(jp_tmp, jp_tmp + 7, jp_tmp + 3);
    point_add(jp_tmp + 6, jp_tmp + 7, jp_tmp + 5);
    point_double(jp_tmp + 7, jp_tmp);
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 6);
    point_add(jp_tmp, jp_tmp + 7, jp_tmp);
    point_add(jp_tmp + 1, jp_tmp, jp_tmp + 1);
    point_add(jp_tmp + 5, jp_tmp + 1, jp_tmp + 5);
    point_add(jp_tmp + 4, jp_tmp + 5, jp_tmp + 4);
    point_add(jp_tmp, jp_tmp + 4, jp_tmp);
    point_add(jp_tmp + 3, jp_tmp, jp_tmp + 3);
    point_add(jp_tmp + 7, jp_tmp + 3, jp_tmp + 7);
    for (int nops = 0; nops < 7; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 5);
    for (int nops = 0; nops < 5; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 2);
    for (int nops = 0; nops < 18; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 1);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp);
    for (int nops = 0; nops < 7; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 4);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 3);
    for (int nops = 0; nops < 5; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 2);
    for (int nops = 0; nops < 17; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 1);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp);
    for (int nops = 0; nops < 23; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 1);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp);
}

// helper: convert from a jac_point to a pair of mpz_t
static inline void from_jac_point(mpz_t X, mpz_t Y, mpz_t Z, const struct jac_point *jp) {
    // convert from bint to gmp
    bint_export_mpz(X, jp->X);
    bint_export_mpz(Y, jp->Y);
    bint_export_mpz(Z, jp->Z);
}

// helper: convert from a pair of mpz_t to a jac_point
static inline void to_jac_point(struct jac_point *jp, const mpz_t X, const mpz_t Y, const mpz_t Z) {
    bint_import_mpz(jp->X, X);
    bint_import_mpz(jp->Y, Y);
    bint_import_mpz(jp->Z, Z);
}

// clear BLS12-381 cofactor
void clear_h(mpz_t X, mpz_t Y, mpz_t Z) {
    to_jac_point(jp_tmp + 1, X, Y, Z);
    clear_h_chain();
    from_jac_point(X, Y, Z, jp_tmp + 7);
}

// add two points together, leaving result in jp_tmp[1]
static inline void add2_help(const mpz_t X1, const mpz_t Y1, const mpz_t Z1, const mpz_t X2, const mpz_t Y2,
                             const mpz_t Z2) {
    to_jac_point(jp_tmp, X1, Y1, Z1);
    to_jac_point(jp_tmp + 1, X2, Y2, Z2);
    point_add(jp_tmp + 1, jp_tmp + 1, jp_tmp);
}

// add 2 points together; don't clear cofactor
void add2(mpz_t X1, mpz_t Y1, mpz_t Z1, const mpz_t X2, const mpz_t Y2, const mpz_t Z2) {
    add2_help(X1, Y1, Z1, X2, Y2, Z2);
    from_jac_point(X1, Y1, Z1, jp_tmp + 1);
}

// add 2 points together, then clear cofactor
void add2_clear_h(mpz_t X1, mpz_t Y1, mpz_t Z1, const mpz_t X2, const mpz_t Y2, const mpz_t Z2) {
    add2_help(X1, Y1, Z1, X2, Y2, Z2);
    clear_h_chain();
    from_jac_point(X1, Y1, Z1, jp_tmp + 7);
}

// precompute the fixed part of the table (based on G' and 2^128 * G') for addrG
static struct jac_point bint_precomp[4][4][4];
void precomp_init(void) {
    memcpy(bint_precomp[0][0][1].X, g_prime_x, sizeof(g_prime_x));
    memcpy(bint_precomp[0][0][1].Y, g_prime_y, sizeof(g_prime_y));
    bint_set1(bint_precomp[0][0][1].Z);
    point_double(&bint_precomp[0][0][2], &bint_precomp[0][0][1]);
    point_add(&bint_precomp[0][0][3], &bint_precomp[0][0][2], &bint_precomp[0][0][1]);

    memcpy(bint_precomp[0][1][0].X, g_prime_ll128_x, sizeof(g_prime_ll128_x));
    memcpy(bint_precomp[0][1][0].Y, g_prime_ll128_y, sizeof(g_prime_ll128_y));
    bint_set1(bint_precomp[0][1][0].Z);
    point_double(&bint_precomp[0][2][0], &bint_precomp[0][1][0]);
    point_add(&bint_precomp[0][3][0], &bint_precomp[0][2][0], &bint_precomp[0][1][0]);

    for (unsigned i = 1; i < 4; ++i) {
        for (unsigned j = 1; j < 4; ++j) {
            point_add(&bint_precomp[0][i][j], &bint_precomp[0][i][0], &bint_precomp[0][0][j]);
        }
    }
}

// precompute the part of the addrG table that involves the input point
static inline void precomp_finish(void) {
    // precondition: bint_precomp[1][0][0] is the point we need to fill the table with
    point_double(&bint_precomp[2][0][0], &bint_precomp[1][0][0]);
    point_add(&bint_precomp[3][0][0], &bint_precomp[2][0][0], &bint_precomp[1][0][0]);

    for (unsigned i = 1; i < 4; ++i) {
        for (unsigned j = 0; j < 4; ++j) {
            for (unsigned k = 0; k < 4; ++k) {
                if (j == 0 && k == 0) {
                    continue;
                }
                point_add(&bint_precomp[i][j][k], &bint_precomp[i][0][0], &bint_precomp[0][j][k]);
            }
        }
    }
}

// this is a 3-point multiplication
//     point 1 is h * (X, Y)
//     point 2 is r1 * G'
//     point 3 is r2 * 2^128 * G'
// where h is the cofactor, G' is an element of the order-q subgroup, and
//     r = r2 * 2^128 + r1
// is a random element of Zq. h is 126 bits, so splitting r saves doublings.
//
// NOTE this function is not constant time, and it leaks bits of r through memory accesses
// TODO(rsw): signed exponent recoding?
static inline void addrG_clear_h_help(const uint8_t *r) {
    const uint8_t *r2 = r + 16;
    bool is_zero = true;
    for (unsigned idx = 0; idx < 16; ++idx) {
        for (uint8_t mask = 3 << 6, shift = 6; mask != 0; mask = mask >> 2, shift -= 2) {
            // double if the point is not zero
            if (!is_zero) {
                point_double(jp_tmp, jp_tmp);
                point_double(jp_tmp, jp_tmp);
            }

            uint8_t h_idx = (BLS12_381_h[idx] & mask) >> shift;
            uint8_t r_idx = (r[idx] & mask) >> shift;
            uint8_t r2_idx = (r2[idx] & mask) >> shift;
            if ((h_idx | r_idx | r2_idx) != 0) {
                struct jac_point *tp = &bint_precomp[h_idx][r_idx][r2_idx];
                if (is_zero) {
                    is_zero = false;
                    memcpy(jp_tmp, tp, sizeof(struct jac_point));
                } else {
                    point_add(jp_tmp, jp_tmp, tp);
                }
            }
        }
    }
}

// compute h*(inX, inY) + r*gPrime via multi-point multiplication
void addrG_clear_h(mpz_t X, mpz_t Y, mpz_t Z, const uint8_t *r) {
    to_jac_point(&bint_precomp[1][0][0], X, Y, Z);  // convert input point
    precomp_finish();                               // precompute the values for the multi-point mult table
    addrG_clear_h_help(r);                          // do the multi-point multiplication
    from_jac_point(X, Y, Z, jp_tmp);                // conver the output back to affine coords
}

// *************************************************************
// S-vdW-Ulas simplified map for curves with nonzero j-invariant
// *************************************************************
// Map to curve with j-invariant != 0, 1728 (i.e., NOT BLS12-381!)
//
// However, we can still use it with BLS12-381 by mapping to an isogenous curve
// and then evaluating the isogeny. In our case, we use an 11-isogenous curve;
// see src/curve/iso_params.h and src/etc/gen_iso_params.sage.
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
// assumes that the argument u is passed in bint_tmp[10]
static inline void swu_help_ct(const unsigned jp_num) {
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
void swu_map(mpz_t x, mpz_t y, mpz_t z, const mpz_t u) {
    swu_help(1, u);
    eval_iso11();
    clear_h_chain();
    from_jac_point(x, y, z, jp_tmp + 7);
}

// constant-time version of swu_map
void swu_map_ct(mpz_t x, mpz_t y, mpz_t z, const mpz_t u) {
    bint_import_mpz(bint_tmp[10], u);
    swu_help_ct(1);
    eval_iso11_ct();
    clear_h_chain();
    from_jac_point(x, y, z, jp_tmp + 7);
}

// evaluate the SWU map twice, add result together, apply isogeny map, and clear cofactor
void swu_map2(mpz_t x, mpz_t y, mpz_t z, const mpz_t u1, const mpz_t u2) {
    swu_help(0, u1);
    swu_help(1, u2);
    // point_add is independent of curve constants, so we can use it on points from the 11-isogenous curve
    point_add(jp_tmp + 1, jp_tmp, jp_tmp + 1);
    eval_iso11();
    clear_h_chain();
    from_jac_point(x, y, z, jp_tmp + 7);
}

// constant-time version of swu_map2
void swu_map2_ct(mpz_t x, mpz_t y, mpz_t z, const mpz_t u1, const mpz_t u2) {
    bint_import_mpz(bint_tmp[10], u1);
    swu_help_ct(0);
    bint_import_mpz(bint_tmp[10], u2);
    swu_help_ct(1);
    point_add(jp_tmp + 1, jp_tmp, jp_tmp + 1);
    eval_iso11_ct();
    clear_h_chain();
    from_jac_point(x, y, z, jp_tmp + 7);
}

// evalute the SWU map once, apply isogeny map, and clear cofactor while adding a random point in subgroup
void swu_map_rG(mpz_t x, mpz_t y, mpz_t z, const mpz_t u, const uint8_t *r) {
    // evaluate SWU map and isogeny
    swu_help(1, u);
    eval_iso11();

    // precompute values for the multi-point mult table
    memcpy(&bint_precomp[1][0][0], jp_tmp + 1, sizeof(struct jac_point));
    precomp_finish();

    addrG_clear_h_help(r);  // multi-point multiplication
    from_jac_point(x, y, z, jp_tmp);
}
