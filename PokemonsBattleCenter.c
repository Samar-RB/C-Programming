#include "BattleByCategory.h"
#include "Defs.h"
#include "LinkedList.h"
#include "MaxHeap.h"
#include "Pokemon.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// Compares attack of two pokemons. Returns 1 if first is stronger, -1 if
// second, 0 if equal.
int comparePokemonAttack(element e1, element e2) {
  PokemonDB *p1 = (PokemonDB *)e1;
  PokemonDB *p2 = (PokemonDB *)e2;

  int atk1 = p1->bio_info.attack;
  int atk2 = p2->bio_info.attack;

  if (atk1 > atk2)
    return 1;
  if (atk2 > atk1)
    return -1;
  return 0;
}

// Frees memory for a pokemon element.
status destroyPokemonElement(element e) {
  PokemonDB *p = (PokemonDB *)e;
  if (p->pok_type) {
    p->pok_type->pokemon_num--;
  }

  if (p->pok_name)
    free(p->pok_name);
  if (p->pok_species)
    free(p->pok_species);

  free(p);
  return success;
}

// Returns the element pointer (shallow copy).
element copyPokemonElement(element e) { return (element)e; }

// Prints a pokemon element.
status printPokemonElement(element e) {
  PokemonDB *p = (PokemonDB *)e;
  if (!p)
    return failure;
  return print_pokemon(p);
}

char *getPokemonCategory(element e) {
  PokemonDB *p = (PokemonDB *)e;
  return p->pok_type->Pokemontype_name;
}

// Calculates attack power with type effectiveness bonuses.
int getAttackWithBonuses(element e1, element e2, int *atk1_out, int *atk2_out) {
  if (!e1 || !e2)
    return 0;

  PokemonDB *p1 = (PokemonDB *)e1;
  PokemonDB *p2 = (PokemonDB *)e2;

  int a1 = p1->bio_info.attack;
  int a2 = p2->bio_info.attack;

  char *t1_name = p1->pok_type->Pokemontype_name;
  char *t2_name = p2->pok_type->Pokemontype_name;

  // Check penalty for P1 (if P2 effective against P1)
  int penalty1 = 0;

  if (p1->pok_type->effective_against_me) {
    for (int i = 0; i < p1->pok_type->num_against_me; i++) {
      if (strcmp(t2_name,
                 p1->pok_type->effective_against_me[i]->Pokemontype_name) ==
          0) {
        penalty1 = 10;
        break;
      }
    }
  }

  if (penalty1 == 0 && p2->pok_type->effective_against_others) {
    for (int i = 0; i < p2->pok_type->num_against_others; i++) {
      if (strcmp(t1_name,
                 p2->pok_type->effective_against_others[i]->Pokemontype_name) ==
          0) {
        penalty1 = 10;
        break;
      }
    }
  }
  a1 -= penalty1;

  // Check penalty for P2 (if P1 effective against P2)
  int penalty2 = 0;

  if (p2->pok_type->effective_against_me) {
    for (int i = 0; i < p2->pok_type->num_against_me; i++) {
      if (strcmp(t1_name,
                 p2->pok_type->effective_against_me[i]->Pokemontype_name) ==
          0) {
        penalty2 = 10;
        break;
      }
    }
  }

  if (penalty2 == 0 && p1->pok_type->effective_against_others) {
    for (int i = 0; i < p1->pok_type->num_against_others; i++) {
      if (strcmp(t2_name,
                 p1->pok_type->effective_against_others[i]->Pokemontype_name) ==
          0) {
        penalty2 = 10;
        break;
      }
    }
  }
  a2 -= penalty2;

  *atk1_out = a1;
  *atk2_out = a2;

  if (a1 > a2)
    return 1;
  if (a2 > a1)
    return -1;
  return 0;
}

// Helper to find a type by name.
Pokemon_Type *findTypeByName(Pokemon_Type *types, int count, const char *name) {
  for (int i = 0; i < count; i++) {
    if (strcmp(types[i].Pokemontype_name, name) == 0) {
      return &types[i];
    }
  }
  return NULL;
}


void handlePrintAllPokemons(Battle battle) {
  displayObjectsByCategories(battle);
}

void handlePrintAllTypes(Pokemon_Type *types, int count) {
  for (int i = 0; i < count; i++) {
    print_type(&types[i]);
  }
}

