main: main.cpp terrainGen.cpp queue.cpp map.cpp world.cpp character.cpp heap.cpp pokeData.cpp pokemon.cpp
	g++ main.cpp terrainGen.cpp queue.cpp map.cpp world.cpp character.cpp heap.cpp pokedata.cpp pokemon.cpp -o pokemon -lncurses -g -Wall -Werror

clean:
	rm -f pokemon *~ *.o core