#!/bin/bash

NUM_RUNS=100000

exit_cleanup() {
    rm -f "${TMPFILE}"
}

[ -z "$1" ] && { echo "Usage: $0 <apps_dir>"; exit 1; }

set -e
set -o pipefail
trap exit_cleanup EXIT

TMPFILE=$(mktemp)
dd status=none if=/dev/urandom of="${TMPFILE}" bs=1M count=1
rm -f "$1"/bench.out

for app in hash_and_check hash_svdw_1 hash_svdw_2 hash_svdw_rG; do
    for flag in q fq; do
        "$1/apps/$app" -${flag} -n ${NUM_RUNS} < "${TMPFILE}" >> "$1"/bench.out
    done
done

for app in hash_swu_1 hash_swu_2 hash_swu_rG; do
    "$1/apps/$app" -q -n ${NUM_RUNS} < "${TMPFILE}" >> "$1"/bench.out
done
