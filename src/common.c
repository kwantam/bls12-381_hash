#include "common.h"

#include "bls12_381_consts.h"

void hash_stdin(SHA256_CTX *ctx) {
    char buf[RDBUF_SIZE];

    while (true) {
        ssize_t nread = read(0, buf, RDBUF_SIZE);
        if (nread < 0) {
            perror("Reading stdin");
            exit(1);
        }
        if (nread == 0) {
            break;
        }

        CHECK_CRYPTO(SHA256_Update(ctx, buf, nread));
    }
}

void next_prng(EVP_CIPHER_CTX *cctx, const SHA256_CTX *hctx, uint32_t idx) {
    // make a copy of the context
    SHA256_CTX lctx;
    memcpy(&lctx, hctx, sizeof(lctx));

    // append the index and compute the hash
    uint32_t idx_bytes = htole32(idx);  // fix endianness of counter
    CHECK_CRYPTO(SHA256_Update(&lctx, &idx_bytes, sizeof(idx_bytes)));

    unsigned char key_iv[SHA256_DIGEST_LENGTH];
    CHECK_CRYPTO(SHA256_Final(key_iv, &lctx));

    EVP_CIPHER_CTX_reset(cctx);
    EVP_EncryptInit(cctx, EVP_aes_128_ctr(), key_iv, key_iv + 16);
}

void show_buf(FILE *of, const unsigned char *buf, size_t len) {
    for (unsigned i = 0; i < len; ++i) {
        fprintf(of, "%02hhx", buf[i]);
    }
    fprintf(of, "\n");
}

bool lt_be(const unsigned char *a, const unsigned char *b, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        if (a[i] < b[i]) {
            return true;
        }
        if (a[i] > b[i]) {
            return false;
        }
    }
    return false;
}

// clang-format off
static unsigned char ZEROS[P_LEN] = {0,};
// clang-format on
static bool next_com(EVP_CIPHER_CTX *cctx, unsigned char *out, size_t len, const unsigned char *max,
                     unsigned char mask) {
    int outl = len;
    EVP_EncryptUpdate(cctx, out, &outl, ZEROS, len);
    out[0] &= mask;
    if (lt_be(out, max, len)) {
        return false;
    }

    return true;
}

void next_modp(EVP_CIPHER_CTX *cctx, mpz_t ret) {
    unsigned char p_out[P_LEN];
    while (next_com(cctx, p_out, P_LEN, BLS12_381_p, 0x1f)) {
    }
    mpz_import(ret, P_LEN, 1, 1, 1, 0, p_out);
}

void next_modq(EVP_CIPHER_CTX *cctx, mpz_t ret) {
    unsigned char q_out[Q_LEN];
    while (next_com(cctx, q_out, Q_LEN, BLS12_381_q, 0x73)) {
    }
    mpz_import(ret, Q_LEN, 1, 1, 1, 0, q_out);
}

static mpz_t mpz_bls12_381_p;
mpz_t *get_p(void) {
    common_init();
    return &mpz_bls12_381_p;
}

static mpz_t mpz_bls12_381_q;
mpz_t *get_q(void) {
    common_init();
    return &mpz_bls12_381_q;
}

static bool init_done = false;
void common_init(void) {
    if (!init_done) {
        init_done = true;
        mpz_init(mpz_bls12_381_q);
        mpz_import(mpz_bls12_381_q, Q_LEN, 1, 1, 1, 0, BLS12_381_q);
        mpz_init(mpz_bls12_381_p);
        mpz_import(mpz_bls12_381_p, P_LEN, 1, 1, 1, 0, BLS12_381_p);
    }
}

void common_uninit(void) {
    if (init_done) {
        init_done = false;
        mpz_clear(mpz_bls12_381_q);
        mpz_clear(mpz_bls12_381_p);
    }
}
