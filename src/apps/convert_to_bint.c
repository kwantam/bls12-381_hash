// print out a number in bint format
//
// (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

#include "bint.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <number>\n", argv[0]);
        return 1;
    }

    // input
    mpz_t in;
    mpz_init_set_str(in, argv[1], 10);

    // output
    int64_t out[BINT_NWORDS];
    bint_import_mpz(out, in);

    // display
    for (unsigned i = 0; i < BINT_NWORDS; ++i) {
        printf("%#" PRIx64 "LL,\n", out[i]);
    }

    return 0;
}
