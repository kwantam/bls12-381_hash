// curve ops for bls12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "curve.h"

#include "bint.h"
#include "bls12_381_consts.h"

#include <stdbool.h>
#include <string.h>

static mpz_t mpz_bls12_381_p, mpz_bls12_381_pp1o2, mpz_bls12_381_pp1o4;
mpz_t *get_p(void) { return &mpz_bls12_381_p; }

static void mpz_init_import(mpz_t out, const uint64_t *in) {
    mpz_init(out);
    mpz_import(out, 6, -1, 8, 0, 0, in);
}

struct jac_point {
    uint64_t X[BINT_NWORDS];
    uint64_t Y[BINT_NWORDS];
    uint64_t Z[BINT_NWORDS];
};
#define NUM_TMP_JP 8
static struct jac_point jp_tmp[NUM_TMP_JP];

#define NUM_TMP_BINT 11
static uint64_t bint_tmp[NUM_TMP_BINT][BINT_NWORDS];

#define NUM_TMP_MPZ 3
static mpz_t cx1, cx2, sqrtM27, invM27, mpz_tmp[NUM_TMP_MPZ];
static bool init_done = false;
void curve_init(void) {
    if (!init_done) {
        init_done = true;
        // p, p+1/4, p+1/2
        mpz_init(mpz_bls12_381_p);
        mpz_init(mpz_bls12_381_pp1o4);
        mpz_init(mpz_bls12_381_pp1o2);
        mpz_import(mpz_bls12_381_p, P_LEN, 1, 1, 1, 0, BLS12_381_p);
        mpz_add_ui(mpz_bls12_381_pp1o4, mpz_bls12_381_p, 1);
        mpz_fdiv_q_2exp(mpz_bls12_381_pp1o2, mpz_bls12_381_pp1o4, 1);
        mpz_fdiv_q_2exp(mpz_bls12_381_pp1o4, mpz_bls12_381_pp1o4, 2);

        // SvdW constants
        mpz_init_import(cx1, Icx1);
        mpz_init_import(cx2, Icx2);
        mpz_init_import(sqrtM27, IsqrtM27);
        mpz_init_import(invM27, IinvM27);

        // temp variables
        for (unsigned i = 0; i < NUM_TMP_MPZ; ++i) {
            mpz_init(mpz_tmp[i]);
        }
    }
}

void curve_uninit(void) {
    if (init_done) {
        init_done = false;
        mpz_clear(mpz_bls12_381_p);
        mpz_clear(mpz_bls12_381_pp1o4);
        mpz_clear(mpz_bls12_381_pp1o2);
        mpz_clear(cx1);
        mpz_clear(cx2);
        mpz_clear(sqrtM27);
        mpz_clear(invM27);
        for (unsigned i = 0; i < NUM_TMP_MPZ; ++i) {
            mpz_clear(mpz_tmp[i]);
        }
    }
}

// in1 ^ 2 mod p
void sqr_modp(mpz_t out, const mpz_t in) {
    mpz_mul(out, in, in);
    mpz_mod(out, out, mpz_bls12_381_p);
}

// sqrt(in1) mod p
void sqrt_modp(mpz_t out, const mpz_t in) { mpz_powm(out, in, mpz_bls12_381_pp1o4, mpz_bls12_381_p); }

// in1 * in2 mod p
void mul_modp(mpz_t out, const mpz_t in1, const mpz_t in2) {
    mpz_mul(out, in1, in2);
    mpz_mod(out, out, mpz_bls12_381_p);
}

// f(x) = x^3 + 4
void bls_fx(mpz_t out, const mpz_t in) {
    sqr_modp(out, in);
    mpz_mul(out, out, in);
    mpz_add_ui(out, out, 4);
    mpz_mod(out, out, mpz_bls12_381_p);
}

// shortcut reductions when we know the values are constrained
static void condadd_p(mpz_t in) {
    if (mpz_cmp_ui(in, 0) < 0) {
        mpz_add(in, in, mpz_bls12_381_p);
    }
}

static void condsub_p(mpz_t in) {
    if (mpz_cmp(in, mpz_bls12_381_p) >= 0) {
        mpz_sub(in, in, mpz_bls12_381_p);
    }
}

