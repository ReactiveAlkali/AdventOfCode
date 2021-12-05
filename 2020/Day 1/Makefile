all: part1 part2

part1: part1.c IntVector.o
	gcc part1.c IntVector.o -o part1

part2: part2.c IntVector.o
	gcc part2.c IntVector.o -o part2

IntVector.o: IntVector.c IntVector.h
	gcc IntVector.c -o IntVector.o -c

clean:
	rm IntVector.o part1 part2
