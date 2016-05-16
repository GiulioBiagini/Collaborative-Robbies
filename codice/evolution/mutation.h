#ifndef __MUTATION_H__
#define __MUTATION_H__



#include "../simulation_const.h"
#include "../entity/action.h"
#include "../entity/robby.h"



void mutate_robby(action_t *robby);

#define MUTATE_PAIR(pair) {\
	mutate_robby((pair)->robby_1);\
	mutate_robby((pair)->robby_2);\
}

#define MUTATE_POPULATION(population) {\
	int i;\
	for (i = (MUTATE_FROM_PAIR - 1); i < PAIRS_NUMBER; i++)\
		MUTATE_PAIR(population[i]);\
}



#endif
