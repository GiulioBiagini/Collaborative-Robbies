#include <stdlib.h>
#include <stdio.h>

#include "entity/view.h"
#include "environment.h"
#include "evolution.h"



/* map */

const int MAP_WIDTH = 10;

const int MAP_HEIGHT = 10;

const int CANS_NUMBER = 10;

/* population */

const view_type_t VIEW_TYPE = UNCOLLABORATIVE_CROSS_VIEW;

const int PAIRS_NUMBER = 100;

/* evolution */

const float MUTATION_PROBABILITY = 0.005;

/* simulation */

const int GENERATIONS_NUMBER = 500;

const int SESSIONS_NUMBER = 200;

const int ACTIONS_PER_SESSION_NUMBER = 200;

/* seed */

const int SIMULATION_SEED = 10;



/* main */

int main(int argc, char **argv) {
	int g;
	int p;
	int s;
	int a;
	
	environment_t *env;
	population_t *tmp;
	population_t *population;
	population_t *new_population;
	
	srand(SIMULATION_SEED);
	
	env = allocate_environment(MAP_WIDTH, MAP_HEIGHT, CANS_NUMBER);
	population = allocate_population(VIEW_TYPE, PAIRS_NUMBER);
	new_population = allocate_population(VIEW_TYPE, PAIRS_NUMBER);
	init_random_population(population);
	
	/* for all generations */
	for (g = 0; g < GENERATIONS_NUMBER; g++) {
		/* for each pair */
		for (p = 0; p < population->pairs_number; p++) {
			/* set pair into the environment and reset fitness value */
			env->pair = &(population->pairs[p]);
			env->pair->fitness_value = 0;
			/* for each cleanin session */
			for (s = 0; s < SESSIONS_NUMBER; s++) {
				/* reset pair fitness, init random map and robbies position */
				init_random_environment(env);
				/* for each action */
				for (a = 0; a < ACTIONS_PER_SESSION_NUMBER; a++) {
					/* update robbies views, relative actions and perform them */
#ifdef DEBUG
					printf(
						"%s\nGENERATION %d/%d - PAIR %d/%d - SESSION %d/%d - ACTION %d/%d\n",
						"-------------------------------------------------------",
						(g + 1), GENERATIONS_NUMBER,
						(p + 1), population->pairs_number,
						(s + 1), SESSIONS_NUMBER,
						(a + 1), ACTIONS_PER_SESSION_NUMBER
					);
#endif
					execute_step(env);
				}
			}
			env->pair->fitness_value /= SESSIONS_NUMBER;
#ifndef DEBUG
			printf(
				"GENERATION %d/%d - PAIR %d/%d -> %f\n",
				(g + 1), GENERATIONS_NUMBER,
				(p + 1), population->pairs_number,
				env->pair->fitness_value
			);
		}
#endif
		/* evolve population */
		evolve(population, new_population, MUTATION_PROBABILITY);
		tmp = population;
		population = new_population;
		new_population = tmp;
	}
	
	return 0;
}
