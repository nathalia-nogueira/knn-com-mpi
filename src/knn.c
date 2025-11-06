#include "knn.h"
/* -------------------------- Helper functions --------------------------*/
// Computes the squared distance between two points
int distanceSquared(int firstPoint[], int secondPoint[], int dimension) {
    int distance, diff;

    distance = 0;
    diff = 0;
    for (int i = 0; i < dimension; i++) {
        diff = secondPoint[i] - firstPoint[i]; 
        distance += diff * diff;
    }

    return distance;
}

/* -------------------------- Core functions --------------------------*/
// Computes the k nearest neighbors of each point in Q with respect to P.
int **knn(int **Q, int nq, int **P, int np, int d, int k) {
    int **result = allocateZeroedMatrix(nq, k);
    int *heap, *heapIndices, heapSize = 0;
    int distance;

    for (int i = 0; i < nq; i++) {
        heap = allocateZeroedArray(k);
        heapIndices = allocateZeroedArray(k);

        // If fewer than k points have been seen, insert into the heap.
        // Once k points are stored, apply decreaseMax for each new point.
        for (int j = 0; j < np; j++) {
            distance = distanceSquared(Q[i], P[j], d);
            if (j < k) {
                insert(heap, &heapSize, distance, heapIndices, j);
            } else {
                decreaseMax(heap, heapSize, distance, heapIndices, j);
            }
        }

        printf("\n");
        // Copy the k nearest neighbors of Q[i] to row i of the result matrix.
        for (int p = 0; p < heapSize; p++) {
            result[i][p] = heapIndices[p];
        }

        destroyArray(heap);
        destroyArray(heapIndices);
    }

    return result;
}