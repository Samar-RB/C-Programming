#include "MaxHeap.h"

struct Heap_t {
  element *array;
  char *heapName;
  int nextIndex;
  int maxCapacity;
  copyFunction copyFunc;
  freeFunction freeFunc;
  equalFunction equalFunc;
  printFunction printFunc;
};

Heap CreateHeap(int maxSize, char *heapName, copyFunction copyPokemon,
                freeFunction freePokemon, equalFunction equalPokemon,
                printFunction printPoke) {

  if (maxSize <= 0 || heapName == NULL)
    return NULL;

  Heap heap = (Heap)malloc(sizeof(struct Heap_t));
  if (!heap) {
    printf("No memory available.\n");
    return NULL;
  }

  heap->array = (element *)malloc(sizeof(element) * maxSize);
  if (!heap->array) {
    free(heap);
    printf("No memory available.\n");
    return NULL;
  }

  heap->heapName = strdup(heapName);
  if (!heap->heapName) {
    free(heap->array);
    free(heap);
    printf("No memory available.\n");
    return NULL;
  }

  heap->nextIndex = 0;
  heap->maxCapacity = maxSize;

  heap->copyFunc = copyPokemon;
  heap->freeFunc = freePokemon;
  heap->equalFunc = equalPokemon;
  heap->printFunc = printPoke;

  return heap;
}

int parent(int position) { return (position - 1) / 2; }

int leftChild(int position) { return (2 * position) + 1; }

int rightChild(int position) { return (2 * position) + 2; }

status isLeaf(Heap heap, int position) {
  if (!heap)
    return failure;

  if (leftChild(position) >= heap->nextIndex)
    return success;

  return failure;
}

void swap(Heap heap, int first_position, int second_position) {
  if (!heap)
    return;
  if (first_position < 0 || first_position >= heap->nextIndex)
    return;
  if (second_position < 0 || second_position >= heap->nextIndex)
    return;

  element temp = heap->array[first_position];
  heap->array[first_position] = heap->array[second_position];
  heap->array[second_position] = temp;
}

void maxHeapify(Heap heap, int position, int largest) {
  if (!heap || position < 0 || position >= heap->nextIndex)
    return;

  if (isLeaf(heap, position) == success)
    return;

  int l = leftChild(position);
  int r = rightChild(position);

  if (l < heap->nextIndex &&
      heap->equalFunc(heap->array[l], heap->array[position]) == 1)
    largest = l;
  else
    largest = position;

  if (r < heap->nextIndex &&
      heap->equalFunc(heap->array[r], heap->array[largest]) == 1)
    largest = r;

  if (largest != position) {
    swap(heap, position, largest);
    maxHeapify(heap, largest, largest);
  }
}

void destroyHeap(Heap heap) {
  if (!heap)
    return;

  for (int i = 0; i < heap->nextIndex; i++) {
    if (heap->array[i])
      heap->freeFunc(heap->array[i]);
  }

  if (heap->array)
    free(heap->array);

  if (heap->heapName)
    free(heap->heapName);

  free(heap);
}

status insertToHeap(Heap heap, element elem) {
  if (!heap || !elem)
    return failure;

  if (heap->nextIndex >= heap->maxCapacity)
    return failure;

  element element_to_add = heap->copyFunc(elem);
  if (!element_to_add)
    return failure;

  heap->array[heap->nextIndex] = element_to_add;
  int current = heap->nextIndex;
  heap->nextIndex++;

  while (current > 0 && heap->equalFunc(heap->array[current],
                                        heap->array[parent(current)]) == 1) {
    swap(heap, current, parent(current));
    current = parent(current);
  }

  return success;
}

status dummyFree(element e) { return success; }

Heap CopyHeap(Heap heap) {
  if (!heap)
    return NULL;
  Heap newHeap = CreateHeap(heap->maxCapacity, heap->heapName, heap->copyFunc,
                            heap->freeFunc, heap->equalFunc, heap->printFunc);
  if (!newHeap)
    return NULL;

  for (int i = 0; i < heap->nextIndex; i++) {
    element copy = heap->copyFunc(heap->array[i]);
    if (!copy) {
      destroyHeap(newHeap);
      return NULL;
    }
    newHeap->array[i] = copy;
  }
  newHeap->nextIndex = heap->nextIndex;
  return newHeap;
}

status printHeap(Heap heap) {
  if (!heap)
    return failure;

  if (heap->nextIndex == 0) {
    printf("%s:\nNo elements.\n\n", heap->heapName);
    return success;
  }

  printf("%s:\n", heap->heapName);

  Heap tempHeap = CopyHeap(heap);
  if (!tempHeap) {
    printf("No memory available.\n");
    return failure;
  }

  tempHeap->freeFunc = dummyFree;

  int count = 1;
  while (tempHeap->nextIndex > 0) {
    element e = PopMaxHeap(tempHeap);
    printf("%d. ", count);
    tempHeap->printFunc(e);
    count++;
  }

  destroyHeap(tempHeap);
  return success;
}

element PopMaxHeap(Heap heap) {
  if (!heap || heap->nextIndex == 0)
    return NULL;

  element maxElem = heap->array[0];

  if (heap->nextIndex == 1) {
    heap->nextIndex--;
    return maxElem;
  }

  heap->array[0] = heap->array[heap->nextIndex - 1];
  heap->nextIndex--;

  maxHeapify(heap, 0, 0);

  return maxElem;
}

element TopMaxHeap(Heap heap) {
  if (!heap || heap->nextIndex == 0)
    return NULL;
  element elem_to_move = heap->array[0];
  return elem_to_move;
}

char *getHeapId(Heap heap) {
  if (!heap)
    return NULL;
  return heap->heapName;
}

int getHeapCurrentSize(Heap heap) {
  if (!heap)
    return 0;
  return heap->nextIndex;
}

element *return_elem(element elem) {
  if (!elem)
    return NULL;

  Heap h = (Heap)elem;
  if (!h || h->nextIndex == 0)
    return NULL;

  return h->array[0];
}
