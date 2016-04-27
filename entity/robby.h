#ifndef __ROBBY_H__
#define __ROBBY_H__



#include "action.h"
#include "../simulation_const.h"



typedef struct pair {
	action_t *robby_1;
	action_t *robby_2;
	double fitness_value;
} pair_t;



pair_t *allocate_pair();

pair_t **allocate_population();

#define INIT_RANDOM_ROBBY(robby) {\
	int j;\
	for (j = 0; j < DNA_SIZE; j++)\
		(robby)[j] = GENERATE_RANDOM_ACTION();\
}

#define INIT_RANDOM_PAIR(pair) {\
	INIT_RANDOM_ROBBY((pair)->robby_1);\
	INIT_RANDOM_ROBBY((pair)->robby_2);\
}

#define INIT_RANDOM_POPULATION(population) {\
	int i;\
	for (i = 0; i < PAIRS_NUMBER; i++)\
		INIT_RANDOM_PAIR(population[i]);\
}



#endif
