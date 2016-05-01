#include <stdlib.h>
#include <string.h>

#include "../simulation_const.h"
#include "../entity/action.h"
#include "../entity/robby.h"
#include "crossover.h"



/* the new population made with crossover */
static pair_t **new_population;

/*
 * sum from 1 to PAIRS_NUMBER (1 + 2 + 3 + ... + PAIRS_NUMBER)
 * used to calculate the probability of each pair to be chosen as a parent
 */
static int ranking_size;

/*
 * an array of ranking_size elements init with:
 * #PAIRS_NUMBER 0, #(PAIRS_NUMBER - 1) 1, #(PAIRS_NUMBER - 2) 2, ...
 * choosing random a number in this array gives the index of the pair
 * to use as parent
 */
static int* ranking_weight;



void init_crossover() {
	int i, j, k;
	
	new_population = allocate_population();
	
	ranking_size = (PAIRS_NUMBER + 1) * (PAIRS_NUMBER / (double) 2);
	ranking_weight = (int*) malloc(ranking_size * sizeof(int));
	for (i = 0, j = 0, k = 0; i < ranking_size; i++, k++) {
		if (k == (PAIRS_NUMBER - j)) {
			k = 0;
			j++;
		}
		ranking_weight[i] = j;
	}
}



#define CROSSOVER_ROBBY(parent_1, parent_2, child) {\
	int cut_point;\
	memcpy(child, parent_1, (cut_point = rand() % DNA_SIZE));\
	memcpy(child + cut_point, parent_2 + cut_point, DNA_SIZE - cut_point);\
}

#define CROSSOVER_PAIR(parent_1, parent_2, child) {\
	CROSSOVER_ROBBY((parent_1)->robby_1, (parent_2)->robby_1, (child)->robby_1);\
	CROSSOVER_ROBBY((parent_1)->robby_2, (parent_2)->robby_2, (child)->robby_2);\
}

#define GET_RANKING_PARENT(population) (\
	population[ranking_weight[rand() % ranking_size]]\
)

void crossover_population(pair_t **population) {
	int i;
	pair_t ** tmp;
	pair_t *parent_1;
	pair_t *parent_2;
	
	/* copy first pairs */
	for (i = 0; i < 10; i++) {
		memcpy(&(new_population[i]->robby_1), &(population[i]->robby_1), DNA_SIZE);
		memcpy(&(new_population[i]->robby_2), &(population[i]->robby_2), DNA_SIZE);
	}
	
	/* crossover */
	for (; i < PAIRS_NUMBER ; i++) {
		parent_1 = GET_RANKING_PARENT(population);
		while ((parent_2 = GET_RANKING_PARENT(population)) == parent_1)
			;
		CROSSOVER_PAIR(parent_1, parent_2, new_population[i]);
	}
	
	tmp = population;
	population = new_population;
	new_population = tmp;
}
