#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define SIZE		10000000
#define ITERATION   50

int *generateData();
int timerConvert(struct timeval start, struct timeval end);

char *numToString(int num);

void verif(int *array);
void swap(int *a, int *b);

void quicksort(int *array, int start, int end) {
	if(start >= end) return;

	int left = start, right = end + 1;
	const int bias = array[start];
	while(1) {
		do {
			right--;
			if(right < 0) break;
		} while(array[right] > bias);
		do {
			left++;
			if(left > SIZE - 1) break;
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
	int i, mean = 0;
	struct timeval start, end;

	for (int j = 0; j < ITERATION; j++) {
		int *tab = (int *)generateData();

		gettimeofday(&start, NULL);
		quicksort(tab, 0, SIZE - 1);
		gettimeofday(&end, NULL);

		mean += timerConvert(start, end);
		printf("%d ms\n", timerConvert(start, end));
		verif(tab);
		free(tab);
	}
	printf("Total mean: %d sec\n", (mean / ITERATION));

	return 0;
}

int *generateData() {
	int *tab = malloc(sizeof(int) * SIZE), i;
	for (i = 0; i < SIZE; i++) tab[i] = rand();

	return tab;
}

void swap(int *a, int *b) {
	int t = *a;
	*a = *b;
	*b = t;
}

void verif(int *array) {
	int i, j;
	for(i = 1; i < SIZE; i++) {
		if(array[i-1] > array[i]) {
			printf("[-] Problem in the sort tab[%d] = %d, tab[%d] = %d\n", i - 1, array[i - 1], i, array[i]);
			for(j = i - 3; j < i + 10; j++)
				printf("[ %d ] = %d\n", j, array[j]);
			return;
		}
	}
}

char *numToString(int num) {
	char temp[10] = {'0'}, *string = NULL, i = 0, j = 0, size = 0;
	sprintf(temp, "%d", num);
	size = strlen(temp) + ((strlen(temp) - 1) / 3);
	string = malloc(sizeof(char) * size);
	for(i = 0, j = 0; i < size; i++) {
		if((size - i) % 4 == 0) string[i] = ' ';
		else string[i] = temp[j], j++;
	}
	string[i] = '\0';
	return string;
}

int timerConvert(struct timeval start, struct timeval end) {
	int executionTime = (end.tv_sec - start.tv_sec) * 1000;
	executionTime += (end.tv_usec - start.tv_usec) / 1000;

	return executionTime;
}
