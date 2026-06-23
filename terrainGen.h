#ifndef TERRAINGEN_H
#define TERRAINGEN_H

#include "map.h"
#include "character.h"

#define GREEN 1
#define BLUE 2
#define RED 3
#define YELLOW 4
#define PURPLE 5
#define WHITE 6
#define CYAN 7
#define ALL_WHITE 8
#define ALL_BLACK 9

int generateTerrain(struct Map *m);
int printTerrain(char grid[21][80], int xLoc, int yLoc, Character *charMap[21][80]);
int pokemart(World *w);
int pokecenter(Character *pc, World *w, Data *d);

#endif