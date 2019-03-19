#ifndef _HAVE_BINT_INTERNAL_H
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef DEBUG_PRINT
#include <stdio.h>
static inline void _bint_debug_print(const uint64_t *ina, const char *pre, bool cr);
#endif

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

static const uint64_t zero[NWORDS] = {0,};

#define _HAVE_BINT_INTERNAL_H
#endif // _HAVE_BINT_INTERNAL_H
