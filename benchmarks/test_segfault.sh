#!/bin/bash

# USAGE: ./test_segfault ./executable thread_count
# ex:	 ./test_segfault ./vector_multiply 100

make clean -C ../ &> /dev/null
make -C ../ &> /dev/null
make clean &> /dev/null
make &> /dev/null

iter=10

for((i=1; i<=$iter; i++))
do
	$@
	if [ $? -ne 0 ]
	then
		echo "Program returned a non-zero return code"
		make clean -C ../ &> /dev/null
		make clean &> /dev/null
		exit 1
	fi
done

make clean -C ../ &> /dev/null
make clean &> /dev/null

echo "Ran $iter iterations successfully"

