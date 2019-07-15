#!/bin/bash
#
# (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>

[ "$#" != "5" ] && { echo "Usage: $0 <bench_file> <hac_t> <hac_t_fo> <h2ac_t> <h2ac_t_fo>"; exit 1; }

set -e
set -o pipefail

. "$(dirname "$0")/bench_setup"
DIV_VAL=$((1000 * ${NUM_RUNS}))
RND_VAL=$((${DIV_VAL} / 2))

if [ "${USE_CYCLES:-0}" = "1" ]; then
    MAP_FIELD=2
else
    MAP_FIELD=1
fi

mapfile -t bench1_vals < <(cut -d \  -f "${MAP_FIELD}" "$1")

if [ "${#bench1_vals[@]}" != 34 ] ; then
    echo "ERROR: input files must have 34 lines"
    exit 1
fi

div_round() {
    echo $(($(($1 + ${RND_VAL})) / ${DIV_VAL}))
}

show_q_fq_cq() {
    [ "$#" != "2" ] && { echo "bad args"; exit 1; }

    if [ "$1" = "1" ]; then
        div_round ${bench1_vals[$idx]}
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

get_avg_timing() {
    # NOTE: the below works because each iteration runs 10x
    #       and we take the worst 10%, so in total the number
    #       of runs is exactly NUM_RUNS. This means we can
    #       use the same rounding subroutine here as above.
    div_round $(perl -e '
@timings = map { (split(" "))[$ARGV[0] ? 1 : 0] } <STDIN>;
@timings = sort {$b <=> $a} @timings;
$tot = 0;
$len = $#timings;
for ($i = 0; $i < $len / 10; $i++) {
    $tot += $timings[$i];
}
print "$tot\n";
' "${USE_CYCLES:-0}" < $1)
    echo "&"
}

echo "\$\\bm{G_1}\$ & Hash-and-check & ~\\,{---} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "%"

echo "& (worst 10\\%) & ~\\,{---} &"
get_avg_timing $2
get_avg_timing $3
echo "---\\\\"
echo "\\cmidrule{2-6}"

echo "& Construction \\#1 & SW &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& SWU &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\cmidrule{2-6}"

echo "& Construction \\#2 & SW &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& SWU &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\cmidrule{2-6}"

echo "& Construction \\#3 & SW &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& SWU &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\midrule[\\heavyrulewidth]"

echo "\$\\bm{G_2}\$ & Hash-and-check & ~\\,{---} &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 0 1
echo "%"

echo "& (worst 10\%) & ~\\,{---} &"
get_avg_timing $4
get_avg_timing $5
echo "---\\\\"

echo "\\cmidrule{2-6}"

echo "& Construction \\#4 & SW &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& SWU &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\cmidrule{2-6}"

echo "& Construction \\#5 & SW &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& SWU &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "\\cmidrule{2-6}"

echo "& Construction \\#6 & SW &"
show_q_fq_cq 1 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"

echo "&& SWU &"
show_q_fq_cq 0 0
show_q_fq_cq 1 0
show_q_fq_cq 1 1
echo "%"
