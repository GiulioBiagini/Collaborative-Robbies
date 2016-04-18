#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__



/* Item */

typedef enum item {
	EMPTY = 0,
	CAN = 1,
	WALL = 2,
	ROBBY = 3
} item_t;

/* Action */

typedef enum __attribute__ ((__packed__)) action {
	MOVE_UP = 0,
	MOVE_DOWN = 1,
	MOVE_LEFT = 2,
	MOVE_RIGHT = 3,
	STAY_PUT = 4,
	PICK_UP = 5,
	RANDOM_ACTION = 6
} action_t;

/* View Type */

typedef enum view_type {
	SINGLE_CROSS_VIEW = 5,
	SINGLE_SQUARE_VIEW = 9,
	DOUBLE_CROSS_VIEW = 10
} view_type_t;



/* Map */

typedef struct map {
	int width;				/* the width of the map */
	int height;				/* the height of the map */
	int cans_number;		/* the starting number of cans into the map */
	item_t **items;			/* the map as a matrix of items */
} map_t;

/* View */

typedef struct view {
	view_type_t type;		/* the type of the view */
	item_t *items;			/* the view as a list of items */
} view_t;

/* Dna */

typedef struct dna {
	int size;				/* the length of the dna */
	action_t *actions;		/* the dna as a list of actions */
} dna_t;

/* Robby */

typedef struct robby {
	view_t *view;			/* the view of the robby */
	dna_t *dna;				/* the dna of the robby */
	action_t *action;		/* the action in the dna relative to the view */
} robby_t;

typedef struct pair {
	robby_t *robby_1;		/* the first robby */
	double fitness_1;		/* the fitness value fo the first robby */
	robby_t *robby_2;		/* the second robby */
	double fitness_2;		/* the fitness value of the second robby */
	double global_fitness;	/* to average fitness of the pair */
} pair_t;

typedef struct population {
	int pairs_number;		/* the number of the pairs in the population */
	pair_t *pairs;			/* the population as a list of pairs */
} population_t;

/* Environment */

typedef struct environment {
	map_t *map;				/* the map */
	int pos_x_1;			/* the x position of the first robby */
	int pos_y_1;			/* the y position of the first robby */
	robby_t *robby_1;		/* the first robby */
	int pos_x_2;			/* the x position of the second robby */
	int pos_y_2;			/* the y position of the second robby */
	robby_t *robby_2;		/* the second robby */
} environment_t;



/* Item */

#define ITEMS_NUMBER 4

#define PRINT_ITEM(item) (\
	(item) == EMPTY ? ' ' : (\
	(item) == CAN ? 'o' : (\
	(item) == WALL ? '#' :\
	'r'))\
)

/* Action */

#define ACTIONS_NUMBER 7

#define GENERATE_RANDOM_ACTION() (\
	rand() % ACTIONS_NUMBER\
)

#define GET_RANDOM_ACTION() (\
	rand() % ((ACTIONS_NUMBER) - 1)\
)

/* Position */

void init_random_position(int *pos_x, int *pos_y, map_t *map);

#define IS_SAME_POSITION(pos_x_1, pos_y_1, pos_x_2, pos_y_2) (\
	(pos_x_1) == (pos_x_2) && (pos_y_1) == (pos_y_2)\
)

/* Map */

map_t *allocate_map(int width, int height, int cans_number);

void init_random_map(map_t *map);

#define IS_POSITION_INTO_MAP(pos_x, pos_y, map) (\
	pos_x >= 0 && pos_x < (map)->width && pos_y >= 0 && pos_y < (map)->height\
)

#define GET_ITEM_INTO_MAP_1(map, pos_x, pos_y) (\
	IS_POSITION_INTO_MAP(pos_x, pos_y, map) ? (map)->items[pos_x][pos_y] : WALL\
)

#define GET_ITEM_INTO_MAP_2(map, pos_x, pos_y, friend_pos_x, friend_pos_y) (\
	IS_SAME_POSITION(pos_x, pos_y, friend_pos_x, friend_pos_y) ? ROBBY :\
	GET_ITEM_INTO_MAP_1(map, pos_x, pos_y)\
)

/* View */

view_t *allocate_view(view_type_t view_type);

