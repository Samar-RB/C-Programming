#ifndef MaxHeap_h
#define MaxHeap_h

#include "Defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Heap_t *Heap;
Heap CreateHeap(int maxSize, char *heapName, copyFunction copyPokemon,
                freeFunction freePokemon, equalFunction equalPokemon,
                printFunction printPokemon);
void destroyHeap(Heap heap);
Heap CopyHeap(Heap heap);

int parent(int position);
int leftChild(int position);
int rightChild(int position);

status isLeaf(Heap heap, int position);

void swap(Heap heap, int first_position, int second_position);

void maxHeapify(Heap heap, int position, int largest);

void destroyHeap(Heap heap);

status insertToHeap(Heap heap, element elem);

status printHeap(Heap heap);

element PopMaxHeap(Heap heap);

element TopMaxHeap(Heap heap);

char *getHeapId(Heap heap);

int getHeapCurrentSize(Heap heap);

element *return_elem(element elem);

#endif /* MaxHeap_h */
