// globals and initialization functions for curve ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "globals.h"

#include "bint.h"
#include "consts.h"
#include "curve.h"

mpz_t cx1, cx2, sqrtM27, invM27, mpz_tmp[NUM_TMP_MPZ], fld_p, pp1o4, pm3o4;
mpz_t ellp_a, ellp_b, pm2, pm1o2;
mpz_t xmap_num[ELLP_XMAP_NUM_LEN], xmap_den[ELLP_XMAP_DEN_LEN];
mpz_t ymap_num[ELLP_YMAP_NUM_LEN], ymap_den[ELLP_YMAP_DEN_LEN];

int64_t bint_tmp[NUM_TMP_BINT][BINT_NWORDS];
int64_t bint_ellp_b[BINT_NWORDS], bint_ellp_a[BINT_NWORDS], bint_one[BINT_NWORDS];
int64_t bint_xnum[ELLP_XMAP_NUM_LEN][BINT_NWORDS], bint_xden[ELLP_XMAP_DEN_LEN][BINT_NWORDS];
int64_t bint_ynum[ELLP_YMAP_NUM_LEN][BINT_NWORDS], bint_yden[ELLP_YMAP_DEN_LEN][BINT_NWORDS];
int64_t bint_cx1[BINT_NWORDS], bint_cx2[BINT_NWORDS], bint_sqrtM27[BINT_NWORDS];
int64_t bint_23[BINT_NWORDS], bint_M27[BINT_NWORDS], bint_81[BINT_NWORDS];

// init an mpz_t and set it from a constant defined in bls12_381_consts.h
static inline void mpz_init_import(mpz_t out, const uint64_t *in) {
    mpz_init(out);
    mpz_import(out, 6, -1, 8, 0, 0, in);
}

// initialize globals
static bool init_done = false;  // shared between init and uninit
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
