#!/bin/bash

if [ "clean" = "$1" ]; then
	rm -f prog1_* prog2_* prog3_* prog4_*
	echo "Cleaned up"
else
	echo "Part 1: Address tracing begins"
	../../../pin -t obj-intel64/part1.so -- ./$1 $2
	echo "Address tracing finished"
	echo "Part 2: CDF computation begins"
	./part2 $1
	echo "CDF computation done"
	if [ -z $3 ] || [ 0 -eq $3 ]; then
		echo "Not plotting the CDF, provide 1 as third argument to script to plot"
	else
		echo "Plotting CDF now, close the plot window to continue"
		python part2_plot.py $1
	fi
	echo "Part 3: Cache Filter and CDF computation"
	./part3 $1
	echo "Cache filtering done"
	./part2 $1 1
	echo "CDF computation for filtered accesses done"
	if [ -z $3 ] || [ 0 -eq $3 ]; then
		echo "Not plotting the CDF, provide 1 as third argument to script to plot"
	else
		echo "Plotting CDF now, close the plot window to continue"
		python part2_plot.py $1 1
	fi
	echo "Part 4: Sharing profile"
	./part4 $1
	echo "Sharing profile done"
	echo "Exiting"
fi
