#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define SIZE 100000000

int *generateData();

char *numToString(int num);

void verif(int *array);
void swap(int *a, int *b);
void timerConvert(struct timeval start, struct timeval end);

void quicksort(int *array, int start, int end)
{
	if(start >= end) return;
	
	int bias = array[start];
	int left = start, right = end + 1;
	while (1) {
        do{
			right--;
            if(right < 0) break;
        }while(array[right] > bias);
        do{
            left++;
            if(left > SIZE - 1) break;
        }while(array[left] < bias);

		if (left < right) swap(&array[left], &array[right]);
		else break;
	}
	swap(&array[start], &array[right]);

	#pragma omp task shared(array) if(start >= 0)
	quicksort(array, start, right - 1);

	#pragma omp task shared(array) if(end < SIZE)
	quicksort(array, right + 1, end);
}

int main()
{
	// omp_set_num_threads(8);
	srand(time(NULL));
	double start, end;
	int num_threads = 0, mean = 0;
	for (int i = 0; i < 100; i++) {
		int *tab = (int *) generateData();

		start = omp_get_wtime();
		#pragma omp parallel
		{
			#pragma omp single nowait
			quicksort(tab, 0, SIZE - 1);
			num_threads = omp_get_num_threads();
			#pragma omp taskwait
		}
		end = omp_get_wtime();
		mean += (int)((end - start) * 1000);
		printf("%d ms -> %d\n", (int)((end - start) * 1000), num_threads);
		verif(tab);
	}
	printf("Total mean: %d\n", (int)(mean / 100.0));

	return 0;
}

int *generateData() {
	int *tab = malloc(sizeof(int) * SIZE), i;
	for(i = 0; i < SIZE; i++) tab[i] = rand();

	return tab;
}

void swap(int *a, int *b) // (int **array, int a, int b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void verif(int *array)
{
	int i, j;
	for(i = 1; i < SIZE; i++){
		if(array[i-1] > array[i]){
			printf("[-] Problem in the sort tab[%d] = %d, tab[%d] = %d\n", i - 1, array[i - 1], i, array[i]);
			for(j = i - 3; j < i + 10; j++) // display.
				printf("[ %d ] = %d\n", j, array[j]);
			return;
		}
	}
	// printf("[+] Array correctly sorted\n\n");
}

char *numToString(int num)
{
	char temp[10] = {'0'}, *string = NULL, i = 0, j = 0, size = 0;
	sprintf(temp, "%d", num);
	size = strlen(temp) + ((strlen(temp) - 1) / 3);
	string = malloc(sizeof(char) * size);
	for(i = 0, j = 0; i < size; i++){
		if((size - i) % 4 == 0) string[i] = ' ';
		else string[i] = temp[j], j++;
	}
	string[i] = '\0';
	return string;
}

void timerConvert(struct timeval start, struct timeval end)
{
	int executionTime = (end.tv_sec - start.tv_sec) * 1000;
	executionTime += (end.tv_usec - start.tv_usec) / 1000;
	printf("%d ms\n", executionTime);
	//  * 1000;
	// printf("%d sec %d ms\n", executionTime / 1000, executionTime - ((executionTime / 1000) * 1000));
}
