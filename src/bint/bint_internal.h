// bls12-381 bigint internal header
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__bint__bint_internal_h__
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static inline void _bint_to_monty(uint64_t *out, const uint64_t *in);
static inline void _bint_from_monty(uint64_t *out, const uint64_t *in);
static inline int _bint_compare(const uint64_t *ina, const uint64_t *inb);
static inline void _bint_condsub_p(uint64_t *io);
static inline void _bint_monty_help(uint64_t *out, uint64_t *tmp);
static inline void _bint_mul(uint64_t *out, const uint64_t *ina, const uint64_t *inb);
static inline void _bint_mul_low(uint64_t *out, const uint64_t *ina, const uint64_t *inb);
static inline void _bint_sqr(uint64_t *out, const uint64_t *ina);

#define BINT_INTERNAL
#include "bint.h"
#undef BINT_INTERNAL

// clang-format off
static const uint64_t zero[NWORDS] = {0,};
// clang-format on

#define __bls_hash__src__bint__bint_internal_h__
#endif  // __bls_hash__src__bint__bint_internal_h__
