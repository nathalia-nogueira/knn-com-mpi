/* Biblioteca com o algoritmo KNN
 * Autoras:
 * - Bianca Mendes Francisco
 * - Nathália Nogueira Alves */

#ifndef _KNN_
#define _KNN_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maxHeap.h"
#include "arrayUtils.h"

/* Calcula os k-vizinhos mais próximos de cada ponto de Q em P.
 * Parametros:
 * - Q: matriz principal. Cada linha contem as coordenadas de um ponto.
 * - nq: numero de pontos em Q.
 * - P: matriz de pontos de consulta. Os k-vizinhos são procurados neste conjunto.
 * - np: numero de pontos em P.
 * - d: dimensionalidade dos pontos.
 * - k: numero de vizinhos retornados.
 */
int *knn(float Q[], int nq, float P[], int np, int d, int k);

#endif