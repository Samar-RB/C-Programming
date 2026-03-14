#include "LinkedList.h"

struct Node {
  element elem;
  struct Node *next;
};

struct LinkedList {
  struct Node *head;
  struct Node *tail;
  int num_linkedlist;
  copyFunction copyfunc;
  freeFunction freefunck;
  printFunction printFunc;
  equalFunction equalFunction;
  connectionFunction relationFunc;
};

List createLinkedList(copyFunction copyHeap, freeFunction freeHeap,
                      printFunction printHeap, equalFunction equalHeap,
                      connectionFunction connection) {
  List list = (List)malloc(sizeof(struct LinkedList));
  if (!list) {
    printf("No memory available.\n");
    return NULL;
  }

  list->head = NULL;
  list->tail = NULL;
  list->num_linkedlist = 0;

  list->copyfunc = copyHeap;
  list->freefunck = freeHeap;
  list->printFunc = printHeap;
  list->equalFunction = equalHeap;
  list->relationFunc = connection;

  return list;
}

void destroyList(List list) {
  if (!list)
    return;

  struct Node *current = list->head;
  while (current != NULL) {
    struct Node *next = current->next;
    list->freefunck(current->elem);
    free(current);
    current = next;
  }

  free(list);
}

status appendNode(List list, element elem) {

  if (!list || !elem)
    return failure;

  element elem_to_add = list->copyfunc(elem);
  if (elem_to_add == NULL)
    return failure;

  struct Node *new_node = malloc(sizeof(struct Node));
  if (new_node == NULL) {
    printf("No memory available.\n");
    return failure;
  }

  new_node->elem = elem_to_add;
  new_node->next = NULL;

  if (list->num_linkedlist == 0) {
    list->head = new_node;
    list->tail = new_node;
  }

  else {
    list->tail->next = new_node;
    list->tail = new_node;
  }

  list->num_linkedlist++;
  return success;
}

status deleteNode(List list, element elem) {
  if (list == NULL || list->head == NULL)
    return failure;

  struct Node *current = list->head;
  struct Node *previous = NULL;

  while (current != NULL) {
    if (list->equalFunction(current->elem, elem) == 0) {

      if (previous == NULL) {
        list->head = current->next;
        if (current == list->tail)
          list->tail = NULL;
      } else {
        previous->next = current->next;

        if (current == list->tail)
          list->tail = previous;
      }
      list->freefunck(current->elem);
      free(current);

      list->num_linkedlist--;
      return success;
    }

    previous = current;
    current = current->next;
  }

  return failure;
}

void displayList(List list) {
  if (list == NULL || list->head == NULL)
    return;

  struct Node *current = list->head;

  while (current != NULL) {
    list->printFunc(current->elem);
    current = current->next;
  }
}

element searchByKeyInList(List list, element elem) {
  if (list == NULL || list->head == NULL)
    return NULL;

  struct Node *current = list->head;

  while (current != NULL) {
    if (list->relationFunc(current->elem, elem) == 0) {
      return current->elem;
    }
    current = current->next;
  }

  return NULL;
}

element *return_element(List list) {
  if (list == NULL || list->num_linkedlist == 0)
    return NULL;

  element *elem_array = malloc(sizeof(element) * list->num_linkedlist);
  if (elem_array == NULL) {
    printf("No memory available.\n");
    return NULL;
  }

  struct Node *current = list->head;
  for (int i = 0; i < list->num_linkedlist; i++) {
    elem_array[i] = current->elem;
    current = current->next;
  }

  return elem_array;
}
