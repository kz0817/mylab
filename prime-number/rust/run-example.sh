#!/bin/sh

cargo build --release

. ../run-helper.sh

start 'Rust' "target/release/prime_number"
