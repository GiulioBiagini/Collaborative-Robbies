#ifndef __SIMULATION_CONST__
#define __SIMULATION_CONST__



#include "entity/view.h"



extern const int MAP_WIDTH;

extern const int MAP_HEIGHT;

extern const int CANS_NUMBER;



extern const view_type_t VIEW_TYPE;

extern int VIEW_SIZE;

extern int IS_COLLABORATIVE_VIEW;

extern int DNA_SIZE;



extern const int PAIRS_NUMBER;

extern const int OLD_PAIRS_IN_NEW_GENERATION;



extern const int GENERATIONS_NUMBER;

extern const int SESSIONS_NUMBER;

extern const int ACTIONS_PER_SESSION_NUMBER;

extern const double MUTATION_PROBABILITY;



extern int SEED;



#define INIT_CONST() {\
	VIEW_SIZE = GET_VIEW_SIZE(VIEW_TYPE);\
	IS_COLLABORATIVE_VIEW = IS_COLLABORATIVE_VIEW(VIEW_TYPE);\
	DNA_SIZE = GET_DNA_SIZE(VIEW_TYPE);\
}



#endif
