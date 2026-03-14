#include "BattleByCategory.h"
#include "LinkedList.h"
#include "MaxHeap.h"
#include "Pokemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

element copyHeap(element elem) {
  if (elem == NULL) {
    return NULL;
  }

  Heap heap_copy = (Heap)elem;

  if (heap_copy) {
    return heap_copy;
  }
  return NULL;
}

status freeHeap(element elem) {
  if (elem == NULL) {
    return failure;
  }

  Heap heap_to_del = (Heap)elem;
  destroyHeap(heap_to_del);
  return success;
}

status print_Heap(element elem) {
  if (elem == NULL) {
    return failure;
  }

  printHeap((Heap)elem);
  return success;
}

int equalHeap(element elem1, element elem2) {
  Heap *heap1 = (Heap *)elem1;
  PokemonDB *poke1 = (PokemonDB *)elem2;

  if (strcmp(getHeapId(*heap1), (poke1->pok_type->Pokemontype_name)) == 0) {
    return 0;
  } else {
    return 1;
  }
}

int connection(element elem1, element elem2) {

  Heap heap1 = (Heap)elem1;
  char *category_str = (char *)elem2;

  if (strstr(getHeapId(heap1), category_str)) {
    return 0;
  } else {
    return 1;
  }
}

struct battle_s {
  List list;
  int maxCapacity;
  int numberOfCategories;
  char *categories;
  equalFunction equalElement;
  copyFunction copyElement;
  freeFunction freeElement;
  getCategoryFunction getCategory;
  getAttackFunction getAttack;
  printFunction printElement;
};

Battle createBattleByCategory(int capacity, int numberOfCategories,
                              char *categories, equalFunction equalElement,
                              copyFunction copyElement,
                              freeFunction freeElement,
                              getCategoryFunction getCategory,
                              getAttackFunction getAttack,
                              printFunction printElement) {
  if (categories == NULL || equalElement == NULL || copyElement == NULL ||
      freeElement == NULL || getCategory == NULL || getAttack == NULL ||
      printElement == NULL || capacity < 0 || numberOfCategories < 0) {
    return NULL;
  }

  Battle battle = (Battle)malloc(sizeof(struct battle_s));
  if (battle == NULL) {
    printf("No memory available.\n");
    return NULL;
  }
  battle->list =
      createLinkedList(copyHeap, freeHeap, print_Heap, equalHeap, connection);
  char *name;
  name = strtok(categories, ",\r\n");
  for (int i = 0; i < numberOfCategories; i++) {
    Heap heap;
    heap = CreateHeap(capacity, name, copyElement, freeElement, equalElement,
                      printElement);
    element elem_to_add = heap;
    appendNode(battle->list, elem_to_add);
    name = strtok(NULL, ",\r\n");
  }
  battle->maxCapacity = capacity;
  battle->numberOfCategories = numberOfCategories;
  battle->categories = categories;
  battle->equalElement = equalElement;
  battle->copyElement = copyElement;
  battle->freeElement = freeElement;
  battle->getCategory = getCategory;
  battle->getAttack = getAttack;
  battle->printElement = printElement;
  return battle;
}

status destroyBattleByCategory(Battle b) {
  if (b == NULL) {
    return failure;
  }

  destroyList(b->list);
  free(b);

  return success;
}

status insertObject(Battle b, element elem) {
  if (!b)
    return failure;
  element elem1 = NULL;

  for (int i = 0; i < b->numberOfCategories; i++) {
    elem1 = searchByKeyInList(b->list, (element)b->getCategory(elem));

    if (elem1 != NULL) {
      if (insertToHeap(elem1, elem) == failure) {
        return failure;
      }
      return success;
    }
  }

  return failure;
}

void displayObjectsByCategories(Battle b) { displayList(b->list); }

element removeMaxByCategory(Battle b, char *category) {
  element elem1 = searchByKeyInList(b->list, (element)category);

  if (elem1 != NULL && getHeapCurrentSize((Heap)elem1) != 0) {
    return PopMaxHeap(elem1);
  }

  return NULL;
}

int getNumberOfObjectsInCategory(Battle b, char *category) {
  for (int i = 0; i < b->numberOfCategories; i++) {
    element elem_to_use = searchByKeyInList(b->list, (element)category);
    if (elem_to_use != NULL) {
      return getHeapCurrentSize(elem_to_use);
    }
  }
  return 0;
}

element fight(Battle b, element elem) {

  int atk_elem1 = 0;
  int atk_elem2 = 0;
  int *pAtk1 = &atk_elem1;
  int *pAtk2 = &atk_elem2;

  int final_attack1 = 0;
  int final_attack2 = 0;

  int max_diff = 0;
  int battle_result = 0;
  int first_comparison = 0;

  element *first_elements = return_element(b->list);
  element element_to_fight = NULL;

  for (int i = 0; i < b->numberOfCategories; i++) {
    if (getHeapCurrentSize((Heap)first_elements[i]) != 0) {
      battle_result =
          b->getAttack(return_elem(first_elements[i]), elem, pAtk1, pAtk2);

      if (first_comparison == 0) {
        max_diff = (*pAtk1 - *pAtk2);
        first_comparison = 1;
      }
    }

    if ((getHeapCurrentSize((Heap)first_elements[i]) != 0 &&
         battle_result != 2 && (*pAtk1 - *pAtk2) > max_diff) ||
        first_comparison == 1) {
      max_diff = (*pAtk1 - *pAtk2);
      element_to_fight = first_elements[i];
      final_attack1 = *pAtk1;
      final_attack2 = *pAtk2;
      first_comparison = 2;
    }
  }

  if (element_to_fight == NULL) {
    free(first_elements);
    return NULL;
  }

  if (max_diff > 0) {
    printf("The final battle between:\n");
    b->printElement(elem);
    printf("In this battle his attack is :%d\n\n", final_attack2);
    printf("against ");
    b->printElement(return_elem(element_to_fight));
    printf("In this battle his attack is :%d\n\n", final_attack1);
    printf("THE WINNER IS:\n");
    b->printElement(return_elem(element_to_fight));
    free(first_elements);
    return element_to_fight;
  } else if (max_diff < 0) {
    printf("The final battle between:\n");
    b->printElement(elem);
    printf("In this battle his attack is :%d\n\n", final_attack2);
    printf("against ");
    b->printElement(return_elem(element_to_fight));
    printf("In this battle his attack is :%d\n\n", final_attack1);
    printf("THE WINNER IS:\n");
    b->printElement(elem);
    free(first_elements);
    return elem;
  } else {
    printf("The final battle between:\n");
    b->printElement(elem);
    printf("In this battle his attack is :%d\n\n", final_attack2);
    printf("against ");
    b->printElement(return_elem(element_to_fight));
    printf("In this battle his attack is :%d\n\n", final_attack1);
    printf("IT IS A DRAW.\n");
    free(first_elements);
    return elem;
  }
}
