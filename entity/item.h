#ifndef __ITEM_H__
#define __ITEM_H__



typedef enum item {
	EMPTY,
	CAN,
	WALL,
	ROBBY
} item_t;



#define ITEMS_NUMBER 4

#define PRINT_ITEM(item) (\
	(item) == EMPTY ? ' ' : (\
	(item) == CAN ? 'o' : (\
	(item) == WALL ? '#' :\
	'r'))\
)



#endif
