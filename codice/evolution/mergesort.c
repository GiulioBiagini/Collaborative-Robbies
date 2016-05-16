#include <stdlib.h>

#include "../simulation_const.h"
#include "mergesort.h"



/* tmp vector used by merge sort */
static pair_t **B;



void init_merge_sort() {
	B = (pair_t**) malloc(PAIRS_NUMBER * sizeof(pair_t*));
}



void merge(pair_t **A, int left, int center, int right) {
	int i, j, k;

	i = left;
	j = center + 1;
	k = 0;

	while (i <= center && j <= right)
		if (A[i]->fitness_value > A[j]->fitness_value)
			B[k++] = A[i++];
		else
			B[k++] = A[j++];

	while (i <= center)
		B[k++] = A[i++];

	while (j <= right)
		B[k++] = A[j++];

	for (k = left; k <= right; k++)
		A[k] = B[k - left];
}

void merge_sort(pair_t **population, int left, int right) {
	int center;

	if (left < right) {
		center = (left + right) / 2;
		merge_sort(population, left, center);
		merge_sort(population, center + 1, right);
		merge(population, left, center, right);
	}
}
