#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <ncurses.h>
#include <iostream>
#include <climits>
#include "terrainGen.h"
#include "map.h"
#include "world.h"
#include "character.h"
#include "queue.h"
#include "pokeData.h"
#include "pokemon.h"

#define GREEN 1
#define BLUE 2
#define RED 3
#define YELLOW 4
#define PURPLE 5
#define WHITE 6
#define CYAN 7

int main(int argc, char *argv[])
{

    Character *player;
    // int playing;
    // char input[30];
    // char command;
    // map *current;
    World *w;
    int x, y;
    Map *m;
    int num_trainers = 10;
    int i;
    Data *data = new Data();

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "pokemon") == 0)
        {
            data->set_vpokemon();
            data->output_list(POKEMON);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "moves") == 0)
        {
            data->set_vmoves();
            data->output_list(MOVES);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "pokemon_moves") == 0)
        {
            data->set_vpokemon_moves();
            data->output_list(POKEMON_MOVES);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "pokemon_species") == 0)
        {
            data->set_vpokemon_species();
            data->output_list(POKEMON_SPECIES);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "experience") == 0)
        {
            data->set_vexperience();
            data->output_list(EXPERIENCE);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "type_names") == 0)
        {
            data->set_vtype_names();
            data->output_list(TYPE_NAMES);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "pokemon_stats") == 0)
        {
            data->set_vpokemon_stats();
            data->output_list(POKEMON_STATS);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "stats") == 0)
        {
            data->set_vstats();
            data->output_list(STATS);
            data->~Data();
            return 0;
        }
        else if (strcmp(argv[i], "pokemon_types") == 0)
        {
            data->set_vpokemon_types();
            data->output_list(POKEMON_TYPES);
            data->~Data();
            return 0;
        }
    }

    srand(time(NULL));

    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--numtrainers") == 0)
        {
            if (i + 1 < argc)
            {
                num_trainers = atoi(argv[i + 1]);
                break;
            }
            else
            {
                fprintf(stderr, "Error: --numtrainers requires an integer\n");
            }
        }
    }

    data->set_vpokemon();
    data->set_vpokemon_moves();
    data->set_vmoves();
    data->set_vpokemon_types();
    data->set_vpokemon_stats();
    data->set_vtype_names();
    data->set_vpokemon_species();
    data->set_vexperience();

    w = new World();
    w->set_num_trainers(num_trainers);
    w->set_map(200, 200);
    w->get_map(200, 200, &m);
    m->get_map_location(&x, &y);

    player = new Character('@', m, w, data);
    m->add_character(player, w);
    w->pop_map(m, data);

    // initialize curses here
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(RED, COLOR_RED, COLOR_BLACK);
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(PURPLE, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(ALL_WHITE, COLOR_WHITE, COLOR_WHITE);

    w->starter_pokemon(player, data);

    printTerrain(m->grid, x, y, m->charMap);

    int move = m->map_turn(w, data);
    int j;
    Map *new_map;
    while (move == 1)
    {
        for (i = 0; i < 21; i++)
        {
            for (j = 0; j < 80; j++)
            {
                w->hikerMap[i][j] = INT_MAX;
            }
        }
        for (i = 0; i < 21; i++)
        {
            for (j = 0; j < 80; j++)
            {
                w->rivalMap[i][j] = INT_MAX;
            }
        }
        w->get_map(w->map_x, w->map_y, &new_map);
        new_map->add_character(player, w);
        new_map->pc_row = player->row;
        new_map->pc_col = player->col;
        player->shortestPathHiker(w->maps[w->map_x][w->map_y], w);
        player->shortestPathRival(w->maps[w->map_x][w->map_y], w);

        if (new_map->pq->size == 1)
        {
            w->pop_map(new_map, data);
        }
        move = w->maps[w->map_x][w->map_y]->map_turn(w, data);
    }

    delete (w);
    endwin();

    return 0;
}