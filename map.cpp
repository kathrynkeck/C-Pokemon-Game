#include <cstdlib>
#include <ncurses.h>
#include <cstring>
#include "map.h"
#include "terrainGen.h"
#include "queue.h"
#include "pokeData.h"

class World;

Map::Map()
{
    int i;
    int j;

    pq = new Queue();

    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            grid[i][j] = 0;
        }
    }
    xLoc = -1;
    yLoc = -1;
    nGate = 0;
    eGate = 0;
    sGate = 0;
    wGate = 0;
    pc_row = 0;
    pc_col = 0;

    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            charMap[i][j] = NULL;
        }
    }
}

Map::~Map()
{
    int i, j;
    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            if ((charMap[i][j]))
            {
                delete (charMap[i][j]);
                charMap[i][j] = NULL;
            }
        }
    }

    pq->queue_destroy();
    delete pq;
}

int Map::get_grid(char grid[21][80])
{
    int i;
    int j;
    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            grid[i][j] = grid[i][j];
        }
    }
    return 0;
}
int Map::get_map_location(int *xLoc, int *yLoc)
{
    *xLoc = this->xLoc;
    *yLoc = this->yLoc;

    return 0;
}
int Map::set_map_location(int xLoc, int yLoc)
{
    this->xLoc = xLoc;
    this->yLoc = yLoc;

    return 0;
}
int Map::get_nGate(int *nGate)
{
    *nGate = this->nGate;

    return 0;
}
int Map::get_sGate(int *sGate)
{
    *sGate = this->sGate;

    return 0;
}
int Map::get_eGate(int *eGate)
{
    *eGate = this->eGate;

    return 0;
}
int Map::get_wGate(int *wGate)
{
    *wGate = this->wGate;

    return 0;
}
int Map::set_nGate(int nGate)
{
    this->nGate = nGate;

    return 0;
}
int Map::set_sGate(int sGate)
{
    this->sGate = sGate;

    return 0;
}
int Map::set_eGate(int eGate)
{
    this->eGate = eGate;

    return 0;
}
int Map::set_wGate(int wGate)
{
    this->wGate = wGate;

    return 0;
}

