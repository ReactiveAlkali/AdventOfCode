#ifndef _INT_VECTOR_H
#define _INT_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct INT_VECTOR IntVector;

// Constructor and destructor for IntVector
IntVector* IntVector_new ();
void IntVector_free (IntVector*);

// Access an element at the given index
int* IntVector_at (IntVector*, size_t);

// Pushes a new value to the back of the vector
bool IntVector_push_back(IntVector*, int);

size_t IntVector_size (IntVector*);

#endif
