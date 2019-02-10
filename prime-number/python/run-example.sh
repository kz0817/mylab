#!/bin/sh

run () {
  echo ===== up to $2 =====
  time -p ./pn.py -u $1
  echo
}

run 1000 1K
run 1000000 1M
run 10000000 10M
run 100000000 100M
