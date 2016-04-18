#include <stdlib.h>

#include "environment.h"



double evaluate(environment_t *env, int sessions_number, int actions_per_session_number) {
	int i;
	int j;
	int *fitness;
	item_t *robby_1_view;
	item_t *robby_2_view;
	
	fitness = (int*) malloc(sessions_number * sizeof(int));
	robby_1_view = (item_t*) malloc(env->robbies_pair->robby_1->view * sizeof(item_t));
	robby_2_view = (item_t*) malloc(env->robbies_pair->robby_2->view * sizeof(item_t));
	
	/* for each session */
	for (i = 0; i < sessions_number; i++) {
		fitness[i] = 0;
		init_random_environment(env);
		/* for each action */
		for (j = 0; j < actions_number; j++) {
			robby_1_view
		}
	}
}
