#!/bin/bash
dir=$(dirname $0)
for i in $(seq 1 $2)
do
	echo $i
	$dir/shift $($dir/alpha -s $i $1) $1
	sleep 1
done
