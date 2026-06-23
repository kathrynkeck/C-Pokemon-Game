#ifndef WORLD_H
#define WORLD_H
#include "map.h"
#include "pokeData.h"
#include "pokemon.h"
#include <vector>

class Character;

class Bag
{
public:
    int num_potions;
    int num_revives;
    int num_pokeballs;
    Bag() : num_potions(3), num_revives(3), num_pokeballs(6) {}
};

class World
{
public:
    Map *maps[401][401];
    int map_x;
    int map_y;
    int hikerMap[21][80];
    int rivalMap[21][80];
    int num_trainers;
    Bag *pc_bag;
    std::vector<Pokemon *> user_pokemon;

    World();
    ~World();
    int get_map(int x, int y, Map **m);
    int set_map(int x, int y);
    int next_turn();
    int trainer_battle(Character *npc, Character *pc, Data *d);
    int set_num_trainers(int nt);
    int pop_map(Map *m, Data *d);
    int starter_pokemon(Character *pc, Data *d);
    int pokemon_battle(int distance, Data *d, Character *pc);
    int map_bag_interface(Character *pc);
    int battle_bag_interface(Character *pc, Pokemon *wild);
    int npc_battle_bag(Character *pc);
    bool catch_pokemon(Pokemon *wild, Character *pc);
};

#endif