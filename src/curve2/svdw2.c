// SvdW map to G2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "arith2.h"
#include "curve2.h"
#include "globals.h"
#include "globals2.h"

// SvdW map to BLS12-381 G2
// see comment in ../curve/svdw.c and ../../paper/ for more info
//
// NOTE: t must be reduced mod p!
// t == x is OK, but x and y must be distinct
static inline void svdw2_map_help(mpz_t2 x, mpz_t2 y, const bool neg_t, const unsigned tmp_os) {
    // x1 = cx1_2 - x12_com
    mpz_sub(x->s, cx1_2, mpz2_tmp[tmp_os]->s);   // cx1_2 - t^2 * sqrt(-3) / (t^2 + 3 + 4I)
    condadd_p(x->s);                             // fix up
    mpz_sub(x->t, fld_p, mpz2_tmp[tmp_os]->t);   // negate other component
    if (check_fx2(y, x, neg_t, false, false)) {  // found it?
        return;
    }

    // x2 = x12_com - cx2_2
    mpz_sub(x->s, mpz2_tmp[tmp_os]->s, cx2_2);   // -cx2_2 + t^2 * sqrt(-3) / (t^2 + 3 + 4I)
    condadd_p(x->s);                             // fix up
    mpz_set(x->t, mpz2_tmp[tmp_os]->t);          // copy in other component
    if (check_fx2(y, x, neg_t, false, false)) {  // found it?
        return;
    }

    // x3 = -1 - (t^2 + 3 + 4I) ^ 2 / (3 t^2)
    sqr_modp2(x, mpz2_tmp[tmp_os + 1]);     // (t^2 + 3 + 4I)^2
    mul_modp2(x, x, mpz2_tmp[tmp_os + 1]);  // (t^2 + 3 + 4I)^3
    mul_modp2(x, x, mpz2_tmp[tmp_os + 2]);  // (t^2 + 3 + 4I)^2 / t^2
    mul_modp2_scalar(x, x, inv3);           // (t^2 + 3 + 4I)^2 / (3 t^2)
    mpz_add_ui(x->s, x->s, 1);              // 1 + (t^2 + 3 + 4I)^2 / (3 t^2)
    mpz2_neg(x, x);                         // -1 - (t^2 + 3 + 4I)^2 / (3 t^2)
    check_fx2(y, x, neg_t, true, false);    // must have found it this time
}

static inline void svdw2_precomp1(const mpz_t2 t, const unsigned tmp_os) {
    sqr_modp2(mpz2_tmp[tmp_os], t);                                           // t^2
    mpz2_add_ui2(mpz2_tmp[tmp_os + 1], mpz2_tmp[tmp_os], 3, 4);               // t^2 + 3 + 4I  (f_2(-1) == 3 + 4I)
    mul_modp2(mpz2_tmp[tmp_os + 2], mpz2_tmp[tmp_os + 1], mpz2_tmp[tmp_os]);  // t^2 (t^2 + 3 + 4I)
}

static inline void svdw2_precomp2(const unsigned tmp_os) {
    sqr_modp2(mpz2_tmp[tmp_os], mpz2_tmp[tmp_os]);                        // t^4
    mul_modp2(mpz2_tmp[tmp_os], mpz2_tmp[tmp_os], mpz2_tmp[tmp_os + 2]);  // t^2 / (t^2 + 3 + 4I)
    mul_modp2_scalar(mpz2_tmp[tmp_os], mpz2_tmp[tmp_os], sqrtM3);         // sqrt(-3) t^2 / (t^2 + 3 + 4I)
}

void svdw2_map(mpz_t2 x, mpz_t2 y, const mpz_t2 t) {
    svdw2_precomp1(t, 0);  // input to inversion
    if (!mpz2_zero_p(mpz2_tmp[2])) {
        invert_modp2(mpz2_tmp[2], mpz2_tmp[2]);  // invert if nonzero
    }
    svdw2_precomp2(0);
    const bool neg_t = mpz_cmp(pm1o2, t->s) < 0;
    svdw2_map_help(x, y, neg_t, 0);
}

