#ifndef CHARACTER_H
#define CHARACTER_H

#include <vector>

#include "map.h"
#include "world.h"
#include "pokemon.h"

#define UP_LEFT 10
#define UP 11
#define UP_RIGHT 12
#define LEFT 13
#define RIGHT 14
#define DOWN_LEFT 15
#define DOWN 16
#define DOWN_RIGHT 17

class Character
{
public:
    char type;
    int row;
    int col;
    int nextRow;
    int nextCol;
    char dir;
    int priority;
    int defeated;
    std::vector<Pokemon *> c_pokemon;

    Character(char type, Map *m, World *w, Data *d);

    int printDist(int grid[21][80]);
    int next_move(Map *m, World *w, int *cost);
    int pc_next_move(Map *m, World *w, int *cost, int direction);
    int shortestPathHiker(Map *m, World *w);
    int shortestPathRival(Map *m, World *w);
    int pc_init(Map *m, World *w);

private:
    int rival_init(Map *m, World *w);
    int hiker_init(Map *m, World *w);
    int pacer_init(Map *m, World *w);
    int wanderer_init(Map *m, World *w);
    int sentry_init(Map *m, World *w);
    int explorer_init(Map *m, World *w);
    int rival_next_move(Map *m, World *w, int *cost);
    int hiker_next_move(Map *m, World *w, int *cost);
    int defeated_rival_next_move(Map *m, World *w, int *cost);
    int defeated_hiker_next_move(Map *m, World *w, int *cost);
    int pacer_next_move(Map *m, World *w, int *cost);
    int wanderer_next_move(Map *m, World *w, int *cost);
    int explorer_next_move(Map *m, World *w, int *cost);
};

#endif