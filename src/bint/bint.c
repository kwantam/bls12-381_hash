#include "bint_internal.h"

#ifdef DEBUG_PRINT
static inline void _bint_debug_print(const uint64_t *ina, const char *pre, bool cr) {
    printf("%s [", pre ? pre : "");
    for (int i = NWORDS - 1; i >= 0; i--) {
        printf("0x%016lx, ", ina[i]);
    }
    if (cr) {
        printf("]\n");
    }
}
#endif

static inline int _bint_compare(const uint64_t *ina, const uint64_t *inb) {
    bool gt = false;
    bool eq = true;
    for (int i = NWORDS - 1; i >= 0; i--) {
        gt |= eq & (ina[i] > inb[i]);
        eq &= ina[i] == inb[i];
    }

    return 2 * gt + eq - 1;
}

static inline void _bint_condsub_p(uint64_t *io) {
    bool geq = _bint_compare(io, p) >= 0;
    uint64_t tmp;
    uint64_t c = 0;
    for (int i = 0; i < NWORDS; i++) {
        tmp = io[i] + mp[i] + c;
        io[i] = geq ? tmp : io[i];
        c = io[i] >> BITS_PER_WORD;
        io[i] &= LO_MASK;
    }
}

static inline void _bint_monty_help(uint64_t *out, uint64_t *tmp) {
    uint64_t tmp2[2 * NWORDS], tmp3[NWORDS], c = 0;

    _bint_mul_low(tmp3, tmp, pP);  // m = (T mod R) N' mod R
    _bint_mul(tmp2, tmp3, p);      // mN

    for (int i = 0; i < NWORDS; i++) {
        tmp[i] = tmp[i] + tmp2[i] + c;
        c = tmp[i] >> BITS_PER_WORD;
    }

    uint64_t *htmp = tmp + NWORDS;
    uint64_t *htmp2 = tmp2 + NWORDS;
    for (int i = 0; i < NWORDS; i++) {
        out[i] = htmp[i] + htmp2[i] + c;
        c = out[i] >> BITS_PER_WORD;
        out[i] &= LO_MASK;
    }
}

void bint_add(uint64_t *out, const uint64_t *ina, const uint64_t *inb) {
    for (int i = 0; i < NWORDS; i++) {
        out[i] = ina[i] + inb[i];
    }
}

void bint_sub(uint64_t *out, const uint64_t *ina, const uint64_t *inb, int bup) {
    for (int i = 0; i < NWORDS; i++) {
        out[i] = ina[i] + (p[i] << bup) - inb[i];
    }
}

void bint_lsh(uint64_t *out, const uint64_t *in, int sh) {
    for (int i = 0; i < NWORDS; i++) {
        out[i] = in[i] << sh;
    }
}

void bint_mul(uint64_t *out, const uint64_t *ina, const uint64_t *inb) {
    uint64_t tmp[2 * NWORDS];
    _bint_mul(tmp, ina, inb);  // T = xy
    _bint_monty_help(out, tmp);
}

void bint_sqr(uint64_t *out, const uint64_t *ina) {
    uint64_t tmp[2 * NWORDS];
    _bint_sqr(tmp, ina);  // T = xx
    _bint_monty_help(out, tmp);
}

void bint_redc(uint64_t *out, const uint64_t *in) { bint_mul(out, in, r); }

void bint_set1(uint64_t *out) {
    for (int i = 0; i < NWORDS; i++) {
        out[i] = r[i];
    }
}

static inline void _bint_to_monty(uint64_t *out, const uint64_t *in) { bint_mul(out, in, rSq); }

