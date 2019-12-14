#!/bin/sh

. ../run-helper.sh

start 'C (fixed-length arary)' ./pn
start 'C (realloc)' ./pn --realloc
