#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define SIZE 50000000

void verif(int *array);
char *numToString(int num);
void swap(int **array, int a, int b);
void timerConvert(struct timeval start, struct timeval end);

void quicksort(int **array, int start, int end)
{
    if(start >= end) return;
    int left = start, right = end + 1;
    const int bias = (*array)[start];
    while(1){
        do{
            right--;
            if(right < 0) break;
        }while((*array)[right] > bias);
        do{
            left++;
            if(left > SIZE - 1) break;
        }while((*array)[left] < bias);
        if(left < right) swap(array, left, right);
        else break;
    }
    swap(array, start, right);
    if(start >= 0) quicksort(array, start, right - 1);
    if(end < SIZE) quicksort(array, right + 1, end);
}

int main()
{
    srand(time(NULL));
    int *tab = malloc(sizeof(int) * SIZE), i;
    struct timeval start, end;
    printf("[\033[34m*\033[0m] Generating Data... (%s)\n", numToString(SIZE));
	gettimeofday(&start, NULL);
	for(i = 0; i < SIZE; i++) tab[i] = rand();
	gettimeofday(&end, NULL);
	printf("[\033[32m+\033[0m] Data generated in ");
    timerConvert(start, end);

    puts("\n[\033[34m*\033[0m] Starting to sort...");
    gettimeofday(&start, NULL);
    quicksort(&tab, 0, SIZE-1);
    gettimeofday(&end, NULL);
    printf("[\033[32m+\033[0m] Array sorted in ");
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
    printf("[+] Array correctly sorted\n\n");
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
    printf("%d sec %d ms\n", executionTime / 1000, executionTime - ((executionTime / 1000) * 1000));
}
