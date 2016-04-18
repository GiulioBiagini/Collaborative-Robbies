#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "environment.h"



/*
 * Position
 */

void init_random_position(int *pos_x, int *pos_y, map_t *map) {
	*pos_x = rand() % map->width;
	*pos_y = rand() % map->height;
}



/*
 * Map
 */

map_t *allocate_map(int width, int height, int cans_number) {
	int x;
	map_t *map;
	
	map = (map_t*) malloc(sizeof(map_t));
	map->width = width;
	map->height = height;
	map->cans_number = cans_number;
	map->items = (item_t**) malloc(map->width * sizeof(item_t*));
	for (x = 0; x < map->width; x++)
		map->items[x] = (item_t*) malloc(map->height * sizeof(item_t));
	
	return map;
}

void init_random_map(map_t *map) {
	int x;
	int y;
	int c;
	
	for (x = 0; x < map->width; x++)
		for (y = 0; y < map->height; y++)
			map->items[x][y] = empty;
	for (c = 0; c < map->cans_number; ) {
		x = rand() % map->width;
		y = rand() % map->height;
		if (map->items[x][y] == empty) {
			map->items[x][y] = can;
			c++;
		}
	}
}



/*
 * View
 */

view_t *allocate_view(view_type_t view_type) {
	view_t *view;
	
	view = (view_t*) malloc(sizeof(view_t));
	view->type = view_type;
	view->items = (item_t*) malloc(view->type * sizeof(item_t));
	
	return view;
}



/*
 * Dna
 */

dna_t *allocate_dna(view_type_t view_type) {
	dna_t *dna;
	
	dna = (dna_t*) malloc(sizeof(dna_t));
	dna->size = GET_DNA_SIZE(view_type);
	dna->actions = (action_t*) malloc(dna->size * sizeof(action_t));
	
	return dna;
}

void init_random_dna(dna_t *dna) {
	int i;
	
	for (i = 0; i < dna->size; i++)
		dna->actions[i] = GENERATE_RANDOM_ACTION();
}



/*
 * Robby
 */

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

void init_random_pair(pair_t *pair) {
	INIT_RANDOM_ROBBY(pair->robby_1);
	INIT_RANDOM_ROBBY(pair->robby_2);
}

void init_random_population(population_t *population) {
	int i;
	
	for (i = 0; i < population->pairs_number; i++)
		init_random_pair(&(population->pairs[i]));
}

void update_robby(robby_t *robby, map_t *map, int pos_x, int pos_y, int o_pos_x, int o_pos_y) {
	int i;
	item_t *view;
	int index;
	
	view = robby->view->items;
	switch (robby->view->type) {
		single_cross_view:
			view[0] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y - 1, o_pos_x, o_pos_y);
			view[1] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y, o_pos_x, o_pos_y);
			view[2] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y, o_pos_x, o_pos_y);
			view[3] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y, o_pos_x, o_pos_y);
			view[4] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y + 1, o_pos_x, o_pos_y);
			break;
		single_square_view:
			view[0] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y - 1, o_pos_x, o_pos_y);
			view[1] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y - 1, o_pos_x, o_pos_y);
			view[2] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y - 1, o_pos_x, o_pos_y);
			view[3] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y, o_pos_x, o_pos_y);
			view[4] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y, o_pos_x, o_pos_y);
			view[5] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y, o_pos_x, o_pos_y);
			view[6] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y + 1, o_pos_x, o_pos_y);
			view[7] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y + 1, o_pos_x, o_pos_y);
			view[8] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y + 1, o_pos_x, o_pos_y);
			break;
		default:
			view[0] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y - 1, o_pos_x, o_pos_y);
			view[1] = GET_ITEM_INTO_MAP_2(map, pos_x - 1, pos_y, o_pos_x, o_pos_y);
			view[2] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y, o_pos_x, o_pos_y);
			view[3] = GET_ITEM_INTO_MAP_2(map, pos_x + 1, pos_y, o_pos_x, o_pos_y);
			view[4] = GET_ITEM_INTO_MAP_2(map, pos_x, pos_y + 1, o_pos_x, o_pos_y);
			view[5] = GET_ITEM_INTO_MAP_2(map, o_pos_x, o_pos_y - 1, pos_x, pos_y);
			view[6] = GET_ITEM_INTO_MAP_2(map, o_pos_x - 1, o_pos_y, pos_x, pos_y);
			view[7] = GET_ITEM_INTO_MAP_2(map, o_pos_x, o_pos_y, pos_x, pos_y);
			view[8] = GET_ITEM_INTO_MAP_2(map, o_pos_x + 1, o_pos_y, pos_x, pos_y);
			view[9] = GET_ITEM_INTO_MAP_2(map, 0_pos_x, o_pos_y + 1, pos_x, pos_y);
	}
	
	/*
	 * example:
	 * view size = 5 & items number = 3 -> |0|2|0|1|2|
	 *
	 * conversion algorithm from base "items number" to base "10":
	 * (2 * 3^0) + (1 * 3^1) + (0 * 3^2) + (2 * 3^3) + (0 * 3^4) = 57
	 */
	
	index = 0;
	for (i = robby->view->type - 1; i >= 0; i--)
		index += (view[i] * pow(ITEMS_NUMBER, robby->view->type - i - 1));
	
	robby->action = *(robby->dna->actions[index]);
}



