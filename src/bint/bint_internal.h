// bls12-381 bigint internal header
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__bint__bint_internal_h__
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static inline void _bint_to_monty(int64_t *out, const int64_t *in);
static inline void _bint_from_monty(int64_t *out, const int64_t *in);
static inline int _bint_compare(const int64_t *ina, const int64_t *inb);
static inline void _bint_condsub_p(int64_t *io);
static inline bool _bint_condsub_p_eq(int64_t *io, const int64_t *cmpval);
static inline void _bint_monty_help(int64_t *out, int64_t *tmp);
static inline void _bint_mul(int64_t *out, const int64_t *ina, const int64_t *inb);
static inline void _bint_mul_low(int64_t *out, const int64_t *ina, const int64_t *inb);
static inline void _bint_sqr(int64_t *out, const int64_t *ina);

#include "bint_chains.h"

#define BINT_INTERNAL
#include "bint.h"
#undef BINT_INTERNAL

// clang-format off
static const int64_t zero[NWORDS] = {0,};
// clang-format on

#define __bls_hash__src__bint__bint_internal_h__
#endif  // __bls_hash__src__bint__bint_internal_h__