static inline bool svdw_check(const mpz_t x, mpz_t y, bool neg_t, bool force);
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
void svdw_map(mpz_t x, mpz_t y, const mpz_t t) {
    // first, save off sign of t (because maybe it's aliased with x)
    const bool neg_t = mpz_cmp(mpz_bls12_381_pp1o2, t) <= 0;  // true (negative) when t >= (p+1)/2

    sqr_modp(mpz_tmp[0], t);                              // t^2
    mpz_ui_sub(mpz_tmp[1], 23, mpz_tmp[0]);               // 23 - t^2
                                                          //
    mul_modp(mpz_tmp[2], mpz_tmp[1], mpz_tmp[0]);         // t^2 * (23 - t^2)
    mpz_invert(mpz_tmp[2], mpz_tmp[2], mpz_bls12_381_p);  // (t^2 * (23 - t^2)) ^ -1
                                                          //
    sqr_modp(mpz_tmp[0], mpz_tmp[0]);                     // t^4
    mul_modp(mpz_tmp[0], mpz_tmp[0], mpz_tmp[2]);         // t^2 / (23 - t^2)
    mul_modp(mpz_tmp[0], mpz_tmp[0], sqrtM27);            // t^2 sqrt(-27) / (23 - t^2)

    // x1
    mpz_add(x, cx1, mpz_tmp[0]);  // (3 - sqrt(-27))/2 + t^2 * sqrt(-27) / (23 - t^2)
    if (svdw_check(x, y, neg_t, false)) {
        condsub_p(x);  // reduce x mod p (mpz_tmp[0] and cx1 were both reduced, so x < 2p)
        return;
    }

    // x2
    mpz_sub(x, cx2, mpz_tmp[0]);  // (3 - sqrt(-27))/2 - t^2 * sqrt(-27) / (23 - t^2)
    if (svdw_check(x, y, neg_t, false)) {
        condadd_p(x);  // reduce x mod p (mpz_tmp[0] and cx2 were both reduced, so x > -p)
        return;
    }

    // x3
    sqr_modp(x, mpz_tmp[1]);     // (23 - t^2)^2
    mul_modp(x, x, mpz_tmp[1]);  // (23 - t^2)^3
    mul_modp(x, x, mpz_tmp[2]);  // (23 - t^2)^2 / t^2
    mul_modp(x, x, invM27);      // - (23 - t^2)^2 / (27 * t^2)
    mpz_sub_ui(x, x, 3);         // -3 - (23 - t^2)^2 / (27 * t^2)
    condadd_p(x);                // reduce x mod p (subtracted p from a reduced value, so x >= -3)
    svdw_check(x, y, neg_t, true);
}

// check if x is a point on the curve, and compute the corresponding y-coord if so
static inline bool svdw_check(const mpz_t x, mpz_t y, bool neg_t, bool force) {
    bls_fx(y, x);  // f(x)
    if (!force && mpz_legendre(y, mpz_bls12_381_p) != 1) {
        // f(x) is not a residue
        return false;
    }

    // found a residue
    sqrt_modp(y, y);  // sqrt(f(x))

    // multiply by 'sign' of t, for t negative when t >= p+1/2
    if (neg_t) {
        mpz_sub(y, mpz_bls12_381_p, y);
    }
    return true;
}

