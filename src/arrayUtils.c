/* Implementação de funções úteis para vetores
 * Autoras:
 * - Bianca Mendes Francisco
 * - Nathália Nogueira Alves */

#include "arrayUtils.h"

int **allocateZeroedMatrix(int rows, int columns){
    int **matrix = (int**) malloc(rows * sizeof(int*));
    if (!matrix) {
        printf("[allocateMatrix] error in malloc.\n");
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        matrix[i] = malloc(columns * sizeof(int));
        if (!matrix[i]) {
            printf("[allocateMatrix] error in malloc.\n");
            return NULL;
        }
    }
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

int *allocateZeroedIntArray(int numElements) {
    int *array = (int*) malloc(numElements * sizeof(int));

    if (!array) {
        printf("[allocateArray] error in malloc.\n");
        return NULL;
    }

    memset(array, 0, numElements * sizeof(*array));
    
    return array;
}

float *allocateZeroedFloatArray(int numElements) {
    float *array = (float*) malloc(numElements * sizeof(float));

    if (!array) {
        printf("[allocateArray] error in malloc.\n");
        return NULL;
    }

    memset(array, 0, numElements * sizeof(*array));
    
    return array;
}

void geraConjuntoDeDados(float *C, int nc, int d) {
    
    for (int i = 0; i < nc * d; i++) {
        C[i] = rand(); // numeros aleatorios entre 0 e RAND_MAX (2.147.483.647)
    }
}

void destroyMatrix(void **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void destroyArray(void *array) {
    free(array);
}