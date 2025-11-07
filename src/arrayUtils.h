#ifndef _ARRAY_UTILS_
#define _ARRAY_UTILS_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void geraConjuntoDeDados(float *C, int nc, int d);
int **allocateZeroedMatrix(int rows, int columns);
int *allocateZeroedIntArray(int numElements);
float *allocateZeroedFloatArray(int numElements);
void destroyMatrix(void **matrix, int rows);
void destroyArray(void *array);


#endif