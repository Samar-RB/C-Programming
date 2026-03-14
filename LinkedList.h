#ifndef LinkedList_h
#define LinkedList_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"


typedef struct Node* node;
typedef struct LinkedList* List;

// create list
List createLinkedList(copyFunction, freeFunction, printFunction, equalFunction, connectionFunction);

// destroy list
void destroyList(List list);

// append node
status appendNode(List list, element elem);

// delete node
status deleteNode(List list, element elem);

// print list
void displayList(List list);

// search by key
element searchByKeyInList(List list, element elem);

// return array of elements
element* return_element(List list);


#endif /* LinkedList_h */