// double a point in Jacobian coordinates
// out == in is OK
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#doubling-dbl-2009-l
static void point_double(struct jac_point *out, const struct jac_point *in) {
    bint_sqr(bint_tmp[0], in->X);                        // A = X^2                      v = 2   w = 1
    bint_sqr(bint_tmp[1], in->Y);                        // B = Y^2                      v = 2   w = 1
    bint_sqr(bint_tmp[2], bint_tmp[1]);                  // C = B^2                      v = 2   w = 1
                                                         //
    bint_add(bint_tmp[3], in->X, bint_tmp[1]);           // X + B                        v = 4   w = 2
    bint_sqr(bint_tmp[3], bint_tmp[3]);                  // (X + B)^2                    v = 2   w = 1
    bint_add(bint_tmp[4], bint_tmp[0], bint_tmp[2]);     // (A + C)                      v = 4   w = 2
    bint_sub(bint_tmp[3], bint_tmp[3], bint_tmp[4], 3);  // (X + B)^2 - A - C            v = 10  w = 9
    bint_lsh(bint_tmp[3], bint_tmp[3], 1);               // D = 2 * ((X + B)^2 - A - C)  v = 20  w = 18
                                                         //
    bint_lsh(bint_tmp[4], bint_tmp[0], 1);               // 2 * A                        v = 4   w = 2
    bint_add(bint_tmp[4], bint_tmp[4], bint_tmp[0]);     // E = 3 * A                    v = 6   w = 3
                                                         //
    bint_sqr(bint_tmp[5], bint_tmp[4]);                  // F = E^2                      v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[6], bint_tmp[3], 1);               // 2 * D                        v = 40  w = 36
    bint_sub(bint_tmp[6], bint_tmp[5], bint_tmp[6], 6);  // F - 2 * D                    v = 66  w = 65
    bint_redc(out->X, bint_tmp[6]);                      // X3 = F - 2 * D               v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[6], in->Z, 1);                     // 2 * Z                        v = 4   w = 2
    bint_mul(out->Z, bint_tmp[6], in->Y);                // 2 * Z * Y                    v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[2], bint_tmp[2], 3);               // 8 * C                        v = 16  w = 8
    bint_sub(bint_tmp[6], bint_tmp[3], out->X, 2);       // D - X3                       v = 24  w = 22
    bint_mul(bint_tmp[6], bint_tmp[6], bint_tmp[4]);     // E * (D - X3)                 v = 2   w = 1
    bint_sub(bint_tmp[6], bint_tmp[6], bint_tmp[2], 5);  // E * (D - X3) - 8 * C         v = 34  w = 33
    bint_redc(out->Y, bint_tmp[6]);                      // Y = E * (D - X3) - 8 * C     v = 2   w = 1
}

// add two points in Jacobian coordinates
// out == in1 or out == in2 is OK
// NOTE: out->Y remains unreduced, but it meets numerical stability criteria
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#addition-add-2007-bl
static void point_add(struct jac_point *out, const struct jac_point *in1, const struct jac_point *in2) {
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
    bint_sub(bint_tmp[6], bint_tmp[3], bint_tmp[2], 2);  // H = U2 - U1                  v = 6   w = 5
                                                         //
    bint_lsh(bint_tmp[7], bint_tmp[6], 1);               // 2 * H                        v = 12  w = 10
    bint_sqr(bint_tmp[7], bint_tmp[7]);                  // I = (2 * H)^2                v = 2   w = 1
                                                         //
    bint_mul(bint_tmp[8], bint_tmp[7], bint_tmp[6]);     // J = H * I                    v = 2   w = 1
                                                         //
    bint_sub(bint_tmp[9], bint_tmp[5], bint_tmp[4], 2);  // S2 - S1                      v = 6   w = 5
    bint_lsh(bint_tmp[9], bint_tmp[9], 1);               // r = 2 * (S2 - S1)            v = 12  w = 10
                                                         //
    bint_mul(bint_tmp[10], bint_tmp[2], bint_tmp[7]);    // V = U1 * I                   v = 2   w = 1
                                                         //
    bint_lsh(out->X, bint_tmp[10], 1);                   // 2 * V                        v = 4   w = 2
    bint_add(out->X, out->X, bint_tmp[8]);               // J + 2 * V                    v = 6   w = 3
    bint_sqr(bint_tmp[7], bint_tmp[9]);                  // r^2                          v = 2   w = 1
    bint_sub(out->X, bint_tmp[7], out->X, 4);            // r^2 - J - 2 * V              v = 18  w = 17
    bint_redc(out->X, out->X);                           // X3 = r^2 - J - 2 * V         v = 2   w = 1
                                                         //
    bint_lsh(bint_tmp[4], bint_tmp[4], 1);               // 2 * S1                       v = 4   w = 2
    bint_mul(bint_tmp[4], bint_tmp[4], bint_tmp[8]);     // 2 * S1 * J                   v = 2   w = 1
    bint_sub(out->Y, bint_tmp[10], out->X, 2);           // V - X3                       v = 6   w = 5
    bint_mul(out->Y, out->Y, bint_tmp[9]);               // r * (V - X3)                 v = 2   w = 1
    bint_sub(out->Y, out->Y, bint_tmp[4], 2);            // r * (V - X3) - 2 * S1 * J    v = 6   w = 5
                                                         //
    bint_add(out->Z, in1->Z, in2->Z);                    // Z1 + Z2                      v = 4   w = 2
    bint_sqr(out->Z, out->Z);                            // (Z1 + Z2)^2                  v = 2   w = 1
    bint_add(bint_tmp[0], bint_tmp[0], bint_tmp[1]);     // Z1Z1 + Z2Z2                  v = 4   w = 2
    bint_sub(out->Z, out->Z, bint_tmp[0], 3);            // (Z1 + Z2)^2 - Z1Z1 - Z2Z2    v = 10  w = 9
    bint_mul(out->Z, out->Z, bint_tmp[6]);               // Z3 = 2 * Z1 * Z2 * H         v = 2   w = 1
}

