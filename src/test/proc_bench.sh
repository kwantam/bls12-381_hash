#!/bin/bash
#
# (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

[ -z "$1" ] && { echo "Usage: $0 <bench_file> [bench_file_2]"; exit 1; }

set -e
set -o pipefail

. "$(dirname "$0")/bench_setup"
DIV_VAL=$((1000 * ${NUM_RUNS}))
RND_VAL=$((${DIV_VAL} / 2))

NUM_BENCH=1
mapfile bench1_vals < "$1"
if [ ! -z "$2" ]; then
    mapfile bench2_vals < "$2"
    NUM_BENCH=2
fi


if [ "${#bench1_vals[@]}" != 27 ] || ( [ "$NUM_BENCH" = "2" ] && [ "${#bench2_vals[@]}" != 27 ] ); then
    echo "ERROR: input files must have 27 lines"
    exit 1
fi

div_round() {
    echo $(($(($1 + ${RND_VAL})) / ${DIV_VAL}))
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

echo "\$\\bm{G_1}\$ & Hash-and-check & --- &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "%"

# HACK
# worst 10% timings from timings.out and timings_fo.out get manually inserted here
echo "& (worst 10\\%) & --- & \\textred{XXX} & \\textred{XXX} & ---\\\\"
echo "\\cmidrule{2-6}"

echo "& Construction \\#1 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\cmidrule{2-6}"

echo "& Construction \\#2 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\cmidrule{2-6}"

echo "& Construction \\#3 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "%"

echo "&& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "\\midrule[\\heavyrulewidth]"

echo "\$\\bm{G_2}\$ & Hash-and-check & --- &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "%"

# HACK
# worst 10% timings from timings2.out and timings2_fo.out get manually inserted here
echo "& (worst 10\%) & --- & \\textred{XXX} & \\textred{XXX} & ---\\\\"

echo "\\cmidrule{2-6}"

echo "& Construction \\#4 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\cmidrule{2-6}"

echo "& Construction \\#5 & \\S\\ref{sec:blsmap} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& \\S\\ref{sec:blsmap2} &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"
