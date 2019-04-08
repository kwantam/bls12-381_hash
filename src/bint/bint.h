// bls12-381 bigint
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__bint__bint_h__

#include "bint_consts.h"

#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>

bool bint_eq0(int64_t *io);
void bint_set1(int64_t *out);
bool bint_is_neg(const int64_t *in);

void bint_add(int64_t *out, const int64_t *ina, const int64_t *inb);
void bint_sub(int64_t *out, const int64_t *ina, const int64_t *inb, const int bup);
void bint_neg(int64_t *out, const int64_t *in, const int bup);
void bint_lsh(int64_t *out, const int64_t *in, const int sh);

void bint_condassign(int64_t *out, const bool first, const int64_t *in1, const int64_t *in2);

void bint_mul(int64_t *out, const int64_t *ina, const int64_t *inb);
void bint_sqr(int64_t *out, const int64_t *in);
void bint_redc(int64_t *out, const int64_t *in);

bool bint_divsqrt(int64_t *__restrict__ out, const int64_t *__restrict__ u, const int64_t *__restrict__ v,
                  const bool force);

void bint_import_mpz(int64_t *out, const mpz_t in);
void bint_export_mpz(mpz_t out, const int64_t *in);

#define __bls_hash__src__bint__bint_h__
#endif  // __bls_hash__src__bint__bint_h__
