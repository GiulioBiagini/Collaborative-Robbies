#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "entity/view.h"
#include "entity/map.h"
#include "entity/robby.h"
#include "evolution.h"



/*
 * Merge Sort
 */

void merge(population_t *A, pair_t **B, int left, int center, int right) {
	int i, j, k;
	
	i = left;
	j = center + 1;
	k = 0;
	
	while (i <= center && j <= right)
		if (A->pairs[i]->fitness_value > A->pairs[j]->fitness_value)
			B[k++] = A->pairs[i++];/*B[k] = A[i];*/
		else
			B[k++] = A->pairs[j++];/*B[k] = A[j];*/
	
	while (i <= center)
		B[k++] = A->pairs[i++];/*B[k] = A[i];*/
	
	while (j <= right)
		B[k++] = A->pairs[j++];/*B[k] = A[j];*/
	
	for (k = left; k <= right; k++)
		A->pairs[k] = B[k - left];/*A[k] = B[k-p];*/
}

void merge_sort(population_t *population, pair_t **tmp, int left, int right) {
	int center;
	
	if (left < right) {
		center = (left + right) / 2;
		merge_sort(population, tmp, left, center);
		merge_sort(population, tmp, center + 1, right);
		merge(population, tmp, left, center, right);
	}
}



/*
 * Mutation
 */

void mutate_robby(robby_t *robby, double mutation_probability) {
	int i;
	action_t new_action;
	
	for (i = 0; i < robby->dna_size; i++) {
		if (RANDOM_0_1() <= mutation_probability) {
			do {
				new_action = GENERATE_RANDOM_ACTION();
			} while (robby->dna[i] == new_action);
			robby->dna[i] = new_action;
		}
	}
}

void mutate_population(population_t *population, double mutation_probability) {
	int i;
	
	for (i = 0; i < population->pairs_number; i++)
		MUTATE_PAIR(population->pairs[i], mutation_probability);
}



/*
 * Crossover


void crossing_over_pair(pair_t *A1, pair_t *A2, pair_t *B1, pair_t *B2){
    int cut_point= rand()%A1->robby_1->dna_size;

    memcpy(B1->robby_1->dna,A1->robby_1->dna,cut_point);
    memcpy(B1->robby_1->dna + cut_point,A2->robby_1->dna + cut_point,A1->robby_1->dna_size - cut_point);

    memcpy(B2->robby_1->dna,A2->robby_1->dna,cut_point);
    memcpy(B2->robby_1->dna + cut_point,A1->robby_1->dna + cut_point,A1->robby_1->dna_size - cut_point);

    memcpy(B1->robby_2->dna,A1->robby_2->dna,cut_point);
    memcpy(B1->robby_2->dna + cut_point,A2->robby_2->dna + cut_point,A1->robby_2->dna_size - cut_point);

    memcpy(B2->robby_2->dna,A2->robby_2->dna,cut_point);
    memcpy(B2->robby_2->dna + cut_point,A1->robby_2->dna + cut_point,A1->robby_2->dna_size - cut_point);

}

int get_chosed_index(int p,int size){
        int i;
        int k;
        i=0;
        k=0;
        while(i<p){
            i=i+size;
            size--;
            k++;
        }
        return k;
}

void crossing_over_population(population_t *A, population_t *B, float mutation_probability){
    int i,j,k;
    for(i=0;i<A->pairs_number;i=i+2){*/
      /*A->pairs[i].fitness_value=rand()%100;*/
      /*j=get_chosed_index(rand()%(A->pairs_number/2*(A->pairs_number+1)),A->pairs_number);
      do{
          k=get_chosed_index(rand()%(A->pairs_number/2*(A->pairs_number+1)),A->pairs_number);
      }while(j==k);
      crossing_over_pair(&(A->pairs[j]),&(A->pairs[k]),&(B->pairs[i]),&(B->pairs[i+1]));
      mutate_pair(&(B->pairs[i]),mutation_probability);
      mutate_pair(&(B->pairs[i+1]),mutation_probability);
    }
}*/



/*
 * Evolution
 */

void evolve(population_t *population, double mutation_probability) {
	pair_t ** tmp_pairs;
	
	tmp_pairs = (pair_t**) malloc(population->pairs_number * sizeof(pair_t*));
    merge_sort(population, tmp_pairs, 0, population->pairs_number - 1);
	free(tmp_pairs);
	
    /*crossing_over_population(src,dst,mutation_probability);*/
	mutate_population(population, mutation_probability);
	
	/*
	population_t *tmp;
	tmp = allocate_population(
		population->pairs[0]->robby_1->view_type,
		population->pairs_number
	);
	*/
}



/*
 * DEBUG 



void print_population(population_t *A){
  int i;
  printf("Print the population!\n");
  for(i=0;i<A->pairs_number;i++){
    printf("%i: %f\n",i,A->pairs[i].fitness_value);
  }
}

void print_dna(robby_t *r){
    int j;
    printf("DNA \n");
    for(j=0;j<r->dna_size;j++){
        printf("%d",r->dna[j]);
    }
    printf("\n");
}
void print_dna_short(robby_t *r){
    int j;
    printf("DNA: ");
    for(j=0;j<5;j++){
        printf("%d",r->dna[j]);
    }
    printf("~");
    for(j=r->dna_size-5;j<r->dna_size;j++){
        printf("%d",r->dna[j]);
    }
    printf("\n");
}
void print_population_dna(population_t *A){
  int i;
  printf("Print the population!\n");
  for(i=0;i<A->pairs_number;i++){
    printf("ROBOT %i: %f\n",i,A->pairs[i].fitness_value);
    print_dna_short(A->pairs[i].robby_1);
    print_dna_short(A->pairs[i].robby_2);
    printf("\n");
  }
}

void generate_random_fitness(population_t *A){
  int i;
  for(i=0;i<A->pairs_number;i++){
    A->pairs[i].fitness_value=rand()%100;
  }
}
*/
