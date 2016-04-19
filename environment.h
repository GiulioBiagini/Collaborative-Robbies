#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__



#include "entity/map.h"
#include "entity/robby.h"



typedef struct environment {
	map_t *map;				/* the map */
	position_t *pos_1;		/* the position of the first robby */
	position_t *pos_2;		/* the position of the second robby */
	pair_t *pair;			/* the pair of robbies */
} environment_t;



environment_t *allocate_environment(int map_width, int map_height, int cans_number);

void init_random_environment(environment_t *env);

#define SET_PAIR(env, new_pair)\
	(env)->pair = new_pair

#define UPDATE_ENVIRONMENT(env)\
	UPDATE_PAIR((env)->pair, (env)->map, (env)->pos_1, (env)->pos_2)

void perform_actions(environment_t *env);



#endif
