#!/bin/sh

run_O3 () {
  echo ===== up to $2 =====
  time -p ./pn-c++-O3 -u $1
  echo
}

run_O3 1000 1K
run_O3 1000000 1M
run_O3 10000000 10M
run_O3 100000000 100M
