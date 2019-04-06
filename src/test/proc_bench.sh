#!/bin/bash

[ -z "$1" ] && { echo "Usage: $0 <bench_file> [bench_file_2]"; exit 1; }

set -e
set -o pipefail

NUM_BENCH=1
mapfile bench1_vals < "$1"
if [ ! -z "$2" ]; then
    mapfile bench2_vals < "$2"
    NUM_BENCH=2
fi


if [ "${#bench1_vals[@]}" != 15 ] || ( [ "$NUM_BENCH" = "2" ] && [ "${#bench2_vals[@]}" != 15 ] ); then
    echo "ERROR: input files must have 15 lines"
    exit 1
fi

div_round() {
    echo $(($(($1 + 5000000000)) / 10000000000))
}

show_q_fq_cq() {
    [ "$#" != "2" ] && { echo "bad args"; exit 1; }

    if [ "$1" = "1" ]; then
        div_round ${bench1_vals[$idx]}
        if [ "$NUM_BENCH" = "2" ]; then
            echo "$\\vert$"
            div_round ${bench2_vals[$idx]}
        fi
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

echo "Hash-and-check & --- &"
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
