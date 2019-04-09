// bls12-381 bigint
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__bint__bint_h__

#include "bint_consts.h"

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

bool bint_eq0(bint_ty io);
void bint_set1(bint_ty out);
bool bint_is_neg(const bint_ty in);

void bint_add(bint_ty out, const bint_ty ina, const bint_ty inb);
void bint_sub(bint_ty out, const bint_ty ina, const bint_ty inb, const int bup);
void bint_neg(bint_ty out, const bint_ty in, const int bup);
void bint_lsh(bint_ty out, const bint_ty in, const int sh);

void bint_condassign(bint_ty out, const bool first, const bint_ty in1, const bint_ty in2);

void bint_mul(bint_ty out, const bint_ty ina, const bint_ty inb);
void bint_sqr(bint_ty out, const bint_ty in);
void bint_redc(bint_ty out, const bint_ty in);

bool bint_divsqrt(bint_ty out, const bint_ty u, const bint_ty v, const bool force);

void bint_import_mpz(bint_ty out, const mpz_t in);
void bint_export_mpz(mpz_t out, const bint_ty in);

#define __bls_hash__src__bint__bint_h__
#endif  // __bls_hash__src__bint__bint_h__
