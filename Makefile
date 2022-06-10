CC=gcc
EXEC=prog
OFLAG=-fopenmp
CFLAGS=-O3
TFLAG=-lpthread

single: single.o functions.o
	$(CC) -o $(EXEC) $^ $(CFLAGS)

pthread: pthread.o functions.o
	$(CC) -o $(EXEC) $^ $(CFLAGS) $(TFLAG)

openmp: openmp.o functions.o
	$(CC) -o $(EXEC) $^ $(CFLAGS) $(OFLAG)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

generate:
	$(CC) -o gen gen_arrays.c
	./gen
	rm gen

clean:
	rm $(EXEC) *.exe *.o

run:
	@./$(EXEC)
