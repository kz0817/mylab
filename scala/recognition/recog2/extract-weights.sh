#!/bin/sh

source=$1
target=$2
class_id=`echo $target | sed "s/[^0-9]*//g"`
search=`echo "^c: " $class_id " => "`
echo grep \"$search\" $source "| sed \"s/^c: [0-9]\\+ => //\""
