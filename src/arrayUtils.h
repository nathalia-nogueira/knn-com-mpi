#ifndef _ARRAY_UTILS_
#define _ARRAY_UTILS_

#include <stdio.h>
#include <stdlib.h>

int **allocateZeroedMatrix(int rows, int columns);
int *allocateZeroedArray(int numElements);
void destroyMatrix(int **matrix, int rows);
void destroyArray(int *array);


#endif