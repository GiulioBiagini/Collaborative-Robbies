#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	} while (IS_SAME_POSITION_2(env->pos_1, env->pos_2));
}

void perform_actions(environment_t *env) {
	position_t new_pos_1;
	position_t new_pos_2;
	
	COPY_POSITION(env->pos_1, &new_pos_1);
	COPY_POSITION(env->pos_2, &new_pos_2);
	
	EVALUATE_ROBBY_1: switch (env->pair->robby_1->action) {
		case MOVE_UP:
			PERFORM_MOVE_UP(&new_pos_1);
			break;
		case MOVE_DOWN:
			PERFORM_MOVE_DOWN(&new_pos_1);
			break;
		case MOVE_LEFT:
			PERFORM_MOVE_LEFT(&new_pos_1);
			break;
		case MOVE_RIGHT:
			PERFORM_MOVE_RIGHT(&new_pos_1);
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
			break;
		case MOVE_DOWN:
			PERFORM_MOVE_DOWN(&new_pos_2);
			break;
		case MOVE_LEFT:
			PERFORM_MOVE_LEFT(&new_pos_2);
			break;
		case MOVE_RIGHT:
			PERFORM_MOVE_RIGHT(&new_pos_2);
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
	}
	
	/* TODO -> continue evaluating robbies (same positions?) */
}
