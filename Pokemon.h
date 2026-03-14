#ifndef POKEMON_H_
#define POKEMON_H_
#include <stdio.h>
#include "Defs.h" 

typedef struct Pokemon_Type {
    char* Pokemontype_name;
    int pokemon_num;
    struct Pokemon_Type **effective_against_me;
    struct Pokemon_Type **effective_against_others;
    int num_against_me;
    int num_against_others;
} Pokemon_Type;

typedef struct {
    float height;
    float weight;
    int attack;
} BiologicalInfo;

typedef struct {
    char* pok_name;

    // pointer to the type object
    Pokemon_Type* pok_type;
    char* pok_species;
    BiologicalInfo bio_info;
} PokemonDB;



BiologicalInfo create_biological_info(float height, float weight, int attack);
PokemonDB* create_pokemon(const char *pok_name, Pokemon_Type* pok_type, const  char *pok_species, float height, float weight, int attack);
Pokemon_Type* create_type(const char* Pokemontype_name);

status add_effective_against_me(Pokemon_Type *pok1, Pokemon_Type *pok2);
status add_effective_against_others(Pokemon_Type *pok1, Pokemon_Type *pok2);
status delete_effective_against_me(Pokemon_Type *pok1, Pokemon_Type *pok2);
status delete_effective_against_others(Pokemon_Type *pok1, Pokemon_Type *pok2);

PokemonDB* allocatePokemons(int numberOfPokemon);
Pokemon_Type* get_type(const char* name, Pokemon_Type *types, int numOfTypes);
Pokemon_Type* allocateTypes(int numberOfTypes);


status  print_pokemon(const PokemonDB *p1);
status  print_type(const Pokemon_Type *pt);

void freeMemoryPokemon(PokemonDB *pokemons, int numberOfPokemon);
void freeMemoryTypes(Pokemon_Type *types, int type_num);

void read_file(FILE *file, int type_num, int num_pokemons, Pokemon_Type *types, PokemonDB *pokemons);


char* trim(char* str);

#endif /* POKEMON_H_ */