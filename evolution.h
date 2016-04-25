#ifndef __EVOLUTION_H__
#define __EVOLUTION_H__



#include "entity/robby.h"



#define RANDOM_0_1() (\
	(double) rand() / (double) RAND_MAX\
)



void merge(population_t *A, pair_t **B, int left, int center, int right);

void merge_sort(population_t *population, pair_t **tmp, int left, int right);



void mutate_robby(robby_t *robby, double mutation_probability);

#define MUTATE_PAIR(pair, mutation_probability) {\
	mutate_robby((pair)->robby_1, mutation_probability);\
	mutate_robby((pair)->robby_2, mutation_probability);\
}

void mutate_population(population_t *population, double mutation_probability);


/*
int get_chosed_index(int p,int size);

void crossing_over_population(population_t *A, population_t *B, float mutation_probability);
*/


void evolve(population_t *population, double mutation_probability);



/*
void print_population(population_t *A);

void generate_random_fitness(population_t *A);

void crossing_over_pair(pair_t *A1, pair_t *A2, pair_t *B1, pair_t *B2);
*/
#endif
