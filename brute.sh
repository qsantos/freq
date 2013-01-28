#!/bin/bash
for i in $(seq 1 $2)
do
	echo $i
	./shift -r $(./alpha $i $1) $1
	sleep 1
done
