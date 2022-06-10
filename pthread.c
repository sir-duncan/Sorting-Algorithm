#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 		10000000
#define NUMBER 		10
#define MAX_THREAD 	16

int *generateData();
int timerConvert(struct timeval start, struct timeval end);

char *numToString(int num);

void verif(int *array);
void *threadFunction();
void swap(int *a, int *b);
void quicksort(int *array, int start, int end, int id);

typedef struct t_object Object;
struct t_object {
	int right, left;
	int *array;
	Object *next;
};

int nbThread = 0, counter = 1;
pthread_t threads[MAX_THREAD];
pthread_spinlock_t lock;

void quicksort(int* array, int start, int end, int id) {
	if(start >= end) return;
	int left = start, right = end + 1;
	const int bias = array[start];

	while(1) {
		do {
			if(--right < 0) break;
		} while (array[right] > bias);
		do {
			if(++left > SIZE - 1) break;
		} while (array[left] < bias);
		if(left < right) swap(&array[left], &array[right]);
		else break;
	}
	swap(&array[start], &array[right]); // swap the bias

	if(nbThread < MAX_THREAD && ((end - right) * 100 / SIZE) >= 100 / MAX_THREAD) {
		Object *doubleganger = malloc(sizeof(Object));
		doubleganger->left = right + 1, doubleganger->right = end;
		doubleganger->array = array;
		if(pthread_create(&threads[nbThread++], NULL, &threadFunction, (void*)doubleganger) == -1)
			exit(EXIT_FAILURE);

		quicksort(array, start, right - 1, id);
	}
	else {
		quicksort(array, start, right - 1, id);
		quicksort(array, right + 1, end, id);
	}
}

int main()
{
	srand(time(NULL));
	pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
	int i, mean = 0, max = 0, min = 0, timer = 0;
	struct timeval start, end;

	for (int j = 0; j < NUMBER; ++j) {
		nbThread = 1;
		int *tab = (int *) generateData();

		Object *doubleganger = malloc(sizeof(Object));
		doubleganger->left = 0, doubleganger->right = SIZE - 1;
		doubleganger->array = tab, doubleganger->next = NULL;

		gettimeofday(&start, NULL);
		if(pthread_create(&threads[0], NULL, &threadFunction, (void*)doubleganger) == -1) exit(EXIT_FAILURE);
		for(i = 0; i < nbThread; i++) pthread_join(threads[i], NULL);
		gettimeofday(&end, NULL);

		timer = timerConvert(start, end);
		mean += timer;
		if (timer > max) max = timer;
		if (min == 0) min = timer;
		if (timer < min) min = timer;

		printf("%d ms -> %d\n", timerConvert(start, end), nbThread);
		verif(tab);
		free(doubleganger);
		free(tab);
	}
	printf("Total mean: %d sec\n", (mean / NUMBER));
	printf("Min: %d ms, Max: %d ms\n", min, max);

	return 0;
}

int *generateData() {
	int *tab = malloc(sizeof(int) * SIZE), i;
	for (i = 0; i < SIZE; ++i) tab[i] = rand();

	return tab;
}

void *threadFunction(void *arg) {
	int id, i;
	for(i = 0; i < MAX_THREAD; ++i) {
		if(pthread_equal(threads[i], pthread_self()) != 0) id = i;
	}
	Object *actuel = (Object*)arg;
	quicksort(actuel->array, actuel->left, actuel->right, id);
	pthread_exit(NULL);
}

char *numToString(int num) {
	char temp[10], *string = NULL, i = 0, j = 0, size = 0;
	sprintf(temp, "%d", num);
	size = strlen(temp) + ((strlen(temp) - 1) / 3);
	string = malloc(sizeof(char) * size);
	for(i = 0, j = 0; i < size; ++i) {
		if((size - i) % 4 == 0) string[i] = ' ';
		else string[i] = temp[j], ++j;
	}
	string[i] = '\0';
	return string;
}

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

int timerConvert(struct timeval start, struct timeval end) {
	int executionTime = (end.tv_sec - start.tv_sec) * 1000;
	executionTime += (end.tv_usec - start.tv_usec) / 1000;

	return executionTime;
}


void verif(int *array) {
	int i, j;
	for(i = 1; i < SIZE; ++i) {
		if(array[i-1] > array[i]) {
			printf("[-] Problem in the sort tab[%s] = %s, tab[%s] = %s\n", numToString(i - 1), numToString(array[i - 1]), numToString(i), numToString(array[i]));
			for(j = i - 3; j < i + 10; ++j) {
				if(j == i) printf("\033[31m[ %s ] = %s\n\033[0m", numToString(j), numToString(array[j]));
				else printf("[ %s ] = %s\n", numToString(j), numToString(array[j]));
			}
			return;
		}
	}
}
