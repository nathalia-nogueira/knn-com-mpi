#ifndef _MAX_HEAP_
#define _MAX_HEAP_

#include <stdio.h>
#include <math.h>

#define SHOW_DECREASE_MAX_STEPS 0
#define MAX_HEAP_SIZE (1024*1024)
#define parent(pos) ( (pos-1)/2 )

void drawHeapTreeV0(float heap[], int size, int nLevels);
void drawHeapTree(float heap[], int size, int nLevels);
void maxHeapify(float heap[], int size, int i, int heapIndices[]);
void heapifyUp(float heap[], int *size, int pos, int heapIndices[]);
void insert(float heap[], int *size, float element, int heapIndices[], int origIndex);
int isMaxHeap(float heap[], int size);
void decreaseMax(float heap[], int size, float new_value, int heapIndices[], int origIndex);

#endif