void handleAddPokemon(Battle battle, Pokemon_Type *types, int count,
                      int capacity) {
  char bufferType[300], bufferName[300], bufferSpecies[300];
  double hVal, wVal;
  long atkVal;

  printf("Please enter Pokemon type name:\n");
  scanf("%299s", bufferType);

  Pokemon_Type *t = findTypeByName(types, count, bufferType);
  if (!t) {
    printf("Type name doesn't exist.\n");
    return;
  }

  if (t->pokemon_num >= capacity) {
    printf("Type at full capacity.\n");
    return;
  }

  printf("Please enter Pokemon name:\n");
  scanf("%299s", bufferName);
  printf("Please enter Pokemon species:\n");
  scanf("%299s", bufferSpecies);
  printf("Please enter Pokemon height:\n");
  scanf("%lf", &hVal);
  printf("Please enter Pokemon weight:\n");
  scanf("%lf", &wVal);
  printf("Please enter Pokemon attack:\n");
  scanf("%ld", &atkVal);

  PokemonDB *p = create_pokemon(bufferName, t, bufferSpecies, (float)hVal,
                                (float)wVal, (int)atkVal);
  if (p) {
    insertObject(battle, p);
    printf("The Pokemon was successfully added.\n");
    print_pokemon(p);
  } else {
    printf("Memory Problem\n");
  }
}

void handleRemoveStrongest(Battle battle, Pokemon_Type *types, int count) {
  char bufferType[300];
  printf("Please enter type name:\n");
  scanf("%299s", bufferType);

  Pokemon_Type *t = findTypeByName(types, count, bufferType);
  if (!t) {
    printf("Type name doesn't exist.\n");
    return;
  }

  element removed = removeMaxByCategory(battle, bufferType);
  if (removed) {
    printf("The strongest Pokemon was removed:\n");
    printPokemonElement(removed);
    destroyPokemonElement(removed);
  } else {
    printf("There is no Pokemon to remove.\n");
  }
}

void handleFight(Battle battle, Pokemon_Type *types, int count) {
  char bufferType[300], bufferName[300], bufferSpecies[300];
  double hVal, wVal;
  long atkVal;

  printf("Please enter Pokemon type name:\n");
  scanf("%299s", bufferType);

  Pokemon_Type *t = findTypeByName(types, count, bufferType);
  if (!t) {
    printf("Type name doesn't exist.\n");
    return;
  }

  printf("Please enter Pokemon name:\n");
  scanf("%299s", bufferName);
  printf("Please enter Pokemon species:\n");
  scanf("%299s", bufferSpecies);
  printf("Please enter Pokemon height:\n");
  scanf("%lf", &hVal);
  printf("Please enter Pokemon weight:\n");
  scanf("%lf", &wVal);
  printf("Please enter Pokemon attack:\n");
  scanf("%ld", &atkVal);

  PokemonDB *fighter = create_pokemon(bufferName, t, bufferSpecies, (float)hVal,
                                      (float)wVal, (int)atkVal);
  if (fighter) {
    printf("You choose to fight with:\n");
    print_pokemon(fighter);

    element opponent = fight(battle, fighter);
    if (!opponent) {
      printf("There are no Pokemons to fight against you.\n");
    }
    destroyPokemonElement(fighter);
  } else {
    printf("Memory Problem\n");
  }
}

/* --- Main Program --- */

