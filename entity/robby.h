#ifndef __ROBBY_H__
#define __ROBBY_H__



#include "entity.h"
#include "map.h"



typedef struct view {
	view_type_t type;		/* the type of the view */
	item_t *items;			/* the view as a list of items */
} view_t;

typedef struct dna {
	int size;				/* the length of the dna */
	action_t *actions;		/* the dna as a list of actions */
} dna_t;

typedef struct robby {
	view_t *view;			/* the view of the robby */
	dna_t *dna;				/* the dna of the robby */
	action_t action;		/* the action relative to the view */
} robby_t;

typedef struct pair {
	robby_t *robby_1;		/* the first robby */
	robby_t *robby_2;		/* the second robby */
	double fitness_1;		/* the fitness value fo the first robby */
	double fitness_2;		/* the fitness value of the second robby */
	double global_fitness;	/* to average fitness of the pair */
} pair_t;

typedef struct population {
	int pairs_number;		/* the number of the pairs in the population */
	pair_t *pairs;			/* the population as a list of pairs */
} population_t;



#define GET_DNA_SIZE(view_type) (\
	(int) pow((double) (ITEMS_NUMBER), (double) (view_type))\
)

view_t *allocate_view(view_type_t view_type);

dna_t *allocate_dna(view_type_t view_type);

robby_t *allocate_robby(view_type_t view_type);

pair_t *allocate_pair(view_type_t view_type);

population_t *allocate_population(view_type_t view_type, int pairs_number);

void init_random_dna(dna_t *dna);

#define INIT_RANDOM_ROBBY(robby)\
	init_random_dna((robby)->dna)

#define INIT_RANDOM_PAIR(pair) {\
	INIT_RANDOM_ROBBY((pair)->robby_1);\
	INIT_RANDOM_ROBBY((pair)->robby_2);\
}

void init_random_population(population_t *population);

#define UPDATE_VIEW(view, map, pos, f_pos)\
	switch (view->type) {\
		case SINGLE_CROSS_VIEW:\
			(view)->items[0] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y - 1, f_pos);\
			(view)->items[1] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y, f_pos);\
			(view)->items[2] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y, f_pos);\
			(view)->items[3] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y, f_pos);\
			(view)->items[4] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y + 1, f_pos);\
			break;\
		case SINGLE_SQUARE_VIEW:\
			(view)->items[0] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y - 1, f_pos);\
			(view)->items[1] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y - 1, f_pos);\
			(view)->items[2] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y - 1, f_pos);\
			(view)->items[3] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y, f_pos);\
			(view)->items[4] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y, f_pos);\
			(view)->items[5] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y, f_pos);\
			(view)->items[6] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y + 1, f_pos);\
			(view)->items[7] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y + 1, f_pos);\
			(view)->items[8] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y + 1, f_pos);\
			break;\
		default:\
			(view)->items[0] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y - 1, f_pos);\
			(view)->items[1] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y, f_pos);\
			(view)->items[2] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y, f_pos);\
			(view)->items[3] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y, f_pos);\
			(view)->items[4] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y + 1, f_pos);\
			(view)->items[5] = GET_ITEM_INTO_MAP_3(map, (f_pos)->x, (f_pos)->y - 1, pos);\
			(view)->items[6] = GET_ITEM_INTO_MAP_3(map, (f_pos)->x - 1, (f_pos)->y, pos);\
			(view)->items[7] = GET_ITEM_INTO_MAP_3(map, (f_pos)->x, (f_pos)->y, pos);\
			(view)->items[8] = GET_ITEM_INTO_MAP_3(map, (f_pos)->x + 1, (f_pos)->y, pos);\
			(view)->items[9] = GET_ITEM_INTO_MAP_3(map, (f_pos)->x, (f_pos)->y + 1, pos);\
	}

void update_robby(robby_t *robby, map_t *map, position_t *pos, position_t *f_pos);

#define UPDATE_PAIR(pair, map, pos_1, pos_2) {\
	update_robby((pair)->robby_1, map, pos_1, pos_2);\
	update_robby((pair)->robby_2, map, pos_2, pos_1);\
}

#define RESET_PAIR_FITNESS(pair) {\
	(pair)->fitness_1 = 0;\
	(pair)->fitness_2 = 0;\
	(pair)->global_fitness = 0;\
}



#endif
