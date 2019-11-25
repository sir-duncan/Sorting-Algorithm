#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 10000000

void verif(int *array);
void timerConvert(int ms);
void swap(int **array, int a, int b);

void quicksort(int **array, int start, int end)
{
    if(start >= end) return;
    int left = start, right = end + 1, i;
    const int bias = (*array)[start];
    while(1){
        do right--; while((*array)[right] > bias);
        do left++; while((*array)[left] < bias);
        if(left < right) swap(array, left, right);
        else break;
    }
    swap(array, start, right);
    quicksort(array, start, right - 1);
    quicksort(array, right + 1, end);
}

int main()
{
    srand(time(NULL));
    int *tab = malloc(sizeof(int) * SIZE), i, t;
    puts("[*] Generating Data...");
    t = clock();
    for(i = 0; i < SIZE; i++) tab[i] = rand();
    printf("[+] Data generated in ");
    timerConvert(clock() - t);

    t = clock();
    puts("\n[*] Starting to sort");
    quicksort(&tab, 0, SIZE-1);
    printf("[+] Sort ended in ");
    timerConvert(clock() - t);
    verif(tab);

    return 0;
}

void swap(int **array, int a, int b)
{
    int temp = (*array)[a];
    (*array)[a] = (*array)[b];
    (*array)[b] = temp;
}

void verif(int *array)
{
    int i, j;
    for(i = 1; i < SIZE; i++){
        if(array[i-1] > array[i]){
            printf("[-] Problem int the sort tab[%d] = %d, tab[%d] = %d\n", i - 1, array[i - 1], i, array[i]);
            for(j = i - 3; j < i + 10; j++) // display
                printf("[ %d ] = %d\n", j, array[j]);
            return;
        }
    }
    printf("[+] Array correctly sorted\n\n");
}

void timerConvert(int ms)
{
    int sec = ms / 1000;
    ms -= (sec * 1000);
    printf("%d sec %d ms\n", sec, ms);
}
