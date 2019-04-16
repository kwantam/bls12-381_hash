#!/usr/bin/python
#
# (C) 2019 Riad S. Wahby <rsw@cs.stanford.edu>
# process outlier timings

import sys

if len(sys.argv) < 2:
    print("USAGE: %s <timing_file>")
    sys.exit(1)

with open(sys.argv[1], "r") as f:
    foo = list(sorted( eval(l) for l in f.readlines() ))

start = len(foo) - len(foo) // 10
avg_10pct = int(0.5 + sum(foo[start:]) / len(foo[start:]) / 10 / 1000)

print(avg_10pct)
