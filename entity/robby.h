#ifndef __ROBBY_H__
#define __ROBBY_H__



#include "item.h"
#include "action.h"
#include "view.h"



typedef struct robby {
	view_type_t view_type;	/* the type of the view */
	int view_size;			/* the size of the view */
	item_t *view;			/* the view of the robby as a list of items */
	int dna_size;			/* the length of the dna */
	action_t *dna;			/* the dna of the robby as a list of actions */
} robby_t;

typedef struct pair {
	robby_t *robby_1;		/* the first robby */
	robby_t *robby_2;		/* the second robby */
	double fitness_value;	/* the fitness value of the pair */
} pair_t;

typedef struct population {
	int pairs_number;		/* the number of the pairs in the population */
	pair_t **pairs;			/* the population as a list of pairs */
} population_t;



robby_t *allocate_robby(view_type_t view_type);

pair_t *allocate_pair(view_type_t view_type);

population_t *allocate_population(view_type_t view_type, int pairs_number);

void init_random_robby(robby_t *robby);

#define INIT_RANDOM_PAIR(pair) {\
	init_random_robby((pair)->robby_1);\
	init_random_robby((pair)->robby_2);\
}

void init_random_population(population_t *population);



#endif
