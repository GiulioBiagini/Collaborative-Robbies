compiler = mpicc

flags = --ansi -Wall -pedantic

entity_files = entity/robby.c
evolution_files = evolution/mergesort.c evolution/crossover.c evolution/mutation.c
environment_files = environment.c
main_files = simulation_const.c main.c

program_name = main

all:
	$(compiler) $(flags) \
	$(entity_files) $(evolution_files) $(environment_files) $(main_files) \
	-o $(program_name) \
	-lm
