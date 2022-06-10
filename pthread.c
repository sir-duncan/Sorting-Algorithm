#include "functions.h"
#include <pthread.h>

#define MAX_THREAD 	16

void *threadFunction();
void quicksort(int *array, int start, int end, int id);

int timerConvert(struct timeval start, struct timeval end);

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
	int mean = 0, max = 0, min = 0, timer = 0;
	struct timeval start, end;

	for (int i = 0; i < ITERATION; ++i) {
		nbThread = 1;
		int *tab = (int *) generateData();
		printf("[%d] Inferencing : ", i);

		Object *doubleganger = malloc(sizeof(Object));
		doubleganger->left = 0, doubleganger->right = SIZE - 1;
		doubleganger->array = tab, doubleganger->next = NULL;

		gettimeofday(&start, NULL);
		if(pthread_create(&threads[0], NULL, &threadFunction, (void*)doubleganger) == -1) exit(EXIT_FAILURE);
		for(int j = 0; j < nbThread; ++j) pthread_join(threads[j], NULL);
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
	printf("Total mean: %d sec\n", (mean / ITERATION));
	printf("Min: %d ms, Max: %d ms\n", min, max);

	return 0;
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

int timerConvert(struct timeval start, struct timeval end) {
	int executionTime = (end.tv_sec - start.tv_sec) * 1000;
	executionTime += (end.tv_usec - start.tv_usec) / 1000;

	return executionTime;
}
