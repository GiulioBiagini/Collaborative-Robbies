#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <mpi.h>

#include "simulation_const.h"
#include "entity/robby.h"
#include "evolution/mergesort.h"
#include "evolution/crossover.h"
#include "evolution/mutation.h"
#include "environment.h"



#define TAG_SEND_RECV_PAIR 666

#define TAG_SEND_RECV_FITNESS 777

#define SEND_PAIR(pair, dest) {\
	MPI_Send(\
		(pair)->robby_1, DNA_SIZE, MPI_CHAR,\
		dest, TAG_SEND_RECV_PAIR, MPI_COMM_WORLD\
	);\
	MPI_Send(\
		(pair)->robby_2, DNA_SIZE, MPI_CHAR,\
		dest, TAG_SEND_RECV_PAIR, MPI_COMM_WORLD\
	);\
}

#define RECV_PAIR(pair, src) {\
	MPI_Recv(\
		(pair)->robby_1, DNA_SIZE, MPI_CHAR,\
		src, TAG_SEND_RECV_PAIR, MPI_COMM_WORLD, MPI_STATUS_IGNORE\
	);\
	MPI_Recv(\
		(pair)->robby_2, DNA_SIZE, MPI_CHAR,\
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
	int g;
	int p;
	int my_id;
	int processes_number;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	MPI_Comm_size(MPI_COMM_WORLD, &processes_number);
	
	INIT_CONST();
	
	srand(SEED + my_id);
	
	/* MASTER */
	if (my_id == 0) {
		int dest;
		pair_t **population;
		
		population = allocate_population();
		INIT_RANDOM_POPULATION(population);
		
		init_merge_sort();
		init_crossover();
		
		for (g = 0; g < GENERATIONS_NUMBER; g++) {
			/* send pairs to processes for evaluation */
			for (p = 0; p < PAIRS_NUMBER; p++) {
				dest = (p % (processes_number - 1)) + 1;
				SEND_PAIR(population[p], dest);
			}
			/* receive fitness values from processes */
			for (p = 0; p < PAIRS_NUMBER; p++) {
				dest = (p % (processes_number - 1)) + 1;
				RECV_FITNESS(&(population[p]->fitness_value), dest);
			}
			/* evolve population */
			MERGE_SORT(population);
			printf("%d -> %f\n", (g + 1), population[0]->fitness_value);
			if (g < GENERATIONS_NUMBER - 1) {
				crossover_population(population);
				MUTATE_POPULATION(population);
			}
		}
		/* print robbies dna */
		printf("Rooby 1: |");
		for (g = 0; g < DNA_SIZE; g++)
			printf("%d|", population[0]->robby_1[g]);
		printf("\nRooby 2: |");
		for (g = 0; g < DNA_SIZE; g++)
			printf("%d|", population[0]->robby_1[g]);
	/* SLAVE */
	} else {
		pair_t *pair;
		
		pair = allocate_pair();
		init_environment();
		
		for (g = 0; g < GENERATIONS_NUMBER; g++) {
			/* receive pair, evaluate it and send fitness value */
			for (p = my_id; p <= PAIRS_NUMBER; p+= (processes_number - 1)) {
				RECV_PAIR(pair, 0);
				evaluate(pair);
				SEND_FITNESS(&(pair->fitness_value), 0);
			}
		}
	}
	
	MPI_Finalize();
	
	return 0;
}
