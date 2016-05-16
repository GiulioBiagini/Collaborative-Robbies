#ifndef __MERGE_SORT_H__
#define __MERGE_SORT_H__



#include "../simulation_const.h"
#include "../entity/robby.h"



void init_merge_sort();

void merge_sort(pair_t **population, int left, int right);

#define MERGE_SORT(population) (\
	merge_sort(population, 0, PAIRS_NUMBER - 1)\
)



#endif
