// globals and initialization functions for E2 ops
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "globals2.h"

#include "consts2.h"
#include "curve.h"
#include "globals.h"

#include <gmp.h>

mpz_t2 mul2tmp[2];     // temps for basic arithmetic ops in fp2
mpz_t2 ssqrtM1[2];     // values for computing sqrts
mpz_t swu2_eta01;      // eta0 and eta1 for SWU map (same value, just multiplied by sqrt(-1)
mpz_t2 swu2_eta23[2];  // eta2 and eta3 for SWU map

// mpz_init for mpz_t2
static void mpz2_init(mpz_t2 io) {
    mpz_init(io.s);
    mpz_init(io.t);
}

// mpz_clear for mpz_t2
static void mpz2_clear(mpz_t2 io) {
    mpz_clear(io.s);
    mpz_clear(io.t);
}

// initialize globals for curve2
static bool init_done = false;  // shared between init and uninit
void curve2_init(void) {
    if (init_done) {
        return;
    }
    init_done = true;

    curve_init();  // need the globals from globals.h, too

    for (unsigned i = 0; i < 2; ++i) {
        mpz2_init(mul2tmp[i]);
    }

    for (unsigned i = 0; i < 2; ++i) {
        mpz2_init(ssqrtM1[i]);
        mpz2_init(swu2_eta23[i]);
    }
    // sqrt(+/- sqrt(-1))
    mpz_import(ssqrtM1[0].s, 6, -1, 8, 0, 0, IssqrtM1);  // sqrt(sqrt(-1))
    mpz_set(ssqrtM1[0].t, ssqrtM1[0].s);                 //
    mpz_set(ssqrtM1[1].s, ssqrtM1[0].s);                 // sqrt(-sqrt(-1))
    mpz_sub(ssqrtM1[1].t, fld_p, ssqrtM1[0].s);          //

    // eta[i], the constants sqrt(xi / sqrt(sqrt(-1))) for the SWU map
    mpz_init(swu2_eta01);                                // eta[0] and eta[1] (eta[1] = sqrt(-1) * eta[0])
    mpz_import(swu2_eta01, 6, -1, 8, 0, 0, Ieta1);       // value for eta01
    mpz_import(swu2_eta23[0].s, 6, -1, 8, 0, 0, Ieta2);  // value for eta23
    mpz_set(swu2_eta23[0].t, swu2_eta23[0].s);
    mpz_set(swu2_eta23[1].s, swu2_eta23[0].s);
    mpz_sub(swu2_eta23[1].t, fld_p, swu2_eta23[0].s);
}

// uninit globals for curve2
void curve2_uninit(void) {
    if (!init_done) {
        return;
    }
    init_done = false;

    curve_uninit();  // uninit globals.h, too

    for (unsigned i = 0; i < 2; ++i) {
        mpz2_clear(mul2tmp[i]);
    }

    for (unsigned i = 0; i < 2; ++i) {
        mpz2_clear(ssqrtM1[i]);
        mpz2_clear(swu2_eta23[i]);
    }
    mpz_clear(swu2_eta01);
}
