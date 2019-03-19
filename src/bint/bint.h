#include "bint_consts.h"

#ifndef _HAVE_BINT_H
#include <gmp.h>
#include <stdint.h>

void bint_set1(uint64_t *out);

void bint_add(uint64_t *out, const uint64_t *ina, const uint64_t *inb);
void bint_sub(uint64_t *out, const uint64_t *ina, const uint64_t *inb, int bup);
void bint_lsh(uint64_t *out, const uint64_t *in, int sh);

void bint_mul(uint64_t *out, const uint64_t *ina, const uint64_t *inb);
void bint_sqr(uint64_t *out, const uint64_t *ina);
void bint_redc(uint64_t *out, const uint64_t *in);

void bint_import_mpz(uint64_t *out, const mpz_t in);
void bint_export_mpz(mpz_t out, const uint64_t *in);

#define _HAVE_BINT_H
#endif  // _HAVE_BINT_H
