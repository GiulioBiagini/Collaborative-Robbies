#include <stdlib.h>
#include <stdio.h>

#include "entity/item.h"
#include "entity/action.h"
#include "entity/map.h"
#include "entity/view.h"
#include "entity/robby.h"
#include "environment.h"



environment_t *allocate_environment(int map_width, int map_height, int cans_number) {
	environment_t *env;
	
	env = (environment_t*) malloc(sizeof(environment_t));
	env->map = allocate_map(map_width, map_height, cans_number);
	env->pos_1 = allocate_position();
	env->pos_2 = allocate_position();
	
	return env;
}

void init_random_environment(environment_t *env) {
	init_random_map(env->map);
	INIT_RANDOM_POSITION(env->pos_1, env->map);
	do {
		INIT_RANDOM_POSITION(env->pos_2, env->map);
	} while (IS_SAME_POSITION_3(env->pos_1, env->pos_2));
}

void execute_step(environment_t *env) {
#ifdef DEBUG
	int i;
	int x;
	int y;
#endif
	
	robby_t *tmp_robby;
	position_t *tmp_robby_pos;
	position_t *tmp_friend_pos;
	
	int index_1;
	int index_2;
	int *tmp_index;
	action_t action_1;
	action_t action_2;
	action_t *tmp_action;
	position_t new_pos_1;
	position_t new_pos_2;
	position_t *tmp_new_pos;
	
	/* update robbies view, calculate corresponding actions and new positions */
	
#ifdef DEBUG
	for (y = 0; y < env->map->height; y++) {
		for (x = 0; x < env->map->width; x++) {
			printf(
				"|%c%c%c",
				IS_SAME_POSITION_2(env->pos_1, x, y) ? '1' : ' ',
				PRINT_ITEM(GET_ITEM_INTO_MAP_1(env->map, x, y)),
				IS_SAME_POSITION_2(env->pos_2, x, y) ? '2' : ' '
			);
		}
		printf("|\n");
	}
#endif
	
	tmp_robby = env->pair->robby_1;
	tmp_robby_pos = env->pos_1;
	tmp_friend_pos = env->pos_2;
	tmp_index = &index_1;
	tmp_action = &action_1;
	tmp_new_pos = &new_pos_1;
	
	UPDATE: UPDATE_VIEW(
		tmp_robby->view, tmp_robby->view_type,
		env->map, tmp_robby_pos, tmp_friend_pos
	);
	*tmp_index = GET_ACTION_INDEX_FROM_VIEW(
		tmp_robby->view_type,
		tmp_robby->view_size,
		tmp_robby->view
	);
	*tmp_action = tmp_robby->dna[*tmp_index];
	if (*tmp_action == RANDOM_ACTION)
		*tmp_action = GET_RANDOM_ACTION();
	
	COPY_POSITION(tmp_robby_pos, tmp_new_pos);
	
	if (*tmp_action == MOVE_UP)
		PERFORM_MOVE_UP(tmp_new_pos);
	else if (*tmp_action == MOVE_DOWN)
		PERFORM_MOVE_DOWN(tmp_new_pos);
	else if (*tmp_action == MOVE_LEFT)
		PERFORM_MOVE_LEFT(tmp_new_pos);
	else if (*tmp_action == MOVE_RIGHT)
		PERFORM_MOVE_RIGHT(tmp_new_pos);
	
	if (tmp_robby == env->pair->robby_1) {
		tmp_robby = env->pair->robby_2;
		tmp_robby_pos = env->pos_2;
		tmp_friend_pos = env->pos_1;
		tmp_index = &index_2;
		tmp_action = &action_2;
		tmp_new_pos = &new_pos_2;
		goto UPDATE;
	} else {
		tmp_robby = env->pair->robby_1;
		tmp_robby_pos = env->pos_1;
		tmp_friend_pos = env->pos_2;
		tmp_index = &index_1;
		tmp_action = &action_1;
		tmp_new_pos = &new_pos_1;
	}
	
	/* evaluate robbies actions */
	
	EVALUATE:
	
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
	
	if (IS_MOVEMENT_ACTION(*tmp_action)) {
		if (
			(!IS_POSITION_INTO_MAP_2(tmp_new_pos, env->map)) ||
			(IS_COLLABORATIVE_VIEW(tmp_robby->view_type) &&
			 IS_SAME_POSITION_3(&new_pos_1, &new_pos_2))
		)
			env->pair->fitness_value -= 5;
		else
			COPY_POSITION(tmp_new_pos, tmp_robby_pos);
	} else if (*tmp_action == PICK_UP) {
		if (GET_ITEM_INTO_MAP_2(env->map, tmp_new_pos) == CAN) {
			SET_ITEM_INTO_MAP(env->map, tmp_new_pos, EMPTY);
			env->pair->fitness_value += 10;
		} else
			env->pair->fitness_value -= 1;
	}
	
	if (tmp_robby == env->pair->robby_1) {
		tmp_robby = env->pair->robby_2;
		tmp_robby_pos = env->pos_2;
		tmp_friend_pos = env->pos_1;
		tmp_index = &index_2;
		tmp_action = &action_2;
		tmp_new_pos = &new_pos_2;
		goto EVALUATE;
	}
	
#ifdef DEBUG
	printf("Fitness: %f\n", env->pair->fitness_value);
#endif
}
