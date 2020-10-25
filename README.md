### Quick Run
`./compile.sh`
`./script.sh prog1 8 1`
Change `prog1` to `prog2`, etc. 8 Indicates running with 8 threads. 1 indicates plots will be displayed (requires Python with `matplotlib`). Remove the 1 as last argument to not plot the results.

### Clean up
`./compile.sh 1`
`./script.sh clean`

`compile.sh` removes binaries, `script.sh` removes traces, csv files, etc.

### Run individual part of assignment
The argument to all part programs is the program binary name (`prog1`, for example). In case of `part1`, the number of threads is also an argument.

If 1 is supplied as third argument to `part2`or `part2_ploy.py`, the CDF computation and plotting for cache-filtered (`part3`) trace is done.

### Plots

Plots are stored in `plots/` directory.