// apply the SvdW map to two points simultaneously
// This saves one inversion vs. two calls to svdw2_map()
void svdw2_map2(mpz_t2 x1, mpz_t2 y1, const mpz_t2 t1, mpz_t2 x2, mpz_t2 y2, const mpz_t2 t2) {
    svdw2_precomp1(t1, 0);  // inversion input for t1
    svdw2_precomp1(t2, 3);  // inversion input for t2

    // invert one or both of mpz_tmp[2] and mpz_tmp[5]
    const bool p10 = mpz2_zero_p(mpz2_tmp[2]);
    const bool p20 = mpz2_zero_p(mpz2_tmp[5]);
    if (p10 && !p20) {
        invert_modp2(mpz2_tmp[5], mpz2_tmp[5]);  // only 2nd is nonzero
    } else if (!p10 && p20) {
        invert_modp2(mpz2_tmp[2], mpz2_tmp[2]);  // only 1st is nonzero
    } else if (!p10 && !p20) {
        mul_modp2(mpz2_tmp[6], mpz2_tmp[5], mpz2_tmp[2]);  // 1st * 2nd
        invert_modp2(mpz2_tmp[6], mpz2_tmp[6]);            // invert
        mul_modp2(mpz2_tmp[2], mpz2_tmp[2], mpz2_tmp[6]);  // multiply out 1st
        mul_modp2(mpz2_tmp[5], mpz2_tmp[5], mpz2_tmp[6]);  // multiply out 2nd
        mpz2_swap(mpz2_tmp[2], mpz2_tmp[5]);               // result was swapped; unswap
    }

    svdw2_precomp2(0);  // x12_com for t1
    svdw2_precomp2(3);  // x12_com for t2

    const bool neg_t1 = mpz_cmp(pm1o2, t1->s) < 0;  // is t1 negative?
    svdw2_map_help(x1, y1, neg_t1, 0);
    const bool neg_t2 = mpz_cmp(pm1o2, t2->s) < 0;  // is t2 negative?
    svdw2_map_help(x2, y2, neg_t2, 3);
}

// SvdW with only field ops
static inline bool check_f2_xOverZ(mpz_t2 x, mpz_t2 y, mpz_t2 z, const bool negate) {
    sqr_modp2(mpz2_tmp[2], x);               // x^2
    mul_modp2(mpz2_tmp[2], mpz2_tmp[2], x);  // x^3
    sqr_modp2(mpz2_tmp[3], z);               // z^2
    mul_modp2(mpz2_tmp[3], mpz2_tmp[3], z);  // z^3

    mpz_set_ui(mpz2_tmp[4]->s, 4);                     // 4
    mpz_set_ui(mpz2_tmp[4]->t, 4);                     // 4I
    mul_modp2(mpz2_tmp[4], mpz2_tmp[4], mpz2_tmp[3]);  // 4*(1 + I) z^3
    mpz2_add(mpz2_tmp[2], mpz2_tmp[2], mpz2_tmp[4]);   // x^3 + 4(1+I) z^3

    if (divsqrt_modp2(y, mpz2_tmp[2], mpz2_tmp[3])) {
        // found a sqrt, put it jacobian coords
        mul_modp2(x, x, z);            // X = x z
        mul_modp2(y, y, mpz2_tmp[3]);  // Y' = y z^3
        if (negate) {
            mpz2_neg(y, y);
        }
        return true;
    }
    return false;
}

// svdw using field ops only --- not constant-time
void svdw2_map_fo(mpz_t2 x, mpz_t2 y, mpz_t2 z, const mpz_t2 t) {
    const bool neg_t = mpz_cmp(pm1o2, t->s) < 0;  // t is negative

    sqr_modp2(mpz2_tmp[0], t);                           // t^2
    mpz2_add_ui2(z, mpz2_tmp[0], 3, 4);                  // t^2 + 3 + 4I           = V
    mul_modp2_scalar(mpz2_tmp[1], mpz2_tmp[0], sqrtM3);  // t^2 * sqrt(-3)

    // x1 : (cx1_2 * (t^2 + 3 + 4I) - t^2 * sqrt(-3)) / (t^2 + 3 + 4I)
    // exceptional case is when z == 0 (if t==0 then we get the correct result automatically)
    if (mpz2_zero_p(z)) {
        mpz_set(x->s, cx1_2);  // x = cx1_2
        mpz_set_ui(x->t, 0);
        mpz_set_ui(z->s, 1);  // z = 1
        mpz_set_ui(z->t, 0);
    } else {
        mul_modp2_scalar(x, z, cx1_2);  // cx1_2 * (t^2 + 3 + 4I)
        mpz2_sub(x, x, mpz2_tmp[1]);    // cx1_2 * (t^2 + 3 + 4I) - t^2 * sqrt(-3)
        condadd_p2(x);                  // reduce mod p
    }
    if (check_f2_xOverZ(x, y, z, neg_t)) {
        return;
    }

    // x2 : (t^2 * sqrt(-3) - cx2_2 * (t^2 + 3 + 4I)) / (t^2 + 3 + 4I)
    mul_modp2_scalar(x, z, cx2_2);      // cx2_2 * (t^2 + 3 + 4I)
    mpz2_sub(x, mpz2_tmp[1], x);        // t^2 * sqrt(-3) - cx2_2 * (t^2 + 3 + 4I)
    condadd_p2(x);                      // reduce mod p
    if (check_f2_xOverZ(x, y, z, neg_t)) {
        return;
    }

    // x3: ((t^2 + 3 + 4I)^2 + 3 t^2) / (-3 t^2)
    sqr_modp2(x, z);                         // (t^2 + 3 + 4I)^2
    mul_modp2_scalar_ui(z, mpz2_tmp[0], 3);  // 3 t^2
    mpz2_add(x, x, z);                       // (t^2 + 3 + 4I)^2 + 3 t^2
    mpz2_neg(z, z);                          // -3 t^2
    check_f2_xOverZ(x, y, z, neg_t);
}
