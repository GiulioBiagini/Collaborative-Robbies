#include <stdlib.h>
#include <stdio.h>

#include "environment.h"



/* map */

const int MAP_WIDTH = 10;

const int MAP_HEIGHT = 10;

const int CANS_NUMBER = 10;

/* population */

const view_type_t VIEW_TYPE = SINGLE_CROSS_VIEW;

const int PAIRS_NUMBER = 100;

/* evolution */

/* TODO */

/* simulation */

const int SIMULATION_SEED = 10;

const int GENERATIONS_NUMBER = 100;

const int SESSIONS_NUMBER = 100;

const int ACTIONS_PER_SESSION_NUMBER = 100;



/* main */

int main(int argc, char **argv) {
	int g;
	int p;
	int s;
	int a;
	
	environment_t *env;
	population_t *population;
	
	env = allocate_environment(MAP_WIDTH, MAP_HEIGHT, CANS_NUMBER);
	population = allocate_population(VIEW_TYPE, PAIRS_NUMBER);
	init_random_population(population);
	
	/* for all generations */
	for (g = 0; g < GENERATIONS_NUMBER; g++) {
		/* for each pair */
		for (p = 0; p < population->pairs_number; p++) {
			/* clear fitness values */
			RESET_PAIR(&(population->pairs[p]));
			/* set pair into the environment */
			SET_PAIR(env, &(population->pairs[p]));
			/* for each cleanin session */
			for (s = 0; s < SESSIONS_NUMBER; s++) {
				/* init random map and robbies position */
				init_random_environment(env);
				/* for each action */
				for (a = 0; a < ACTIONS_PER_SESSION_NUMBER; a++) {
					/* update robbies views and relative actions */
					UPDATE_ENVIRONMENT(env);
					/* evaluate robbies actions */
					
					/* perform robbies actions */
					perform_actions(env);
				}
			}
		}
		/* TODO -> EVOLVE POPULATION */
	}
	
	return 0;
}
