#ifndef __VIEW_H__
#define __VIEW_H__



#include <math.h>

#include "item.h"
#include "map.h"



typedef enum view_type {
	UNCOLLABORATIVE_SINGLE_CROSS_VIEW,
	UNCOLLABORATIVE_SINGLE_SQUARE_VIEW,
	COLLABORATIVE_SINGLE_CROSS_VIEW,
	COLLABORATIVE_SINGLE_SQUARE_VIEW
} view_type_t;



#define GET_VIEW_SIZE(view_type) (\
	(view_type) == UNCOLLABORATIVE_SINGLE_CROSS_VIEW ? 5 : (\
	(view_type) == UNCOLLABORATIVE_SINGLE_SQUARE_VIEW ? 9 : (\
	(view_type) == COLLABORATIVE_SINGLE_CROSS_VIEW ? 5 : 9))\
)

#define GET_UNCOLLABORATIVE_DNA_SIZE(view_type) (\
	/* ITEMS_NUMBER - 1 because robby can't see the other robbies */\
	pow((double) (ITEMS_NUMBER - 1), (double) GET_VIEW_SIZE(view_type))\
)

#define GET_COLLABORATIVE_DNA_SIZE(view_type) (\
	pow((double) (ITEMS_NUMBER), (double) GET_VIEW_SIZE(view_type))\
)

#define GET_DNA_SIZE(view_type) (\
	(view_type) == UNCOLLABORATIVE_SINGLE_CROSS_VIEW ||\
	(view_type) == UNCOLLABORATIVE_SINGLE_SQUARE_VIEW ?\
	GET_UNCOLLABORATIVE_DNA_SIZE(view_type) :\
	GET_COLLABORATIVE_DNA_SIZE(view_type)\
)

#define UPDATE_VIEW(view, view_type, map, pos, f_pos)\
	switch (view_type) {\
		case UNCOLLABORATIVE_SINGLE_CROSS_VIEW:\
			view[0] = GET_ITEM_INTO_MAP_1(map, (pos)->x, (pos)->y - 1);\
			view[1] = GET_ITEM_INTO_MAP_1(map, (pos)->x - 1, (pos)->y);\
			view[2] = GET_ITEM_INTO_MAP_1(map, (pos)->x, (pos)->y);\
			view[3] = GET_ITEM_INTO_MAP_1(map, (pos)->x + 1, (pos)->y);\
			view[4] = GET_ITEM_INTO_MAP_1(map, (pos)->x, (pos)->y + 1);\
			break;\
		case UNCOLLABORATIVE_SINGLE_SQUARE_VIEW:\
			view[0] = GET_ITEM_INTO_MAP_1(map, (pos)->x - 1, (pos)->y - 1);\
			view[1] = GET_ITEM_INTO_MAP_1(map, (pos)->x, (pos)->y - 1);\
			view[2] = GET_ITEM_INTO_MAP_1(map, (pos)->x + 1, (pos)->y - 1);\
			view[3] = GET_ITEM_INTO_MAP_1(map, (pos)->x - 1, (pos)->y);\
			view[4] = GET_ITEM_INTO_MAP_1(map, (pos)->x, (pos)->y);\
			view[5] = GET_ITEM_INTO_MAP_1(map, (pos)->x + 1, (pos)->y);\
			view[6] = GET_ITEM_INTO_MAP_1(map, (pos)->x - 1, (pos)->y + 1);\
			view[7] = GET_ITEM_INTO_MAP_1(map, (pos)->x, (pos)->y + 1);\
			view[8] = GET_ITEM_INTO_MAP_1(map, (pos)->x + 1, (pos)->y + 1);\
			break;\
		case COLLABORATIVE_SINGLE_CROSS_VIEW:\
			view[0] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y - 1, f_pos);\
			view[1] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y, f_pos);\
			view[2] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y, f_pos);\
			view[3] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y, f_pos);\
			view[4] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y + 1, f_pos);\
			break;\
		default:\
			view[0] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y - 1, f_pos);\
			view[1] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y - 1, f_pos);\
			view[2] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y - 1, f_pos);\
			view[3] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y, f_pos);\
			view[4] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y, f_pos);\
			view[5] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y, f_pos);\
			view[6] = GET_ITEM_INTO_MAP_3(map, (pos)->x - 1, (pos)->y + 1, f_pos);\
			view[7] = GET_ITEM_INTO_MAP_3(map, (pos)->x, (pos)->y + 1, f_pos);\
			view[8] = GET_ITEM_INTO_MAP_3(map, (pos)->x + 1, (pos)->y + 1, f_pos);\
	}

int get_action_index_from_view(int view_size, item_t *view);



#endif
