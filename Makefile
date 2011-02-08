rb: rb.o
	gcc -o build/rb build/rb.o
rb.o: recursive-backtracking.c
	gcc -Wall -O -c recursive-backtracking.c -o build/rb.o

clean:
	rm build/rb build/rb.o
