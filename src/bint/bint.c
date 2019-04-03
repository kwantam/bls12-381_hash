// bls12-381 bigint
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "bint_internal.h"

// compare ina and inb, returning -1, 0, 1 for <, =, > respectively
static inline int _bint_compare(const int64_t *ina, const int64_t *inb) {
    bool gt = false;
    bool eq = true;
    for (int i = NWORDS - 1; i >= 0; i--) {
        gt |= eq & (ina[i] > inb[i]);
        eq &= ina[i] == inb[i];
    }

    return 2 * gt + eq - 1;
}

// conditionally subtract p from io
static inline void _bint_condsub_p(int64_t *io) {
    bool geq = _bint_compare(io, p) >= 0;
    int64_t c = 0;
    for (int i = 0; i < NWORDS; ++i) {
        int64_t tmp = io[i] + mp[i] + c;
        io[i] = geq ? tmp : io[i];
        c = io[i] >> BITS_PER_WORD;
        io[i] &= LO_MASK;
    }
}

// conditionally subtract p from io and compare to cmpval (which must be fully reduced!)
static inline bool _bint_condsub_p_eq(int64_t *io, const int64_t *cmpval) {
    bool match = true;
    bool geq = _bint_compare(io, p) >= 0;
    int64_t tmp;
    int64_t c = 0;
    for (int i = 0; i < NWORDS; ++i) {
        tmp = io[i] + mp[i] + c;
        io[i] = geq ? tmp : io[i];
        c = io[i] >> BITS_PER_WORD;
        io[i] &= LO_MASK;
        match &= io[i] == cmpval[i];
    }
    return match;
}

bool bint_eq0(int64_t *io) { return _bint_condsub_p_eq(io, zero); }

bool bint_is_neg(const int64_t *in) {
    int64_t tmp[NWORDS];
    _bint_from_monty(tmp, in);
    return _bint_compare(tmp, pOver2) == 1;
}

static inline void _bint_monty_help(int64_t *out, int64_t *tmp) {
    int64_t tmp2[2 * NWORDS], tmp3[NWORDS], c = 0;

    _bint_mul_low(tmp3, tmp, pP);  // m = (T mod R) N' mod R
    _bint_mul(tmp2, tmp3, p);      // mN

    for (int i = 0; i < NWORDS; ++i) {
        tmp[i] = tmp[i] + tmp2[i] + c;
        c = tmp[i] >> BITS_PER_WORD;
    }

    int64_t *htmp = tmp + NWORDS;
    int64_t *htmp2 = tmp2 + NWORDS;
    for (int i = 0; i < NWORDS; ++i) {
        out[i] = htmp[i] + htmp2[i] + c;
        c = out[i] >> BITS_PER_WORD;
        out[i] &= LO_MASK;
    }
}

void bint_add(int64_t *out, const int64_t *ina, const int64_t *inb) {
    for (int i = 0; i < NWORDS; ++i) {
        out[i] = ina[i] + inb[i];
    }
}

void bint_sub(int64_t *out, const int64_t *ina, const int64_t *inb, int bup) {
    for (int i = 0; i < NWORDS; ++i) {
        out[i] = ina[i] + (p[i] << bup) - inb[i];
    }
}

void bint_neg(int64_t *out, const int64_t *in, int bup) {
    for (int i = 0; i < NWORDS; ++i) {
        out[i] = (p[i] << bup) - in[i];
    }
}

void bint_lsh(int64_t *out, const int64_t *in, int sh) {
    for (int i = 0; i < NWORDS; ++i) {
        out[i] = in[i] << sh;
    }
}

void bint_condswap(int64_t *__restrict__ in1, int64_t *__restrict__ in2, const bool do_swap) {
    uint64_t swap_mask = 0LL - ((uint64_t)do_swap);
    for (int i = 0; i < NWORDS; ++i) {
        uint64_t word_mask = (in1[i] ^ in2[i]) & swap_mask;
        in1[i] = in1[i] ^ word_mask;
        in2[i] = in2[i] ^ word_mask;
    }
}

void bint_condassign(int64_t *out, const bool first, const int64_t *in1, const int64_t *in2) {
    uint64_t mask1 = 0LL - ((uint64_t)first);
    uint64_t mask2 = ~mask1;
    for (int i = 0; i < NWORDS; ++i) {
        out[i] = (in1[i] & mask1) | (in2[i] & mask2);
    }
}

void bint_mul(int64_t *out, const int64_t *ina, const int64_t *inb) {
    int64_t tmp[2 * NWORDS];
    _bint_mul(tmp, ina, inb);  // T = xy
    _bint_monty_help(out, tmp);
}

void bint_sqr(int64_t *out, const int64_t *ina) {
    int64_t tmp[2 * NWORDS];
    _bint_sqr(tmp, ina);  // T = xx
    _bint_monty_help(out, tmp);
}

void bint_redc(int64_t *out, const int64_t *in) { bint_mul(out, in, r); }

void bint_set1(int64_t *out) {
    for (int i = 0; i < NWORDS; ++i) {
        out[i] = r[i];
    }
}

static inline void _bint_to_monty(int64_t *out, const int64_t *in) { bint_mul(out, in, rSq); }

