// curve operations and point repr
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "ops.h"

#include "bint.h"
#include "consts.h"
#include "curve.h"
#include "globals.h"

#include <string.h>

// **************************
// BLS12-381 curve operations
// **************************
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
void point_add(struct jac_point *out, const struct jac_point *in1, const struct jac_point *in2) {
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
struct jac_point jp_tmp[NUM_TMP_JP];

// Addition chain: Bos-Coster (win=7) : 147 links, 8 variables
// input point is taken from jp_tmp[1], output is in jp_tmp[7]
// TODO(rsw): is there a faster addition-subtraction chain?
void clear_h_chain(void) {
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
struct jac_point bint_precomp[4][4][4];
void precomp_init(void) {
    memset(bint_precomp[0][0][0].X, 0, BINT_NWORDS * sizeof(int64_t));
    bint_set1(bint_precomp[0][0][0].Y);
    memset(bint_precomp[0][0][0].Z, 0, BINT_NWORDS * sizeof(int64_t));

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
void precomp_finish(void) {
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
void addrG_clear_h_help(const uint8_t *r) {
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
    from_jac_point(X, Y, Z, jp_tmp);                // convert result
}
