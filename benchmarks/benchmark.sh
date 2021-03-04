#!/bin/bash

# USAGE: ./bencmark ./executable
# ex:	 ./bencmark ./vector_multiply

make clean -C ../ &> /dev/null
make -C ../ &> /dev/null
make clean &> /dev/null
make &> /dev/null

iter=5

for tcount in 2 5 10 50 100
do
	sum=0
	for ((i=1; i<=$iter; i++))
	do
		output=$($@ $tcount | head -n 1)
		arr=($output)
		sum=$((sum+${arr[2]}))
	done
	avg=$((sum/$iter))

	echo "${1} - Average for $tcount threads is: $avg"
done

make clean &> /dev/null
make clean -C ../ &> /dev/null

