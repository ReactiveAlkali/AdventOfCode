/*
 * Implementation of a mininum priority queue that holds ints
 */

#include <stdlib.h>
#include "MinPQueue.h"

struct MIN_PRIORITY_QUEUE {
	int size;
	int capacity;
	int *array;
}

// Creates a new priority queue
MinPQueue *createMinPQueue(int initSize) {
	MinPQueue *newQueue = malloc(sizeof(MinPQueue));
	
	return newQueue;
}
