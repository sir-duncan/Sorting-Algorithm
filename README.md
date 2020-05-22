# Quicksort in C
Code for quicksort algorithm (single and multi threaded)

For better performance, compile it with the optimisation flag :
gcc -O3 file.c -o output -lpthread

Average Sorting time (out of 30 runs) on a Ryzen 7 1800X, 8 Cores, 16 Threads :

                            Single Threaded  |  Multi Threaded
                            -----------------------------------
    50 millions numbers   :  3s 273ms 849µs  |  0s 839ms 165µs

    100 millions numbers  :  6s 572ms 160µs  |  1s 683ms 023µs
