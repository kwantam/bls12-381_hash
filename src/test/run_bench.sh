#!/bin/bash

NUM_RUNS=100000
TESTS_hash_and_check=(q fq)
TESTS_hash_svdw_1=(q fq cq)
TESTS_hash_svdw_2=(q fq cq)
TESTS_hash_svdw_rG=(q fq)
TESTS_hash_swu_1=(q cq)
TESTS_hash_swu_2=(q cq)
TESTS_hash_swu_rG=(q)
TESTS=(hash_and_check hash_svdw_1 hash_swu_1 hash_svdw_2 hash_swu_2 hash_svdw_rG hash_swu_rG)

[ -z "$1" ] && { echo "Usage: $0 <apps_dir>"; exit 1; }

set -e
set -o pipefail
rm -f "$1"/bench.out

for app in "${TESTS[@]}"; do
    app_tests="TESTS_${app}[@]"
    for test in ${!app_tests}; do
        "$1/apps/$app" -${test} -n ${NUM_RUNS} <<< "benchmark_input" >> "$1"/bench.out
    done
done