int Map::map_turn(World *w, Data *d)
{
    int quit_game = 0;
    int gclock;
    int valid_move;
    char ch;
    int cost;
    int move_map = false;
    Character *curr;

    while (!quit_game)
    {
        curr = pq->pqueue_dequeue(&gclock);
        // printTerrain(m->grid, x, y, w->charMap);
        // mvprintw(22, 0, "current: %c, row: %d, col: %d, next move: %d %d", curr->type, curr->row, curr->col, curr->nextRow, curr->nextCol);
        // getch();

        if (curr->type == '@')
        {
            if (charMap[curr->nextRow][curr->nextCol] == NULL)
            {
                charMap[curr->row][curr->col] = NULL;
                curr->row = curr->nextRow;
                curr->col = curr->nextCol;
                charMap[curr->row][curr->col] = curr;
            }
            else if (!charMap[curr->nextRow][curr->nextCol]->defeated && charMap[curr->nextRow][curr->nextCol]->type != '@')
            {
                w->trainer_battle(charMap[curr->nextRow][curr->nextCol], curr, d);
            }
            pc_row = curr->row;
            pc_col = curr->col;

            if (grid[pc_row][pc_col] == ':' && rand() % 10 == 0)
            {
                // pokemon battle
                int distance = abs(xLoc - 200) + abs(yLoc - 200);
                w->pokemon_battle(distance, d, curr);
            }

            printTerrain(grid, w->map_x, w->map_y, charMap);

            curr->shortestPathHiker(this, w);
            curr->shortestPathRival(this, w);

            valid_move = 0;
            while (!valid_move)
            {
                ch = getch();
                valid_move = 1;
                switch (ch)
                {
                case '7':
                case 'y':
                    // move up-left
                    if (curr->pc_next_move(this, w, &cost, UP_LEFT) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '8':
                case 'k':
                    // move up
                    if (curr->pc_next_move(this, w, &cost, UP) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '9':
                case 'u':
                    // move up-right
                    if (curr->pc_next_move(this, w, &cost, UP_RIGHT) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '6':
                case 'l':
                    // move right
                    if (curr->pc_next_move(this, w, &cost, RIGHT) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '3':
                case 'n':
                    // move down-right
                    if (curr->pc_next_move(this, w, &cost, DOWN_RIGHT) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '2':
                case 'j':
                    // move down
                    if (curr->pc_next_move(this, w, &cost, DOWN) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '1':
                case 'b':
                    // move down-left
                    if (curr->pc_next_move(this, w, &cost, DOWN_LEFT) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '4':
                case 'h':
                    // move left
                    if (curr->pc_next_move(this, w, &cost, LEFT) == 1)
                    {
                        quit_game = true;
                        move_map = true;
                    }
                    break;
                case '>':
                    // enter pokemart or pokecenter
                    if (grid[curr->row][curr->col] == 'M')
                    {
                        pokemart(w);
                    }
                    else if (grid[curr->row][curr->col] == 'C')
                    {
                        pokecenter(curr, w, d);
                    }
                    else
                    {
                        valid_move = 0;
                        mvprintw(23, 0, "Invalid input, try again");
                        refresh();
                    }
                    break;
                case '5':
                case ' ':
                case '.':
                    // rest for a turn
                    cost = 10;
                    curr->nextRow = curr->row;
                    curr->nextCol = curr->col;
                    break;
                case 't':
                    // display list of trainers on map, symbols, relative positions to PC
                    display_trainers();
                    break;
                case 'q':
                case 'Q':
                    // quit game
                    quit_game = true;
                    continue;
                    break;
                case 'f':
                    int x_map, y_map;
                    mvprintw(0, 0, "Fly to (X Y): ");
                    clrtoeol();
                    echo();
                    curs_set(1);
                    if (scanw("%d %d", &x_map, &y_map) != 2)
                    {
                        mvprintw(23, 0, "Invalid format");
                        valid_move = 0;
                        noecho();
                        curs_set(0);
                        refresh();
                    }
                    else
                    {
                        if (y_map >= -200 && y_map <= 200 && x_map >= -200 && x_map <= 200)
                        {
                            Map *fly_map;
                            w->map_x = x_map + 200;
                            w->map_y = y_map + 200;
                            if (!w->maps[w->map_x][w->map_y])
                            {
                                w->set_map(w->map_x, w->map_y);
                            }
                            w->get_map(w->map_x, w->map_y, &fly_map);
                            charMap[curr->row][curr->col] = NULL;
                            curr->pc_init(fly_map, w);
                            curr->nextRow = curr->row;
                            curr->nextCol = curr->col;
                            noecho();
                            curs_set(0);
                            refresh();
                            return 1;
                        }
                        else
                        {
                            mvprintw(23, 0, "Map out of bounds");
                            valid_move = 0;
                            noecho();
                            curs_set(0);
                            refresh();
                        }
                    }
                    break;
                case 'B':
                    w->map_bag_interface(curr);
                    valid_move = 0;
                    printTerrain(grid, xLoc, yLoc, charMap);
                    break;
                default:
                    valid_move = 0;
                    mvprintw(23, 0, "Invalid input, try again");
                    refresh();
                    break;
                }
            }
        }
        else
        {
            if (curr->row != curr->nextRow || curr->col != curr->nextCol)
            {
                if (charMap[curr->nextRow][curr->nextCol] == NULL)
                {
                    charMap[curr->row][curr->col] = NULL;
                    curr->row = curr->nextRow;
                    curr->col = curr->nextCol;
                    charMap[curr->row][curr->col] = curr;
                }
                else if (charMap[curr->nextRow][curr->nextCol]->type == '@' && !curr->defeated)
                {
                    w->trainer_battle(curr, charMap[curr->nextRow][curr->nextCol], d);
                }
                else
                {
                    curr->nextRow = curr->row;
                    curr->nextCol = curr->col;
                }
            }
            curr->next_move(this, w, &cost);
        }

        if (cost < 10)
        {
            cost = 10;
        }
        if (!move_map)
        {
            pq->pqueue_enqueue(curr, gclock + cost);
        }
    }

    if (move_map == true)
    {
        charMap[curr->row][curr->col] = NULL;
        curr->row = curr->nextRow;
        curr->col = curr->nextCol;

        return 1;
    }
    return 0;
}

int Map::add_character(Character *c, World *w)
{
    int cost;

    charMap[c->row][c->col] = c;

    if (c->type == '@' && pq->size > 0)
    {
        pq->pqueue_view(&cost);
    }
    else if (c->type == '@')
    {
        cost = 10;
    }
    else
    {
        c->next_move(this, w, &cost);
    }
    pq->pqueue_enqueue(c, cost);

    return 0;
}

int Map::display_trainers()
{
    int r, c;
    int i;
    erase();
    addstr("Trainers on map\n");
    char *list[1680];
    char curr[100];
    int count = 0;
    for (r = 0; r < 21; r++)
    {
        for (c = 0; c < 80; c++)
        {
            if (charMap[r][c])
            {
                if (charMap[r][c]->type == '@')
                {
                    continue;
                }
                char v[30] = "";
                char h[20] = "";
                char a[10] = "";

                if (r > pc_row)
                {
                    sprintf(v, "%d south", r - pc_row);
                }
                else if (r < pc_row)
                {
                    sprintf(v, "%d north", pc_row - r);
                }

                if (r != pc_row && c != pc_col)
                {
                    sprintf(a, " and ");
                }

                if (c > pc_col)
                {
                    sprintf(h, "%d east", c - pc_col);
                }
                else if (c < pc_col)
                {
                    sprintf(h, "%d west", pc_col - c);
                }

                snprintf(curr, sizeof(curr), "%c, %s%s%s", charMap[r][c]->type, v, a, h);
                list[count] = strdup(curr);
                count++;
            }
        }
    }

    for (i = 0; i < count && i < 21; i++)
    {
        mvprintw(i + 1, 0, list[i]);
    }
    mvprintw(23, 0, "press escape to go back to map");
    refresh();

    int more = true;
    int page = 0;
    int key;
    while (more)
    {
        key = getch();
        switch (key)
        {
        case KEY_UP:
            if (page != 0)
            {
                page--;
            }

            erase();
            addstr("Trainers on map\n");
            for (i = 0; i + (page * 21) < count && i < 21; i++)
            {
                mvprintw(i + 1, 0, list[i + (page * 21)]);
            }
            mvprintw(23, 0, "press escape to go back to map");
            refresh();
            break;
        case KEY_DOWN:
            if (count > (page + 1) * 21)
            {
                page++;
            }

            erase();
            addstr("Trainers on map\n");
            for (i = 0; i + (page * 21) < count && i < 21; i++)
            {
                mvprintw(i + 1, 0, list[i + (page * 21)]);
            }
            mvprintw(23, 0, "press escape to go back to map");
            refresh();
            break;
        case 27:
            more = false;
            break;
        }
    }

    for (i = 0; i < count; i++)
    {
        free(list[i]);
    }
    return 0;
}