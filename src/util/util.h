// utilities for bls12-381 hashing
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__util__util_h__

#include <gmp.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

// WARNING: COUNT_CYCLES will *NOT* return an accurate result on all machines.
//  To ensure that the correct count is used, you MUST do the following:
//  1. Make sure that your CPU supports the following flags (check /proc/cpuinfo):
//         constant_tsc, nonstop_tsc
//  2. TURN OFF all power saving / frequency scaling (SpeedStep, Turbo Boost, etc.)
//     options, e.g., in your BIOS.
//  3. For good measure, you should probably also turn off hyperthreading.
//
// If you can't do all of the above, you can probably still use the Linux perf
// subsystem to count cycles---it knows how to get cycle count correctly even
// when your machine's clock is varying (by using performance counters instead
// of the timestamp counter). See ../test/run_bench.sh.
//
// NOTE: the recommendation to use CPUID/RDTSC and RDTSCP/CPUID comes from
//       Paoloni, G., "How to Benchmark Code Execution Times on Intel IA-32
//       and IA-64 Instruction Set Architectures." Intel Corp, Sep. 2010.
//       https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-32-ia-64-benchmark-code-execution-paper.pdf
#if COUNT_CYCLES == 1
#include <inttypes.h>
#include <cpuid.h>
#include <x86intrin.h>
#define CYCLE_COUNT_START   \
    unsigned a, b, c, d;    \
    __cpuid(0, a, b, c, d); \
    c_start = __rdtsc()
#define CYCLE_COUNT_END   \
    c_end = __rdtscp(&a); \
    __cpuid(0, a, b, c, d)
#else
#define CYCLE_COUNT_START c_start = 0
#define CYCLE_COUNT_END c_end = 0
#endif

#define CHECK_CRYPTO(C)                                                                        \
    do {                                                                                       \
        if ((C) != 1) {                                                                        \
            fprintf(stderr, "libcrypto error: %s\n", ERR_error_string(ERR_get_error(), NULL)); \
            exit(1);                                                                           \
        }                                                                                      \
    } while (0)

// getting commandline options
struct cmdline_opts {
    unsigned nreps;
    bool quiet;
    bool test;
    bool test2;
    bool field_only;
    bool constant_time;
};
struct cmdline_opts get_cmdline_opts(int argc, char **argv);

// utility (used for testing exceptional cases of SvdW maps to G1)
void mpz_set_pm1(mpz_t out);

// hashing to Fq and Fp
void hash_stdin(SHA256_CTX *ctx);
void next_prng(EVP_CIPHER_CTX *cctx, const SHA256_CTX *hctx, uint32_t idx);
bool next_modp(EVP_CIPHER_CTX *cctx, mpz_t ret);
uint8_t *next_128b(EVP_CIPHER_CTX *cctx, mpz_t *out);

// these are ugly, but they let us avoid repeating a bunch of boilerplate in every executable
#define HASH_INIT_GENERIC(C_INIT_FN, MP_TYPE, MP_INIT_FN, ...)     \
    int retval = 0;                                                \
    const struct cmdline_opts opts = get_cmdline_opts(argc, argv); \
    const bool do_print = opts.test || !opts.quiet;                \
    C_INIT_FN();                                                   \
    MP_TYPE __VA_ARGS__;                                           \
    MP_INIT_FN(__VA_ARGS__, (void *)NULL);                         \
    ERR_load_crypto_strings();                                     \
    SHA256_CTX hash_ctx;                                           \
    CHECK_CRYPTO(SHA256_Init(&hash_ctx));                          \
    EVP_CIPHER_CTX *prng_ctx = EVP_CIPHER_CTX_new();               \
    CHECK_CRYPTO(prng_ctx != NULL);                                \
    hash_stdin(&hash_ctx);                                         \
    uint64_t c_start, c_end;                                       \
    struct timespec start, end;                                    \
    clock_gettime(CLOCK_MONOTONIC, &start);                        \
    CYCLE_COUNT_START

#define HASH_CLEAR_GENERIC(C_UNINIT_FN, MP_CLEAR_FN, ...)                                  \
    CYCLE_COUNT_END;                                                                       \
    clock_gettime(CLOCK_MONOTONIC, &end);                                                  \
    long elapsed = 1000000000 * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec; \
    fprintf(opts.quiet ? stdout : stderr, "%ld %" PRIu64 "\n", elapsed, c_end - c_start);  \
    EVP_CIPHER_CTX_free(prng_ctx);                                                         \
    MP_CLEAR_FN(__VA_ARGS__, (void *)NULL);                                                \
    C_UNINIT_FN();                                                                         \
    return retval

// for hash_*
#define HASH_INIT(...) HASH_INIT_GENERIC(curve_init, mpz_t, mpz_inits, __VA_ARGS__)
#define HASH_CLEAR(...) HASH_CLEAR_GENERIC(curve_uninit, mpz_clears, __VA_ARGS__)

// for hash2_*
#define HASH2_INIT(...) HASH_INIT_GENERIC(curve2_init, mpz_t2, mpz2_inits, __VA_ARGS__)
#define HASH2_CLEAR(...) HASH_CLEAR_GENERIC(curve2_uninit, mpz2_clears, __VA_ARGS__)

#define __bls_hash__src__util__util_h__
#endif  // __bls_hash__src__util__util_h__
