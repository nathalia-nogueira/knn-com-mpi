#ifndef _MAX_HEAP_
#define _MAX_HEAP_

#include <stdio.h>
#include <math.h>

#define SHOW_DECREASE_MAX_STEPS 0
#define MAX_HEAP_SIZE (1024*1024)
#define parent(pos) ( (pos-1)/2 )

void drawHeapTreeV0(int heap[], int size, int nLevels);
void drawHeapTree(int heap[], int size, int nLevels);
void maxHeapify(int heap[], int size, int i, int heapIndices[]);
void heapifyUp(int heap[], int *size, int pos, int heapIndices[]);
void insert(int heap[], int *size, int element, int heapIndices[], int origIndex);
int isMaxHeap(int heap[], int size);
void decreaseMax(int heap[], int size, int new_value, int heapIndices[], int origIndex);

#endif