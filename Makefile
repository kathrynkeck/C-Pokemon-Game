main: main.cpp terrainGen.cpp queue.cpp map.cpp world.cpp character.cpp heap.cpp pokeData.cpp pokemon.cpp
	g++ main.cpp terrainGen.cpp queue.cpp map.cpp world.cpp character.cpp heap.cpp pokedata.cpp pokemon.cpp -o main -lncurses -g -Wall -Werror

clean:
	rm -f main *~ *.o core