#include <stdlib.h>

#include "item.h"
#include "action.h"
#include "view.h"
#include "robby.h"



robby_t *allocate_robby(view_type_t view_type) {
	robby_t *robby;
	
	robby = (robby_t*) malloc(sizeof(robby_t));
	robby->view_type = view_type;
	robby->view_size = GET_VIEW_SIZE(robby->view_type);
	robby->view = (item_t*) malloc(robby->view_size * sizeof(item_t));
	robby->dna_size = GET_DNA_SIZE(view_type);
	robby->dna = (action_t*) malloc(robby->dna_size * sizeof(action_t));
	
	return robby;
}

pair_t *allocate_pair(view_type_t view_type) {
	pair_t *pair;
	
	pair = (pair_t*) malloc(sizeof(pair_t));
	pair->robby_1 = allocate_robby(view_type);
	pair->robby_2 = allocate_robby(view_type);
	
	return pair;
}

population_t *allocate_population(view_type_t view_type, int pairs_number) {
	int i;
	population_t *population;
	
	population = (population_t*) malloc(pairs_number * sizeof(population_t));
	population->pairs_number = pairs_number;
	population->pairs = (pair_t**) malloc(population->pairs_number * sizeof(pair_t*));
	for (i = 0; i < population->pairs_number; i++)
		population->pairs[i] = allocate_pair(view_type);
	
	return population;
}

void init_random_robby(robby_t *robby) {
	int i;
	
	for (i = 0; i < robby->dna_size; i++)
		robby->dna[i] = GENERATE_RANDOM_ACTION();
}

void init_random_population(population_t *population) {
	int i;
	
	for (i = 0; i < population->pairs_number; i++)
		INIT_RANDOM_PAIR(population->pairs[i]);
}
