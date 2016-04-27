#include <stdlib.h>
#include <stdio.h>

#include "entity/item.h"
#include "entity/action.h"
#include "entity/map.h"
#include "entity/view.h"
#include "entity/robby.h"
#include "environment.h"



/* map */
static item_t **map;
/* robby 1 position */
static int pos_x_1;
static int pos_y_1;
/* robby 2 position */
static int pos_x_2;
static int pos_y_2;

static view_type_t view_type;
static int view_size;
/* views of robbies */
static item_t *view_1;
static item_t *view_2;
/* index of corresponding actions in dna */
static int index_1;
static int index_2;
/* actions relative to the views */
static action_t action_1;
static action_t action_2;

static int sessions_number;
static int actions_per_session_number;



/*
 * Map
 */

allocate_map() {
	map = (item_t**) malloc(MAP_WIDTH * sizeof(item_t*));
	for (x = 0; x < map_width; x++)
		map[x] = (item_t*) malloc(map_height * sizeof(item_t));
}

void init_random_map() {
	int x;
	int y;
	int c;
	
	for (x = 0; x < map_width; x++)
		for (y = 0; y < map_height; y++)
			map[x][y] = EMPTY;
	for (c = 0; c < cans_number; ) {
		x = rand() % map_width;
		y = rand() % map_height;
		if (map[x][y] == EMPTY) {
			map[x][y] = CAN;
			c++;
		}
	}
}

/*
 * Position
 */

#define IS_SAME_POSITION(pos_x_1, pos_y_1, pos_x_2, pos_y_2) (\
	pos_x_1 == pos_x_2 && pos_y_1 == pos_y_2\
)

#define INIT_RANDOM_POSITIONS() {\
	pos_1_x = rand() % map_width;\
	pos_1_y = rand() % map_height;\
	do {\
		pos_2_x = rand() % map_width;\
		pos_2_y = rand() % map_height;\
	} while (IS_SAME_POSITIONS(pos_1_x, pos_1_y, pos_2_x, pos_2_y));\
}

#define IS_POSITION_INTO_MAP(pos_x, pos_y) (\
	(pos_x) >= 0 && (pos_x) < map_width &&\
	(pos_y) >= 0 && (pos_y) < map_height\
)

#define GET_ITEM_INTO_MAP(pos_x, pos_y, friend_pos_x, frined_pos_y) (\
	IS_SAME_POSITION(pos_x, pos_y, friend_pos_x, friend_pos_y) ? ROBBY :\
	map[pos_x][pos_y]\
)



/*
 * View
 */

#define ALLOCATE_VIEWS() {\
	view_1 = (item_t*) malloc(GET_VIEW_SIZE(view_type) * sizeof(item_t));\
	view_2 = (item_t*) malloc(GET_VIEW_SIZE(view_type) * sizeof(item_t));\
}

#define UPDATE_UNCOLLABORATIVE_CROSS_VIEW(view, pos_x, pos_y) {\
	view[0] = map[pos_x][pos_y - 1];\
	view[1] = map[pos_x - 1][pos_y];\
	view[2] = map[pos_x][pos_y];\
	view[3] = map[pos_x + 1][pos_y];\
	view[4] = map[pos_x][pos_y + 1];\
}

#define UPDATE_UNCOLLABORATIVE_SQUARE_VIEW(view, pos_x, pos_y) {\
	view[0] = map[pos_x - 1][pos_y - 1];\
	view[1] = map[pos_x][pos_y - 1];\
	view[2] = map[pos_x + 1][pos_y - 1];\
	view[3] = map[pos_x - 1][pos_y];\
	view[4] = map[pos_x][pos_y];\
	view[5] = map[pos_x + 1][pos_y];\
	view[6] = map[pos_x - 1][pos_y + 1];\
	view[7] = map[pos_x][pos_y + 1];\
	view[8] = map[pos_x + 1][pos_y + 1];\
}

