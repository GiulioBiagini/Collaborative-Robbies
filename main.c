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

const int PAIRS_NUMBER = 100;

const int GENERATIONS_NUMBER = 100;

const int SESSIONS_NUMBER = 200;

const int ACTIONS_PER_SESSION_NUMBER = 200;

const double MUTATION_PROBABILITY = 0.005;

const int SEED = 10;



#define TAG_SEND_RECV_PAIR 666

#define TAG_SEND_RECV_FITNESS 777

#define SEND_PAIR(pair, dest) {\
	MPI_Send(\
		(pair)->robby_1->dna, (pair)->robby_1->dna_size, MPI_CHAR,\
		dest, TAG_SEND_RECV_PAIR, MPI_COMM_WORLD\
	);\
	MPI_Send(\
		(pair)->robby_2->dna, (pair)->robby_2->dna_size, MPI_CHAR,\
		dest, TAG_SEND_RECV_PAIR, MPI_COMM_WORLD\
	);\
}

#define RECV_PAIR(pair, src) {\
	MPI_Recv(\
		(pair)->robby_1->dna, (pair)->robby_1->dna_size, MPI_CHAR,\
		src, TAG_SEND_RECV_PAIR, MPI_COMM_WORLD, MPI_STATUS_IGNORE\
	);\
	MPI_Recv(\
		(pair)->robby_2->dna, (pair)->robby_2->dna_size, MPI_CHAR,\
		src, TAG_SEND_RECV_PAIR, MPI_COMM_WORLD, MPI_STATUS_IGNORE\
	);\
}

#define SEND_FITNESS(fitness_value, dest) (\
	MPI_Send(\
		fitness_value, 1, MPI_DOUBLE,\
		dest, TAG_SEND_RECV_FITNESS, MPI_COMM_WORLD\
	)\
)

#define RECV_FITNESS(fitness_value, src) (\
	MPI_Recv(\
		fitness_value, 1, MPI_DOUBLE,\
		src, TAG_SEND_RECV_FITNESS, MPI_COMM_WORLD, MPI_STATUS_IGNORE\
	)\
)



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
		
		/* allocate population and init with random dna */
		population = allocate_population(VIEW_TYPE, PAIRS_NUMBER);
		init_random_population(population);
		
		for (g = 0; g < GENERATIONS_NUMBER; g++) {
			/* send pairs to processes for evaluation */
			for (p = 0; p < population->pairs_number; p++) {
				dest = (p % (processes_number - 1)) + 1;
				SEND_PAIR(population->pairs[p], dest);
#ifdef DEBUG_MPI
				printf(
					"%d - pair %d sent to process %d\n",
					my_id, (p + 1), dest
				);
#endif
			}
			/* receive fitness values from processes */
			for (p = 0; p < population->pairs_number; p++) {
				dest = (p % (processes_number - 1)) + 1;
				RECV_FITNESS(&(population->pairs[p]->fitness_value), dest);
#ifdef DEBUG_MPI
				printf(
					"%d - fitness of pair %d received from process %d (%f)\n",
					my_id, (p + 1), dest, population->pairs[p]->fitness_value
				);
			}
			/* print fitness values */
			for (p = 0; p < population->pairs_number; p++) {
#endif
				printf(
					"GENERATION %d/%d, PAIR %d/%d, FITNESS: %f\n",
					(g + 1), GENERATIONS_NUMBER,
					(p + 1), PAIRS_NUMBER,
					population->pairs[p]->fitness_value
				);
			}
			
			/* evolve population */
			evolve(population, MUTATION_PROBABILITY);
		}
	} else {
		int g;
		int p;
		int s;
		int a;
		environment_t *env;
		
		/* allocate environment */
		env = allocate_environment(VIEW_TYPE, MAP_WIDTH, MAP_HEIGHT, CANS_NUMBER);
		
		for (g = 0; g < GENERATIONS_NUMBER; g++) {
			for (p = my_id; p <= PAIRS_NUMBER; p+= (processes_number - 1)) {
				/* receive pair for evaluation */
				RECV_PAIR(env->pair, 0);
#ifdef DEBUG_MPI
				printf(
					"%d - pair %d received\n",
					my_id, p
				);
#endif
				/* evaluate pair */
				env->pair->fitness_value = 0;
				for (s = 0; s < SESSIONS_NUMBER; s++) {
					init_random_environment(env);
					for (a = 0; a < ACTIONS_PER_SESSION_NUMBER; a++)
						execute_step(env);
				}
				env->pair->fitness_value /= SESSIONS_NUMBER;
				
				/* send fitness value */
				SEND_FITNESS(&(env->pair->fitness_value), 0);
#ifdef DEBUG_MPI
				printf(
					"%d - fitness of pair %d sent (%f)\n",
					my_id, p, env->pair->fitness_value
				);
#endif
			}
		}
	}
	
	MPI_Finalize();
	
	return 0;
}
