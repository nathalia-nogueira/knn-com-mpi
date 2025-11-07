#include "knn.h"
/* -------------------------- Helper functions --------------------------*/
// Computes the squared distance between two points
float distanceSquared(float firstPoint[], float secondPoint[], int dimension) {
    float distance, diff;

    distance = 0;
    diff = 0;
    for(int i = 0; i < dimension; i++) {
        diff = secondPoint[i] - firstPoint[i]; 
        distance += diff * diff;
    }

    return distance;
}

/* -------------------------- Core functions --------------------------*/
// Computes the k nearest neighbors of each point in Q with respect to P.
int *knn(float Q[], int nq, float P[], int np, int d, int k) {
    int *result = allocateZeroedIntArray(nq * k);
    float *heap;
    int *heapIndices;
    int heapSize;
    float distance;
    float *qPoint, *pPoint;

    for (int i = 0; i < nq; i++) {
        heap = allocateZeroedFloatArray(k);
        heapIndices = allocateZeroedIntArray(k);
        heapSize = 0;

        qPoint = Q + i * d;

        // If fewer than k points have been seen, insert into the heap.
        // Once k points are stored, apply decreaseMax for each new point.
        for (int j = 0; j < np; j++) {
            pPoint = P + j * d;

            distance = distanceSquared(qPoint, pPoint, d);
            
            if (j < k) {
                insert(heap, &heapSize, distance, heapIndices, j);
            } else {
                decreaseMax(heap, heapSize, distance, heapIndices, j);
            }
        }

        // Copy the k nearest neighbors of Q[i] to row i of the result matrix.
        int l = heapSize - 1;
        for (int p = 0; p < heapSize; p++) {
            result[i * k + p] = heapIndices[l];
            l--;
        }

        destroyArray(heap);
        destroyArray(heapIndices);
    }

    return result;
}