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

show_next() {
    div_round ${amd_vals[$idx]}
    echo "$\\vert$"
    div_round ${intel_vals[$idx]}
    let idx++ 1
}

idx=0

echo "Hash-and-check & & "
show_next; echo " & "
show_next; echo " & --- \\\\"
echo "%"; echo "%"
echo "\\midrule"

echo "Construction \\#1 & \\S\\ref{sec:blsmap} & "
show_next; echo " & "
show_next; echo " & "
show_next; echo "\\\\"
echo "%"; echo "%"

echo " & \\S\\ref{sec:blsmap2} & --- & "
show_next; echo " & "
show_next; echo "\\\\"
echo "%"; echo "%"
echo "\\midrule"

echo "Construction \\#2 & \\S\\ref{sec:blsmap} & "
show_next; echo " & "
show_next; echo " & "
show_next; echo "\\\\"
echo "%"; echo "%"

echo " & \\S\\ref{sec:blsmap2} & --- & "
show_next; echo " & "
show_next; echo "\\\\"
echo "%"; echo "%"
echo "\\midrule"

echo "Construction \\#3 & \\S\\ref{sec:blsmap} & "
show_next; echo " & "
show_next; echo " & --- \\\\"
echo "%"; echo "%"

echo " & \\S\\ref{sec:blsmap2} & --- & "
show_next; echo " & --- \\\\"
echo "%"; echo "%"
