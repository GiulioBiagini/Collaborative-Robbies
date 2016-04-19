#ifndef __ENTITY_H__
#define __ENTITY_H__



typedef enum item {
	EMPTY = 0,
	CAN = 1,
	WALL = 2,
	ROBBY = 3
} item_t;

typedef enum __attribute__ ((__packed__)) action {
	MOVE_UP = 0,
	MOVE_DOWN = 1,
	MOVE_LEFT = 2,
	MOVE_RIGHT = 3,
	STAY_PUT = 4,
	PICK_UP = 5,
	RANDOM_ACTION = 6
} action_t;

typedef enum view_type {
	SINGLE_CROSS_VIEW = 5,
	SINGLE_SQUARE_VIEW = 9,
	DOUBLE_CROSS_VIEW = 10
} view_type_t;



#define ITEMS_NUMBER 4

#define ACTIONS_NUMBER 7

#define PRINT_ITEM(item) (\
	(item) == EMPTY ? ' ' : (\
	(item) == CAN ? 'o' : (\
	(item) == WALL ? '#' :\
	'r'))\
)

#define GENERATE_RANDOM_ACTION() (\
	rand() % ACTIONS_NUMBER\
)

#define GET_RANDOM_ACTION() (\
	rand() % ((ACTIONS_NUMBER) - 1)\
)



#endif
