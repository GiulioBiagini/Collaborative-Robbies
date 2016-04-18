#include <stdlib.h>
#include <stdio.h>

#include "environment.h"



/* map */

const int MAP_WIDTH = 10;

const int MAP_HEIGHT = 10;

const int CANS_NUMBER = 10;

/* population */

const int POPULATION_SIZE = 100;

/* evolution */

/* TODO */

/* simulation */

const int SIMULATION_SEED = 10;

const int GENERATIONS_NUMBER = 100;

const int SESSIONS_NUMBER = 100;

const int ACTIONS_PER_SESSION_NUMBER = 100;



void evaluate_pair(pair_t *pair, environment_t *env) {
	int s;
	int a;
	
	for (s = 0; < SESSIONS_NUMBER; s++) {
		for (a = 0; a < actions_number; a++) {
			
		}
	}
}

void (population_t *population) {
	int g;
	int p;
	environment_t *env;
	
	for (g = 0; g < GENERATIONS_NUMBER; g++) {
		for (p = 0; p < population->pairs_number; p++) {
			evaluate_pair(population->pairs[p], env);
		}
	}
}



/* main */

int main(int argc, char **argv) {
	
	population_t *population;
	init_random_population(population);
	
	f(population);
	
	return 0;
}
