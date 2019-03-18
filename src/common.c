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
    return &mpz_bls12_381_p;
}

static mpz_t mpz_bls12_381_q;
mpz_t *get_q(void) {
    return &mpz_bls12_381_q;
}

struct jac_point {
    mpz_t X;
    mpz_t Y;
    mpz_t Z;
};

static void init_jac_point(struct jac_point *p) {
    mpz_init(p->X);
    mpz_init(p->Y);
    mpz_init(p->Z);
}

static void init_jac_point_xy(struct jac_point *p, const mpz_t x, const mpz_t y) {
    mpz_init_set(p->X, x);
    mpz_init_set(p->Y, y);
    mpz_init_set_ui(p->Z, 1);
}

static void clear_jac_point(struct jac_point *p) {
    mpz_clear(p->X);
    mpz_clear(p->Y);
    mpz_clear(p->Z);
}

#define NUM_TMP_MPZ 11
#define NUM_TMP_JP 8
static bool init_done = false;
static mpz_t mpz_tmp[NUM_TMP_MPZ];
static struct jac_point jp_tmp[NUM_TMP_JP];
void common_init(void) {
    if (!init_done) {
        init_done = true;
        mpz_init(mpz_bls12_381_q);
        mpz_init(mpz_bls12_381_p);
        mpz_import(mpz_bls12_381_q, Q_LEN, 1, 1, 1, 0, BLS12_381_q);
        mpz_import(mpz_bls12_381_p, P_LEN, 1, 1, 1, 0, BLS12_381_p);
        for (unsigned i = 0; i < NUM_TMP_MPZ; ++i) {
            mpz_init(mpz_tmp[i]);
        }
        for (unsigned i = 0; i < NUM_TMP_JP; ++i) {
            init_jac_point(jp_tmp + i);
        }
    }
}

void common_uninit(void) {
    if (init_done) {
        init_done = false;
        mpz_clear(mpz_bls12_381_q);
        mpz_clear(mpz_bls12_381_p);
        for (unsigned i = 0; i < NUM_TMP_MPZ; ++i) {
            mpz_clear(mpz_tmp[i]);
        }
        for (unsigned i = 0; i < NUM_TMP_JP; ++i) {
            clear_jac_point(jp_tmp + i);
        }
    }
}

static void sqr_modp(mpz_t out, const mpz_t in) {
    mpz_mul(out, in, in);
    mpz_mod(out, out, mpz_bls12_381_p);
}

static void mul_modp(mpz_t out, const mpz_t in1, const mpz_t in2) {
    mpz_mul(out, in1, in2);
    mpz_mod(out, out, mpz_bls12_381_p);
}

// double a point in Jacobian coordinates
// out == in is OK
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#doubling-dbl-2009-l
static void point_double(struct jac_point *out, const struct jac_point *in) {
    sqr_modp(mpz_tmp[0], in->X);                        // A = X^2
    sqr_modp(mpz_tmp[1], in->Y);                        // B = Y^2
    sqr_modp(mpz_tmp[2], mpz_tmp[1]);                   // C = B^2

    mpz_add(mpz_tmp[3], in->X, mpz_tmp[1]);             // X + B
    sqr_modp(mpz_tmp[3], mpz_tmp[3]);                   // (X + B)^2
    mpz_sub(mpz_tmp[3], mpz_tmp[3], mpz_tmp[0]);        // (X + B)^2 - A
    mpz_sub(mpz_tmp[3], mpz_tmp[3], mpz_tmp[2]);        // (X + B)^2 - A - C
    mpz_mul_2exp(mpz_tmp[3], mpz_tmp[3], 1);            // D = 2 * ((X + B)^2 - A - C)

    mpz_mul_ui(mpz_tmp[4], mpz_tmp[0], 3);              // E = 3 * A

    sqr_modp(mpz_tmp[5], mpz_tmp[4]);                   // F = E^2

    mpz_mul_2exp(out->X, mpz_tmp[3], 1);                // 2 * D
    mpz_sub(out->X, mpz_tmp[5], out->X);                // F - 2 * D
    mpz_mod(out->X, out->X, mpz_bls12_381_p);           // X3 = F - 2 * D

    mpz_mul_2exp(out->Z, in->Z, 1);                     // 2 * Z
    mul_modp(out->Z, out->Z, in->Y);                    // Z3 = 2 * Z * Y

    mpz_mul_2exp(mpz_tmp[2], mpz_tmp[2], 3);            // 8 * C
    mpz_sub(out->Y, mpz_tmp[3], out->X);                // D - X3
    mul_modp(out->Y, out->Y, mpz_tmp[4]);               // E * (D - X3)
    mpz_sub(out->Y, out->Y, mpz_tmp[2]);                // E * (D - X3) - 8 * C
    mpz_mod(out->Y, out->Y, mpz_bls12_381_p);           // Y3 = E * (D - X3) - 8 * C
}

