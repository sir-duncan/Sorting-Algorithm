#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <windows.h>
#include <pthread.h>

#define SIZE 50000000

void verif(int *array);
void timerConvert(struct timeval start, struct timeval end);
void swap(int **array, int a, int b);
void quicksort(int **array, int start, int end);

typedef struct t_object {
	int **tab;
	int right, left;
} Object;

int nbThread, spliting;

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
	spliting++;
	quicksort(array, start, right - 1);
	quicksort(array, right + 1, end);
}

int main()
{
	nbThread = 0, spliting = 0;
	srand(time(NULL));
	struct timeval start, end;
	int *tab = malloc(sizeof(int) * SIZE), i;
	printf("[*] Generating Data... (%d)\n", SIZE);
	gettimeofday(&start, NULL);
	for(i = 0; i < SIZE; i++) tab[i] = rand();
	gettimeofday(&end, NULL);
	printf("[+] Data generated in ");
	timerConvert(start, end);

	puts("\n[*] Starting to sort");
	gettimeofday(&start, NULL);
	quicksort(&tab, 0, SIZE - 1);
	gettimeofday(&end, NULL);
	printf("[+] Sort ended with %d threads (%d splits) in ", nbThread, spliting);
	timerConvert(start, end);
	verif(tab);

	return 0;
}

void swap(int **array, int a, int b)
{
	int temp = (*array)[a];
	(*array)[a] = (*array)[b];
	(*array)[b] = temp;
}

void timerConvert(struct timeval start, struct timeval end)
{
	int executionTime = (end.tv_sec - start.tv_sec) * 1000;
    executionTime += (end.tv_usec - start.tv_usec) / 1000;
    printf("%d sec %d ms\n", executionTime / 1000, executionTime - ((executionTime / 1000) * 1000));
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
