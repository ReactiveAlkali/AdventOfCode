all: part1 part2

part1: BingoCard.o BingoCard.hpp part1.cpp
	g++ -Wall part1.cpp BingoCard.o -o part1

part2: BingoCard.o BingoCard.hpp part2.cpp
	g++ -Wall part2.cpp BingoCard.o -o part2

BingoCard.o: BingoCard.cpp BingoCard.hpp
	g++ -Wall BingoCard.cpp -o BingoCard.o -c

clean:
	rm BingoCard.o part1 part2
