#include "maxHeap.h"

// (c) by W.Zola set/25
//
// EXAMPLE Sequential MAX-Heap, 
//                    decreaseMax operation 
//                    and others

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

void drawHeapTreeV0(float heap[], int size, int nLevels) {
    int offset = 0;
        
    int nElements = 1;
    for (int level = 0; level < nLevels; level++) {
        
        // print all elements in this level     
        for(int i = offset; i < size && i<(offset+nElements); i++) {
            printf( "[%.2f]", heap[i] );   
        }   
        printf( "\n" );
        
        offset += nElements;
        nElements *= 2;
    }
}

void printNSpaces(int n) {
    for (int i = 0; i < n; i++)
        printf(" ");
}

void drawHeapTree(float heap[], int size, int nLevels) {
    int offset = 0;
    
    int nElements = 1;
    for (int level = 0; level < nLevels; level++) {
        
        // print all elements in this level     
        for (int i = offset; i < size && i < (offset+nElements); i++) {
            printNSpaces(((pow(2,nLevels-1-level))*2)-2);
            printf("[%.2f]", heap[i]);
            printNSpaces(((pow(2,nLevels-1-level))*2)-2);
        }   
        printf("\n");
        
        offset += nElements;
        nElements *= 2;
    }
}

void swap_floats(float *a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void swap_ints(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(float heap[], int size, int i, int heapIndices[]) {
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left] > heap[largest])
            largest = left;

        if (right < size && heap[right] > heap[largest])
            largest = right;

        if (largest != i) {
            swap_floats(&heap[i], &heap[largest]);
            swap_ints(&heapIndices[i], &heapIndices[largest]);
            i = largest;
        } else {
            break;
        }
    }
}

void heapifyUp(float heap[], int *size, int pos, int heapIndices[]) {
    float val = heap[pos];
    int index = heapIndices[pos];

    while (pos > 0 && val > heap[parent(pos)]) {
        heap[pos] = heap[parent(pos)];
        heapIndices[pos] = heapIndices[parent(pos)];
        pos = parent(pos);
    }

    heap[pos] = val;
    heapIndices[pos] = index;
}
        
void insert(float heap[], int *size, float element, int heapIndices[], int origIndex) {
    *size += 1;
    int last = *size - 1;
     
    heap[last] = element; 
    heapIndices[last] = origIndex;
    heapifyUp(heap, size, last, heapIndices);
}   

int isMaxHeap(float heap[], int size) {   
    for (int i=1; i<size; i++ )
        if (heap[i] <= heap[parent(i)]) {
            continue;
        } else {
            printf("\nbroke at [%d]=%.2f\n", i, heap[i]);
            printf("father at [%d]=%.2f\n", parent(i), heap[parent(i)]);
           return 0;
        }   
    return 1;       
}

void decreaseMax(float heap[], int size, float new_value, int heapIndices[], int origIndex) {
    if (size == 0) 
        return;

    if (heap[0] > new_value) {
        heap[0] = new_value;
        heapIndices[0] = origIndex;
        #if SHOW_DECREASE_MAX_STEPS 
            int nLevels = (int)log2(size) + 1;
            printf("-------------- heap after decreasing top to %.2f\n", new_value);
            drawHeapTree(heap, size, nLevels);
            printf("    ~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        #endif
        maxHeapify(heap, size, 0, heapIndices);
    }  
}
