#!/bin/sh

./gradlew build

. ../run-helper.sh

start 'Java' "java -cp build/libs/kotolin-all.jar NpKt"
