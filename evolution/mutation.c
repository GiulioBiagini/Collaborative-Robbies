#include <stdlib.h>

#include "../simulation_const.h"
#include "../entity/action.h"
#include "mutation.h"



#define RANDOM_0_1() (\
	(double) rand() / (double) RAND_MAX\
)

void mutate_robby(action_t *robby) {
	int i;
	action_t new_action;

	for (i = 0; i < DNA_SIZE; i++) {
		if (RANDOM_0_1() <= MUTATION_PROBABILITY) {
			while ((new_action = GENERATE_RANDOM_ACTION()) == robby[i])
				;
			robby[i] = new_action;
		}
	}
}
