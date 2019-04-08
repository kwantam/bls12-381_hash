// bls12-381 bigint for Fp2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu

#ifndef __bls_hash__src__bint2__bint2_h__

#include "bint_consts.h"
#include "fp2.h"

#include <stdbool.h>
#include <stdint.h>

#define BINT2_NWORDS (2 * BINT_NWORDS)

bool bint2_eq0(int64_t *io);
void bint2_set1(int64_t *out);
void bint2_seti(int64_t *out);
bool bint2_is_neg(const int64_t *in);

void bint2_add(int64_t *out, const int64_t *ina, const int64_t *inb);
void bint2_sub(int64_t *out, const int64_t *ina, const int64_t *inb, const int bup);
void bint2_neg(int64_t *out, const int64_t *ina, const int bup);
void bint2_lsh(int64_t *out, const int64_t *ina, const int sh);

void bint2_condassign(int64_t *out, const bool first, const int64_t *in1, const int64_t *in2);

void bint2_mul(int64_t *out, const int64_t *ina, const int64_t *inb);
void bint2_sqr(int64_t *out, const int64_t *in);
void bint2_redc(int64_t *out, const int64_t *in);

void bint2_mul_i(int64_t *out, const int64_t *in, const int bup);
void bint2_mul_sc(int64_t *out, const int64_t *ina, const int64_t *inb);
void bint2_mul_sc_i(int64_t *out, const int64_t *ina, const int64_t *inb);
void bint2_norm(int64_t *out, const int64_t *in);

void bint2_import_mpz2(int64_t *out, const mpz_t2 in);
void bint2_export_mpz2(mpz_t2 out, const int64_t *in);

bool bint2_sqrt(int64_t *__restrict__ out, int64_t *__restrict__ in);
bool bint2_divsqrt(int64_t *__restrict__ out, int64_t *__restrict__ u, int64_t *__restrict__ v);

#define __bls_hash__src__bint2__bint2_h__
#endif  // __bls_hash__src__bint2__bint2_h__
