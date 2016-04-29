#include <stdlib.h>
#include <stdio.h>

#include "simulation_const.h"
#include "entity/item.h"
#include "entity/action.h"
#include "entity/view.h"
#include "entity/robby.h"
#include "environment.h"



/*
 * Map
 */

static item_t **map;

void allocate_map() {
	int x;
	map = (item_t**) malloc(MAP_WIDTH * sizeof(item_t*));
	for (x = 0; x < MAP_WIDTH; x++)
		map[x] = (item_t*) malloc(MAP_HEIGHT * sizeof(item_t));
}

void init_random_map() {
	int x;
	int y;
	int c;
	
	for (x = 0; x < MAP_WIDTH; x++)
		for (y = 0; y < MAP_HEIGHT; y++)
			map[x][y] = EMPTY;
	for (c = 0; c < CANS_NUMBER; ) {
		x = rand() % MAP_WIDTH;
		y = rand() % MAP_HEIGHT;
		if (map[x][y] == EMPTY) {
			map[x][y] = CAN;
			c++;
		}
	}
}



/*
 * Position
 */

static int pos_x_1;
static int pos_y_1;

static int pos_x_2;
static int pos_y_2;

#define IS_SAME_POSITION(pos_x_1, pos_y_1, pos_x_2, pos_y_2) (\
	pos_x_1 == pos_x_2 && pos_y_1 == pos_y_2\
)

#define INIT_RANDOM_POSITIONS() {\
	pos_x_1 = rand() % MAP_WIDTH;\
	pos_y_1 = rand() % MAP_HEIGHT;\
	do {\
		pos_x_2 = rand() % MAP_WIDTH;\
		pos_y_2 = rand() % MAP_HEIGHT;\
	} while (IS_SAME_POSITION(pos_x_1, pos_y_1, pos_x_2, pos_y_2));\
}

#define IS_POSITION_INTO_MAP(pos_x, pos_y) (\
	(pos_x) >= 0 && (pos_x) < MAP_WIDTH &&\
	(pos_y) >= 0 && (pos_y) < MAP_HEIGHT\
)

#define GET_ITEM_INTO_MAP(pos_x, pos_y) (\
	IS_POSITION_INTO_MAP(pos_x, pos_y) ? map[pos_x][pos_y] : WALL\
)

#define GET_ITEM_INTO_ENV(pos_x, pos_y, friend_pos_x, friend_pos_y) (\
	IS_SAME_POSITION(pos_x, pos_y, friend_pos_x, friend_pos_y) ? ROBBY :\
	GET_ITEM_INTO_MAP(pos_x, pos_y)\
)

#define UPDATE_POSITION(action, pos_x, pos_y) {\
	if (action == MOVE_UP)\
		pos_y--;\
	else if (action == MOVE_DOWN)\
		pos_y++;\
	else if (action == MOVE_LEFT)\
		pos_x--;\
	else if (action == MOVE_RIGHT)\
		pos_x++;\
}



/*
 * View
 */

static item_t *view_1;

static item_t *view_2;

#define ALLOCATE_VIEWS() {\
	view_1 = (item_t*) malloc(VIEW_SIZE * sizeof(item_t));\
	view_2 = (item_t*) malloc(VIEW_SIZE * sizeof(item_t));\
}

#define UPDATE_UNCOLLABORATIVE_CROSS_VIEW(view, pos_x, pos_y) {\
	view[0] = GET_ITEM_INTO_MAP(pos_x, (pos_y) - 1);\
	view[1] = GET_ITEM_INTO_MAP((pos_x) - 1, pos_y);\
	view[2] = GET_ITEM_INTO_MAP(pos_x, pos_y);\
	view[3] = GET_ITEM_INTO_MAP((pos_x) + 1, pos_y);\
	view[4] = GET_ITEM_INTO_MAP(pos_x, (pos_y) + 1);\
}

#define UPDATE_UNCOLLABORATIVE_SQUARE_VIEW(view, pos_x, pos_y) {\
	view[0] = GET_ITEM_INTO_MAP((pos_x) - 1, (pos_y) - 1);\
	view[1] = GET_ITEM_INTO_MAP(pos_x, (pos_y) - 1);\
	view[2] = GET_ITEM_INTO_MAP((pos_x) + 1, (pos_y) - 1);\
	view[3] = GET_ITEM_INTO_MAP((pos_x) - 1, pos_y);\
	view[4] = GET_ITEM_INTO_MAP(pos_x, pos_y);\
	view[5] = GET_ITEM_INTO_MAP((pos_x) + 1, pos_y);\
	view[6] = GET_ITEM_INTO_MAP((pos_x) - 1, (pos_y) + 1);\
	view[7] = GET_ITEM_INTO_MAP(pos_x, (pos_y) + 1);\
	view[8] = GET_ITEM_INTO_MAP((pos_x) + 1, (pos_y) + 1);\
}