int main(int argc, char *argv[]) {
  if (argc < 4) {
    printf("Please enter 3 arguments\n");
    return 0;
  }

  int numberOfTypes = atoi(argv[1]);
  int typeCapacity = atoi(argv[2]);
  char *configPath = argv[3];

  FILE *file = fopen(configPath, "r");
  if (!file) {
    printf("Null File");
    return 1;
  }

  Pokemon_Type *allTypes = allocateTypes(numberOfTypes);
  if (!allTypes) {
    fclose(file);
    return 1;
  }

  char lineBuf[305];

  // Parse Types
  fgets(lineBuf, sizeof(lineBuf), file);

  if (fgets(lineBuf, sizeof(lineBuf), file)) {
    char *token = strtok(lineBuf, ",\r\n");
    if (token && strncmp(token, "Pokemons", 8) != 0) {
      int i = 0;
      while (token && i < numberOfTypes) {
        Pokemon_Type *t = create_type(token);
        if (t) {
          allTypes[i] = *t;
          free(t);
        }
        i++;
        token = strtok(NULL, ",\r\n");
      }
    }
  }

  // Prepare categories
  char categoriesStr[1024] = "";
  for (int i = 0; i < numberOfTypes; i++) {
    strcat(categoriesStr, allTypes[i].Pokemontype_name);
    if (i < numberOfTypes - 1)
      strcat(categoriesStr, ",");
  }

  Battle battle = createBattleByCategory(
      typeCapacity, numberOfTypes, categoriesStr, comparePokemonAttack,
      copyPokemonElement, destroyPokemonElement, getPokemonCategory,
      getAttackWithBonuses, printPokemonElement);

  if (!battle) {
    freeMemoryTypes(allTypes, numberOfTypes);
    fclose(file);
    return 0;
  }

  // Parse Effectiveness
  while (fgets(lineBuf, sizeof(lineBuf), file)) {
    if (strncmp(lineBuf, "Pokemons", 8) == 0)
      break;

    char tempBuf[305];
    strcpy(tempBuf, lineBuf);

    char *token = strtok(tempBuf, " \t:");
    if (!token)
      continue;

    Pokemon_Type *srcType = NULL;
    for (int i = 0; i < numberOfTypes; i++) {
      if (strcmp(allTypes[i].Pokemontype_name, token) == 0) {
        srcType = &allTypes[i];
        break;
      }
    }
    if (!srcType)
      continue;

    char *mode = strtok(NULL, ":");
    if (!mode)
      continue;

    int isAgainstMe = -1;
    if (strstr(mode, "effective-against-me"))
      isAgainstMe = 1;
    else if (strstr(mode, "effective-against-other"))
      isAgainstMe = 0;

    if (isAgainstMe != -1) {
      char *target = strtok(NULL, ",\r\n");
      while (target) {
        while (isspace(*target))
          target++;
        Pokemon_Type *tgtType = findTypeByName(allTypes, numberOfTypes, target);
        if (tgtType) {
          if (isAgainstMe)
            add_effective_against_me(srcType, tgtType);
          else
            add_effective_against_others(srcType, tgtType);
        }
        target = strtok(NULL, ",\r\n");
      }
    }
  }

  // Parse Pokemons
  while (fgets(lineBuf, sizeof(lineBuf), file)) {
    if (!strchr(lineBuf, ','))
      break;

    char *name = strtok(lineBuf, ",");
    char *species = strtok(NULL, ",");
    char *hStr = strtok(NULL, ",");
    char *wStr = strtok(NULL, ",");
    char *aStr = strtok(NULL, ",");
    char *tName = strtok(NULL, ",\r\n");

    if (name && species && hStr && wStr && aStr && tName) {
      while (isspace(*tName))
        tName++;
      char *end = tName + strlen(tName) - 1;
      while (end > tName && isspace(*end))
        *end-- = 0;

      Pokemon_Type *pTypePtr = findTypeByName(allTypes, numberOfTypes, tName);
      if (!pTypePtr) {
        destroyBattleByCategory(battle);
        freeMemoryTypes(allTypes, numberOfTypes);
        fclose(file);
        exit(0);
      }

      PokemonDB *newPoke = create_pokemon(name, pTypePtr, species, atof(hStr),
                                          atof(wStr), atoi(aStr));
      if (!newPoke) {
        destroyBattleByCategory(battle);
        freeMemoryTypes(allTypes, numberOfTypes);
        fclose(file);
        exit(0);
      }
      insertObject(battle, newPoke);
    }
  }

  // Interactive Menu
  char choice;
  bool running = true;
  while (running) {
    printf("Please choose one of the following numbers:\n");
    printf("1 : Print all Pokemons by types\n");
    printf("2 : Print all Pokemons types\n");
    printf("3 : Insert Pokemon to battles training camp\n");
    printf("4 : Remove strongest Pokemon by type\n");
    printf("5 : Fight\n");
    printf("6 : Exit\n");

    scanf(" %c", &choice);

    switch (choice) {
    case '1':
      handlePrintAllPokemons(battle);
      break;
    case '2':
      handlePrintAllTypes(allTypes, numberOfTypes);
      break;
    case '3':
      handleAddPokemon(battle, allTypes, numberOfTypes, typeCapacity);
      break;
    case '4':
      handleRemoveStrongest(battle, allTypes, numberOfTypes);
      break;
    case '5':
      handleFight(battle, allTypes, numberOfTypes);
      break;
    case '6':
      destroyBattleByCategory(battle);
      freeMemoryTypes(allTypes, numberOfTypes);
      printf("All the memory cleaned and the program is safely closed.\n");
      running = false;
      break;
    default:
      printf("Please choose a valid number.\n");
      break;
    }
  }

  fclose(file);
  return 0;
}