#define UPDATE_COLLABORATIVE_CROSS_VIEW(view, pos_x, pos_y, f_pos_x, f_pos_y) {\
	view[0] = GET_ITEM_INTO_MAP(pos_x, pos_y - 1, f_pos_x, f_pos_y);\
	view[1] = GET_ITEM_INTO_MAP(pos_x - 1, pos_y, f_pos_x, f_pos_y);\
	view[2] = GET_ITEM_INTO_MAP(pos_x, pos_y, f_pos_x, f_pos_y);\
	view[3] = GET_ITEM_INTO_MAP(pos_x + 1, pos_y, f_pos_x, f_pos_y);\
	view[4] = GET_ITEM_INTO_MAP(pos_x, pos_y + 1, f_pos_x, f_pos_y);\
}

#define UPDATE_COLLABORATIVE_SQUARE_VIEW(view, pos_x, pos_y, f_pos_x, f_pos_y) {\
	view[0] = GET_ITEM_INTO_MAP(pos_x - 1, pos_y - 1, f_pos_x, f_pos_y);\
	view[1] = GET_ITEM_INTO_MAP(pos_x, pos_y - 1, f_pos_x, f_pos_y);\
	view[2] = GET_ITEM_INTO_MAP(pos_x + 1, pos_y - 1, f_pos_x, f_pos_y);\
	view[3] = GET_ITEM_INTO_MAP(pos_x - 1, pos_y, f_pos_x, f_pos_y);\
	view[4] = GET_ITEM_INTO_MAP(pos_x, pos_y, f_pos_x, f_pos_y);\
	view[5] = GET_ITEM_INTO_MAP(pos_x + 1, pos_y, f_pos_x, f_pos_y);\
	view[6] = GET_ITEM_INTO_MAP(pos_x - 1, pos_y + 1, f_pos_x, f_pos_y);\
	view[7] = GET_ITEM_INTO_MAP(pos_x, pos_y + 1, f_pos_x, f_pos_y);\
	view[8] = GET_ITEM_INTO_MAP(pos_x + 1, pos_y + 1, f_pos_x, f_pos_y);\
}

#define UPDATE_VIEW(view, pos_x, pos_y, f_pos_x, f_pos_y) {\
	switch (view_type) {\
		case UNCOLLABORATIVE_CROSS_VIEW:\
			UPDATE_UNCOLLABORATIVE_CROSS_VIEW(view, pos_x, pos_y);\
			break;\
		case UNCOLLABORATIVE_SQUARE_VIEW:\
			UPDATE_UNCOLLABORATIVE_SQUARE_VIEW(view, pos_x, pos_y);\
			break;\
		case COLLABORATIVE_CROSS_VIEW:\
			UPDATE_COLLABORATIVE_CROSS_VIEW(view, pos_x, pos_y, f_pos_x, f_pos_y);\
			break;\
		default:\
			UPDATE_COLLABORATIVE_SQUARE_VIEW(view, pos_x, pos_y, f_pos_x, f_pos_y);\
	}\
}

/*
 * example:
 * view size = 5 & items number = 3 -> |0|2|0|1|2|
 *
 * conversion algorithm from base "items number" to base "10":
 * (2 * 3^0) + (1 * 3^1) + (0 * 3^2) + (2 * 3^3) + (0 * 3^4) = 57
 */

int get_action_index_from_uncollaborative_view(item_t *view) {
	int i;
	int index;
	
	index = 0;
	for (i = view_size - 1; i >= 0; i--)
		index += (view[i] * pow((ITEMS_NUMBER - 1), view_size - i - 1));
	
	return index;
}

int get_action_index_from_collaborative_view(item_t *view) {
	int i;
	int index;
	
	index = 0;
	for (i = view_size - 1; i >= 0; i--)
		index += (view[i] * pow(ITEMS_NUMBER, view_size - i - 1));
	
	return index;
}

#define GET_ACTION_INDEX_FROM_VIEW(view) (\
	IS_UNCOLLABORATIVE_VIEW(view_type) ?\
	get_action_index_from_uncollaborative_view(view) :\
	get_action_index_from_collaborative_view(view)\
)



/*
 * Environment
 */

void init_environment(view_type_t vt, int vs, int mw, int mh, int cn, int sn, int apsn) {
	view_type = vt;
	view_size = vs;
	
	map_width = mw;
	map_height = mh;
	cans_number = cn;
	allocate_map();
	
	ALLOCATE_VIEWS();
	
	sessions_number = sn;
	actions_per_session_number = apsn;
}

