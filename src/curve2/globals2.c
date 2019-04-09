// globals and initialization functions for E2 ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "globals2.h"

#include "consts2.h"
#include "curve.h"
#include "globals.h"

#include <gmp.h>

mpz_t2 mpz2_tmp[NUM_MPZ2_TMP];     // temps for basic arithmetic ops in fp2
mpz_t2 mpz2mul[2];                 // private temps for mul and sqr
                                   //
mpz_t cx1_2, cx2_2, sqrtM3, inv3;  // values for SvdW map (all have no "imaginary" part)
                                   //
mpz_t swu2_eta01;                  // eta0 and eta1 for SWU map (same value, just multiplied by sqrt(-1)
mpz_t2 swu2_eta23[2];              // eta2 and eta3 for SWU map

// initialize globals for curve2
static bool init_done = false;  // shared between init and uninit
void curve2_init(void) {
    if (init_done) {
        return;
    }
    init_done = true;

    curve_init();  // need the globals from globals.h, too

    for (unsigned i = 0; i < NUM_MPZ2_TMP; ++i) {
        mpz2_init(mpz2_tmp[i]);
    }

    for (unsigned i = 0; i < 2; ++i) {
        mpz2_init(mpz2mul[i]);
        mpz2_init(swu2_eta23[i]);
    }

    // eta[i], the constants sqrt(xi / sqrt(sqrt(-1))) for the SWU map
    mpz_init(swu2_eta01);                                 // eta[0] and eta[1] (eta[1] = sqrt(-1) * eta[0])
    mpz_import(swu2_eta01, 6, -1, 8, 0, 0, Ieta1);        // value for eta01
    mpz_import(swu2_eta23[0]->s, 6, -1, 8, 0, 0, Ieta2);  // value for eta23
    mpz_set(swu2_eta23[0]->t, swu2_eta23[0]->s);
    mpz_set(swu2_eta23[1]->s, swu2_eta23[0]->s);
    mpz_sub(swu2_eta23[1]->t, fld_p, swu2_eta23[0]->s);

    // SvdW constants
    mpz_init_import(cx1_2, Icx12);
    mpz_init(cx2_2);
    mpz_sub_ui(cx2_2, cx1_2, 1);  // cx2 is cx1 - 1
    mpz_init_import(sqrtM3, IsqrtM3);
    mpz_init_import(inv3, Iinv3);
}

// uninit globals for curve2
void curve2_uninit(void) {
    if (!init_done) {
        return;
    }
    init_done = false;

    curve_uninit();  // uninit globals.h, too

    for (unsigned i = 0; i < NUM_MPZ2_TMP; ++i) {
        mpz2_clear(mpz2_tmp[i]);
    }

    for (unsigned i = 0; i < 2; ++i) {
        mpz2_clear(mpz2mul[i]);
        mpz2_clear(swu2_eta23[i]);
    }
    mpz_clear(swu2_eta01);
    mpz_clear(cx1_2);
    mpz_clear(cx2_2);
    mpz_clear(sqrtM3);
    mpz_clear(inv3);
}
