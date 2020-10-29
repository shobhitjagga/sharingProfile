### Quick Run

`./compile.sh`

`./script.sh prog1 8 1`

Change `prog1` to `prog2`, etc. 8 Indicates running with 8 threads. 1 indicates plots will be displayed (requires Python with `matplotlib`). Remove the 1 as last argument to not plot the results.

### Clean up

`./compile.sh clean`

`./script.sh clean`

`compile.sh` removes binaries while `script.sh` removes traces, CDFs, etc.

### Run individual part of assignment

The argument to all part programs is the program binary name (`prog1`, for example). In case of `part1`, the number of threads is also an argument.

If 1 is supplied as third argument to `part2`or `part2_ploy.py`, the CDF computation and plotting (respectively) for cache-filtered (`part3`) trace is done.

#### Part 1

This program is a pintool. The thread-wise instrumentation is done for all predicated instructions that access memory. The memory accessed by each such instruction is broken down into a minimum-length machine access sequence under the given constraint of not crossing the cache block of 64B.

#### Part 2

This program computes CDF for either the machine access trace or the cache-filtered miss trace. The computation is invariant of the trace type and the argument to the program is the program binary name that has been instrumented followed by a 1 if cache-filtered miss trace is to be processed (given the trace is present).

#### Part 3

This program utilizes a single set-associative LRU cache used in assignment 1 by our group. The cache is assumed to be empty at the beginning. The misses are stored to produce a cache-filtered miss trace and the program outputs basic cache statistics like lookups, hits, and misses. The input to the program is the program binary name that has been instrumented.

#### Part 4

This program computes the sharing profile of the program binary name supplied to it, assuming the machine-access trace is already present. 


