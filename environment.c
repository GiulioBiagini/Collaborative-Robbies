#include <stdlib.h>
#include <stdio.h>

#include "entity/item.h"
#include "entity/action.h"
#include "entity/view.h"
#include "entity/map.h"
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
	RESET_PAIR_FITNESS(env->pair);
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
	printf("Map before:\n");
	for (x = 0; x < env->map->width; x++) {
		for (y = 0; y < env->map->height; y++) {
			printf(
				"|%c%c%c",
				IS_SAME_POSITION_2(env->pos_1, x, y) ? '1' : ' ',
				PRINT_ITEM(GET_ITEM_INTO_MAP_1(env->map, x, y)),
				IS_SAME_POSITION_2(env->pos_2, x, y) ? '2' : ' '
			);
		}
		printf("|\n");
	}
	printf("\nUpdate views...\n");
#endif
	
	UPDATE_VIEW(
		env->pair->robby_1->view, env->pair->robby_1->view_type,
		env->map, env->pos_1, env->pos_2
	);
	UPDATE_VIEW(
		env->pair->robby_2->view, env->pair->robby_2->view_type,
		env->map, env->pos_2, env->pos_1
	);
	
	action_index_1 = get_action_index_from_view(
		env->pair->robby_1->view_size,
		env->pair->robby_1->view
	);
	action_index_2 = get_action_index_from_view(
		env->pair->robby_2->view_size,
		env->pair->robby_2->view
	);
	
	action_1 = env->pair->robby_1->dna[action_index_1];
	action_2 = env->pair->robby_2->dna[action_index_2];
	
#ifdef DEBUG
	printf("Robby 1: |");
	for (i = 0; i < env->pair->robby_1->view_size; i++)
		printf("%c|", PRINT_ITEM(env->pair->robby_1->view[i]));
	printf(" -> %s\n", PRINT_ACTION(action_1));
	printf("Robby 2: |");
	for (i = 0; i < env->pair->robby_2->view_size; i++)
		printf("%c|", PRINT_ITEM(env->pair->robby_2->view[i]));
	printf(" -> %s\n", PRINT_ACTION(action_2));
#endif
	
	
#ifdef DEBUG
	
#endif
	
	/*
	COPY_POSITION(env->pos_1, &new_pos_1);
	COPY_POSITION(env->pos_2, &new_pos_2);
	
	EVALUATE_ROBBY_1: switch (env->pair->robby_1->action) {
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
		case STAY_PUT:
			break;
		case PICK_UP:
			if (GET_ITEM_INTO_MAP_2(env->map, &new_pos_1) == CAN)
				SET_ITEM_INTO_MAP(env->map, &new_pos_1, EMPTY);
			break;
		default:
			env->pair->robby_1->action = GET_RANDOM_ACTION();
			goto EVALUATE_ROBBY_1;
	}
	
	EVALUATE_ROBBY_2: switch (env->pair->robby_2->action) {
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
		case STAY_PUT:
			break;
		case PICK_UP:
			if (GET_ITEM_INTO_MAP_2(env->map, &new_pos_2) == CAN)
				SET_ITEM_INTO_MAP(env->map, &new_pos_2, EMPTY);
			break;
		default:
			env->pair->robby_2->action = GET_RANDOM_ACTION();
			goto EVALUATE_ROBBY_2;
	}*/
	/*
	if (IS_SAME_POSITION_2(&new_pos_1, &new_pos_2)) {
		if (robby_1_moved)
	}*/
}
