// consts for bls12-381
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#ifndef __bls_hash__src__curve__bls12_381_consts_h__

#include <stdint.h>

// base field characteristic
#define P_LEN 48
static const uint8_t BLS12_381_p[] = {
    0x1a, 0x01, 0x11, 0xea, 0x39, 0x7f, 0xe6, 0x9a, 0x4b, 0x1b, 0xa7, 0xb6, 0x43, 0x4b, 0xac, 0xd7,
    0x64, 0x77, 0x4b, 0x84, 0xf3, 0x85, 0x12, 0xbf, 0x67, 0x30, 0xd2, 0xa0, 0xf6, 0xb0, 0xf6, 0x24,
    0x1e, 0xab, 0xff, 0xfe, 0xb1, 0x53, 0xff, 0xff, 0xb9, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xaa, 0xab,
};

// group order
#define Q_LEN 32
static const uint8_t BLS12_381_q[] = {
    0x73, 0xed, 0xa7, 0x53, 0x29, 0x9d, 0x7d, 0x48, 0x33, 0x39, 0xd8, 0x08, 0x09, 0xa1, 0xd8, 0x05,
    0x53, 0xbd, 0xa4, 0x02, 0xff, 0xfe, 0x5b, 0xfe, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x01,
};

// cofactor
static const uint8_t BLS12_381_h[] = {
    0x39, 0x6c, 0x8c, 0x00, 0x55, 0x55, 0xe1, 0x56, 0x8c, 0x00, 0xaa, 0xab, 0x00, 0x00, 0xaa, 0xab,
};

// base point G' : ./hash_and_try -n 1 <<< "bls12_381 random base point"
// NOTE this is in Montgomery repr suitable for use with bint.
//      You can show its value using something like
//          bint_export_mpz(x, g_prime_x);
//          gmp_printf("%Zx\n", x);
static const uint64_t g_prime_x[] = {
    0x002a30064e34c56dLL, 0x003aa6a55dfeb22dLL, 0x00450d6874c1a102LL, 0x0083d18a9f4ac8c2LL,
    0x000c3bb918f2486eLL, 0x00806dbe57c6fc11LL, 0x0000091321f4828aLL,
};
static const uint64_t g_prime_y[] = {
    0x00d4765ae8b96829LL, 0x0063f371895612abLL, 0x00f2c99cf9caa880LL, 0x00e72319c3567913LL,
    0x002ddfc71c8ebc2fLL, 0x005a909fdcdc5999LL, 0x00000a4f73e2948bLL,
};

// 2^128 * the above point
static const uint64_t g_prime_ll128_x[] = {
    0x00f235688080385bLL, 0x00750328d31b23faLL, 0x00c634b7cb283adfLL, 0x001c83ec63aca33fLL,
    0x0040980e917a26baLL, 0x00802a031c0c2221LL, 0x00000c16a40fc4ddLL,
};
static const uint64_t g_prime_ll128_y[] = {
    0x0094fe9d733feeb8LL, 0x0021d5871856647fLL, 0x004c004271fdf9f8LL, 0x00f42764d0bb02d3LL,
    0x00d8b6561fa5dce6LL, 0x0016a09ea3dff281LL, 0x000019cd78e1cf92LL,
};

// constants for the Shallue and van de Woestijne mapping
static const uint64_t Icx1[] = {
    0x8a05fffffffcfffdLL, 0x9a47883a261e0006LL, 0x991afbb3b4e9c39aLL,
    0x2f3d52163e2e67c0LL, 0x1d4c358f9e646af5LL, 0x0000000000000001LL,
};
static const uint64_t Icx2[] = {
    0x2ff900000002aab1LL, 0x846477c48b35fff9LL, 0xce15d6ed41c73289LL,
    0x3539f96eb556aafeLL, 0x2dcf7226a4e741e2LL, 0x1a0111ea397fe699LL,
};
static const uint64_t IsqrtM27[] = {
    0x140bfffffff9fff7LL, 0x348f10744c3c000dLL, 0x3235f76769d38735LL,
    0x5e7aa42c7c5ccf81LL, 0x3a986b1f3cc8d5eaLL, 0x0000000000000002LL,
};
static const uint64_t IinvM27[] = {
    0xe1b371c71c718b10LL, 0x4e79097a56dc4bd9LL, 0xb0e977c69aa27452LL,
    0x761b0f37a1e26286LL, 0xfbf7043de3811ad0LL, 0x124c9ad43b6cf79bLL,
};

#define __bls_hash__src__curve__bls12_381_consts_h__
#endif  // __bls_hash__src__curve__bls12_381_consts_h__
