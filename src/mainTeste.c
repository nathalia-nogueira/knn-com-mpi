#include <stdio.h>
#include <stdlib.h>
#include "knn.h"

int main() {
    int P_data[] = {
        1, 2,
        2, 4,
        5, 7,
        8, 9
    };

    int Q_data[] = {
        2, 3,
        6, 8
    };

    int np = 4;  
    int nq = 2;  
    int d  = 2;  
    int k  = 2;

    int *result = knn(Q_data, nq, P_data, np, d, k);

    printf("k nearest neighbors (indices in P):\n");
    for (int i = 0; i < nq; i++) {
        printf("Q[%d]: ", i);
        for (int j = 0; j < k; j++)
            printf("%d ", result[i * k + j]);
        printf("\n");
    }

    destroyArray(result);

    return 0;
}