#define UPDATE_COLLABORATIVE_CROSS_VIEW(view, pos_x, pos_y, f_pos_x, f_pos_y) {\
	view[0] = GET_ITEM_INTO_ENV(pos_x, (pos_y) - 1, f_pos_x, f_pos_y);\
	view[1] = GET_ITEM_INTO_ENV((pos_x) - 1, pos_y, f_pos_x, f_pos_y);\
	view[2] = GET_ITEM_INTO_ENV(pos_x, pos_y, f_pos_x, f_pos_y);\
	view[3] = GET_ITEM_INTO_ENV((pos_x) + 1, pos_y, f_pos_x, f_pos_y);\
	view[4] = GET_ITEM_INTO_ENV(pos_x, (pos_y) + 1, f_pos_x, f_pos_y);\
}

#define UPDATE_COLLABORATIVE_SQUARE_VIEW(view, pos_x, pos_y, f_pos_x, f_pos_y) {\
	view[0] = GET_ITEM_INTO_ENV((pos_x) - 1, (pos_y) - 1, f_pos_x, f_pos_y);\
	view[1] = GET_ITEM_INTO_ENV(pos_x, (pos_y) - 1, f_pos_x, f_pos_y);\
	view[2] = GET_ITEM_INTO_ENV((pos_x) + 1, (pos_y) - 1, f_pos_x, f_pos_y);\
	view[3] = GET_ITEM_INTO_ENV((pos_x) - 1, pos_y, f_pos_x, f_pos_y);\
	view[4] = GET_ITEM_INTO_ENV(pos_x, pos_y, f_pos_x, f_pos_y);\
	view[5] = GET_ITEM_INTO_ENV((pos_x) + 1, pos_y, f_pos_x, f_pos_y);\
	view[6] = GET_ITEM_INTO_ENV((pos_x) - 1, (pos_y) + 1, f_pos_x, f_pos_y);\
	view[7] = GET_ITEM_INTO_ENV(pos_x, (pos_y) + 1, f_pos_x, f_pos_y);\
	view[8] = GET_ITEM_INTO_ENV((pos_x) + 1, (pos_y) + 1, f_pos_x, f_pos_y);\
}

#define UPDATE_VIEWS() {\
	switch (VIEW_TYPE) {\
		case UNCOLLABORATIVE_CROSS_VIEW:\
			UPDATE_UNCOLLABORATIVE_CROSS_VIEW(view_1, pos_x_1, pos_y_1);\
			UPDATE_UNCOLLABORATIVE_CROSS_VIEW(view_2, pos_x_2, pos_y_2);\
			break;\
		case UNCOLLABORATIVE_SQUARE_VIEW:\
			UPDATE_UNCOLLABORATIVE_SQUARE_VIEW(view_1, pos_x_1, pos_y_1);\
			UPDATE_UNCOLLABORATIVE_SQUARE_VIEW(view_2, pos_x_2, pos_y_2);\
			break;\
		case COLLABORATIVE_CROSS_VIEW:\
			UPDATE_COLLABORATIVE_CROSS_VIEW(view_1, pos_x_1, pos_y_1, pos_x_2, pos_y_2);\
			UPDATE_COLLABORATIVE_CROSS_VIEW(view_2, pos_x_2, pos_y_2, pos_x_1, pos_y_1);\
			break;\
		default:\
			UPDATE_COLLABORATIVE_SQUARE_VIEW(view_1, pos_x_1, pos_y_1, pos_x_2, pos_y_2);\
			UPDATE_COLLABORATIVE_SQUARE_VIEW(view_2, pos_x_2, pos_y_2, pos_x_1, pos_y_1);\
	}\
}



/*
 * Index
 *
 * example:
 * view size = 5 & items number = 3 -> |0|2|0|1|2|
 *
 * conversion algorithm from base "items number" to base "10":
 * (2 * 3^0) + (1 * 3^1) + (0 * 3^2) + (2 * 3^3) + (0 * 3^4) = 57
 */

static int index_1;

static int index_2;

int get_action_index_from_uncollaborative_view(item_t *view) {
	int i;
	int index;
	
	index = 0;
	for (i = VIEW_SIZE - 1; i >= 0; i--)
		index += (view[i] * pow((ITEMS_NUMBER - 1), VIEW_SIZE - i - 1));
	
	return index;
}

int get_action_index_from_collaborative_view(item_t *view) {
	int i;
	int index;
	
	index = 0;
	for (i = VIEW_SIZE - 1; i >= 0; i--)
		index += (view[i] * pow(ITEMS_NUMBER, VIEW_SIZE - i - 1));
	
	return index;
}

#define UPDATE_INDEXES() {\
	if (IS_COLLABORATIVE_VIEW) {\
		index_1 = get_action_index_from_collaborative_view(view_1);\
		index_2 = get_action_index_from_collaborative_view(view_2);\
	} else {\
		index_1 = get_action_index_from_uncollaborative_view(view_1);\
		index_2 = get_action_index_from_uncollaborative_view(view_2);\
	}\
}



/*
 * Action
 */

static action_t action_1;

static action_t action_2;

#define UPDATE_ACTION(robby, index, action) {\
	if ((action = robby[index]) == RANDOM_ACTION)\
		action = GET_RANDOM_ACTION();\
}

