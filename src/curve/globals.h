// curve ops for bls12-381 hashing --- internal header file
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__globals_h__

#include "bint_consts.h"
#include "iso.h"

#include <gmp.h>
#include <stdint.h>

#define NUM_TMP_BINT 32
#define NUM_TMP_MPZ 33  // NOTE: needs to be at least 3 + ELLP_YMAP_DEN_LEN + ELLP_YMAP_NUM_LEN

// mpzs for curve ops
extern mpz_t cx1, cx2, sqrtM27, invM27, mpz_tmp[NUM_TMP_MPZ], fld_p, pp1o4, pm3o4;
extern mpz_t ellp_a, ellp_b, pm2, pm1o2;
extern mpz_t xmap_num[ELLP_XMAP_NUM_LEN], xmap_den[ELLP_XMAP_DEN_LEN];
extern mpz_t ymap_num[ELLP_YMAP_NUM_LEN], ymap_den[ELLP_YMAP_DEN_LEN];

// bints for curve ops
extern int64_t bint_tmp[NUM_TMP_BINT][BINT_NWORDS];
extern int64_t bint_ellp_b[BINT_NWORDS], bint_ellp_a[BINT_NWORDS], bint_one[BINT_NWORDS];
extern int64_t bint_xnum[ELLP_XMAP_NUM_LEN][BINT_NWORDS], bint_xden[ELLP_XMAP_DEN_LEN][BINT_NWORDS];
extern int64_t bint_ynum[ELLP_YMAP_NUM_LEN][BINT_NWORDS], bint_yden[ELLP_YMAP_DEN_LEN][BINT_NWORDS];
extern int64_t bint_cx1[BINT_NWORDS], bint_cx2[BINT_NWORDS], bint_sqrtM27[BINT_NWORDS];
extern int64_t bint_23[BINT_NWORDS], bint_M27[BINT_NWORDS], bint_81[BINT_NWORDS];

#define __bls_hash__src__curve__globals_h__
#endif  // __bls_hash__src__curve__globals_h__
