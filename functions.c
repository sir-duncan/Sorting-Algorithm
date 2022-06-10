#include "functions.h"

int *generateData()
{
	int *tab = malloc(sizeof(int) * SIZE), i;
	for (i = 0; i < SIZE; ++i)
		tab[i] = rand();

	return tab;
}

void swap(int *a, int *b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

void verif(int *array)
{
	int i, j;
	for (i = 1; i < SIZE; ++i)
	{
		if (array[i - 1] > array[i])
		{
			printf("[-] Problem in the sort tab[%d] = %d, tab[%d] = %d\n", i - 1, array[i - 1], i, array[i]);
			for (j = i - 3; j < i + 10; ++j)
				printf("[ %d ] = %d\n", j, array[j]);
			return;
		}
	}
}

char *numToString(int num)
{
	char temp[10] = {'0'}, *string = NULL, i = 0, j = 0, size = 0;
	sprintf(temp, "%d", num);
	size = strlen(temp) + ((strlen(temp) - 1) / 3);
	string = malloc(sizeof(char) * size);
	for (i = 0, j = 0; i < size; ++i)
	{
		if ((size - i) % 4 == 0)
			string[i] = ' ';
		else
			string[i] = temp[j], ++j;
	}
	string[i] = '\0';
	return string;
}
