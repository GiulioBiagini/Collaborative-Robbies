#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

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
    if (A->pairs[i].global_fitness>A->pairs[j].global_fitness) {
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
  free(B);
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

void print_dna(robby_t *r){
    int j;
    printf("DNA \n");
    for(j=0;j<r->dna->size;j++){
        printf("%d",r->dna->actions[j]);
    }
    printf("\n");
}
void print_population_dna(population_t *A){
  int i;
  printf("Print the population!\n");
  for(i=0;i<A->pairs_number;i++){
    printf("ROBOT %i: %f\n",i,A->pairs[i].global_fitness);
    print_dna(A->pairs[i].robby_1);
    print_dna(A->pairs[i].robby_2);
  }
}

void generate_random_fitness(population_t *A){
  int i;
  printf("Add trash in fitness !\n");
  for(i=0;i<A->pairs_number;i++){
    A->pairs[i].global_fitness=rand()%100;
  }
}


void crossing_over_pair(pair_t *A1, pair_t *A2, pair_t *B1, pair_t *B2){
    int cut_point= rand()%A1->robby_1->dna->size;
    printf("cut_point: %d\n",cut_point);

    memcpy(A1->robby_1->dna->actions,B1->robby_1->dna->actions,cut_point);
    memcpy(A2->robby_1->dna->actions + cut_point,B1->robby_1->dna->actions + cut_point,A1->robby_1->dna->size - cut_point);

    memcpy(A2->robby_1->dna->actions,B2->robby_1->dna->actions,cut_point);
    memcpy(A1->robby_1->dna->actions + cut_point,B2->robby_1->dna->actions + cut_point,A1->robby_1->dna->size - cut_point);

    memcpy(A1->robby_2->dna->actions,B1->robby_2->dna->actions,cut_point);
    memcpy(A2->robby_2->dna->actions + cut_point,B1->robby_2->dna->actions + cut_point,A1->robby_2->dna->size - cut_point);

    memcpy(A2->robby_2->dna->actions,B2->robby_2->dna->actions,cut_point);
    memcpy(A1->robby_2->dna->actions + cut_point,B2->robby_2->dna->actions + cut_point,A1->robby_2->dna->size - cut_point);

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

void crossing_over_population(population_t *A, population_t *B, int mutation_rate){
    int i,j,k;
    for(i=0;i<A->pairs_number;i=i+2){
      /*A->pairs[i].global_fitness=rand()%100;*/
      do{
          j=get_chosed_index(rand()%(A->pairs_number/2*(A->pairs_number+1)),A->pairs_number);
          k=get_chosed_index(rand()%(A->pairs_number/2*(A->pairs_number+1)),A->pairs_number);
          printf("i: %d // J e K: %d %d\n",i,j,k);
      }while(j==k);
      crossing_over_pair(&(A->pairs[j]),&(A->pairs[k]),&(B->pairs[i]),&(B->pairs[i+1]));
    }
}

int main(){
    int pop_leng=2;
    population_t *A;
    population_t *B;
    A=allocate_population(SINGLE_CROSS_VIEW,pop_leng);
    init_random_population(A);
    print_population_dna(A);
    generate_random_fitness(A);
    print_population(A);
    sort_by_fitness(A);
    print_population(A);
    B=allocate_population(SINGLE_CROSS_VIEW,pop_leng);
    crossing_over_population(A,B,10);
    print_population_dna(B);
    return 0;
}