void evaluate(pair_t *pair) {
#ifdef DEBUG
	int i;
	int x;
	int y;
#endif
	int s;
	int a;
	
	pair->fitness_value = 0;
	
	/* for each session */
	for (s = 0; s < sessions_number; s++) {
		init_random_map();
		INIT_RANDOM_POSITIONS();
		/* for each action */
		for (a = 0; a < actions_per_session_number; a++) {
			
#ifdef DEBUG
			for (y = 0; y < map_height; y++) {
				for (x = 0; x < map_width; x++) {
					printf(
						"|%c%c%c",
						IS_SAME_POSITION(pos_x_1, pos_y_1, x, y) ? '1' : ' ',
						PRINT_ITEM(GET_ITEM_INTO_MAP_1(x, y)),
						IS_SAME_POSITION(pos_x_2, pos_y_2, x, y) ? '2' : ' '
					);
				}
				printf("|\n");
			}
#endif
			
			/* update robby 1 view, index, action and new position */
			UPDATE_VIEW(view_1, pos_x_1, pos_y_1, pos_x_2, pos_y_2);
			index_1 = GET_ACTION_INDEX_FROM_VIEW(view_1);
			action_1 = pair->robby_1[index_1];
			if (action_1 == RANDOM_ACTION)
				action_1 = GET_RANDOM_ACTION();
			if (action_1 == MOVE_UP) {
				new_pos_x_1 = pos_x_1;
				new_pos_y_1 = pos_y_1 - 1;
			} else if (action_1 == MOVE_DOWN) {
				new_pos_x_1 = pos_x_1;
				new_pos_y_1 = pos_y_1 + 1;
			} else if (action_1 == MOVE_LEFT) {
				new_pos_x_1 = pos_x_1 - 1;
				new_pos_y_1 = pos_y_1;
			} else if (action_1 == MOVE_RIGHT) {
				new_pos_x_1 = pos_x_1 + 1;
				new_pos_y_1 = pos_y_1;
			}
			
			/* update robby 2 view, index, action and new position */
			UPDATE_VIEW(view_2, pos_x_2, pos_y_2, pos_x_1, pos_y_1);
			index_2 = GET_ACTION_INDEX_FROM_VIEW(view_2);
			action_2 = pair->robby_2[index_2];
			if (action_2 == RANDOM_ACTION)
				action_2 = GET_RANDOM_ACTION();
			if (action_2 == MOVE_UP) {
				new_pos_x_2 = pos_x_2;
				new_pos_y_2 = pos_y_2 - 1;
			} else if (action_2 == MOVE_DOWN) {
				new_pos_x_2 = pos_x_2;
				new_pos_y_2 = pos_y_2 + 1;
			} else if (action_2 == MOVE_LEFT) {
				new_pos_x_2 = pos_x_2 - 1;
				new_pos_y_2 = pos_y_2;
			} else if (action_2 == MOVE_RIGHT) {
				new_pos_x_2 = pos_x_2 + 1;
				new_pos_y_2 = pos_y_2;
			}
			
#ifdef DEBUG
	printf(tmp_robby == env->pair->robby_1 ? "Robby 1: |" : "Robby 2: |");
	for (i = 0; i < tmp_robby->view_size; i++)
		printf("%c|", PRINT_ITEM(tmp_robby->view[i]));
	printf(
		(tmp_robby->dna[*tmp_index] == RANDOM_ACTION) ?
		" -> dna[%d] -> random: %s\n" : " -> dna[%d] -> %s\n",
		*tmp_index,
		PRINT_ACTION(*tmp_action)
	);
#endif
			
			/* evaluate robby 1 movement */
			if (IS_MOVEMENT_ACTION(action_1)) {
				if (
					(!IS_POSITION_INTO_MAP(new_pos_x_1, new_pos_y_1)) ||
					(IS_COLLABORATIVE_VIEW(view_type) &&
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
					(IS_COLLABORATIVE_VIEW(view_type) &&
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
		}
	}
	
	pair->fitness_value /= sessions_number;
			
#ifdef DEBUG
	printf("Fitness: %f\n", env->pair->fitness_value);
#endif
}
