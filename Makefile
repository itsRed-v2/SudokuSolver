solver: solver.c
	gcc -o solver solver.c

run: solver
	./solver