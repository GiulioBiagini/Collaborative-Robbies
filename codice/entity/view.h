#ifndef __VIEW_H__
#define __VIEW_H__



#include <math.h>

#include "item.h"



typedef enum view_type {
	UNCOLLABORATIVE_CROSS_VIEW,
	UNCOLLABORATIVE_SQUARE_VIEW,
	COLLABORATIVE_CROSS_VIEW,
	COLLABORATIVE_SQUARE_VIEW
} view_type_t;



#define GET_VIEW_SIZE(view_type) (\
	(view_type) == UNCOLLABORATIVE_CROSS_VIEW ? 5 : (\
	(view_type) == UNCOLLABORATIVE_SQUARE_VIEW ? 9 : (\
	(view_type) == COLLABORATIVE_CROSS_VIEW ? 5 : 9))\
)

/* In a Uncollaborative View, robby can't see othe robbies */
#define IS_UNCOLLABORATIVE_VIEW(view_type) (\
	(view_type) == UNCOLLABORATIVE_CROSS_VIEW ||\
	(view_type) == UNCOLLABORATIVE_SQUARE_VIEW\
)

/* In a Collaborative View, robby can see othe robbies */
#define IS_COLLABORATIVE_VIEW(view_type) (\
	(view_type) == COLLABORATIVE_CROSS_VIEW ||\
	(view_type) == COLLABORATIVE_SQUARE_VIEW\
)

/* ITEMS_NUMBER - 1 because robby can't see the other robbies */
#define GET_UNCOLLABORATIVE_DNA_SIZE(view_type) (\
	pow((double) (ITEMS_NUMBER - 1), (double) GET_VIEW_SIZE(view_type))\
)

/* ITEMS_NUBER because robby can see other robbies */
#define GET_COLLABORATIVE_DNA_SIZE(view_type) (\
	pow((double) (ITEMS_NUMBER), (double) GET_VIEW_SIZE(view_type))\
)

#define GET_DNA_SIZE(view_type) (\
	IS_UNCOLLABORATIVE_VIEW(view_type) ?\
	GET_UNCOLLABORATIVE_DNA_SIZE(view_type) :\
	GET_COLLABORATIVE_DNA_SIZE(view_type)\
)



#endif
