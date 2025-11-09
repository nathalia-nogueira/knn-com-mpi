/* Implementação do KNN
 * Autoras:
 * - Bianca Mendes Francisco
 * - Nathália Nogueira Alves */

#include "knn.h"
/* -------------------------- Funções auxiliares --------------------------*/
// Calcula a distância quadrática entre dois pontos
float distanceSquared(float firstPoint[], float secondPoint[], int dimensionality) {
    float distance, diff;

    distance = 0;
    diff = 0;
    for(int i = 0; i < dimensionality; i++) {
        diff = secondPoint[i] - firstPoint[i]; 
        distance += diff * diff;
    }

    return distance;
}

/* -------------------------- Core functions --------------------------*/
// Calcula os k-vizinhos mais próximos de cada ponto de Q em P..
int *knn(float Q[], int nq, float P[], int np, int d, int k) {
    int *result = allocateZeroedIntArray(nq * k);
    float *heap;
    int *heapIndices;
    int heapSize;
    float distance;
    float *qPoint, *pPoint;

    heap = allocateZeroedFloatArray(k);
    heapIndices = allocateZeroedIntArray(k);

    for (int i = 0; i < nq; i++) {
        heapSize = 0;

        qPoint = Q + i * d;

        // Se menos de k pontos foram vistos, insere na heap.
        // Quando k pontos já estão armazenados, aplica decreaseMax em cada um dos novos.
        for (int j = 0; j < np; j++) {
            pPoint = P + j * d;

            distance = distanceSquared(qPoint, pPoint, d);
            
            if (j < k) {
                insert(heap, &heapSize, distance, heapIndices, j);
            } else {
                decreaseMax(heap, heapSize, distance, heapIndices, j);
            }
        }

        // Copia os k-vizinhos mais próximos de Q[i] para a linha i da matriz resultado
        for (int p = 0; p < heapSize; p++) {
            result[i * k + p] = heapIndices[p];
        }

    }

    destroyArray(heap);
    destroyArray(heapIndices);

    return result;
}