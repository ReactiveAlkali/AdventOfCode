/*
 * SOLUTION INCOMPLETE WILL ATTEMPT AGAIN LATER
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// nodes for a house graph
typedef struct HOUSE_NODE House;
struct HOUSE_NODE {
	House *north;
	House *east;
	House *south;
	House *west;
}

/*
 * Creates and adds a new house node to the graph
 *
 * Accepts a house node and the direction to which to add it
 * Returns a pointer to the added house 
 */
House *addHouse(House *house, char dir) {
	House *newHouse = malloc(sizeof(struct HOUSE_NODE));

	assert(house != NULL);

	// adds the newhouse in the specified direction from the given house
	switch(dir) {
		case '^' :
			house->north = newHouse;
			newHouse->south = house;
		case 'v' :
			house->south = newHouse;
			newHouse->north = house;
		case '>' :
			house->east = newHouse;
			newHouse->west = house;
		case '<' :
			house->west = newHouse;
			newHouse->west = house;
	}

	return newHouse;
}

/*
 * Finds any neighbours of the given house and set correct pointers
 */
void attachNeighbours(House *house, char dir) {
	
}
