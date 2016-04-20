
void Merge(population_t *A, int p, int q, int r);

void MergeSort(population_t *A, int p, int r);

void sort_by_fitness(population_t *A);

void print_population(population_t *A);

void generate_random_fitness(population_t *A);

void crossing_over_pair(pair_t *A1, pair_t *A2, pair_t *B1, pair_t *B2);

int get_chosed_index(int p,int size);

void crossing_over_population(population_t *A, population_t *B, float mutation_probability);

void mutate_pair(pair_t *pair,float mutation_probability);

/* Pubblica */
void evolve(population_t *src, population_t *dst, float mutation_probability);
