#!/bin/sh

./gradlew build

. ../run-helper.sh

start 'Java [Int]' "java -cp build/libs/kotolin-all.jar NpKt"
start 'Java [Long]' "java -cp build/libs/kotolin-all.jar NpKt --long"
start 'Java (GraalVM) [Int]' "graalvm/bin/java -cp build/libs/kotolin-all.jar NpKt"
start 'Java (GraalVM) [Long]' "graalvm/bin/java -cp build/libs/kotolin-all.jar NpKt --long"
