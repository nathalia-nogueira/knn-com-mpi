#include <stdio.h>
#include "knn.h"

int main() {
    int P_data[4][2] = { {1, 2}, {2, 4}, {5, 7}, {8, 9} };
    int Q_data[2][2] = { {2, 3}, {6, 8} };

    // Converte para ponteiros de int*, como o knn espera
    int *P[4], *Q[2];
    for (int i = 0; i < 4; i++) P[i] = P_data[i];
    for (int i = 0; i < 2; i++) Q[i] = Q_data[i];

    int k = 2;
    int **result = knn(Q, 2, P, 4, 2, k);

    printf("k nearest neighbors (indices in P):\n");
    for (int i = 0; i < 2; i++) {
        printf("Q[%d]: ", i);
        for (int j = 0; j < k; j++)
            printf("%d ", result[i][j]);
        printf("\n");
    }

    // Libera memória
    for (int i = 0; i < 2; i++)
        free(result[i]);
    free(result);

    return 0;
}
