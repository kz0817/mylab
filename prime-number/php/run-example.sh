#!/bin/sh

. ../run-helper.sh

start 'PHP' 'php np.php'
start 'PHP' 'php -dopcache.jit=on -dmemory_limit=256M -dopcache.enable_cli=1 -dopcache.jit_buffer_size=32M np.php'
