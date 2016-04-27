#include <stdlib.h>

#include "action.h"
#include "../simulation_const.h"
#include "robby.h"



#define ALLOCATE_ROBBY() (\
	(action_t*) malloc(DNA_SIZE * sizeof(action_t))\
)

pair_t *allocate_pair() {
	pair_t *pair;
	
	pair = (pair_t*) malloc(sizeof(pair_t));
	pair->robby_1 = ALLOCATE_ROBBY();
	pair->robby_2 = ALLOCATE_ROBBY();
	
	return pair;
}

pair_t **allocate_population() {
	int i;
	pair_t **population;
	
	population = (pair_t**) malloc(PAIRS_NUMBER * sizeof(pair_t*));
	for (i = 0; i < PAIRS_NUMBER; i++)
		population[i] = allocate_pair();
	
	return population;
}