#define UPDATE_SINGLE_CROSS_VIEW(view, map, pos_x, pos_y, o_pos_x, o_pos_y)\
	(view)->items[0] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y - 1, o_pos_x, o_pos_y);\
	(view)->items[1] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y, o_pos_x, o_pos_y);\
	(view)->items[2] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y, o_pos_x, o_pos_y);\
	(view)->items[3] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y, o_pos_x, o_pos_y);\
	(view)->items[4] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y + 1, o_pos_x, o_pos_y);

#define UPDATE_SINGLE_SQUARE_VIEW(view, map, pos_x, pos_y, o_pos_x, o_pos_y)\
	(view)->items[0] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y - 1, o_pos_x, o_pos_y);\
	(view)->items[1] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y - 1, o_pos_x, o_pos_y);\
	(view)->items[2] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y - 1, o_pos_x, o_pos_y);\
	(view)->items[3] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y, o_pos_x, o_pos_y);\
	(view)->items[4] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y, o_pos_x, o_pos_y);\
	(view)->items[5] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y, o_pos_x, o_pos_y);\
	(view)->items[6] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y + 1, o_pos_x, o_pos_y);\
	(view)->items[7] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y + 1, o_pos_x, o_pos_y);\
	(view)->items[8] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y + 1, o_pos_x, o_pos_y);

#define UPDATE_DOUBLE_CROSS_VIEW(view, map, pos_x, pos_y, o_pos_x, o_pos_y)\
	(view)->items[0] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y - 1, o_pos_x, o_pos_y);\
	(view)->items[1] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y, o_pos_x, o_pos_y);\
	(view)->items[2] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y, o_pos_x, o_pos_y);\
	(view)->items[3] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y, o_pos_x, o_pos_y);\
	(view)->items[4] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y + 1, o_pos_x, o_pos_y);\
	(view)->items[5] = GET_ITEM_INTO_MAP_2(map, o_pos_x, o_pos_y - 1, pos_x, pos_y);\
	(view)->items[6] = GET_ITEM_INTO_MAP_2(map, o_pos_x - 1, o_pos_y, pos_x, pos_y);\
	(view)->items[7] = GET_ITEM_INTO_MAP_2(map, o_pos_x, o_pos_y, pos_x, pos_y);\
	(view)->items[8] = GET_ITEM_INTO_MAP_2(map, o_pos_x + 1, o_pos_y, pos_x, pos_y);\
	(view)->items[9] = GET_ITEM_INTO_MAP_2(map, o_pos_x, o_pos_y + 1, pos_x, pos_y);

#define UPDATE_VIEW(view, map, pos_x, pos_y, o_pos_x, o_pos_y)\
	switch (view->type) {\
		case SINGLE_CROSS_VIEW:\
			UPDATE_SINGLE_CROSS_VIEW(view, map, pos_x, pos_y, o_pos_x, o_pos_y);\
			break;\
		case SINGLE_SQUARE_VIEW:\
			UPDATE_SINGLE_SQUARE_VIEW(view, map, pos_x, pos_y, o_pos_x, o_pos_y);\
			break;\
		default:\
			UPDATE_DOUBLE_CROSS_VIEW(view, map, pos_x, pos_y, o_pos_x, o_pos_y);\
	}

/* Dna */

#define GET_DNA_SIZE(view_type) (\
	(int) pow((double) (ITEMS_NUMBER), (double) (view_type))\
)

dna_t *allocate_dna(view_type_t view_type);

void init_random_dna(dna_t *dna);

/* Robby */

robby_t *allocate_robby(view_type_t view_type);

pair_t *allocate_pair(view_type_t view_type);

population_t *allocate_population(view_type_t view_type, int pairs_number);

#define INIT_RANDOM_ROBBY(robby) (\
	init_random_dna((robby)->dna)\
)

void init_random_pair(pair_t *pair);

void init_random_population(population_t *population);

void update_robby(robby_t *robby, map_t *map, int pos_x, int pos_y, int o_pos_x, int o_pos_y);

/* Environment */

environment_t *allocate_environment(int map_width, int map_height, int cans_number);

void init_random_environment(environment_t *env);

void set_pair(environment_t *env, pair_t *pair);

void update_environment(environment_t *env);

void execute_environment(environment_t *env);

void print_environment(environment_t *env);



#endif
