#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "environment.h"
#include "evolution.h"

/*
 A: Array
 p: left
 q: center
 r: right*/
void Merge(population_t *A, int p, int q, int r) {
  int i, j, k;
  pair_t *B;

  B = (pair_t*) malloc(A->pairs_number*sizeof(pair_t));/*TODO solve the malloc things */
  i = p;
  j = q+1;
  k = 0;
  while (i<=q && j<=r) {
    if (A->pairs[i].global_fitness<A->pairs[j].global_fitness) {
      /*B[k] = A[i];*/
      B[k] = (A->pairs[i]);
      i++;
    } else {
      B[k] = (A->pairs[j]);
      /*B[k] = A[j];*/
      j++;
    }
    k++;
  }
  while (i<=q) {
    /*B[k] = A[i];*/
    B[k] = (A->pairs[i]);
    i++;
    k++;
  }
  while (j<=r) {
    B[k] = (A->pairs[j]);
    /*B[k] = A[j];*/
    j++;
    k++;
  }
  for (k=p; k<=r; k++){
    /*A[k] = B[k-p];*/
    (A->pairs[k]) = (B[k-p]);
  }
  return;
}


void MergeSort(population_t *A, int p, int r) {
  int q;

  if (p<r) {
    q = (p+r)/2;
    MergeSort(A, p, q);
    MergeSort(A, q+1, r);
    Merge(A, p, q, r);
  }
  return;
}

void sort_by_fitness(population_t *A){
  MergeSort(A, 0, A->pairs_number-1);
}

void print_population(population_t *A){
  int i;
  printf("Print the population!\n");
  for(i=0;i<A->pairs_number;i++){
    printf("%i: %f\n",i,A->pairs[i].global_fitness);
  }
}

void trashing_fitness(population_t *A){
  int i;
  printf("Add trash in fitness !\n");
  for(i=0;i<A->pairs_number;i++){
    A->pairs[i].global_fitness=rand()%100;
  }
}
int main(){
    population_t *A;

    A=allocate_population(SINGLE_CROSS_VIEW,10);
    init_random_population(A);
    trashing_fitness(A);
    print_population(A);
    sort_by_fitness(A);
    print_population(A);
    return 0;
}
