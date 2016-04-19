#ifdef DEBUG
	
	#include "environment.h"
	
	
	
	void print_environment(environment_t *env) {
		int x;
		int y;
		int i;
		view_t *subject_view;
		view_t *other_view;
		
		printf("Map:\n");
		for (x = 0; x < env->map->width; x++) {
			for (y = 0; y < env->map->height; y++) {
				if (IS_SAME_POSITION(x, y, env->pos_x_1, env->pos_y_1))
					printf((GET_ITEM_INTO_MAP_1(env->map, x, y)) == CAN ? "U" : "u");
				else if (IS_SAME_POSITION(x, y, env->pos_x_2, env->pos_y_2))
					printf((GET_ITEM_INTO_MAP_1(env->map, x, y)) == CAN ? "D" : "d");
				else
					printf("|%c", PRINT_ITEM(env->map->items[x][y]));
			}
			printf("|\n");
		}
		
		subject_view = env->robby_1->view;
		other_view = env->robby_2->view;
		for (i = 1; i < 3; i++) {
			printf("Robby %d:\n", i);
			switch (subject_view->type) {
				case SINGLE_CROSS_VIEW:
					printf(
						"| |%c| |\n|%c|%c|%c|\n| |%c| |\n",
						PRINT_ITEM(subject_view->items[0]),
						PRINT_ITEM(subject_view->items[1]),
						PRINT_ITEM(subject_view->items[2]),
						PRINT_ITEM(subject_view->items[3]),
						PRINT_ITEM(subject_view->items[4])
					);
					break;
				case SINGLE_SQUARE_VIEW:
					printf(
						"|%c|%c|%c|\n|%c|%c|%c|\n|%c|%c|%c|\n",
						PRINT_ITEM(subject_view->items[0]),
						PRINT_ITEM(subject_view->items[1]),
						PRINT_ITEM(subject_view->items[2]),
						PRINT_ITEM(subject_view->items[3]),
						PRINT_ITEM(subject_view->items[4]),
						PRINT_ITEM(subject_view->items[5]),
						PRINT_ITEM(subject_view->items[6]),
						PRINT_ITEM(subject_view->items[7]),
						PRINT_ITEM(subject_view->items[8])
					);
					break;
				default:
					printf(
						"| |%c| || |%c| |\n|%c|%c|%c||%c|%c|%c|\n| |%c| || |%c| |\n",
						PRINT_ITEM(subject_view->items[0]),
						PRINT_ITEM(subject_view->items[1]),
						PRINT_ITEM(subject_view->items[2]),
						PRINT_ITEM(subject_view->items[3]),
						PRINT_ITEM(subject_view->items[4]),
						PRINT_ITEM(other_view->items[0]),
						PRINT_ITEM(other_view->items[1]),
						PRINT_ITEM(other_view->items[2]),
						PRINT_ITEM(other_view->items[3]),
						PRINT_ITEM(other_view->items[4])
					);
			}
			subject_view = env->robby_2->view;
			other_view = env->robby_1->view;
		}
	}
	
#endif
