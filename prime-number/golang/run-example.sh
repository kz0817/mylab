#!/bin/sh

. ../run-helper.sh

start 'Golang [int32]' "./pn"
start 'Golang [int64]' "./pn --int64"
