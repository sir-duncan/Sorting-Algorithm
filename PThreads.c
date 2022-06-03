#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define SIZE 100000000
#define MAX_THREAD 16

void verif();
char *numToString(int num);
void *threadFunction();
void swap(int a, int b);
void quicksort(int start, int end, int id);
void timerConvert(struct timeval start, struct timeval end);

typedef struct t_object Object;
struct t_object {
	int right, left;
	Object *next;
};

int nbThread = 0, counter = 1, tab[SIZE];
pthread_t threads[MAX_THREAD];
pthread_mutex_t lock;

void quicksort(int start, int end, int id)
{
	if(start >= end) return;
	int left = start, right = end + 1;
	//swap(start, rand() % (end - start) + 1);
	const int bias = tab[start];

	while(1){
        do{
            right--;
            if(right < 0) break;
        }while(tab[right] > bias);
        do{
            left++;
            if(left > SIZE - 1) break;
        }while(tab[left] < bias);
        if(left < right) swap(left, right);
        else break;
	}
	swap(start, right); // swap the bias

    if(nbThread < MAX_THREAD && ((end - right) * 100 / SIZE) >= 100 / MAX_THREAD) {
        Object *doubleganger = malloc(sizeof(Object));
        doubleganger->left = right + 1, doubleganger->right = end;
        if(pthread_create(&threads[nbThread++], NULL, &threadFunction, (void*)doubleganger) == -1)
            exit(EXIT_FAILURE);
        //printf("Taking %d : [ %s ; %s ] => %f %%\n", id, numToString(start), numToString(right - 1), (float)(right - start) * 100.0 / (float)SIZE);
        quicksort(start, right - 1, id);
    }
    else {
        quicksort(start, right - 1, id);
        quicksort(right + 1, end, id);
    }
}

int main()
{
	nbThread = 1;
	srand(time(NULL));
	int i, result, temp = 0;
    struct timeval start, end;
    printf("[\033[34m*\033[0m] Generating Data... (%s)\n", numToString(SIZE));
	gettimeofday(&start, NULL);
	for(i = 0; i < SIZE; i++) tab[i] = rand();
	gettimeofday(&end, NULL);
	printf("[\033[32m+\033[0m] Data generated in ");
    timerConvert(start, end);

	puts("\n[\033[34m*\033[0m] Starting to sort...");
    Object *doubleganger = malloc(sizeof(Object));
	doubleganger->left = 0, doubleganger->right = SIZE - 1, doubleganger->next = NULL;
	gettimeofday(&start, NULL);
	if(pthread_create(&threads[0], NULL, &threadFunction, (void*)doubleganger) == -1) exit(EXIT_FAILURE);

    for(i = 0; i < MAX_THREAD; i++)
        pthread_join(threads[i], NULL);

	gettimeofday(&end, NULL);
	printf("[\033[32m+\033[0m] Array sorted with %d threads in ", nbThread);
    timerConvert(start, end);
	verif();

	return 0;
}

void *threadFunction(void *arg)
{
	int id, i;
	for(i = 0; i < MAX_THREAD; i++)
		if(pthread_equal(threads[i], pthread_self()) != 0) id = i;
    Object *actuel = (Object*)arg;
    //printf("Taking %d : [ %s ; %s ] => %f %%\n", id, numToString(actuel->left), numToString(actuel->right), (float)(actuel->right - actuel->left) * 100.0 / (float)SIZE);
    quicksort(actuel->left, actuel->right, id);
    pthread_exit(NULL);
}

char *numToString(int num)
{
	char temp[10], *string = NULL, i = 0, j = 0, size = 0;
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

void swap(int a, int b)
{
	int temp = tab[a];
	tab[a] = tab[b];
	tab[b] = temp;
}

void timerConvert(struct timeval start, struct timeval end)
{
    int executionTime = (end.tv_sec - start.tv_sec) * 1000;
    executionTime += (end.tv_usec - start.tv_usec) / 1000;
    printf("%d sec %d ms\n", executionTime / 1000, executionTime - ((executionTime / 1000) * 1000));
}


void verif()
{
	int i, j;
	for(i = 1; i < SIZE; i++){
		if(tab[i-1] > tab[i]){
			printf("[\033[31m-\033[0m] Problem in the sort tab[%s] = %s, tab[%s] = %s\n", numToString(i - 1), numToString(tab[i - 1]), numToString(i), numToString(tab[i]));
			for(j = i - 3; j < i + 10; j++) // display
				if(j == i) printf("\033[31m[ %s ] = %s\n\033[0m", numToString(j), numToString(tab[j]));
				else printf("[ %s ] = %s\n", numToString(j), numToString(tab[j]));
			return;
		}
	}
	printf("[\033[32m+\033[0m] Array correctly sorted\n\n");
}
