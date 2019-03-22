#!/bin/bash

set -e
set -o pipefail
exec dd if=/dev/urandom bs=1M count=1 status=none | "$1" -n 1024 -t | "$2" "$3"
