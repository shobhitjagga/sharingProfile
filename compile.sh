#!/bin/bash

if [ -z $1 ]; then
	gcc -O3 -static -pthread prog1.c -o prog1
	gcc -O3 -static -pthread prog2.c -o prog2
	gcc -O3 -static -pthread prog3.c -o prog3
	gcc -O3 -static -pthread prog4.c -o prog4
	echo "programs 1 to 4 compiled"
	make obj-intel64/part1.so
	echo "part1 compiled"
	g++ -o part2 part2.cpp
	echo "part2 compiled"
	cd part3_files && make
	cd ..
	cp part3_files/part3 .
	echo "part3 compiled"
	g++ -o part4 part4.cpp
	echo "part4 compiled"
else
	rm -f part1 part2 part3 part4
	cd part3_files && make clean
	cd ..
	rm -f prog1 prog2 prog3 prog4
	echo "Cleaned up"
fi
