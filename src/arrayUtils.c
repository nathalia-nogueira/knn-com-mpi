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

int *allocateZeroedArray(int numElements) {
    int *array = (int*) malloc(numElements * sizeof(int));

    if (!array) {
        printf("[allocateArray] error in malloc.\n");
        return NULL;
    }

    for (int i = 0; i < numElements; i++) {
        array[i] = 0;
    }
    
    return array;
}

void destroyMatrix(int **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void destroyArray(int *array) {
    free(array);
}