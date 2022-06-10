CC=gcc
EXEC=prog
OFLAG=-fopenmp
CFLAGS=-O3
TFLAG=-lpthread

single:
	$(CC) -o $(EXEC) single.c $(CFLAGS)

pthread:
	$(CC) -o $(EXEC) pthread.c $(CFLAGS) $(TFLAG)

openmp:
	$(CC) -o $(EXEC) openmp.c $(CFLAGS) $(OFLAG)

generate:
	$(CC) -o gen gen_arrays.c
	./gen
	rm gen

clean:
	rm $(EXEC) *.exe

run:
	./$(EXEC)
