#include "IntVector.h"
#include <stdlib.h>

#define INT_VECTOR_STARTING_CAPACITY 8

struct INT_VECTOR
{
  int* array;
  size_t size;
  size_t capacity;
};

IntVector*
IntVector_new ()
{
  IntVector* new_vector = malloc(sizeof(IntVector));

  // only initialize if memory allocation was successful
  if (new_vector)
    {
      new_vector->array = malloc(sizeof(int) * INT_VECTOR_STARTING_CAPACITY);
      new_vector->size = 0;
      new_vector->capacity = INT_VECTOR_STARTING_CAPACITY;

      // check memory allocation
      if (new_vector->array == NULL)
        {
          free(new_vector);
          new_vector = NULL;
        }
    }
  
  return new_vector;
}

void
IntVector_free (IntVector* vector)
{
  if (vector)
    {
      free(vector->array);
      free(vector);
    }
}

size_t
IntVector_size (IntVector* vec)
{
  return vec->size;
}

int*
IntVector_at (IntVector* vector, size_t index)
{
  int* element = NULL;
  if (vector != NULL && index < vector->size)
    element = &vector->array[index];
  return element;
}

bool
IntVector_push_back (IntVector* vector, int value)
{
  bool success = vector != NULL;
  
  // increase capacity if needed
  if (success && vector->capacity == vector->size)
    {
      int* temp = realloc(vector->array, vector->capacity * 2 * sizeof(int));
      if (temp == NULL)
        success = false;
      else
        {
          vector->capacity = vector->capacity * 2;
          vector->array = temp;
        }
    }

  // add the new element to the vector
  if (success)
    {
      vector->array[vector->size] = value;
      vector->size++;
    }

  return success;
}