// add two points in Jacobian coordinates; requires in2->Z == 1
// out == in1 or out == in2 is OK
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#addition-madd-2007-bl
static void point_add_mixed(struct jac_point *out, const struct jac_point *in1, const struct jac_point *in2) {
    sqr_modp(mpz_tmp[0], in1->Z);                       // Z1Z1 = Z1^2
    mul_modp(mpz_tmp[2], in1->Z, in2->Y);               // Y2 * Z1

    mul_modp(mpz_tmp[1], in2->X, mpz_tmp[0]);           // U2 = X2 * Z1Z1
    mul_modp(mpz_tmp[2], mpz_tmp[2], mpz_tmp[0]);       // S2 = Y2 * Z1 * Z1Z1

    mpz_sub(mpz_tmp[3], mpz_tmp[1], in1->X);            // H = U2 - X1

    sqr_modp(mpz_tmp[4], mpz_tmp[3]);                   // HH = H^2

    mpz_mul_2exp(mpz_tmp[5], mpz_tmp[4], 2);            // I = 4 * HH

    mul_modp(mpz_tmp[6], mpz_tmp[5], mpz_tmp[3]);       // J = H * I

    mpz_sub(mpz_tmp[7], mpz_tmp[2], in1->Y);            // S2 - Y1
    mpz_mul_2exp(mpz_tmp[7], mpz_tmp[7], 1);            // r = 2 * (S2 - Y1)

    mul_modp(mpz_tmp[8], in1->X, mpz_tmp[5]);           // V = X1 * I

    sqr_modp(mpz_tmp[5], mpz_tmp[7]);                   // r^2

    mpz_mul_2exp(out->X, mpz_tmp[8], 1);                // 2 * V
    mpz_add(out->X, out->X, mpz_tmp[6]);                // J + 2 * V
    mpz_sub(out->X, mpz_tmp[5], out->X);                // r^2 - J - 2 * V
    mpz_mod(out->X, out->X, mpz_bls12_381_p);           // X3 = r^2 - J - 2 * V

    mpz_mul_2exp(out->Y, in1->Y, 1);                    // 2 * Y1
    mul_modp(out->Y, out->Y, mpz_tmp[6]);               // 2 * Y1 * J
    mpz_sub(mpz_tmp[8], mpz_tmp[8], out->X);            // V - X3
    mul_modp(mpz_tmp[8], mpz_tmp[8], mpz_tmp[7]);       // r * (V - X3)
    mpz_sub(out->Y, mpz_tmp[8], out->Y);                // r * (V - X3) - 2 * Y1 * J
    mpz_mod(out->Y, out->Y, mpz_bls12_381_p);           // Y3 = r * (V - X3) - 2 * Y1 * J

    mpz_add(out->Z, in1->Z, mpz_tmp[3]);                // Z1 + H
    sqr_modp(out->Z, out->Z);                           // (Z1 + H)^2
    mpz_sub(out->Z, out->Z, mpz_tmp[0]);                // (Z1 + H)^2 - Z1Z1
    mpz_sub(out->Z, out->Z, mpz_tmp[4]);                // (Z1 + H)^2 - Z1Z1 - HH
    mpz_mod(out->Z, out->Z, mpz_bls12_381_p);           // Z3 = (Z1 + H)^2 - Z1Z1 - HH
}

