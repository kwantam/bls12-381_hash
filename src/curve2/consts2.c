// constants for fp2
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "consts2.h"

// a constant used to compute sqrt(sqrt(-1)) and sqrt(-sqrt(-1))
const uint64_t IssqrtM1[6] = {
    0xc81084fbede3cc09LL, 0xee67992f72ec05f4LL, 0x77f76e17009241c5LL,
    0x48395dabc2d3435eLL, 0x6831e36d6bd17ffeLL, 0x6af0e0437ff400bLL,
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
