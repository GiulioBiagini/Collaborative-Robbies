#ifndef __EVOLUTION_H__
#define __EVOLUTION_H__



#include "entity/robby.h"



void init_evolution();

void merge_sort(pair_t **population, int left, int right);

void generate_population(pair_t **population);

void mutate_population(pair_t **population);



#endif
