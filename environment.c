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
	
	int action_index_1;
	int action_index_2;
	action_t action_1;
	action_t action_2;
	int robby_1_moved;
	int robby_2_moved;
	position_t new_pos_1;
	position_t new_pos_2;
	
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
	
	/* update robby 1 view and calculate corresponding action */
	
	UPDATE_VIEW(
		env->pair->robby_1->view, env->pair->robby_1->view_type,
		env->map, env->pos_1, env->pos_2
	);
	action_index_1 = GET_ACTION_INDEX_FROM_VIEW(
		env->pair->robby_1->view_type,
		env->pair->robby_1->view_size,
		env->pair->robby_1->view
	);
	action_1 = env->pair->robby_1->dna[action_index_1];
	
#ifdef DEBUG
	printf("Robby 1: |");
	for (i = 0; i < env->pair->robby_1->view_size; i++)
		printf("%c|", PRINT_ITEM(env->pair->robby_1->view[i]));
	printf(" -> %d -> %s", action_index_1, PRINT_ACTION(action_1));
#endif
	
	if (action_1 == RANDOM_ACTION) {
		action_1 = GET_RANDOM_ACTION();
#ifdef DEBUG
		printf(" -> %s\n", PRINT_ACTION(action_1));
	} else {
		printf("\n");
#endif
	}
	
	/* update robby 2 view and calculate corresponding action */
	
	UPDATE_VIEW(
		env->pair->robby_2->view, env->pair->robby_2->view_type,
		env->map, env->pos_2, env->pos_1
	);
	action_index_2 = GET_ACTION_INDEX_FROM_VIEW(
		env->pair->robby_2->view_type,
		env->pair->robby_2->view_size,
		env->pair->robby_2->view
	);
	action_2 = env->pair->robby_2->dna[action_index_2];
	
#ifdef DEBUG
	printf("Robby 2: |");
	for (i = 0; i < env->pair->robby_2->view_size; i++)
		printf("%c|", PRINT_ITEM(env->pair->robby_2->view[i]));
	printf(" -> %d -> %s", action_index_2, PRINT_ACTION(action_2));
#endif
	
	if (action_2 == RANDOM_ACTION) {
		action_2 = GET_RANDOM_ACTION();
#ifdef DEBUG
		printf(" -> %s\n", PRINT_ACTION(action_2));
	} else {
		printf("\n");
#endif
	}
	
	/* perform virtual movements */
	
	COPY_POSITION(env->pos_1, &new_pos_1);
	COPY_POSITION(env->pos_2, &new_pos_2);
	
	switch (action_1) {
		case MOVE_UP:
			PERFORM_MOVE_UP(&new_pos_1);
			robby_1_moved = 1;
			break;
		case MOVE_DOWN:
			PERFORM_MOVE_DOWN(&new_pos_1);
			robby_1_moved = 1;
			break;
		case MOVE_LEFT:
			PERFORM_MOVE_LEFT(&new_pos_1);
			robby_1_moved = 1;
			break;
		case MOVE_RIGHT:
			PERFORM_MOVE_RIGHT(&new_pos_1);
			robby_1_moved = 1;
			break;
		default:
			break;
	}
	
	switch (action_2) {
		case MOVE_UP:
			PERFORM_MOVE_UP(&new_pos_2);
			robby_2_moved = 1;
			break;
		case MOVE_DOWN:
			PERFORM_MOVE_DOWN(&new_pos_2);
			robby_2_moved = 1;
			break;
		case MOVE_LEFT:
			PERFORM_MOVE_LEFT(&new_pos_2);
			robby_2_moved = 1;
			break;
		case MOVE_RIGHT:
			PERFORM_MOVE_RIGHT(&new_pos_2);
			robby_2_moved = 1;
			break;
		default:
			break;
	}
	
	/* update fitness value and execute movements */
	
	if (!IS_POSITION_INTO_MAP_2(&new_pos_1, env->map)) {
		env->pair->fitness_value -= 5;
#ifdef DEBUG
		printf("Robby 1: CRASH INTO WALL\n");
#endif
	} else if (
		IS_SAME_POSITION_3(&new_pos_1, &new_pos_2) &&
		IS_COLLABORATIVE_VIEW(env->pair->robby_1->view_type) &&
		robby_1_moved
	) {
		env->pair->fitness_value -= 5;
#ifdef DEBUG
		printf("Robby 1: CRASH WITH ROBBY 2\n");
#endif
	} else if (robby_1_moved) {
		COPY_POSITION(&new_pos_1, env->pos_1);
#ifdef DEBUG
		printf("Robby 1: ok\n");
#endif
	}
	
	if (!IS_POSITION_INTO_MAP_2(&new_pos_2, env->map)) {
		env->pair->fitness_value -= 5;
#ifdef DEBUG
		printf("Robby 2: CRASH INTO WALL\n");
#endif
	} else if (
		IS_SAME_POSITION_3(&new_pos_2, &new_pos_1) &&
		IS_COLLABORATIVE_VIEW(env->pair->robby_2->view_type) &&
		robby_2_moved
	) {
		env->pair->fitness_value -= 5;
#ifdef DEBUG
		printf("Robby 2: CRASH WITH ROBBY 1\n");
#endif
	} else if (robby_2_moved) {
		COPY_POSITION(&new_pos_2, env->pos_2);
#ifdef DEBUG
		printf("Robby 2: ok\n");
#endif
	}
	
	/* perform pick up and update fitness values */
	
	if (action_1 == PICK_UP) {
		if(GET_ITEM_INTO_MAP_2(env->map, &new_pos_1) == CAN) {
			SET_ITEM_INTO_MAP(env->map, &new_pos_1, EMPTY);
			env->pair->fitness_value += 10;
#ifdef DEBUG
			printf("Robby 1: ok\n");
#endif
		} else {
			env->pair->fitness_value -= 1;
#ifdef DEBUG
			printf("Robby 1: NO CAN HERE\n");
#endif
		}
	}
	
	if (action_2 == PICK_UP) {
		if(GET_ITEM_INTO_MAP_2(env->map, &new_pos_2) == CAN) {
			SET_ITEM_INTO_MAP(env->map, &new_pos_2, EMPTY);
			env->pair->fitness_value += 10;
#ifdef DEBUG
			printf("Robby 2: ok\n");
#endif
		} else if (action_1 == PICK_UP && IS_SAME_POSITION_3(&new_pos_2, &new_pos_1)) {
#ifdef DEBUG
			printf("Robby 2: ok\n");
#endif
		} else {
			env->pair->fitness_value -= 1;
#ifdef DEBUG
			printf("Robby 2: NO CAN HERE\n");
#endif
		}
	}
	
#ifdef DEBUG
	printf("Fitness: %f\n", env->pair->fitness_value);
#endif
}
