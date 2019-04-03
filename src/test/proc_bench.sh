#!/bin/bash

[ "$#" != "2" ] && { echo "Usage: $0 <amd_bench> <intel_bench>"; exit 1; }

set -e
set -o pipefail

mapfile amd_vals < "$1"
mapfile intel_vals < "$2"

if [ "${#amd_vals[@]}" != 15 ] || [ "${#intel_vals[@]}" != 15 ]; then
    echo "ERROR: input files must have 15 lines"
    exit 1
fi

div_round() {
    echo $(($(($1 + 50000000)) / 100000000))
}

show_q_fq_cq() {
    [ "$#" != "2" ] && { echo "bad args"; exit 1; }

    if [ "$1" = "1" ]; then
        div_round ${amd_vals[$idx]}
        echo "$\\vert$"
        div_round ${intel_vals[$idx]}
        let idx++ 1
    else
        echo "---"
    fi

    if [ "$2" = "1" ]; then
        echo "\\\\"
    else
        echo "&"
    fi
}

idx=0

echo "Hash-and-check & &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "\\midrule"

echo "Construction \\#1 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\midrule"

echo "Construction \\#2 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\midrule"

echo "Construction \\#3 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "%"

echo "& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "%"
