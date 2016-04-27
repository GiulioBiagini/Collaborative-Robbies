#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "entity/action.h"
#include "entity/robby.h"
#include "simulation_const.h"
#include "evolution.h"



/*
 * Merge Sort
 */

static pair_t **B;

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



/*
 * Crossover
 */

static pair_t **new_population;

void generate_robby(action_t *parent_1, action_t *parent_2, action_t *child) {
	int cut_point;
	
	cut_point = rand() % DNA_SIZE;
	
	memcpy(child, parent_1, cut_point);
	memcpy(child + cut_point, parent_2 + cut_point, DNA_SIZE - cut_point);
}

#define GENERATE_PAIR(parent_1, parent_2, child) {\
	generate_robby((parent_1)->robby_1, (parent_2)->robby_1, (child)->robby_1);\
	generate_robby((parent_1)->robby_2, (parent_2)->robby_2, (child)->robby_2);\
}

pair_t *get_random_pair_weight(pair_t **population) {
	int i;
	int k;
	float p;
	int size;
	
	p = (rand() % (PAIRS_NUMBER + 1)) * (PAIRS_NUMBER / 2);
	size = PAIRS_NUMBER;
	i = 0;
	k = 0;
	while(i < p) {
		i = i + size;
		size--;
		k++;
	}
	return population[k];
}

void generate_population(pair_t **population) {
	int i;
	pair_t ** tmp;
	pair_t *parent_1;
	pair_t *parent_2;
	
	for (i = 0; i < PAIRS_NUMBER; i++) {
		parent_1 = get_random_pair_weight(population);
		do {
			parent_2 = get_random_pair_weight(population);
		} while (parent_1 == parent_2);
		GENERATE_PAIR(parent_1, parent_2, new_population[i]);
	}
	
	tmp = population;
	population = new_population;
	new_population = tmp;
}



/*
 * Mutation
 */

#define RANDOM_0_1() (\
	(double) rand() / (double) RAND_MAX\
)

void mutate_robby(action_t *robby) {
	int i;
	action_t new_action;
	
	for (i = 0; i < DNA_SIZE; i++) {
		if (RANDOM_0_1() <= MUTATION_PROBABILITY) {
			do {
				new_action = GENERATE_RANDOM_ACTION();
			} while (robby[i] == new_action);
			robby[i] = new_action;
		}
	}
}

#define MUTATE_PAIR(pair) {\
	mutate_robby((pair)->robby_1);\
	mutate_robby((pair)->robby_2);\
}

#define MUTATE_POPULATION(population) {\
	int i;\
	for (i = 0; i < PAIRS_NUMBER; i++)\
		MUTATE_PAIR((population)[i]);\
}



/*
 * Evolution
 */

void init_evolution() {
	B = (pair_t**) malloc(PAIRS_NUMBER * sizeof(pair_t*));
	new_population = allocate_population();
}

void evolve(pair_t **population) {
	/* sort pairs according to their fitness values */
    merge_sort(population, 0, PAIRS_NUMBER - 1);
	/* generate new pairs with crossover technique */
	generate_population(population);
	/* mutate pairs */
	MUTATE_POPULATION(population);
}
