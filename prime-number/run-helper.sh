#!/bin/sh

run () {
  echo === $1 ===
  \time -f "Commnad  : %C\nReal time: %e sec\nMax RSS  : %M KiB\n" $2 -u $3 2>&1
}

start () {
  run "$1" "$2" 1000
  run "$1" "$2" 1000000
  run "$1" "$2" 10000000
  run "$1" "$2" 100000000
}
