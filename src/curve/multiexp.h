// multiexp macros
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__multiexp_h__

#define BINT_MEXP_PRECOMP(NUM)                                                                                        \
    jac_point##NUM bint##NUM##_precomp[4][4];                                                                         \
    void precomp##NUM##_init(void) {                                                                                  \
        memcpy(bint##NUM##_precomp[0][1].X, g##NUM##_prime_x, sizeof(g##NUM##_prime_x));                              \
        memcpy(bint##NUM##_precomp[0][1].Y, g##NUM##_prime_y, sizeof(g##NUM##_prime_y));                              \
        memset(bint##NUM##_precomp[0][1].Z, 0, sizeof(bint##NUM##_precomp[0][1].Z));                                  \
        bint_set1(bint##NUM##_precomp[0][1].Z);                                                                       \
        point##NUM##_double(&bint##NUM##_precomp[0][2], &bint##NUM##_precomp[0][1]);                                  \
        point##NUM##_add(&bint##NUM##_precomp[0][3], &bint##NUM##_precomp[0][2], &bint##NUM##_precomp[0][1]);         \
    }                                                                                                                 \
                                                                                                                      \
    void precomp##NUM##_finish(const jac_point##NUM *in) {                                                            \
        if (in != NULL) {                                                                                             \
            memcpy(&bint##NUM##_precomp[1][0], in, sizeof(jac_point##NUM));                                           \
        }                                                                                                             \
        point##NUM##_double(&bint##NUM##_precomp[2][0], &bint##NUM##_precomp[1][0]);                                  \
        point##NUM##_add(&bint##NUM##_precomp[3][0], &bint##NUM##_precomp[2][0], &bint##NUM##_precomp[1][0]);         \
                                                                                                                      \
        for (unsigned i = 1; i < 4; ++i) {                                                                            \
            for (unsigned j = 1; j < 4; ++j) {                                                                        \
                point##NUM##_add(&bint##NUM##_precomp[i][j], &bint##NUM##_precomp[i][0], &bint##NUM##_precomp[0][j]); \
            }                                                                                                         \
        }                                                                                                             \
    }

#define BINT_MEXP_FUNCTION(NUM, ZVAL, VISIBILITY, DUMMY)                                                         \
    VISIBILITY void addrG##NUM##_clear_h##NUM##_help(const uint8_t *r, const bool constant_time) {               \
        {                                                                                                        \
            const uint8_t h_idx = BLS12_381_##ZVAL[0] >> 6; /* definitely not 0 */                               \
            const uint8_t r_idx = r[0] >> 6;                                                                     \
            memcpy(jp##NUM##_tmp, &bint##NUM##_precomp[h_idx][r_idx], sizeof(jac_point##NUM));                   \
        }                                                                                                        \
        for (unsigned idx = 0; idx < ZM1_LEN; ++idx) {                                                           \
            /* 0th iteration, we've already done the above copy */                                               \
            uint8_t mask = (idx == 0) ? 0x30 : 0xc0;                                                             \
            uint8_t shift = (idx == 0) ? 4 : 6;                                                                  \
            for (; mask != 0; mask = mask >> 2, shift -= 2) {                                                    \
                point##NUM##_double(jp##NUM##_tmp, jp##NUM##_tmp);                                               \
                point##NUM##_double(jp##NUM##_tmp, jp##NUM##_tmp);                                               \
                                                                                                                 \
                const uint8_t h_idx = (BLS12_381_##ZVAL[idx] & mask) >> shift;                                   \
                const uint8_t r_idx = (r[idx] & mask) >> shift;                                                  \
                if ((h_idx | r_idx) != 0) {                                                                      \
                    point##NUM##_add(jp##NUM##_tmp, jp##NUM##_tmp, &bint##NUM##_precomp[h_idx][r_idx]);          \
                } else if (constant_time) {                                                                      \
                    point##NUM##_add(jp##NUM##_tmp + (DUMMY), jp##NUM##_tmp + (DUMMY), jp##NUM##_tmp + (DUMMY)); \
                }                                                                                                \
            }                                                                                                    \
        }                                                                                                        \
    }

#define __bls_hash__src__curve__multiexp_h__
#endif  // __bls_hash__src__curve__multiexp_h__
