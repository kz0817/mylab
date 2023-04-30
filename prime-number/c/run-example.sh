#!/bin/sh

. ../run-helper.sh

start 'C (realloc): int' "./pn-int --realloc"
start 'C (realloc): long' "./pn-long --realloc"
