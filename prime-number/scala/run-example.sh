#!/bin/sh

. ../run-helper.sh

# CLASSPATH shall be defined in the following file
classpath_file=./run-class-path.sh
if [ -f $classpath_file ]; then
  echo source file $classpath_file
  . $classpath_file
fi

start 'Scala' "java Main"
