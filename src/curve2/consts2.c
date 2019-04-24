// constants for fp2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "consts2.h"

// constants for SvdW map
// this is cx1 ; cx2 is cx1 - 1
const uint64_t Icx12[6] = {
    0x2e01fffffffeffffLL, 0xde17d813620a0002LL, 0xddb3a93be6f89688LL, 0xba69c6076a0f77eaLL, 0x5f19672fdf76ce51LL, 0LL,
};
const uint64_t IsqrtM3[6] = {
    0x5c03fffffffdfffdLL, 0xbc2fb026c4140004LL, 0xbb675277cdf12d11LL, 0x74d38c0ed41eefd5LL, 0xbe32ce5fbeed9ca3LL, 0LL,
};
const uint64_t Iinv3[6] = {
    0x26a9ffffffffc71dLL, 0x1472aaa9cb8d5555LL, 0x9a208c6b4f20a418LL,
    0x984f87adf7ae0c7fLL, 0x32126fced787c88fLL, 0x11560bf17baa99bcLL,
};

// constants to compute values of eta for SWU map
const uint64_t Ieta1[6] = {
    0x6c88d0aa3e03ba01LL, 0xc4ee7b8d4b9e063aLL, 0xc8186bb3d4eccef7LL,
    0xed85f8b53954258eLL, 0xe305cc456ad9e235LL, 0x2c4a7244a026bd3LL,
};
const uint64_t Ieta2[6] = {
    0x6426a813ae01f51aLL, 0xc6638358daf3514dLL, 0xc60679cc7973076dLL,
    0x12b58b8d32f26594LL, 0x641892a0f9a4bb29LL, 0x85fa8cd9105715eLL,
};

// base point G2'
// NOTE this is in Montgomery repr suitable for use with bint2
const bint2_ty g2_prime_x = {
    0x91901dbda7c9fLL,  0xe30c4917bbda92LL, 0xf149611396c4ebLL, 0xa6d5e5212ddb7eLL, 0x5b9cc82e08fb6cLL,
    0xf9bec8c308e949LL, 0x9d3998f3b06LL,    0xdc2d754f04e704LL, 0x5c3e75b042900cLL, 0xc6f644bbca34fcLL,
    0xf463eefbebecfLL,  0x4d2cf98f025a3LL,  0xcd5e8a31aa182eLL, 0x534f6025c36LL,
};

const bint2_ty g2_prime_y = {
    0x380c2dd9e1475cLL, 0xaa2b2b949184f4LL, 0x3b1e5094c36db0LL, 0xa3e9987d2ffe58LL, 0x93cdd624306e5cLL,
    0x34d8c4d568b1b1LL, 0x116a77f0868cLL,   0xaaf69212060defLL, 0xe1647934b9e913LL, 0xfafede73927baeLL,
    0x66876d2d98c640LL, 0xd6999cfcc5ad62LL, 0xa10e0409f34aebLL, 0x111688843fe7LL,
};
