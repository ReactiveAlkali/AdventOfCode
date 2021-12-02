/*
 * Header file for a minimum priority queue that stores ints
 */

#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

typedef struct MIN_PRIORITY_QUEUE MinPQueue;

MinPQueue *createMinPQueue(int);	
void destroyMinPQueue(MinPQueue *);

void insert(MinPQueue *, int);
int minimum(MinPQueue *);
int extractMin(MinPQueue *);

#endif