#define UPDATE_ACTIONS(pair) {\
	UPDATE_ACTION((pair)->robby_1, index_1, action_1);\
	UPDATE_ACTION((pair)->robby_2, index_2, action_2);\
}



/*
 * Environment
 */

void init_environment() {
	allocate_map();
	ALLOCATE_VIEWS();
}

void evaluate(pair_t *pair) {
#ifdef DEBUG
	int i;
	int x;
	int y;
#endif
	int s;
	int a;
	int new_pos_x_1;
	int new_pos_y_1;
	int new_pos_x_2;
	int new_pos_y_2;
	
	pair->fitness_value = 0;
	
	/* for each session */
	for (s = 0; s < SESSIONS_NUMBER; s++) {
		init_random_map();
		INIT_RANDOM_POSITIONS();
		
		/* for each action */
		for (a = 0; a < ACTIONS_PER_SESSION_NUMBER; a++) {
			
#ifdef DEBUG
			for (y = 0; y < MAP_HEIGHT; y++) {
				for (x = 0; x < MAP_WIDTH; x++) {
					printf(
						"|%c%c%c",
						IS_SAME_POSITION(pos_x_1, pos_y_1, x, y) ? '1' : ' ',
						PRINT_ITEM(GET_ITEM_INTO_MAP(x, y)),
						IS_SAME_POSITION(pos_x_2, pos_y_2, x, y) ? '2' : ' '
					);
				}
				printf("|\n");
			}
#endif
			
			/* update robbies views, indexes, actions */
			UPDATE_VIEWS();
			UPDATE_INDEXES();
			UPDATE_ACTIONS(pair);
			
#ifdef DEBUG
			printf("Robby 1: |");
			for (i = 0; i < VIEW_SIZE; i++)
				printf("%c|", PRINT_ITEM(view_1[i]));
			printf(
				(pair->robby_1[index_1] == RANDOM_ACTION) ?
				" -> dna[%d] -> random: %s\n" : " -> dna[%d] -> %s\n",
				index_1,
				PRINT_ACTION(action_1)
			);
			printf("Robby 2: |");
			for (i = 0; i < VIEW_SIZE; i++)
				printf("%c|", PRINT_ITEM(view_2[i]));
			printf(
				(pair->robby_2[index_2] == RANDOM_ACTION) ?
				" -> dna[%d] -> random: %s\n" : " -> dna[%d] -> %s\n",
				index_2,
				PRINT_ACTION(action_2)
			);
#endif
			
			/* update robbies new positions */
			new_pos_x_1 = pos_x_1;
			new_pos_y_1 = pos_y_1;
			new_pos_x_2 = pos_x_2;
			new_pos_y_2 = pos_y_2;
			UPDATE_POSITION(action_1, new_pos_x_1, new_pos_y_1);
			UPDATE_POSITION(action_2, new_pos_x_2, new_pos_y_2);
			
			/* evaluate robby 1 movement */
			if (IS_MOVEMENT_ACTION(action_1)) {
				if (
					(!IS_POSITION_INTO_MAP(new_pos_x_1, new_pos_y_1)) ||
					(IS_COLLABORATIVE_VIEW &&
					 IS_SAME_POSITION(new_pos_x_1, new_pos_y_1, new_pos_x_2, new_pos_y_2))
				)
					pair->fitness_value -= 5;
				else {
					pos_x_1 = new_pos_x_1;
					pos_y_1 = new_pos_y_1;
				}
			}
			
			/* evaluate robby 2 movement */
			if (IS_MOVEMENT_ACTION(action_2)) {
				if (
					(!IS_POSITION_INTO_MAP(new_pos_x_2, new_pos_y_2)) ||
					(IS_COLLABORATIVE_VIEW &&
					 IS_SAME_POSITION(new_pos_x_2, new_pos_y_2, new_pos_x_2, new_pos_y_2))
				)
					pair->fitness_value -= 5;
				else {
					pos_x_2 = new_pos_x_2;
					pos_y_2 = new_pos_y_2;
				}
			}
			
			/* evaluate robby 1 pick up */
			if (action_1 == PICK_UP) {
				if (map[pos_x_1][pos_y_1] == CAN) {
					map[pos_x_1][pos_y_1] = EMPTY;
					pair->fitness_value += 10;
				} else
					pair->fitness_value -= 1;
			}
			
			/* evaluate robby 2 pick up */
			if (action_2 == PICK_UP) {
				if (map[pos_x_2][pos_y_2] == CAN) {
					map[pos_x_2][pos_y_2] = EMPTY;
					pair->fitness_value += 10;
				} else if (action_1 != PICK_UP || !IS_SAME_POSITION(pos_x_1, pos_y_1, pos_x_2, pos_y_2))
					pair->fitness_value -= 1;
			}
#ifdef DEBUG
			printf("Fitness: %f\n", pair->fitness_value);
#endif
		}
#ifdef DEBUG
		printf("\n\n");
#endif
	}
	
	pair->fitness_value = (double) pair->fitness_value / (double) SESSIONS_NUMBER;
}
