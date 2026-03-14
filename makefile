PokemonBattles : BattleByCategory.o LinkedList.o PokemonsBattleCenter.o MaxHeap.o Pokemon.o
	gcc BattleByCategory.o LinkedList.o PokemonsBattleCenter.o MaxHeap.o Pokemon.o -o PokemonsBattles
BattleByCategory.o: BattleByCategory.c BattleByCategory.h Defs.h LinkedList.h MaxHeap.h Pokemon.h
	gcc -c BattleByCategory.c
LinkedList.o: LinkedList.c LinkedList.h Defs.h MaxHeap.h
	gcc -c LinkedList.c
PokemonsBattleCenter.o: PokemonsBattleCenter.c Pokemon.h Defs.h LinkedList.h MaxHeap.h BattleByCategory.h
	gcc -c PokemonsBattleCenter.c
MaxHeap.o: MaxHeap.c MaxHeap.h Defs.h Pokemon.h
	gcc -c MaxHeap.c
Pokemon.o: Pokemon.c Defs.h Pokemon.h
	gcc -c Pokemon.c
clean :
	rm PokemonsBattleCenter.o BattleByCategory.o LinkedList.o MaxHeap.o Pokemon.o PokemonsBattles
