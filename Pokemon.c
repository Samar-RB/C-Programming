#include "Pokemon.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int type_num = 0;

BiologicalInfo create_biological_info(float height, float weight, int attack) {
  BiologicalInfo bio;
  bio.height = height;
  bio.weight = weight;
  bio.attack = attack;
  return bio;
}

PokemonDB *create_pokemon(const char *pok_name, Pokemon_Type *pok_type,
                          const char *pok_species, float height, float weight,
                          int attack) {

  if (!pok_name || !pok_type || !pok_species || height < 0 || weight < 0 ||
      attack < 0) {
    return NULL;
  }

  PokemonDB *new_pokemon = (PokemonDB *)malloc(sizeof(PokemonDB));
  if (!new_pokemon) {
    return NULL;
  }

  new_pokemon->pok_name = strdup(pok_name);
  new_pokemon->pok_species = strdup(pok_species);
  new_pokemon->pok_type = pok_type;
  new_pokemon->bio_info = create_biological_info(height, weight, attack);
  pok_type->pokemon_num++;

  return new_pokemon;
}

Pokemon_Type *create_type(const char *Pokemontype_name) {
  if (!Pokemontype_name)
    return NULL;

  Pokemon_Type *new_type = (Pokemon_Type *)malloc(sizeof(Pokemon_Type));
  if (!new_type) {
    fprintf(stderr, "Memory Problem\n");
    exit(EXIT_FAILURE);
  }

  new_type->Pokemontype_name = strdup(Pokemontype_name);
  new_type->effective_against_me = NULL;
  new_type->effective_against_others = NULL;
  new_type->num_against_me = 0;
  new_type->num_against_others = 0;
  new_type->pokemon_num = 0;

  return new_type;
}

status add_effective_against_me(Pokemon_Type *pok1, Pokemon_Type *pok2) {
  if (!pok1 || !pok2)
    return failure;
  for (int i = 0; i < pok1->num_against_me; i++) {
    if (pok1->effective_against_me[i] == pok2)
      return failure;
  }
  Pokemon_Type **tmp = (Pokemon_Type **)realloc(pok1->effective_against_me,
                                                (pok1->num_against_me + 1) *
                                                    sizeof(Pokemon_Type *));
  if (!tmp) {
    fprintf(stderr, "Memory Problem\n");
    exit(EXIT_FAILURE);
  }
  pok1->effective_against_me = tmp;
  pok1->effective_against_me[pok1->num_against_me] = pok2;
  pok1->num_against_me++;
  return success;
}

status add_effective_against_others(Pokemon_Type *pok1, Pokemon_Type *pok2) {
  if (!pok1 || !pok2)
    return failure;
  for (int i = 0; i < pok1->num_against_others; i++) {
    if (pok1->effective_against_others[i] == pok2) {
      return failure;
    }
  }
  Pokemon_Type **tmp = (Pokemon_Type **)realloc(pok1->effective_against_others,
                                                (pok1->num_against_others + 1) *
                                                    sizeof(Pokemon_Type *));

  if (!tmp) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  pok1->effective_against_others = tmp;
  pok1->effective_against_others[pok1->num_against_others] = pok2;
  pok1->num_against_others++;

  return success;
}

status delete_effective_against_me(Pokemon_Type *pok1, Pokemon_Type *pok2) {
  if (!pok1 || !pok2)
    return failure;

  if (pok1->Pokemontype_name == NULL) {
    printf("Type name '%s' doesn't exist.\n", pok1->Pokemontype_name);
    return failure;
  }

  int idx = -1;
  for (int i = 0; i < pok1->num_against_me; i++) {
    if (pok1->effective_against_me[i] == pok2) {
      idx = i;
      break;
    }
  }

  if (idx == -1) {
    return failure;
  }

  for (int i = idx; i < pok1->num_against_me - 1; i++) {
    pok1->effective_against_me[i] = pok1->effective_against_me[i + 1];
  }

  pok1->num_against_me--;

  if (pok1->num_against_me == 0) {
    free(pok1->effective_against_me);
    pok1->effective_against_me = NULL;
  } else {
    Pokemon_Type **tmp =
        (Pokemon_Type **)realloc(pok1->effective_against_me,
                                 pok1->num_against_me * sizeof(Pokemon_Type *));

    if (!tmp && pok1->num_against_me > 0) {
      perror("Memory reallocation failed");
      exit(EXIT_FAILURE);
    }

    pok1->effective_against_me = tmp;
  }

  return success;
}

