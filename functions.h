# ifndef FUNCTION_H
# define FUNCTION_H

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

#define SIZE		10000000
#define ITERATION	5

int *generateData();

char *numToString(int num);

void verif(int *array);
void swap(int *a, int *b);

typedef struct t_object {
	int right, left;
	int *array;
	struct t_object *next;
} Object;


# endif