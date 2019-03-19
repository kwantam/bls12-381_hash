#include "common.h"

#include "bint.h"
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
    uint64_t X[BINT_NWORDS];
    uint64_t Y[BINT_NWORDS];
    uint64_t Z[BINT_NWORDS];
};
#define NUM_TMP_JP 8
static struct jac_point jp_tmp[NUM_TMP_JP];

#define NUM_TMP_BINT 11
static uint64_t bint_tmp[NUM_TMP_BINT][BINT_NWORDS];
static mpz_t mpz_tmp;
static bool init_done = false;
void common_init(void) {
    if (!init_done) {
        init_done = true;
        mpz_init(mpz_bls12_381_q);
        mpz_init(mpz_bls12_381_p);
        mpz_init(mpz_tmp);
        mpz_import(mpz_bls12_381_q, Q_LEN, 1, 1, 1, 0, BLS12_381_q);
        mpz_import(mpz_bls12_381_p, P_LEN, 1, 1, 1, 0, BLS12_381_p);
    }
}

void common_uninit(void) {
    if (init_done) {
        init_done = false;
        mpz_clear(mpz_bls12_381_q);
        mpz_clear(mpz_bls12_381_p);
        mpz_clear(mpz_tmp);
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
    bint_sqr(bint_tmp[0], in->X);                           // A = X^2                      v = 2   w = 1
    bint_sqr(bint_tmp[1], in->Y);                           // B = Y^2                      v = 2   w = 1
    bint_sqr(bint_tmp[2], bint_tmp[1]);                     // C = B^2                      v = 2   w = 1

    bint_add(bint_tmp[3], in->X, bint_tmp[1]);              // X + B                        v = 4   w = 2
    bint_sqr(bint_tmp[3], bint_tmp[3]);                     // (X + B)^2                    v = 2   w = 1
    bint_add(bint_tmp[4], bint_tmp[0], bint_tmp[2]);        // (A + C)                      v = 4   w = 2
    bint_sub(bint_tmp[3], bint_tmp[3], bint_tmp[4], 3);     // (X + B)^2 - A - C            v = 10  w = 9
    bint_lsh(bint_tmp[3], bint_tmp[3], 1);                  // D = 2 * ((X + B)^2 - A - C)  v = 20  w = 18

    bint_lsh(bint_tmp[4], bint_tmp[0], 1);                  // 2 * A                        v = 4   w = 2
    bint_add(bint_tmp[4], bint_tmp[4], bint_tmp[0]);        // E = 3 * A                    v = 6   w = 3

    bint_sqr(bint_tmp[5], bint_tmp[4]);                     // F = E^2                      v = 2   w = 1

    bint_lsh(bint_tmp[6], bint_tmp[3], 1);                  // 2 * D                        v = 40  w = 36
    bint_sub(bint_tmp[6], bint_tmp[5], bint_tmp[6], 6);     // F - 2 * D                    v = 66  w = 65
    bint_redc(out->X, bint_tmp[6]);                         // X3 = F - 2 * D               v = 2   w = 1

    bint_lsh(bint_tmp[6], in->Z, 1);                        // 2 * Z                        v = 4   w = 2
    bint_mul(out->Z, bint_tmp[6], in->Y);                   // 2 * Z * Y                    v = 2   w = 1

    bint_lsh(bint_tmp[2], bint_tmp[2], 3);                  // 8 * C                        v = 16  w = 8
    bint_sub(bint_tmp[6], bint_tmp[3], out->X, 2);          // D - X3                       v = 24  w = 22
    bint_mul(bint_tmp[6], bint_tmp[6], bint_tmp[4]);        // E * (D - X3)                 v = 2   w = 1
    bint_sub(bint_tmp[6], bint_tmp[6], bint_tmp[2], 5);     // E * (D - X3) - 8 * C         v = 34  w = 33
    bint_redc(out->Y, bint_tmp[6]);                         // Y = E * (D - X3) - 8 * C     v = 2   w = 1
}

// add two points in Jacobian coordinates
// out == in1 or out == in2 is OK
// from EFD: https://www.hyperelliptic.org/EFD/g1p/auto-shortw-jacobian-0.html#addition-add-2007-bl
static void point_add(struct jac_point *out, const struct jac_point *in1, const struct jac_point *in2) {
    bint_sqr(bint_tmp[0], in1->Z);                          // Z1Z1 = Z1^2                  v = 2   w = 1

    bint_sqr(bint_tmp[1], in2->Z);                          // Z2Z2 = Z2^2                  v = 2   w = 1

    bint_mul(bint_tmp[2], bint_tmp[1], in1->X);             // U1 = X1 * Z2Z2               v = 2   w = 1
    bint_mul(bint_tmp[3], bint_tmp[0], in2->X);             // U2 = X2 * Z1Z1               v = 2   w = 1

    bint_mul(bint_tmp[4], in1->Y, in2->Z);                  // Y1 * Z2                      v = 2   w = 1
    bint_mul(bint_tmp[4], bint_tmp[4], bint_tmp[1]);        // S1 = Y1 * Z2 * Z2Z2          v = 2   w = 1

    bint_mul(bint_tmp[5], in2->Y, in1->Z);                  // Y2 * Z1                      v = 2   w = 1
    bint_mul(bint_tmp[5], bint_tmp[5], bint_tmp[0]);        // S2 = Y2 * Z1 * Z1Z1          v = 2   w = 1

    bint_sub(bint_tmp[6], bint_tmp[3], bint_tmp[2], 2);     // H = U2 - U1                  v = 6   w = 5

    bint_lsh(bint_tmp[7], bint_tmp[6], 1);                  // 2 * H                        v = 12  w = 10
    bint_sqr(bint_tmp[7], bint_tmp[7]);                     // I = (2 * H)^2                v = 2   w = 1

    bint_mul(bint_tmp[8], bint_tmp[7], bint_tmp[6]);        // J = H * I                    v = 2   w = 1

    bint_sub(bint_tmp[9], bint_tmp[5], bint_tmp[4], 2);     // S2 - S1                      v = 6   w = 5
    bint_lsh(bint_tmp[9], bint_tmp[9], 1);                  // r = 2 * (S2 - S1)            v = 12  w = 10

    bint_mul(bint_tmp[10], bint_tmp[2], bint_tmp[7]);       // V = U1 * I                   v = 2   w = 1

    bint_lsh(out->X, bint_tmp[10], 1);                      // 2 * V                        v = 4   w = 2
    bint_add(out->X, out->X, bint_tmp[8]);                  // J + 2 * V                    v = 6   w = 3
    bint_sqr(bint_tmp[7], bint_tmp[9]);                     // r^2                          v = 2   w = 1
    bint_sub(out->X, bint_tmp[7], out->X, 4);               // r^2 - J - 2 * V              v = 18  w = 17
    bint_redc(out->X, out->X);                              // X3 = r^2 - J - 2 * V         v = 2   w = 1

    bint_lsh(bint_tmp[4], bint_tmp[4], 1);                  // 2 * S1                       v = 4   w = 2
    bint_mul(bint_tmp[4], bint_tmp[4], bint_tmp[8]);        // 2 * S1 * J                   v = 2   w = 1
    bint_sub(out->Y, bint_tmp[10], out->X, 2);              // V - X3                       v = 6   w = 5
    bint_mul(out->Y, out->Y, bint_tmp[9]);                  // r * (V - X3)                 v = 2   w = 1
    bint_sub(out->Y, out->Y, bint_tmp[4], 2);               // r * (V - X3) - 2 * S1 * J    v = 6   w = 5
    // v=6 w=5 is small enough that we can skip reduction

    bint_add(out->Z, in1->Z, in2->Z);                       // Z1 + Z2                      v = 4   w = 2
    bint_sqr(out->Z, out->Z);                               // (Z1 + Z2)^2                  v = 2   w = 1
    bint_add(bint_tmp[0], bint_tmp[0], bint_tmp[1]);        // Z1Z1 + Z2Z2                  v = 4   w = 2
    bint_sub(out->Z, out->Z, bint_tmp[0], 3);               // (Z1 + Z2)^2 - Z1Z1 - Z2Z2    v = 10  w = 9
    bint_mul(out->Z, out->Z, bint_tmp[6]);                  // Z3 = 2 * Z1 * Z2 * H         v = 2   w = 1
}

// clear BLS12-381 G1 cofactor
// outX == inX and/or outY == inY is OK
// Addition chain: Bos-Coster (win=7) : 147 links, 8 variables
// TODO: is there a faster addition-subtraction chain?
void clear_cofactor(mpz_t outX, mpz_t outY, const mpz_t inX, const mpz_t inY) {
    bint_import_mpz(jp_tmp[1].X, inX);
    bint_import_mpz(jp_tmp[1].Y, inY);
    bint_set1(jp_tmp[1].Z);

    point_double(jp_tmp + 3, jp_tmp + 1);
    point_add(jp_tmp + 2, jp_tmp + 3, jp_tmp + 1);
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

    bint_export_mpz(outX, jp_tmp[7].X);
    bint_export_mpz(outY, jp_tmp[7].Y);
    bint_export_mpz(mpz_tmp, jp_tmp[7].Z);

    mpz_invert(mpz_tmp, mpz_tmp, mpz_bls12_381_p);          // Z^-1
    mul_modp(outY, outY, mpz_tmp);                          // Y / Z
    sqr_modp(mpz_tmp, mpz_tmp);                             // Z^-2
    mul_modp(outY, outY, mpz_tmp);                          // Y / Z^3
    mul_modp(outX, outX, mpz_tmp);                          // X / Z^2
}

struct cmdline_opts get_cmdline_opts(int argc, char **argv) {
    struct cmdline_opts ret = {0, true, false};
    int opt_ret;
    bool found_err = false;
    while ((opt_ret = getopt(argc, argv, "n:Cq")) >= 0) {
        switch (opt_ret) {
            case 'n':
                ret.nreps = atoi(optarg);  // NOLINT(cert-err34-c)
                break;

            case 'C':
                ret.clear_h = false;
                break;

            case 'q':
                ret.quiet = true;
                break;

            default:
                found_err = true;
        }
        if (found_err) {
            break;
        }
    }
    if (found_err || optind != argc) {
        printf("Usage: %s [-n <npoints>] [-q] [-C]\n", argv[0]);
        exit(1);
    }
    if (ret.nreps == 0) {
        ret.nreps = 16;
    }

    return ret;
}