// convert from a jac_point to a pair of mpz_t
static void from_jac_point(mpz_t outX, mpz_t outY, const struct jac_point *jp) {
    // convert from bint to gmp
    bint_export_mpz(outX, jp->X);
    bint_export_mpz(outY, jp->Y);
    bint_export_mpz(mpz_tmp[0], jp->Z);

    // convert from Jacobian to affine coordinates
    mpz_invert(mpz_tmp[0], mpz_tmp[0], mpz_bls12_381_p);  // Z^-1
    mul_modp(outY, outY, mpz_tmp[0]);                     // Y / Z
    sqr_modp(mpz_tmp[0], mpz_tmp[0]);                     // Z^-2
    mul_modp(outY, outY, mpz_tmp[0]);                     // Y / Z^3
    mul_modp(outX, outX, mpz_tmp[0]);                     // X / Z^2
}

// convert from a pair of mpz_t to a jac_point
static void to_jac_point(struct jac_point *jp, const mpz_t inX, const mpz_t inY) {
    bint_import_mpz(jp->X, inX);
    bint_import_mpz(jp->Y, inY);
    bint_set1(jp->Z);
}

// Addition chain: Bos-Coster (win=7) : 147 links, 8 variables
// input point is assumed to be in jp_tmp[1]
// TODO(rsw): is there a faster addition-subtraction chain?
static void clear_h_chain(mpz_t outX, mpz_t outY) {
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

    from_jac_point(outX, outY, jp_tmp + 7);
}

// clear BLS12-381 cofactor
// outX == inX and/or outY == inY is OK
void clear_h(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY) {
    to_jac_point(jp_tmp + 1, inX, inY);
    clear_h_chain(outX, outY);
}

// add two points together, leave result in jp_tmp[1]
static inline void add2_help(const mpz_t inX1, const mpz_t inY1, const mpz_t inX2, const mpz_t inY2) {
    to_jac_point(jp_tmp, inX1, inY1);
    to_jac_point(jp_tmp + 1, inX2, inY2);
    point_add(jp_tmp + 1, jp_tmp + 1, jp_tmp);
}

// add 2 points together; don't clear cofactor
void add2(mpz_t outX, mpz_t outY, const mpz_t inX1, const mpz_t inY1, const mpz_t inX2, const mpz_t inY2) {
    add2_help(inX1, inY1, inX2, inY2);
    from_jac_point(outX, outY, jp_tmp + 1);
}

// add 2 points together, then clear cofactor
// ouX == inX{1,2} and/or outY == inY{1,2} is OK
void add2_clear_h(mpz_t outX, mpz_t outY, const mpz_t inX1, const mpz_t inY1, const mpz_t inX2, const mpz_t inY2) {
    add2_help(inX1, inY1, inX2, inY2);
    clear_h_chain(outX, outY);
}

// precompute the fixed part of the table for addrG
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
static void precomp_finish(void) {
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
void addrG_clear_h(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY, const uint8_t *r) {
    // first, precompute the values for the table
    to_jac_point(&bint_precomp[1][0][0], inX, inY);
    precomp_finish();

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

    from_jac_point(outX, outY, jp_tmp);
}