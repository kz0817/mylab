#!/bin/sh

. ../run-helper.sh

# CLASSPATH shall be defined in the following file
classpath_file=./run-class-path.sh
if [ -f $classpath_file ]; then
  echo source file $classpath_file
  . $classpath_file
fi
start 'Scala [Int]' "java Main"
start 'Scala [Long]' "java Main --long"
start 'Scala (GraalVM) [Int]' "graalvm/bin/java Main"
start 'Scala (GraalVM) [Long]' "graalvm/bin/java Main --long"
