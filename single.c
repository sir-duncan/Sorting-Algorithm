#include "functions.h"

int timerConvert(struct timeval start, struct timeval end);

void quicksort(int *array, int start, int end) {
	if(start >= end) return;

	int left = start, right = end + 1;
	const int bias = array[start];
	while(1) {
		do {
			if(--right < 0) break;
		} while(array[right] > bias);
		do {
			if(++left > SIZE - 1) break;
		} while(array[left] < bias);
		if(left < right) swap(&array[left], &array[right]);
		else break;
	}
	swap(&array[start], &array[right]);
	if(start >= 0) quicksort(array, start, right - 1);
	if(end < SIZE) quicksort(array, right + 1, end);
}

int main()
{
	srand(time(NULL));
	int mean = 0, max = 0, min = 0, timer = 0;
	struct timeval start, end;

	int *ref = (int *)generateData();
	for (int i = 0; i < ITERATION; ++i) {
		int *tab = malloc(SIZE * sizeof(int));
		for (int j = 0; j < SIZE; ++j) tab[j] = ref[j];
		printf("[%d] Inferencing : ", i);

		gettimeofday(&start, NULL);
		quicksort(tab, 0, SIZE - 1);
		gettimeofday(&end, NULL);

		timer = timerConvert(start, end);
		mean += timer;
		if (timer > max) max = timer;
		if (min == 0) min = timer;
		if (timer < min) min = timer;

		printf("%d ms\n", timer);
		verif(tab);
		free(tab);
	}
	printf("Total mean: %d ms\n", (mean / ITERATION));
	printf("Min: %d ms, Max: %d ms\n", min, max);

	return 0;
}

int timerConvert(struct timeval start, struct timeval end) {
	int executionTime = (end.tv_sec - start.tv_sec) * 1000;
	executionTime += (end.tv_usec - start.tv_usec) / 1000;

	return executionTime;
}
