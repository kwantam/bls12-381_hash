#include "bint_consts.h"

#ifndef _HAVE_BINT_H
#include <gmp.h>
#include <stdint.h>

void bint_to_monty(uint64_t *out, const uint64_t *in);
void bint_from_monty(uint64_t *out, const uint64_t *in);

void bint_mul(uint64_t *out, const uint64_t *ina, const uint64_t *inb);
void bint_sqr(uint64_t *out, const uint64_t *ina);

void bint_inv(uint64_t *out, const uint64_t *ina);
void bint_sqrt(uint64_t *out, const uint64_t *ina);
void bint_invsqrt(uint64_t *out, const uint64_t *ina);
int bint_jacobi(const uint64_t *ina);

void bint_import_mpz(uint64_t *out, const mpz_t in);
void bint_export_mpz(mpz_t out, const uint64_t *in);

#define _HAVE_BINT_H
#endif // _HAVE_BINT_H
