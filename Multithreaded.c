#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
//#include <windows.h>
#include <pthread.h>

#define SIZE 50000000

void verif(int *array);
void timerConvert(long ms);
char *numToString(int num);
void *threadFunction(void *arg);
void swap(int **array, int a, int b);
void quicksort(int **array, int start, int end);

typedef struct t_object {
	int **tab;
	int right, left;
} Object;

int nbThread, spliting;
pthread_mutex_t lock;

void quicksort(int **array, int start, int end)
{
	if(start >= end) return;
	int left = start, right = end + 1, random = (end - start) / 2;
	swap(array, start, random);
	const int bias = (*array)[start];

	while(1){
		do right--; while((*array)[right] > bias);
		do left++; while((*array)[left] < bias);
		if(left < right) swap(array, left, right);
		else break;
	}
	swap(array, start, right); // swap the bias

	printf("bias : tab[%s] = %s\n", numToString(right), numToString((*array)[right]));

/*	Object *doubleganger = malloc(sizeof(Object));
	doubleganger->tab = array, doubleganger->left = start, doubleganger->right = right - 1;

	if(nbThread < 8){
		pthread_mutex_lock(&lock);
		nbThread++;
		printf("New Threads : [ %s : %s ]\n", numToString(start), numToString(right - 1));
		pthread_t first;
		if(pthread_create(&first, NULL, &threadFunction, (void*)doubleganger) == -1) exit(EXIT_FAILURE);
		pthread_mutex_unlock(&lock);
		quicksort(array, right + 1, end);
		pthread_join(first, NULL);
	}
	else{
		quicksort(array, start, right - 1);
		quicksort(array, right + 1, end);
	}*/
}

int main()
{
	nbThread = 0, spliting = 0;
	srand(time(NULL));
	int *tab = malloc(sizeof(int) * SIZE), i;
	long t1, t2;
	printf("[*] Generating Data... (%s)\n", numToString(SIZE));
	t1 = clock();
	for(i = 0; i < SIZE; i++) tab[i] = rand();
	t2 = clock();
	printf("[+] Data generated in %ld ms\n", ((t2 - t1)* 1000) / CLOCKS_PER_SEC);
	puts("\n[*] Starting to sort");
	t1 = clock();
	quicksort(&tab, 0, SIZE - 1);
	t2 = clock();
	printf("[+] Sort ended with %d threads (%d splits) in %ld ms\n", nbThread, spliting, ((t2 - t1)* 1000) / CLOCKS_PER_SEC);
	verif(tab);

	return 0;
}

char *numToString(int num)
{
	char temp[10], *string = NULL, i = 0, j = 0, size = 0;
	sprintf(temp, "%d", num);
	size = strlen(temp) + (strlen(temp) / 3);
	string = malloc(sizeof(char) * size);
	for(i = 0, j = 0; i < size; i++){
		if((size - i) % 4 == 0) string[i] = ' ';
		else string[i] = temp[j], j++;
	}
	string[i] = '\0';
	return string;
}

void swap(int **array, int a, int b)
{
	int temp = (*array)[a];
	(*array)[a] = (*array)[b];
	(*array)[b] = temp;
}

void timerConvert(long ms)
{
	//long sec = ms;
	//ms -= (sec * 1000);
	printf("%ld ms\n", ms);
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
	Object *actuel = (Object*)arg;
	quicksort(actuel->tab, actuel->left, actuel->right);
}
