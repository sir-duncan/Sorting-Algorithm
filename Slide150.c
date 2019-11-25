#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <pthread.h>

#define SIZE 10000000

void bulle(int *tab);
void verif(int *array);
void timerConvert(int ms);
void *threadFunction(void *arg);
void swap(int **array, int a, int b);
int check(int *array, int start, int end);
void display(int *array, int start, int end);
void quicksort(int **array, int start, int end);

typedef struct t_object {
    int **tab;
    int right, left;
} Object;

int nbThread;

void quicksort(int **array, int start, int end)
{
    if(start >= end) return;
    //printf("[ %d ; %d ]\n", start, end);
    int left = start, right = end + 1, i;
    const int bias = (*array)[start];

    while(1){
        do right--; while((*array)[right] > bias);
        do left++; while((*array)[left] < bias);
        if(left < right) swap(array, left, right);
        else break;
    }
    swap(array, start, right);

    //printf("bias : %d\n", bias);
    Object *doubleganger = malloc(sizeof(Object));
    doubleganger->tab = array;
    doubleganger->left = start, doubleganger->right = right - 1;

    /*for(i = 0; i < SIZE; i++) printf("%d - ", (*array)[i]);
    printf("\n");
    printf("Creating: [ %d ; %d ] et [ %d ; %d ]\n", start, right - 1, right + 1, end);*/
    if(nbThread < 30){
        pthread_t first;
        if(pthread_create(&first, NULL, &threadFunction, (void*)doubleganger) == -1) exit(EXIT_FAILURE);
        pthread_join(first, NULL);
    }
    else
        quicksort(array, start, right - 1);
    quicksort(array, right + 1, end);
}

int main()
{
    nbThread = 0;
    srand(time(NULL));
    int *tab = malloc(sizeof(int) * SIZE), i, t;
    puts("[*] Generating Data...");
    t = clock();
    for(i = 0; i < SIZE; i++) tab[i] = rand();
    printf("[+] Data generated in ");
    timerConvert(clock() - t);
//    for(i = 0; i < SIZE; i++) printf("%d - ", tab[i]);
//    printf("\n");

    t = clock();
    puts("\n[*] Starting to sort");
    quicksort(&tab, 0, SIZE-1);
    Sleep((5 * nbThread));
    printf("[+] Sort ended in ");
    timerConvert(clock() - t);
    verif(tab);

    printf("nbThread = %d\n", nbThread);
    return 0;
}

void swap(int **array, int a, int b)
{
    int temp = (*array)[a];
    (*array)[a] = (*array)[b];
    (*array)[b] = temp;
}

void timerConvert(int ms)
{
    int sec = ms / 1000;
    ms -= (sec * 1000);
    printf("%d sec %d ms\n", sec, ms);
}

int check(int *array, int start, int end)
{
    int i;
    for(i = start + 1; i < end; i++){
        if(array[i-1] > array[i]){
            return 1;
        }
    }
    return 0;
}

void verif(int *array)
{
    int i, j;
    for(i = 1; i < SIZE; i++){
        if(array[i-1] > array[i]){
            printf("[-] Problem int the sort tab[%d] = %d, tab[%d] = %d\n", i - 1, array[i - 1], i, array[i]);
            for(j = i-3; j < i + 10; j++) // display
                printf("[ %d ] = %d\n", j, array[j]);
            return;
        }
    }
    printf("[+] Array correctly sorted\n\n");
}

void *threadFunction(void *arg)
{
    nbThread++;
    Object *actuel = (Object*)arg;
    quicksort(actuel->tab, actuel->left, actuel->right);
}

void display(int *array, int start, int end)
{
    int i;
    for(i = start; i < end + 1; i++) printf("%d - ", array[i]);
    printf("\n");
}
