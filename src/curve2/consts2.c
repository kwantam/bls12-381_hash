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