// add two points in Jacobian coordinates
// out == in1 or out == in2 is OK
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#addition-add-2007-bl
static void point_add(struct jac_point *out, const struct jac_point *in1, const struct jac_point *in2) {
    sqr_modp(mpz_tmp[0], in1->Z);                       // Z1Z1 = Z1^2

    sqr_modp(mpz_tmp[1], in2->Z);                       // Z2Z2 = Z2^2

    mul_modp(mpz_tmp[2], mpz_tmp[1], in1->X);           // U1 = X1 * Z2Z2
    mul_modp(mpz_tmp[3], mpz_tmp[0], in2->X);           // U2 = X2 * Z1Z1

    mul_modp(mpz_tmp[4], in1->Y, in2->Z);               // Y1 * Z2
    mul_modp(mpz_tmp[4], mpz_tmp[4], mpz_tmp[1]);       // S1 = Y1 * Z2 * Z2Z2

    mul_modp(mpz_tmp[5], in2->Y, in1->Z);               // Y2 * Z1
    mul_modp(mpz_tmp[5], mpz_tmp[5], mpz_tmp[0]);       // S2 = Y2 * Z1 * Z1Z1

    mpz_sub(mpz_tmp[6], mpz_tmp[3], mpz_tmp[2]);        // H = U2 - U1

    mpz_mul_2exp(mpz_tmp[7], mpz_tmp[6], 1);            // 2 * H
    sqr_modp(mpz_tmp[7], mpz_tmp[7]);                   // I = (2 * H)^2

    mul_modp(mpz_tmp[8], mpz_tmp[7], mpz_tmp[6]);       // J = H * I

    mpz_sub(mpz_tmp[9], mpz_tmp[5], mpz_tmp[4]);        // S2 - S1
    mpz_mul_2exp(mpz_tmp[9], mpz_tmp[9], 1);            // r = 2 * (S2 - S1)

    mul_modp(mpz_tmp[10], mpz_tmp[2], mpz_tmp[7]);      // V = U1 * I

    mpz_mul_2exp(out->X, mpz_tmp[10], 1);               // 2 * V
    mpz_add(out->X, out->X, mpz_tmp[8]);                // J + 2 * V
    sqr_modp(mpz_tmp[7], mpz_tmp[9]);                   // r^2
    mpz_sub(out->X, mpz_tmp[7], out->X);                // r^2 - J - 2 * V
    mpz_mod(out->X, out->X, mpz_bls12_381_p);           // X3 = r^2 - J - 2 * V

    mpz_mul_2exp(mpz_tmp[4], mpz_tmp[4], 1);            // 2 * S1
    mul_modp(mpz_tmp[4], mpz_tmp[4], mpz_tmp[8]);       // 2 * S1 * J
    mpz_sub(out->Y, mpz_tmp[10], out->X);               // V - X3
    mul_modp(out->Y, out->Y, mpz_tmp[9]);               // r * (V - X3)
    mpz_sub(out->Y, out->Y, mpz_tmp[4]);                // r * (V - X3) - 2 * S1 * J
    mpz_mod(out->Y, out->Y, mpz_bls12_381_p);           // Y3 = r * (V - X3) - 2 * S1 * J

    mpz_mul(out->Z, in1->Z, in2->Z);                    // Z1 * Z2
    mpz_mul_2exp(out->Z, out->Z, 1);                    // 2 * Z1 * Z2
    mpz_mul(out->Z, out->Z, mpz_tmp[6]);                // Z3 = 2 * Z1 * Z2 * H
}

// clear BLS12-381 G1 cofactor
// outX == inX and/or outY == inY is OK
// Addition chain: Bos-Coster (win=7) : 147 links, 9 variables
// TODO: is there a faster addition-subtraction chain?
void clear_cofactor(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY) {
    struct jac_point in_v;
    struct jac_point *in = &in_v;
    init_jac_point_xy(in, inX, inY);

    point_double(jp_tmp + 3, in);
    point_add_mixed(jp_tmp + 2, jp_tmp + 3, in);
    point_double(jp_tmp + 5, jp_tmp + 3);
    point_double(jp_tmp + 4, jp_tmp + 5);
    point_add(jp_tmp + 1, jp_tmp + 4, jp_tmp + 2);
    point_double(jp_tmp + 7, jp_tmp + 4);
    point_add(jp_tmp, jp_tmp + 7, jp_tmp + 3);
    point_add(jp_tmp + 6, jp_tmp + 7, jp_tmp + 5);
    point_double(jp_tmp + 7, jp_tmp);
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 6);
    point_add(jp_tmp, jp_tmp + 7, jp_tmp);
    point_add(jp_tmp + 1, jp_tmp, jp_tmp + 1);
    point_add(jp_tmp + 5, jp_tmp + 1, jp_tmp + 5);
    point_add(jp_tmp + 4, jp_tmp + 5, jp_tmp + 4);
    point_add(jp_tmp, jp_tmp + 4, jp_tmp);
    point_add(jp_tmp + 3, jp_tmp, jp_tmp + 3);
    point_add(jp_tmp + 7, jp_tmp + 3, jp_tmp + 7);
    for (int nops = 0; nops < 7; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 5);
    for (int nops = 0; nops < 5; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 2);
    for (int nops = 0; nops < 18; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 1);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp);
    for (int nops = 0; nops < 7; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 4);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 3);
    for (int nops = 0; nops < 5; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 2);
    for (int nops = 0; nops < 17; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 1);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp);
    for (int nops = 0; nops < 23; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp + 1);
    for (int nops = 0; nops < 9; nops++) {
        point_double(jp_tmp + 7, jp_tmp + 7);
    }
    point_add(jp_tmp + 7, jp_tmp + 7, jp_tmp);

    mpz_invert(mpz_tmp[0], jp_tmp[7].Z, mpz_bls12_381_p);   // Z^-1
    sqr_modp(mpz_tmp[1], mpz_tmp[0]);                       // Z^-2
    mul_modp(outX, jp_tmp[7].X, mpz_tmp[1]);                // x = X * Z^-2
    mul_modp(outY, jp_tmp[7].Y, mpz_tmp[1]);                // Y * Z^-2
    mul_modp(outY, outY, mpz_tmp[0]);                       // y = Y * Z^-3
}
