#include <math.h>

#include "item.h"
#include "view.h"



/*
 * example:
 * view size = 5 & items number = 3 -> |0|2|0|1|2|
 *
 * conversion algorithm from base "items number" to base "10":
 * (2 * 3^0) + (1 * 3^1) + (0 * 3^2) + (2 * 3^3) + (0 * 3^4) = 57
 */

/* ITEMS_NUMBER - 1 because robby can't see the other robbies */
int get_action_index_from_uncollaborative_view(int view_size, item_t *view) {
	int i;
	int index;
	
	index = 0;
	for (i = view_size - 1; i >= 0; i--)
		index += (view[i] * pow((ITEMS_NUMBER - 1), view_size - i - 1));
	
	return index;
}

int get_action_index_from_collaborative_view(int view_size, item_t *view) {
	int i;
	int index;
	
	index = 0;
	for (i = view_size - 1; i >= 0; i--)
		index += (view[i] * pow(ITEMS_NUMBER, view_size - i - 1));
	
	return index;
}
