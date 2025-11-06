#ifndef _KNN_
#define _KNN_

#include <stdio.h>
#include <stdlib.h>
#include "maxHeap.h"
#include "arrayUtils.h"

/* Computes the k nearest neighbors of each point in Q with respect to P.
 * Parameters:
 * - Q: matrix of query points. Each row contains the coordinates of one point.
 * - nq: number of points in Q.
 * - P: matrix of reference points. The neighbors are searched within this set.
 * - np: number of points in P.
 * - d: dimensionality of the points.
 * - k: number of nearest neighbors to return.
 */
int *knn(int Q[], int nq, int P[], int np, int d, int k);

#endif