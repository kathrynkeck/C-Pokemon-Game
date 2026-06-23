#ifndef MAP_H
#define MAP_H

#include "pokeData.h"

class Queue;
class Character;
class World;

class Map
{
public:
    char grid[21][80];
    int xLoc;
    int yLoc;
    int nGate;
    int sGate;
    int wGate;
    int eGate;
    int pc_row;
    int pc_col;
    Queue *pq;
    Character *charMap[21][80];
    Map();
    ~Map();
    int get_grid(char grid[21][80]);
    int get_map_location(int *xLoc, int *yLoc);
    int set_map_location(int xLoc, int yLoc);
    int get_nGate(int *nGate);
    int get_sGate(int *sGate);
    int get_eGate(int *eGate);
    int get_wGate(int *wGate);
    int set_nGate(int nGate);
    int set_sGate(int sGate);
    int set_eGate(int eGate);
    int set_wGate(int wGate);

    int map_turn(World *w, Data *d);
    int add_character(Character *c, World *w);
    int display_trainers();
};

#endif