status delete_effective_against_others(Pokemon_Type *pok1, Pokemon_Type *pok2) {
  if (!pok1 || !pok2)
    return failure;

  int idx = -1;

  for (int i = 0; i < pok1->num_against_others; i++) {
    if (pok1->effective_against_others[i] == pok2) {
      idx = i;
      break;
    }
  }

  if (idx == -1) {
    printf("Type '%s' is not effective against '%s'.\n", pok2->Pokemontype_name,
           pok1->Pokemontype_name);
    return failure;
  }

  for (int i = idx; i < pok1->num_against_others - 1; i++) {
    pok1->effective_against_others[i] = pok1->effective_against_others[i + 1];
  }

  pok1->num_against_others--;

  if (pok1->num_against_others == 0) {
    free(pok1->effective_against_others);
    pok1->effective_against_others = NULL;
  } else {
    Pokemon_Type **tmp = (Pokemon_Type **)realloc(
        pok1->effective_against_others,
        pok1->num_against_others * sizeof(Pokemon_Type *));

    if (!tmp && pok1->num_against_others > 0) {
      perror("Memory reallocation failed");
      exit(EXIT_FAILURE);
    }

    pok1->effective_against_others = tmp;
  }

  return success;
}

PokemonDB *allocatePokemons(int numberOfPokemon) {
  if (numberOfPokemon <= 0)
    return NULL;
  PokemonDB *pokemons = (PokemonDB *)calloc(numberOfPokemon, sizeof(PokemonDB));
  if (!pokemons) {
    fprintf(stderr, "Memory Problem\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < numberOfPokemon; i++) {
    pokemons[i].pok_name = NULL;
    pokemons[i].pok_species = NULL;
    pokemons[i].pok_type = NULL;
    pokemons[i].bio_info.height = 0.0f;
    pokemons[i].bio_info.weight = 0.0f;
    pokemons[i].bio_info.attack = 0;
  }
  return pokemons;
}

Pokemon_Type *get_type(const char *name, Pokemon_Type *types, int numOfTypes) {
  if (!name || !types || numOfTypes <= 0)
    return NULL;

  for (int i = 0; i < numOfTypes; i++) {
    if (types[i].Pokemontype_name &&
        strcmp(types[i].Pokemontype_name, name) == 0) {
      return &types[i];
    }
  }
  return NULL;
}

Pokemon_Type *allocateTypes(int numberOfTypes) {
  if (numberOfTypes <= 0)
    return NULL;

  Pokemon_Type *types =
      (Pokemon_Type *)malloc(numberOfTypes * sizeof(Pokemon_Type));
  if (!types) {
    fprintf(stderr, "Memory Problem\n");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < numberOfTypes; i++) {
    types[i].Pokemontype_name = NULL;
    types[i].effective_against_me = NULL;
    types[i].effective_against_others = NULL;
    types[i].num_against_me = 0;
    types[i].num_against_others = 0;
    types[i].pokemon_num = 0;
  }

  return types;
}

status print_pokemon(const PokemonDB *p1) {
  if (!p1 || !p1->pok_name || !p1->pok_species || !p1->pok_type) {
    printf("The Pokemon doesn't exist.\n");
    return failure;
  }

  printf("%s :\n", p1->pok_name);
  printf("%s, %s Type.\n", p1->pok_species, p1->pok_type->Pokemontype_name);
  printf("Height: %.2f m    Weight: %.2f kg    Attack: %d\n\n",
         p1->bio_info.height, p1->bio_info.weight, p1->bio_info.attack);

  return success;
}

status print_type(const Pokemon_Type *pt) {
  if (!pt) {
    printf("Error: Pokemon type pointer is NULL.\n");
    return failure;
  }

  if (!pt->Pokemontype_name) {
    printf("Error: Pokemon type name is NULL.\n");
    return failure;
  }
  printf("Type %s -- %d pokemons\n", pt->Pokemontype_name, pt->pokemon_num);
  if (pt->num_against_me > 0 && pt->effective_against_me) {
    printf("\tThese types are super-effective against %s:",
           pt->Pokemontype_name);

    for (int i = 0; i < pt->num_against_me; i++) {
      if (pt->effective_against_me[i])
        printf("%s", pt->effective_against_me[i]->Pokemontype_name);

      if (i < pt->num_against_me - 1)
        printf(" ,");
    }
    printf("\n");
  }
  if (pt->num_against_others > 0 && pt->effective_against_others) {
    printf("\t%s moves are super-effective against:", pt->Pokemontype_name);

    for (int i = 0; i < pt->num_against_others; i++) {
      if (pt->effective_against_others[i])
        printf("%s", pt->effective_against_others[i]->Pokemontype_name);

      if (i < pt->num_against_others - 1)
        printf(" ,");
    }
    printf("\n");
  }

  printf("\n");
  return success;
}

void freeMemoryPokemon(PokemonDB *pokemons, int numberOfPokemon) {
  if (!pokemons)
    return;
  for (int i = 0; i < numberOfPokemon; i++) {
    if (pokemons[i].pok_name)
      free(pokemons[i].pok_name);
    if (pokemons[i].pok_species)
      free(pokemons[i].pok_species);
  }
  free(pokemons);
}

void freeMemoryTypes(Pokemon_Type *types, int type_num) {
  if (!types)
    return;
  for (int i = 0; i < type_num; i++) {
    if (types[i].Pokemontype_name)
      free(types[i].Pokemontype_name);
    if (types[i].effective_against_me)
      free(types[i].effective_against_me);
    if (types[i].effective_against_others)
      free(types[i].effective_against_others);
  }
  free(types);
}

char *trim(char *str) {
  while (isspace((unsigned char)*str))
    str++;
  if (*str == 0)
    return str;
  char *end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;
  end[1] = '\0';
  return str;
}

void read_file(FILE *file, int type_num, int num_pokemons, Pokemon_Type *types,
               PokemonDB *pokemons) {
  if (!file || !types || !pokemons)
    return;
  char line[1024];
  int currentLine = 0;
  int count_pok = 0;
  int reading_pokemon = 0;
  int type_idx = 0;

  if (fgets(line, sizeof(line), file) != NULL) {
  }

  if (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\r\n")] = 0;
    char *word = strtok(line, ",");
    while (word && type_idx < type_num) {
      Pokemon_Type *t = create_type(trim(word));
      if (t) {
        // Copy content
        types[type_idx] = *t;
        free(t);
        type_idx++;
      }
      word = strtok(NULL, ",");
    }
  }

  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\r\n")] = 0;
    if (strlen(line) == 0)
      continue;

    if (strstr(line, "Pokemons") != NULL) {
      reading_pokemon = 1;
      continue;
    }

    if (reading_pokemon) {
      if (count_pok >= num_pokemons)
        continue;
      char *word = strtok(line, ",");
      if (!word)
        continue;
      word = trim(word);
      pokemons[count_pok].pok_name = strdup(word);
      word = strtok(NULL, ",");
      if (!word)
        continue;
      word = trim(word);
      pokemons[count_pok].pok_species = strdup(word);
      word = strtok(NULL, ",");
      if (!word)
        continue;
      float height = atof(word);
      word = strtok(NULL, ",");
      if (!word)
        continue;
      float weight = atof(word);
      word = strtok(NULL, ",");
      if (!word)
        continue;
      int attack = atoi(word);
      word = strtok(NULL, ",");
      if (!word)
        continue;
      word = trim(word);
      Pokemon_Type *t = get_type(word, types, type_num);
      if (!t) {
        printf("Type '%s' not found\n", word);
      } else {
        pokemons[count_pok].pok_type = t;
        pokemons[count_pok].bio_info =
            create_biological_info(height, weight, attack);
        t->pokemon_num++;
      }

      count_pok++;
      continue;
    }

    // Parsing effectiveness lines
    if (line[0] == '\t') {
      char *tab = line;
      char *space = strchr(tab, ' ');
      if (!space)
        continue;

      int name_len = space - (tab + 1);
      if (name_len <= 0)
        continue;

      char name[301];
      strncpy(name, tab + 1, name_len);
      name[name_len] = '\0';

      Pokemon_Type *curr_type = get_type(trim(name), types, type_num);
      if (!curr_type)
        continue;

      // Effective against me
      char *me_pos = strstr(line, "effective-against-me:");
      if (me_pos) {
        me_pos += strlen("effective-against-me:");
        char *against_me = strtok(me_pos, ",");
        while (against_me) {
          Pokemon_Type *t = get_type(trim(against_me), types, type_num);
          if (t)
            add_effective_against_me(curr_type, t);
          against_me = strtok(NULL, ",");
        }
      } else {
        // Effective against others
        char *other_pos = strstr(line, "effective-against-other:");
        if (other_pos) {
          other_pos += strlen("effective-against-other:");
          char *against_other = strtok(other_pos, ",");
          while (against_other) {
            Pokemon_Type *t = get_type(trim(against_other), types, type_num);
            if (t)
              add_effective_against_others(curr_type, t);
            against_other = strtok(NULL, ",");
          }
        }
      }
    }
  }
}