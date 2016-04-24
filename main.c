#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "mpi.h"

#include "entity/view.h"
#include "environment.h"
#include "evolution.h"



const int MAP_WIDTH = 10;

const int MAP_HEIGHT = 10;

const int CANS_NUMBER = 10;

const int VIEW_TYPE = UNCOLLABORATIVE_CROSS_VIEW;

const int PAIRS_NUMBER = 1;

const int GENERATIONS_NUMBER = 1;

const int SESSIONS_NUMBER = 10;

const int ACTIONS_PER_SESSION_NUMBER = 10;

const int MUTATION_PROBABILITY = 0.005;

const int SEED = 10;



#define SEND_PAIR(pair, dest)\
	MPI_Send((pair)->robby_1->dna, (pair)->robby_1->dna_size, MPI_INT, dest, 666, MPI_COMM_WORLD);\
	MPI_Send((pair)->robby_2->dna, (pair)->robby_2->dna_size, MPI_INT, dest, 666, MPI_COMM_WORLD)

#define RECV_PAIR(pair, src)\
	MPI_Recv((pair)->robby_1->dna, (pair)->robby_1->dna_size, MPI_INT, src, 666, MPI_COMM_WORLD, MPI_STATUS_IGNORE);\
	MPI_Recv((pair)->robby_2->dna, (pair)->robby_2->dna_size, MPI_INT, src, 666, MPI_COMM_WORLD, MPI_STATUS_IGNORE)

#define SEND_FITNESS(fitness_value, dest)\
	MPI_Send(fitness_value, 1, MPI_INT, dest, 777, MPI_COMM_WORLD)

#define RECV_FITNESS(fitness_value, src)\
	MPI_Recv(fitness_value, 1, MPI_INT, src, 777, MPI_COMM_WORLD, MPI_STATUS_IGNORE)



int main(int argc, char **argv) {
	int my_id;
	int processes_number;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Comm_size(MPI_COMM_WORLD, &processes_number);
	
	srand(SEED);
	
	if (my_id == 0) {
		int g;
		int p;
		int dest;
		population_t *population;
		population_t *new_population;
		population_t *tmp;
		
		/* allocate population */
		population = allocate_population(VIEW_TYPE, PAIRS_NUMBER);
		new_population = allocate_population(VIEW_TYPE, PAIRS_NUMBER);
		
		/* init population with random dna */
		init_random_population(population);
		
		for (g = 0; g < GENERATIONS_NUMBER; g++) {
			/* send pairs to processes for evaluation */
			for (p = 0; p < population->pairs_number; p++) {
				dest = (p % (processes_number - 1)) + 1;
				SEND_PAIR(&(population->pairs[p]), dest);
			}
			/* receive fitness values from processes */
			for (p = 0; p < population->pairs_number; p++) {
				dest = (p % (processes_number - 1)) + 1;
				RECV_FITNESS(&(population->pairs[p].fitness_value), dest);
				printf(
					"GENERATION %d/%d, PAIR %d/%d, FITNESS: %f\n",
					(g + 1), GENERATIONS_NUMBER,
					(p + 1), PAIRS_NUMBER,
					population->pairs[p].fitness_value
				);
			}
			/* evolve population */
			evolve(population, new_population, MUTATION_PROBABILITY);
			tmp = population;
			population = new_population;
			new_population = tmp;
		}
	} else {
		int p;
		int s;
		int a;
		pair_t *pair;
		environment_t *env;
		
		/* allocate pair and environment */
		pair = allocate_pair(VIEW_TYPE);
		env = allocate_environment(MAP_WIDTH, MAP_HEIGHT, CANS_NUMBER);
		
		for (p = 0; p < PAIRS_NUMBER; p++) {
			/* p is my pair */
			if ((p % (processes_number - 1)) + 1 == my_id) {
				/* receive pair for evaluation */
				RECV_PAIR(pair, 0);
				/* evaluate pair */
				pair->fitness_value = 0;
				for (s = 0; s < SESSIONS_NUMBER; s++) {
					init_random_environment(env);
					for (a = 0; a < ACTIONS_PER_SESSION_NUMBER; a++)
						execute_step(env);
				}
				/* send fitness value */
				SEND_FITNESS(&(pair->fitness_value), 0);
			}
		}
	}
	
	MPI_Finalize();
	
	return 0;
}
