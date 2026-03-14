#ifndef Defs_h
#define Defs_h
#include <stdbool.h>

typedef enum e_status {success, failure} status;

typedef void * element;
typedef element(*copyFunction) (element);
typedef status(*freeFunction) (element);
typedef status(*printFunction) (element);
typedef int(*equalFunction) (element, element);
typedef char*(*getCategoryFunction)(element);
typedef int(*getAttackFunction)(element firstElem ,element secondElem ,int* attackFirst,int* attackSecond); 

// Check if two elements are connected
typedef int(*connectionFunction) (element, element);
#endif /* Defs_h */
