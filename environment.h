#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__



#include "entity/robby.h"



void init_environment(
	view_type_t view_type, int view_size,
	int map_width, int map_height, int cans_number,
	int sessions_number, int actions_per_session_number
);

void evaluate(pair_t *pair);



#endif
