#include <stdlib.h>

#include "item.h"
#include "map.h"



position_t *allocate_position() {
	position_t *pos;
	
	pos = (position_t*) malloc(sizeof(position_t));
	
	return pos;
}

map_t *allocate_map(int width, int height, int cans_number) {
	int x;
	map_t *map;
	
	map = (map_t*) malloc(sizeof(map_t));
	map->width = width;
	map->height = height;
	map->cans_number = cans_number;
	map->items = (item_t**) malloc(map->width * sizeof(item_t*));
	for (x = 0; x < map->width; x++)
		map->items[x] = (item_t*) malloc(map->height * sizeof(item_t));
	
	return map;
}

void init_random_map(map_t *map) {
	int x;
	int y;
	int c;
	
	for (x = 0; x < map->width; x++)
		for (y = 0; y < map->height; y++)
			map->items[x][y] = EMPTY;
	for (c = 0; c < map->cans_number; ) {
		x = rand() % map->width;
		y = rand() % map->height;
		if (map->items[x][y] == EMPTY) {
			map->items[x][y] = CAN;
			c++;
		}
	}
}
