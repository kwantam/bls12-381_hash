#ifdef BINT_INTERNAL

#ifndef _HAVE_BINT_CONSTS_H_INTERNAL
#include <stdint.h>
#define NWORDS 7
#define BITS_PER_WORD 56
#define BITS_EXTRA 11
#define LO_MASK ((1LL << BITS_PER_WORD) - 1)
static const uint64_t mp[7] =
    { 0x1000000005555L
    , 0x14eac000046L
    , 0x5f094f09dbe154L
    , 0xc7aed4098cf2dL
    , 0xb453289b88b47bL
    , 0x1965b4e45849bcL
    , 0xffe5feee15c680L
    };
static const uint64_t p[7] =
    { 0xfeffffffffaaabL
    , 0xfffeb153ffffb9L
    , 0xa0f6b0f6241eabL
    , 0xf38512bf6730d2L
    , 0x4bacd764774b84L
    , 0xe69a4b1ba7b643L
    , 0x1a0111ea397fL
    };
static const uint64_t pP[7] =
    { 0xf3fffcfffcfffdL
    , 0xdb92d9d113e889L
    , 0xf0c8e30b48286aL
    , 0x8eb2db4c16ef2eL
    , 0x68cf5819ecca0eL
    , 0xfc9468b316fee2L
    , 0xa0ceb06106feaaL
    };
static const uint64_t rI[7] =
    { 0x3433825937d5f3L
    , 0x6111a3ad18fad0L
    , 0x4e13432f445babL
    , 0x2c8bd44589b2c2L
    , 0x4b9b5a29e226deL
    , 0xeb2d58dea73611L
    , 0x1055a9f965d8L
    };
static const uint64_t rSq[7] =
    { 0x6d1c34510370edL
    , 0xec45c53e243d62L
    , 0x93317d3b1d65aL
    , 0x5d74088b4f36a0L
    , 0x865d118c10ea72L
    , 0xfd5cd507320a75L
    , 0xc8d4cc8a759L
    };
static const uint64_t r[7] =
    { 0xd800000347fcb8L
    , 0xcde6d2002b119L
    , 0x83a2090c7212e0L
    , 0xda0f73e037669fL
    , 0x1297bb09b09b42L
    , 0x12ca7c515d98fL
    , 0x577a659fcfaL
    };
#define _HAVE_BINT_CONSTS_H_INTERNAL
#endif // _HAVE_BINT_CONSTS_H_INTERNAL

#else // not BINT_INTERNAL

#ifndef _HAVE_BINT_CONSTS_H
#define BINT_NWORDS 7
#define BINT_BITS_PER_WORD 56
#define _HAVE_BINT_CONSTS_H
#endif // _HAVE_BINT_CONSTS_H

#endif // BINT_INTERNAL
