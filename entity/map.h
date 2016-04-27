#ifndef __MAP_H__
#define __MAP_H__



#include <stdlib.h>

#include "item.h"



typedef struct position {
	int x;					/* x coord of position */
	int y;					/* y coord of position */
} position_t;

typedef struct map {
	int width;				/* the width of the map */
	int height;				/* the height of the map */
	int cans_number;		/* the starting number of cans into the map */
	item_t **items;			/* the map as a matrix of items */
} map_t;



#define COPY_POSITION(src_pos, dest_pos) {\
	(dest_pos)->x = (src_pos)->x;\
	(dest_pos)->y = (src_pos)->y;\
}

#define GET_ITEM_INTO_MAP_1(map, pos_x, pos_y) (\
	(!IS_POSITION_INTO_MAP_1(pos_x, pos_y, map)) ? WALL :\
	(map)->items[pos_x][pos_y]\
)

#define GET_ITEM_INTO_MAP_2(map, pos) (\
	GET_ITEM_INTO_MAP_1(map, (pos)->x, (pos)->y)\
)

#define GET_ITEM_INTO_MAP_3(map, pos_x, pos_y, f_pos) (\
	IS_SAME_POSITION_1(pos_x, pos_y, (f_pos)->x, (f_pos)->y) ? ROBBY :\
	GET_ITEM_INTO_MAP_1(map, pos_x, pos_y)\
)

#define SET_ITEM_INTO_MAP(map, pos, item)\
	(map)->items[(pos)->x][(pos)->y] = (item)

#define PERFORM_MOVE_UP(pos)\
	(pos)->y--

#define PERFORM_MOVE_DOWN(pos)\
	(pos)->y++

#define PERFORM_MOVE_LEFT(pos)\
	(pos)->x--

#define PERFORM_MOVE_RIGHT(pos)\
	(pos)->x++



#endif
