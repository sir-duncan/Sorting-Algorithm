# Quicksort in C
Code for quicksort algorithm (single and multi threaded)

For better performance, compile it with the optimisation flag :
gcc -O3 file.c -o output -lpthread

Average Sorting time (out of 30 runs) on a Ryzen 7 1800X, 8 Cores, 16 Threads :

| # Number | Single Thread | PThreads | OpenMP |
| :---: | :---: | :---: | :---: |
| 50 millions | 3s 273ms | 839ms | |
| 100 millions | 6s 572ms | 1s 683ms | |
| 500 millions | | | |