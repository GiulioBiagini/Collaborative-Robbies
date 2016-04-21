#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__



#include "entity/item.h"
#include "entity/action.h"
#include "entity/map.h"
#include "entity/view.h"
#include "entity/robby.h"



typedef struct environment {
	pair_t *pair;			/* the pair of robbies */
	map_t *map;				/* the map */
	position_t *pos_1;		/* the position of the first robby */
	position_t *pos_2;		/* the position of the second robby */
} environment_t;



environment_t *allocate_environment(int map_width, int map_height, int cans_number);

void init_random_environment(environment_t *env);

void execute_step(environment_t *env);



#endif
