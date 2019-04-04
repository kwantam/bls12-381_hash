// curve operations and point repr
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__ops_h__

#include "bint_consts.h"

#include <gmp.h>
#include <stdint.h>

// forward declarations for bint import/export functions to avoid including bint.h here
void bint_import_mpz(int64_t *out, const mpz_t in);
void bint_export_mpz(mpz_t out, const int64_t *in);

// Jacobian coordinates: x = X/Z^2, y = Y/Z^3
struct jac_point {
    int64_t X[BINT_NWORDS];
    int64_t Y[BINT_NWORDS];
    int64_t Z[BINT_NWORDS];
};

// temporary points for intermediate computations (mostly used in clear_h_chain())
#define NUM_TMP_JP 8
extern struct jac_point jp_tmp[NUM_TMP_JP];

// precomputed values for three-point multiplication
extern struct jac_point bint_precomp[4][4][4];

// precompute the part of the addrG table that involves the input point
void precomp_finish(void);

// addition chain for clearing cofactor
void clear_h_chain(void);

// helper for construction #3: add random multiple of gPrime and clear h
void addrG_clear_h_help(const uint8_t *r);

// add two points in Jacobian coordinates
void point_add(struct jac_point *out, const struct jac_point *in1, const struct jac_point *in2);

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

#define __bls_hash__src__curve__ops_h__
#endif  // __bls_hash__src__curve__ops_h__
