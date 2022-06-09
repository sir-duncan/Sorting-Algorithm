# Quicksort in C

Code for quicksort algorithm (single and multi threaded)

# Compilation

For better performance, compile with the optimisation flag :

`gcc -O3 file.c -o output`

and add special flag for multi-threading:

`-lpthread` for PThreads

`-fopenmp` for OpenMP

# Results

Average Sorting time (out of 50 runs) on a Ryzen 7 1800X, 8 Cores, 16 Threads :

| # Number | Single Thread | PThreads | OpenMP |
| :---: | :---: | :---: | :---: |
| 10 millions | 663ms | 184ms | 176ms |
| 50 millions | 3s 273ms | 934ms | 926ms |
| 100 millions | 6s 572ms | 1s 839ms | 1s 822ms |
| 500 millions | 34s 313ms | 9s 149ms | 9s 095ms |
