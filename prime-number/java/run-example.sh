#!/bin/sh

./gradlew build

. ../run-helper.sh

start 'Java' "java -cp build/libs/prime_number.jar App"
