#include <stdlib.h>
#include <math.h>

#include "robby.h"



view_t *allocate_view(view_type_t view_type) {
	view_t *view;
	
	view = (view_t*) malloc(sizeof(view_t));
	view->type = view_type;
	view->items = (item_t*) malloc(view->type * sizeof(item_t));
	
	return view;
}

dna_t *allocate_dna(view_type_t view_type) {
	dna_t *dna;
	
	dna = (dna_t*) malloc(sizeof(dna_t));
	dna->size = GET_DNA_SIZE(view_type);
	dna->actions = (action_t*) malloc(dna->size * sizeof(action_t));
	
	return dna;
}

robby_t *allocate_robby(view_type_t view_type) {
	robby_t *robby;
	
	robby = (robby_t*) malloc(sizeof(robby_t));
	robby->view = allocate_view(view_type);
	robby->dna = allocate_dna(view_type);
	
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
	population->pairs = (pair_t*) malloc(population->pairs_number * sizeof(pair_t));
	for (i = 0; i < population->pairs_number; i++)
		population->pairs[i] = *(allocate_pair(view_type));
	
	return population;
}

void init_random_dna(dna_t *dna) {
	int i;
	
	for (i = 0; i < dna->size; i++)
		dna->actions[i] = GENERATE_RANDOM_ACTION();
}

void init_random_population(population_t *population) {
	int i;
	
	for (i = 0; i < population->pairs_number; i++)
		INIT_RANDOM_PAIR(&(population->pairs[i]));
}

/*
 * example:
 * view size = 5 & items number = 3 -> |0|2|0|1|2|
 *
 * conversion algorithm from base "items number" to base "10":
 * (2 * 3^0) + (1 * 3^1) + (0 * 3^2) + (2 * 3^3) + (0 * 3^4) = 57
 */
void update_robby(robby_t *robby, map_t *map, position_t *pos, position_t *f_pos) {
	int i;
	int index;
	
	UPDATE_VIEW(robby->view, map, pos, f_pos);
	
	index = 0;
	for (i = robby->view->type - 1; i >= 0; i--)
		index += (robby->view->items[i] * pow(ITEMS_NUMBER, robby->view->type - i - 1));
	
	robby->action = robby->dna->actions[index];
}