static inline void _bint_from_monty(int64_t *out, const int64_t *in) {
    int64_t tmp2[2 * NWORDS], tmp3[NWORDS], c;

    _bint_mul_low(tmp3, in, pP);  // m = (T mod R) N' mod R
    _bint_mul(tmp2, tmp3, p);     // mN

    c = 0;  // (T + mN)
    for (int i = 0; i < NWORDS; ++i) {
        tmp2[i] = in[i] + tmp2[i] + c;
        c = tmp2[i] >> BITS_PER_WORD;
    }
    int64_t *htmp2 = tmp2 + NWORDS;
    for (int i = 0; i < NWORDS - 1; ++i) {
        out[i] = htmp2[i] + c;
        c = out[i] >> BITS_PER_WORD;
        out[i] &= LO_MASK;
    }
    out[NWORDS - 1] = htmp2[NWORDS - 1] + c;
    _bint_condsub_p(out);
}

#define MUL_LOOP(A, B, C, D)                                        \
    do {                                                            \
        for (int i = (A); i < (B); ++i) {                           \
            for (int j = (C); j < (D); j++) {                       \
                tmp += (__int128_t)ina[j] * (__int128_t)inb[i - j]; \
            }                                                       \
            out[i] = ((int64_t)tmp) & LO_MASK;                      \
            tmp = tmp >> BITS_PER_WORD;                             \
        }                                                           \
    } while (0)

static inline void _bint_mul(int64_t *out, const int64_t *ina, const int64_t *inb) {
    __int128_t tmp = 0;
    MUL_LOOP(0, NWORDS, 0, i + 1);
    MUL_LOOP(NWORDS, 2 * NWORDS - 1, i + 1 - NWORDS, NWORDS);
    out[2 * NWORDS - 1] = (int64_t)tmp;
}

static inline void _bint_mul_low(int64_t *out, const int64_t *ina, const int64_t *inb) {
    __int128_t tmp = 0;
    MUL_LOOP(0, NWORDS, 0, i + 1);
}
#undef MUL_LOOP

static inline void _bint_sqr(int64_t *out, const int64_t *ina) {
    __int128_t tmp = 0;
    int64_t tmp2 = 0;

    tmp = (__int128_t)ina[0] * (__int128_t)ina[0];
    out[0] = ((int64_t)tmp) & LO_MASK;
    tmp = tmp >> BITS_PER_WORD;

    for (unsigned i = 1; i < NWORDS; ++i) {
        for (unsigned j = 0; j < (i + 1) / 2; j++) {
            tmp2 = ina[j] << 1;
            tmp += (__int128_t)tmp2 * (__int128_t)ina[i - j];
        }

        if (i % 2 == 0) {
            tmp += (__int128_t)ina[i / 2] * (__int128_t)ina[i / 2];
        }

        out[i] = ((int64_t)tmp) & LO_MASK;
        tmp = tmp >> BITS_PER_WORD;
    }

    for (unsigned k = 1; k < NWORDS - 1; k++) {
        unsigned i = NWORDS + k - 1;
        for (unsigned j = 0; j < (NWORDS - k) / 2; j++) {
            tmp2 = ina[j + k] << 1;
            tmp += (__int128_t)tmp2 * (__int128_t)ina[i - j - k];
        }

        if (i % 2 == 0) {
            tmp += (__int128_t)ina[i / 2] * (__int128_t)ina[i / 2];
        }

        out[i] = ((int64_t)tmp) & LO_MASK;
        tmp = tmp >> BITS_PER_WORD;
    }

    tmp += (__int128_t)ina[NWORDS - 1] * (__int128_t)ina[NWORDS - 1];
    out[2 * NWORDS - 2] = ((int64_t)tmp) & LO_MASK;
    out[2 * NWORDS - 1] = tmp >> BITS_PER_WORD;
}

void bint_import_mpz(int64_t *out, const mpz_t in) {
    size_t count = 0;
    mpz_export(out, &count, -1, 8, 0, 64 - BITS_PER_WORD, in);
    // clear remaining words, if any
    for (; count < NWORDS; ++count) {
        out[count] = 0;
    }
    _bint_to_monty(out, out);
}

void bint_export_mpz(mpz_t out, const int64_t *in) {
    int64_t tmp[NWORDS];
    _bint_from_monty(tmp, in);
    mpz_import(out, NWORDS, -1, 8, 0, 64 - BITS_PER_WORD, tmp);
}

bool bint_divsqrt(int64_t *__restrict__ out, const int64_t *u, const int64_t *v, const bool force) {
    int64_t uvk1[NWORDS], uvk2[NWORDS];
    bint_mul(uvk1, u, v);        // uv
    bint_sqr(uvk2, v);           // v^2
    bint_mul(uvk2, uvk2, uvk1);  // uv^3
    divsqrt_chain(out, uvk2);    // (uv^3)^((p-3)/4)
    bint_mul(out, out, uvk1);    // uv(uv^3)^((p-3)/4)

    // don't check for equality if we're asked to force
    if (force) {
        return true;
    }

    // completely reduce u for comparison
    bint_redc(uvk1, u);
    _bint_condsub_p(uvk1);

    bint_sqr(uvk2, out);      // out^2
    bint_mul(uvk2, uvk2, v);  // v * out^2
    return _bint_condsub_p_eq(uvk2, uvk1);
}
