#!/bin/sh

./gradlew build

. ../run-helper.sh

start 'Java [int]' "java -cp build/libs/prime_number.jar App"
start 'Java [long]' "java -cp build/libs/prime_number.jar App --long"
start 'Java (GraalVM) [int]' "graalvm/bin/java -cp build/libs/prime_number.jar App"
start 'Java (GraalVM) [long]' "graalvm/bin/java -cp build/libs/prime_number.jar App --long"
