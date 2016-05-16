#ifndef __ACTION_H__
#define __ACTION_H__




#include <stdlib.h>



typedef enum __attribute__ ((__packed__)) action {
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT,
	STAY_PUT,
	PICK_UP,
	RANDOM_ACTION
} action_t;



#define ACTIONS_NUMBER 7

#define PRINT_ACTION(action) (\
	(action) == MOVE_UP ? "move up" : (\
	(action) == MOVE_DOWN ? "move down" : (\
	(action) == MOVE_LEFT ? "move left" : (\
	(action) == MOVE_RIGHT ? "move right" : (\
	(action) == STAY_PUT ? "stay put" : (\
	(action) == PICK_UP ? "pick up" :\
	"random action")))))\
)

#define GENERATE_RANDOM_ACTION() (\
	rand() % ACTIONS_NUMBER\
)

#define GET_RANDOM_ACTION() (\
	rand() % ((ACTIONS_NUMBER) - 1)\
)

#define IS_MOVEMENT_ACTION(action) (\
	(action) == MOVE_UP ||\
	(action) == MOVE_DOWN ||\
	(action) == MOVE_LEFT ||\
	(action) == MOVE_RIGHT\
)



#endif
