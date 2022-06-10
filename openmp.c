#include "functions.h"
#include <omp.h>

void quicksort(int *array, int start, int end)
{
	if(start >= end) return;
	
	int bias = array[start];
	int left = start, right = end + 1;
	while (1) {
        do{
            if(--right < 0) break;
        }while(array[right] > bias);
        do{
            if(++left > SIZE - 1) break;
        }while(array[left] < bias);

		if (left < right) swap(&array[left], &array[right]);
		else break;
	}
	swap(&array[start], &array[right]);

	if (((end - right) * 100 / SIZE) >= 100 / 16) {
		#pragma omp task shared(array)
		quicksort(array, right + 1, end);

		if (start >= 0)
			quicksort(array, start, right - 1);

	} else {
		quicksort(array, start, right - 1);
		quicksort(array, right + 1, end);
	}
}

int main()
{
	srand(time(NULL));
	double start, end;
	int num_threads = 0, mean = 0, min = 0, max = 0, timer = 0;

	int *ref = (int *)generateData();
	for (int i = 0; i < ITERATION; ++i) {
		int *tab = malloc(SIZE * sizeof(int));
		for (int j = 0; j < SIZE; ++j) tab[j] = ref[j];
		printf("[%d] Inferencing : ", i);

		start = omp_get_wtime();
		#pragma omp parallel
		{
			#pragma omp single nowait
			quicksort(tab, 0, SIZE - 1);
			num_threads = omp_get_num_threads();
			#pragma omp taskwait
		}
		end = omp_get_wtime();

		timer = (int)((end - start) * 1000);
		mean += timer;
		if (timer > max) max = timer;
		if (min == 0) min = timer;
		if (timer < min) min = timer;

		printf("%d ms -> %d\n", (int)((end - start) * 1000), num_threads);
		verif(tab);
		free(tab);
	}
	printf("Total mean: %d sec\n", (mean / ITERATION));
	printf("Min: %d ms, Max: %d ms\n", min, max);

	return 0;
}