static inline void _bint_from_monty(uint64_t *out, const uint64_t *in) {
    uint64_t tmp2[2 * NWORDS], tmp3[NWORDS], c;

    _bint_mul_low(tmp3, in, pP);  // m = (T mod R) N' mod R
    _bint_mul(tmp2, tmp3, p);     // mN

    c = 0;  // (T + mN)
    for (int i = 0; i < NWORDS; i++) {
        tmp2[i] = in[i] + tmp2[i] + c;
        c = tmp2[i] >> BITS_PER_WORD;
    }
    uint64_t *htmp2 = tmp2 + NWORDS;
    for (int i = 0; i < NWORDS - 1; i++) {
        out[i] = htmp2[i] + c;
        c = out[i] >> BITS_PER_WORD;
        out[i] &= LO_MASK;
    }
    out[NWORDS - 1] = htmp2[NWORDS - 1] + c;
    _bint_condsub_p(out);
}

#define MUL_LOOP(A, B, C, D)                                          \
    do {                                                              \
        for (int i = (A); i < (B); i++) {                             \
            for (int j = (C); j < (D); j++) {                         \
                tmp += (__uint128_t)ina[j] * (__uint128_t)inb[i - j]; \
            }                                                         \
            out[i] = ((uint64_t)tmp) & LO_MASK;                       \
            tmp = tmp >> BITS_PER_WORD;                               \
        }                                                             \
    } while (0)

static inline void _bint_mul(uint64_t *out, const uint64_t *ina, const uint64_t *inb) {
    __uint128_t tmp = 0;
    MUL_LOOP(0, NWORDS, 0, i + 1);
    MUL_LOOP(NWORDS, 2 * NWORDS - 1, i + 1 - NWORDS, NWORDS);
    out[2 * NWORDS - 1] = (uint64_t)tmp;
}

static inline void _bint_mul_low(uint64_t *out, const uint64_t *ina, const uint64_t *inb) {
    __uint128_t tmp = 0;
    MUL_LOOP(0, NWORDS, 0, i + 1);
}
#undef MUL_LOOP

static inline void _bint_sqr(uint64_t *out, const uint64_t *ina) {
    __uint128_t tmp = 0;
    uint64_t tmp2 = 0;

    tmp = (__uint128_t)ina[0] * (__uint128_t)ina[0];
    out[0] = ((uint64_t)tmp) & LO_MASK;
    tmp = tmp >> BITS_PER_WORD;

    for (unsigned i = 1; i < NWORDS; i++) {
        for (unsigned j = 0; j < (i + 1) / 2; j++) {
            tmp2 = ina[j] << 1;
            tmp += (__uint128_t)tmp2 * (__uint128_t)ina[i - j];
        }

        if (i % 2 == 0) {
            tmp += (__uint128_t)ina[i / 2] * (__uint128_t)ina[i / 2];
        }

        out[i] = ((uint64_t)tmp) & LO_MASK;
        tmp = tmp >> BITS_PER_WORD;
    }

    for (unsigned k = 1; k < NWORDS - 1; k++) {
        unsigned i = NWORDS + k - 1;
        for (unsigned j = 0; j < (NWORDS - k) / 2; j++) {
            tmp2 = ina[j + k] << 1;
            tmp += (__uint128_t)tmp2 * (__uint128_t)ina[i - j - k];
        }

        if (i % 2 == 0) {
            tmp += (__uint128_t)ina[i / 2] * (__uint128_t)ina[i / 2];
        }

        out[i] = ((uint64_t)tmp) & LO_MASK;
        tmp = tmp >> BITS_PER_WORD;
    }

    tmp += (__uint128_t)ina[NWORDS - 1] * (__uint128_t)ina[NWORDS - 1];
    out[2 * NWORDS - 2] = ((uint64_t)tmp) & LO_MASK;
    out[2 * NWORDS - 1] = tmp >> BITS_PER_WORD;
}

void bint_import_mpz(uint64_t *out, const mpz_t in) {
    mpz_export(out, NULL, -1, 8, 0, 64 - BITS_PER_WORD, in);
    _bint_to_monty(out, out);
}

void bint_export_mpz(mpz_t out, const uint64_t *in) {
    uint64_t tmp[NWORDS];
    _bint_from_monty(tmp, in);
    mpz_import(out, NWORDS, -1, 8, 0, 64 - BITS_PER_WORD, tmp);
}