/*
 * Environment
 */

environment_t *allocate_environment(int map_width, int map_height, int cans_number) {
	environment_t *env;
	
	env = (environment_t*) malloc(sizeof(environment_t));
	env->map = allocate_map(map_width, map_height, cans_number);
	
	return env;
}

void init_random_environment(environment_t *env) {
	init_random_map(env->map);
	init_random_position(&(env->pos_x_1), &(env->pos_y_1), env->map);
	do {
		init_random_position(&(env->pos_x_2), &(env->pos_y_2), env->map);
	} while (IS_SAME_POSITION(env->pos_x_1, env->pos_y_1, env->pos_x_2, env->pos_y_2));
}

void set_pair(environment_t *env, pair_t *pair) {
	env->robby_1 = pair->robby_1;
	env->robby_2 = pair->robby_2;
}

void update_environment(environment_t *env) {
	update_robby(
		env->robby_1, env->map, env->pos_x_1, env->pos_y_1, env->pos_x_2, env->pos_y_2
	);
	update_robby(
		env->robby_2, env->map, env->pos_x_2, env->pos_y_2, env->pos_x_1, env->pos_y_1
	);
}

void execute_environment(environment_t *env) {
	/* TODO */
}

void print_environment(environment_t *env) {
	int x;
	int y;
	int i;
	view_t *subject_view;
	view_t *other_view;
	
	printf("Map:\n");
	for (x = 0; x < env->map->width; x++) {
		for (y = 0; y < env->map->height; y++) {
			if (IS_SAME_POSITION(x, y, env->pos_x_1, env->pos_y_1))
				printf((GET_ITEM_INTO_MAP_1(env->map, x, y)) == can ? "U" : "u");
			else if (IS_SAME_POSITION(x, y, env->pos_x_2, env->pos_y_2))
				printf((GET_ITEM_INTO_MAP_1(env->map, x, y)) == can ? "D" : "d");
			else
				printf("|%c", PRINT_ITEM(env->map->items[x][y]));
		}
		printf("|\n");
	}
	
	subject_view = env->robby_1->view;
	other_view = env->robby_2->view;
	for (i = 1; i < 3; i++) {
		printf("Robby %d:\n", i);
		switch (view->type) {
			case single_cross_view:
				printf(
					"| |%c| |\n|%c|%c|%c|\n| |%c| |\n",
					PRINT_ITEM(subject_view->items[0]),
					PRINT_ITEM(subject_view->items[1]),
					PRINT_ITEM(subject_view->items[2]),
					PRINT_ITEM(subject_view->items[3]),
					PRINT_ITEM(subject_view->items[4])
				);
				break;
			case single_square_view:
				printf(
					"|%c|%c|%c|\n|%c|%c|%c|\n|%c|%c|%c|\n",
					PRINT_ITEM(subject_view->items[0]),
					PRINT_ITEM(subject_view->items[1]),
					PRINT_ITEM(subject_view->items[2]),
					PRINT_ITEM(subject_view->items[3]),
					PRINT_ITEM(subject_view->items[4]),
					PRINT_ITEM(subject_view->items[5]),
					PRINT_ITEM(subject_view->items[6]),
					PRINT_ITEM(subject_view->items[7]),
					PRINT_ITEM(subject_view->items[8])
				);
				break;
			default:
				printf(
					"| |%c| || |%c| |\n|%c|%c|%c||%c|%c|%c|\n| |%c| || |%c| |\n",
					PRINT_ITEM(subject_view->items[0]),
					PRINT_ITEM(subject_view->items[1]),
					PRINT_ITEM(subject_view->items[2]),
					PRINT_ITEM(subject_view->items[3]),
					PRINT_ITEM(subject_view->items[4]),
					PRINT_ITEM(other_view->items[0]),
					PRINT_ITEM(other_view->items[1]),
					PRINT_ITEM(other_view->items[2]),
					PRINT_ITEM(other_view->items[3]),
					PRINT_ITEM(other_view->items[4])
				);
		}
		subject_view = env->robby_2->view;
		other_view = env->robby_1->view;
	}
}
