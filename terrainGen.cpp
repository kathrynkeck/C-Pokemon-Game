#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "queue.h"
#include "terrainGen.h"
#include "map.h"
#include "heap.h"
#include <cmath>
#include <climits>
#include <ncurses.h>
#include <vector>

int gridContains0(char grid[21][80])
{
    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            if (grid[i][j] == 0)
            {
                return 1;
            }
        }
    }

    return 0;
}

int generateTerrain(Map *m)
{
    int xLoc;
    int yLoc;
    m->get_map_location(&xLoc, &yLoc);

    // tree region start
    int treeRow = rand() % 21;
    int treeCol = rand() % 80;
    m->grid[treeRow][treeCol] = '^';
    int tree = (treeRow * 100) + treeCol;
    // tall grass region 1 start
    int tg1Row = rand() % 21;
    int tg1Col = rand() % 40;
    m->grid[tg1Row][tg1Col] = ':';
    int tg1 = (tg1Row * 100) + tg1Col;
    // tall grass region 2 start
    int tg2Row = rand() % 21;
    int tg2Col = (rand() % 40) + 40;
    int tg2 = (tg2Row * 100) + tg2Col;
    m->grid[tg2Row][tg2Col] = ':';
    // water region start
    int waterRow = rand() % 21;
    int waterCol = rand() % 80;
    // check start point isn't the same
    while ((waterRow == tg1Row && waterCol == tg1Col) || (waterRow == tg2Row && waterCol == tg2Col))
    {
        waterRow = rand() % 21;
        waterCol = rand() % 80;
    }
    m->grid[waterRow][waterCol] = '~';
    int water = (waterRow * 100) + waterCol;
    // short grass 1 region start
    int sg1Row = rand() % 21;
    int sg1Col = rand() % 40;
    // check start point isn't the same
    while ((sg1Row == tg1Row && sg1Col == tg1Col) || (sg1Row == waterRow && sg1Col == waterCol))
    {
        sg1Row = rand() % 21;
        sg1Col = rand() % 40;
    }
    m->grid[sg1Row][sg1Col] = '.';
    int sg1 = (sg1Row * 100) + sg1Col;
    // short grass 2 region start
    int sg2Row = rand() % 21;
    int sg2Col = (rand() % 40) + 40;
    // check start point isn't the same
    while ((sg2Row == tg2Row && sg2Col == tg2Col) || (sg2Row == waterRow && sg2Col == waterCol))
    {
        sg2Row = rand() % 21;
        sg2Col = (rand() % 40) + 40;
    }
    m->grid[sg2Row][sg2Col] = '.';
    int sg2 = (sg2Row * 100) + sg2Col;
    // boulder region start
    int boulderRow = rand() % 21;
    int boulderCol = rand() % 80;
    // check start point isn't the same
    while ((boulderRow == tg1Row && boulderCol == tg1Col) || (boulderRow == tg2Row && boulderCol == tg2Col) || (boulderRow == waterRow && boulderCol == waterCol) || (boulderRow == sg1Row && boulderCol == sg1Col) || (boulderRow == sg2Row && boulderCol == sg2Col))
    {
        boulderRow = rand() % 21;
        boulderCol = rand() % 80;
    }
    m->grid[boulderRow][boulderCol] = '%';
    int boulder = (boulderRow * 100) + boulderCol;

    // create queue
    Queue *q = new Queue();

    if ((q->enqueue(tg1)))
    {
        return -1;
    }
    if ((q->enqueue(sg1)))
    {
        return -1;
    }
    if ((q->enqueue(water)))
    {
        return -1;
    }
    if ((q->enqueue(boulder)))
    {
        return -1;
    }
    if ((q->enqueue(tree)))
    {
        return -1;
    }
    if ((q->enqueue(sg2)))
    {
        return -1;
    }
    if ((q->enqueue(tg2)))
    {
        return -1;
    }

    int tmp;
    q->queue_view(&tmp);

    int size;
    q->queue_size(&size);
    int cur;
    int curRow;
    int curCol;
    char type;

    // Expand regions to fill board
    while (size)
    {
        q->dequeue(&cur);
        curRow = cur / 100;
        curCol = cur % 100;
        type = m->grid[curRow][curCol];

        if (type == '.')
        {
            if (curRow > 0 && m->grid[curRow - 1][curCol] == 0)
            {
                m->grid[curRow - 1][curCol] = '.';
                q->enqueue(((curRow - 1) * 100) + curCol);
            }
            if (curCol > 0 && m->grid[curRow][curCol - 1] == 0)
            {
                m->grid[curRow][curCol - 1] = '.';
                q->enqueue((curRow * 100) + (curCol - 1));
            }
            if (curRow < 20 && m->grid[curRow + 1][curCol] == 0)
            {
                m->grid[curRow + 1][curCol] = '.';
                q->enqueue(((curRow + 1) * 100) + curCol);
            }
            if (curCol < 79 && m->grid[curRow][curCol + 1] == 0)
            {
                m->grid[curRow][curCol + 1] = '.';
                q->enqueue((curRow * 100) + (curCol + 1));
            }
            if (curRow > 0 && curCol > 0 && m->grid[curRow - 1][curCol - 1] == 0)
            {
                m->grid[curRow - 1][curCol - 1] = '.';
                q->enqueue(((curRow - 1) * 100) + (curCol - 1));
            }
            if (curRow > 0 && curCol < 79 && m->grid[curRow - 1][curCol + 1] == 0)
            {
                m->grid[curRow - 1][curCol + 1] = '.';
                q->enqueue(((curRow - 1) * 100) + (curCol + 1));
            }
            if (curRow < 20 && curCol > 0 && m->grid[curRow + 1][curCol - 1] == 0)
            {
                m->grid[curRow + 1][curCol - 1] = '.';
                q->enqueue(((curRow + 1) * 100) + (curCol - 1));
            }
            if (curRow < 20 && curCol < 79 && m->grid[curRow + 1][curCol + 1] == 0)
            {
                m->grid[curRow + 1][curCol + 1] = '.';
                q->enqueue(((curRow + 1) * 100) + (curCol + 1));
            }
        }
        else if (type == ':')
        {
            if (curRow > 0 && m->grid[curRow - 1][curCol] == 0)
            {
                m->grid[curRow - 1][curCol] = ':';
                q->enqueue(((curRow - 1) * 100) + curCol);
            }
            if (curCol > 0 && m->grid[curRow][curCol - 1] == 0)
            {
                m->grid[curRow][curCol - 1] = ':';
                q->enqueue((curRow * 100) + (curCol - 1));
            }
            if (curRow < 20 && m->grid[curRow + 1][curCol] == 0)
            {
                m->grid[curRow + 1][curCol] = ':';
                q->enqueue(((curRow + 1) * 100) + curCol);
            }
            if (curCol < 79 && m->grid[curRow][curCol + 1] == 0)
            {
                m->grid[curRow][curCol + 1] = ':';
                q->enqueue((curRow * 100) + (curCol + 1));
            }
            if (curRow > 0 && curCol > 0 && m->grid[curRow - 1][curCol - 1] == 0)
            {
                m->grid[curRow - 1][curCol - 1] = '.';
                q->enqueue(((curRow - 1) * 100) + (curCol - 1));
            }
            if (curRow > 0 && curCol < 79 && m->grid[curRow - 1][curCol + 1] == 0)
            {
                m->grid[curRow - 1][curCol + 1] = ':';
                q->enqueue(((curRow - 1) * 100) + (curCol + 1));
            }
            if (curRow < 20 && curCol > 0 && m->grid[curRow + 1][curCol - 1] == 0)
            {
                m->grid[curRow + 1][curCol - 1] = ':';
                q->enqueue(((curRow + 1) * 100) + (curCol - 1));
            }
            if (curRow < 20 && curCol < 79 && m->grid[curRow + 1][curCol + 1] == 0)
            {
                m->grid[curRow + 1][curCol + 1] = ':';
                q->enqueue(((curRow + 1) * 100) + (curCol + 1));
            }
        }
        else if (type == '~')
        {
            if (curRow > 0 && m->grid[curRow - 1][curCol] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol] = '~';
                q->enqueue(((curRow - 1) * 100) + curCol);
            }
            if (curCol > 0 && m->grid[curRow][curCol - 1] == 0)
            {
                m->grid[curRow][curCol - 1] = '~';
                q->enqueue((curRow * 100) + (curCol - 1));
            }
            if (curRow < 20 && m->grid[curRow + 1][curCol] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol] = '~';
                q->enqueue(((curRow + 1) * 100) + curCol);
            }
            if (curCol < 79 && m->grid[curRow][curCol + 1] == 0)
            {
                m->grid[curRow][curCol + 1] = '~';
                q->enqueue((curRow * 100) + (curCol + 1));
            }
            if (curRow > 0 && curCol > 0 && m->grid[curRow - 1][curCol - 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol - 1] = '~';
                q->enqueue(((curRow - 1) * 100) + (curCol - 1));
            }
            if (curRow > 0 && curCol < 79 && m->grid[curRow - 1][curCol + 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol + 1] = '~';
                q->enqueue(((curRow - 1) * 100) + (curCol + 1));
            }
            if (curRow < 20 && curCol > 0 && m->grid[curRow + 1][curCol - 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol - 1] = '~';
                q->enqueue(((curRow + 1) * 100) + (curCol - 1));
            }
            if (curRow < 20 && curCol < 79 && m->grid[curRow + 1][curCol + 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol + 1] = '~';
                q->enqueue(((curRow + 1) * 100) + (curCol + 1));
            }
        }
        else if (type == '%')
        {
            if (curRow > 0 && m->grid[curRow - 1][curCol] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol] = '%';
                q->enqueue(((curRow - 1) * 100) + curCol);
            }
            if (curCol > 0 && m->grid[curRow][curCol - 1] == 0)
            {
                m->grid[curRow][curCol - 1] = '%';
                q->enqueue((curRow * 100) + (curCol - 1));
            }
            if (curRow < 20 && m->grid[curRow + 1][curCol] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol] = '%';
                q->enqueue(((curRow + 1) * 100) + curCol);
            }
            if (curCol < 79 && m->grid[curRow][curCol + 1] == 0)
            {
                m->grid[curRow][curCol + 1] = '%';
                q->enqueue((curRow * 100) + (curCol + 1));
            }
            if (curRow > 0 && curCol > 0 && m->grid[curRow - 1][curCol - 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol - 1] = '%';
                q->enqueue(((curRow - 1) * 100) + (curCol - 1));
            }
            if (curRow > 0 && curCol < 79 && m->grid[curRow - 1][curCol + 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol + 1] = '%';
                q->enqueue(((curRow - 1) * 100) + (curCol + 1));
            }
            if (curRow < 20 && curCol > 0 && m->grid[curRow + 1][curCol - 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol - 1] = '%';
                q->enqueue(((curRow + 1) * 100) + (curCol - 1));
            }
            if (curRow < 20 && curCol < 79 && m->grid[curRow + 1][curCol + 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol + 1] = '%';
                q->enqueue(((curRow + 1) * 100) + (curCol + 1));
            }
        }
        else if (type == '^')
        {
            if (curRow > 0 && m->grid[curRow - 1][curCol] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol] = '^';
                q->enqueue(((curRow - 1) * 100) + curCol);
            }
            if (curCol > 0 && m->grid[curRow][curCol - 1] == 0)
            {
                m->grid[curRow][curCol - 1] = '^';
                q->enqueue((curRow * 100) + (curCol - 1));
            }
            if (curRow < 20 && m->grid[curRow + 1][curCol] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol] = '^';
                q->enqueue(((curRow + 1) * 100) + curCol);
            }
            if (curCol < 79 && m->grid[curRow][curCol + 1] == 0)
            {
                m->grid[curRow][curCol + 1] = '^';
                q->enqueue((curRow * 100) + (curCol + 1));
            }
            if (curRow > 0 && curCol > 0 && m->grid[curRow - 1][curCol - 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol - 1] = '^';
                q->enqueue(((curRow - 1) * 100) + (curCol - 1));
            }
            if (curRow > 0 && curCol < 79 && m->grid[curRow - 1][curCol + 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow - 1][curCol + 1] = '^';
                q->enqueue(((curRow - 1) * 100) + (curCol + 1));
            }
            if (curRow < 20 && curCol > 0 && m->grid[curRow + 1][curCol - 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol - 1] = '^';
                q->enqueue(((curRow + 1) * 100) + (curCol - 1));
            }
            if (curRow < 20 && curCol < 79 && m->grid[curRow + 1][curCol + 1] == 0 && rand() % 3 == 0)
            {
                m->grid[curRow + 1][curCol + 1] = '^';
                q->enqueue(((curRow + 1) * 100) + (curCol + 1));
            }
        }
        q->queue_size(&size);
    }

    q->queue_destroy();

    // outer boulder terrain
    int i;
    for (i = 0; i < 80; i++)
    {
        m->grid[0][i] = '%';
    }
    for (i = 0; i < 80; i++)
    {
        m->grid[20][i] = '%';
    }
    for (i = 0; i < 21; i++)
    {
        m->grid[i][0] = '%';
    }
    for (i = 0; i < 21; i++)
    {
        m->grid[i][79] = '%';
    }

    // sporatic trees
    int treeC;
    int treeR;
    for (i = 0; i < 40; i++)
    {
        treeC = rand() % 80;
        treeR = rand() % 21;
        if (m->grid[treeR][treeC] == '.' || m->grid[treeR][treeC] == ':')
        {
            m->grid[treeR][treeC] = '^';
        }
    }

    // sporatic boulders
    int boulderC;
    int boulderR;
    for (i = 0; i < 40; i++)
    {
        boulderC = rand() % 80;
        boulderR = rand() % 21;
        if (m->grid[boulderR][boulderC] == '.' || m->grid[boulderR][boulderC] == ':')
        {
            m->grid[boulderR][boulderC] = '%';
        }
    }

    // create north south path
    int nGate;
    m->get_nGate(&nGate);
    if (!nGate)
    {
        nGate = rand() % 74 + 3;
        m->set_nGate(nGate);
    }
    int sGate;
    m->get_sGate(&sGate);
    if (!sGate)
    {
        sGate = rand() % 74 + 3;
        m->set_sGate(sGate);
    }
    int hPoint = rand() % 15 + 3;

    for (i = 0; i < hPoint + 1; i++)
    {
        m->grid[i][nGate] = '#';
    }

    for (i = hPoint; i < 21; i++)
    {
        m->grid[i][sGate] = '#';
    }

    if (nGate < sGate)
    {
        for (i = nGate; i < sGate; i++)
        {
            m->grid[hPoint][i] = '#';
        }
    }
    else
    {
        for (i = sGate; i < nGate; i++)
        {
            m->grid[hPoint][i] = '#';
        }
    }

    // create east west path
    int wGate;
    m->get_wGate(&wGate);
    if (!wGate)
    {
        wGate = rand() % 15 + 3;
        m->set_wGate(wGate);
    }
    int eGate;
    m->get_eGate(&eGate);
    if (!eGate)
    {
        eGate = rand() % 15 + 3;
        m->set_eGate(eGate);
    }
    int vPoint = rand() % 72 + 3;

    for (i = 0; i < vPoint + 1; i++)
    {
        m->grid[wGate][i] = '#';
    }

    for (i = vPoint; i < 80; i++)
    {
        m->grid[eGate][i] = '#';
    }

    if (wGate < eGate)
    {
        for (i = wGate; i < eGate; i++)
        {
            m->grid[i][vPoint] = '#';
        }
    }
    else
    {
        for (i = eGate; i < wGate; i++)
        {
            m->grid[i][vPoint] = '#';
        }
    }

    // get rid of edge gates
    if (yLoc == 0)
    {
        m->grid[0][nGate] = '%';
        m->grid[1][nGate] = '^';
    }
    else if (yLoc == 400)
    {
        m->grid[20][sGate] = '%';
        m->grid[19][sGate] = '^';
    }

    if (xLoc == 0)
    {
        m->grid[wGate][0] = '%';
        m->grid[wGate][1] = '^';
    }
    else if (xLoc == 400)
    {
        m->grid[eGate][79] = '%';
        m->grid[eGate][78] = '^';
    }

    // calculate distance from center
    int xDist = xLoc - 200;
    int yDist = yLoc - 200;
    xDist = abs(xDist);
    yDist = abs(yDist);

    int dist = xDist + yDist;
    int p;

    if ((xLoc == 200) && (yLoc == 200))
    {
        p = 100;
    }
    else if (dist < 200)
    {
        p = ((-45 * dist) / 200) + 50;
    }
    else
    {
        p = 5;
    }

    // add pokemon center
    if (rand() % 100 < p)
    {
        int centerCol = rand() % 72 + 4;
        while (m->grid[0][centerCol] == '#')
        {
            centerCol = rand() % 72 + 4;
        }
        int centerRow = 0;
        while (m->grid[centerRow][centerCol] != '#')
        {
            centerRow++;
        }
        m->grid[centerRow - 1][centerCol] = 'C';
        m->grid[centerRow - 2][centerCol] = 'C';
        if (m->grid[centerRow - 1][centerCol + 1] == '#')
        {
            m->grid[centerRow - 1][centerCol - 1] = 'C';
            m->grid[centerRow - 2][centerCol - 1] = 'C';
        }
        else
        {
            m->grid[centerRow - 1][centerCol + 1] = 'C';
            m->grid[centerRow - 2][centerCol + 1] = 'C';
        }
    }

    // add pokemart
    if (rand() % 100 < p)
    {
        int martCol = rand() % 72 + 4;
        while (m->grid[20][martCol] == '#')
        {
            martCol = rand() % 72 + 4;
        }
        int martRow = 20;
        while (m->grid[martRow][martCol] != '#')
        {
            martRow--;
        }
        m->grid[martRow + 1][martCol] = 'M';
        m->grid[martRow + 2][martCol] = 'M';
        if (m->grid[martRow + 1][martCol + 1] == '#')
        {
            m->grid[martRow + 1][martCol - 1] = 'M';
            m->grid[martRow + 2][martCol - 1] = 'M';
        }
        else
        {
            m->grid[martRow + 1][martCol + 1] = 'M';
            m->grid[martRow + 2][martCol + 1] = 'M';
        }
    }

    return 0;
}

int printTerrain(char grid[21][80], int x, int y, Character *charMap[21][80])
{
    erase();

    int color = 0;

    mvprintw(0, 0, "Map (%d, %d)", x - 200, y - 200);

    for (int i = 0; i < 21; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            if (charMap[i][j] != NULL)
            {
                if (charMap[i][j]->type == '@')
                    color = CYAN;
                else if (charMap[i][j]->defeated)
                    color = WHITE;
                else
                    color = RED;
                attron(COLOR_PAIR(color));
                mvaddch(i + 1, j, charMap[i][j]->type);
                attroff(COLOR_PAIR(color));
            }
            else
            {
                if (grid[i][j] == '%')
                    color = PURPLE;
                else if (grid[i][j] == '^')
                    color = YELLOW;
                else if (grid[i][j] == '#')
                    color = WHITE;
                else if (grid[i][j] == '~')
                    color = BLUE;
                else if (grid[i][j] == '.')
                    color = GREEN;
                else if (grid[i][j] == ':')
                    color = GREEN;
                else
                    color = RED;
                attron(COLOR_PAIR(color));
                mvaddch(i + 1, j, grid[i][j]);
                attroff(COLOR_PAIR(color));
            }
        }
    }

    refresh();

    return 0;
}

int pokemart(World *w)
{
    erase();
    addstr("Welcome to the Pok\u00e9mart!\n");
    attron(COLOR_PAIR(GREEN));
    mvprintw(3, 0, "All supplies restored!");
    attroff(COLOR_PAIR(GREEN));
    w->pc_bag->num_revives = 3;
    w->pc_bag->num_potions = 3;
    w->pc_bag->num_pokeballs = 6;

    mvaddstr(22, 0, "press < to go back to the map");
    refresh();
    while (getch() != '<')
        ;

    return -1;
}

void transfer_poke(Character *pc, World *w)
{
    size_t i;
    Pokemon *p;
    char choice;

    erase();
    mvprintw(0, 0, "Choose a pokemon to tranfer (you cannot undo this!)");
    mvprintw(21, 0, "Press < to go back");
    for (i = 0; i < w->user_pokemon.size(); i++)
    {
        p = w->user_pokemon.at(i);
        if (i == 9)
        {
            mvprintw(i + 3, 5, "[a] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
        }
        else if (i == 10)
        {
            mvprintw(i + 3, 5, "[b] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
        }
        else if (i == 11)
        {
            mvprintw(i + 3, 5, "[c] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
        }
        else
        {
            mvprintw(i + 3, 5, "[%ld] %s (%s) level: %d HP: %d/%d", i + 1, p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
        }
    }
    refresh();
    while (true)
    {
        choice = getch();
        switch (choice)
        {
        case '1':
            if (w->user_pokemon.size() < 1)
                break;
            p = w->user_pokemon.at(0);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '2':
            if (w->user_pokemon.size() < 2)
                break;
            p = w->user_pokemon.at(1);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '3':
            if (w->user_pokemon.size() < 3)
                break;
            p = w->user_pokemon.at(2);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '4':
            if (w->user_pokemon.size() < 4)
                break;
            p = w->user_pokemon.at(3);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }
            break;
        case '5':
            if (w->user_pokemon.size() < 5)
                break;
            p = w->user_pokemon.at(4);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '6':
            if (w->user_pokemon.size() < 6)
                break;
            p = w->user_pokemon.at(5);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '7':
            if (w->user_pokemon.size() < 7)
                break;
            p = w->user_pokemon.at(6);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '8':
            if (w->user_pokemon.size() < 8)
                break;
            p = w->user_pokemon.at(7);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '9':
            if (w->user_pokemon.size() < 9)
                break;
            p = w->user_pokemon.at(8);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case 'a':
            if (w->user_pokemon.size() < 10)
                break;
            p = w->user_pokemon.at(9);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case 'b':
            if (w->user_pokemon.size() < 11)
                break;
            p = w->user_pokemon.at(10);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case 'c':
            if (w->user_pokemon.size() < 12)
                break;
            p = w->user_pokemon.at(11);

            if (pc->c_pokemon.size() == 1)
            {
                mvprintw(19, 0, "Can't transfer your only pokemon!");
            }
            else
            {
                pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                w->user_pokemon.erase(std::remove(w->user_pokemon.begin(), w->user_pokemon.end(), p), w->user_pokemon.end());
                mvprintw(19, 0, "%s transfered", p->name.c_str());
            }

            break;
        case '<':
            return;
        }
        refresh();
    }
}

void choose_party(Character *pc, World *w)
{
    size_t i;
    Pokemon *p;
    bool in_party[12] = {false};
    char choice;
    erase();
    while (true)
    {
        erase();
        mvprintw(0, 0, "Choose a pokemon in the party to remove it. Choose a pokemon not in the party to add it.");
        mvprintw(1, 5, "Purple pokemon are currently the party!");
        mvprintw(2, 10, "Currently %d pokemon in party out of 6 spots", (int)pc->c_pokemon.size());
        mvprintw(21, 0, "press < to exit the pokemon center");
        mvprintw(20, 0, "Press t to choose a pokemon to transfer");
        for (i = 0; i < w->user_pokemon.size(); i++)
        {
            p = w->user_pokemon.at(i);
            if (std::find(pc->c_pokemon.begin(), pc->c_pokemon.end(), p) != pc->c_pokemon.end())
            {
                attron(COLOR_PAIR(PURPLE));
                if (i == 9)
                {
                    mvprintw(i + 5, 5, "[a] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else if (i == 10)
                {
                    mvprintw(i + 5, 5, "[b] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else if (i == 11)
                {
                    mvprintw(i + 5, 5, "[c] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else
                {
                    mvprintw(i + 5, 5, "[%ld] %s (%s) level: %d HP: %d/%d", i + 1, p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                attroff(COLOR_PAIR(PURPLE));
                in_party[i] = true;
            }
            else
            {
                if (i == 9)
                {
                    mvprintw(i + 5, 5, "[a] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else if (i == 10)
                {
                    mvprintw(i + 5, 5, "[b] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else if (i == 11)
                {
                    mvprintw(i + 5, 5, "[c] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else
                {
                    mvprintw(i + 5, 5, "[%ld] %s (%s) level: %d HP: %d/%d", i + 1, p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
            }
        }
        refresh();
        choice = getch();
        move(20, 0);
        clrtoeol();
        switch (choice)
        {
        case '1':
            if (w->user_pokemon.size() < 1)
                break;
            p = w->user_pokemon.at(0);
            if (in_party[0])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '2':
            if (w->user_pokemon.size() < 2)
                break;
            p = w->user_pokemon.at(1);
            if (in_party[1])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '3':
            if (w->user_pokemon.size() < 3)
                break;
            p = w->user_pokemon.at(2);
            if (in_party[2])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '4':
            if (w->user_pokemon.size() < 4)
                break;
            p = w->user_pokemon.at(3);
            if (in_party[3])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '5':
            if (w->user_pokemon.size() < 5)
                break;
            p = w->user_pokemon.at(4);
            if (in_party[4])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '6':
            if (w->user_pokemon.size() < 6)
                break;
            p = w->user_pokemon.at(5);
            if (in_party[5])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '7':
            if (w->user_pokemon.size() < 7)
                break;
            p = w->user_pokemon.at(6);
            if (in_party[6])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '8':
            if (w->user_pokemon.size() < 8)
                break;
            p = w->user_pokemon.at(7);
            if (in_party[7])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '9':
            if (w->user_pokemon.size() < 9)
                break;
            p = w->user_pokemon.at(8);
            if (in_party[8])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case 'a':
            if (w->user_pokemon.size() < 10)
                break;
            p = w->user_pokemon.at(9);
            if (in_party[9])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case 'b':
            if (w->user_pokemon.size() < 11)
                break;
            p = w->user_pokemon.at(10);
            if (in_party[10])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case 'c':
            if (w->user_pokemon.size() < 12)
                break;
            p = w->user_pokemon.at(11);
            if (in_party[11])
            {
                if (pc->c_pokemon.size() == 1)
                {
                    mvprintw(19, 0, "Can't remove the only pokemon in the party!");
                }
                else
                {
                    pc->c_pokemon.erase(std::remove(pc->c_pokemon.begin(), pc->c_pokemon.end(), p), pc->c_pokemon.end());
                    mvprintw(19, 0, "%s removed from party", p->name.c_str());
                }
            }
            else
            {
                if (pc->c_pokemon.size() == 6)
                {
                    mvprintw(19, 0, "Party is full!");
                }
                else
                {
                    pc->c_pokemon.push_back(p);
                    mvprintw(19, 0, "%s added to party!", p->name.c_str());
                }
            }
            refresh();
            break;
        case '<':
            return;
        case 't':
            transfer_poke(pc, w);
            break;
        }
        for (int j = 0; j < 12; j++)
        {
            in_party[j] = false;
        }
        refresh();
    }
}

moves *find_move(pokemon_moves *pm, Data *d)
{
    if (d->vmoves.at(pm->move_id - 1)->id != pm->move_id)
    {
        fprintf(stderr, "correct move not found\n");
        return NULL;
    }

    return d->vmoves.at(pm->move_id);
}

void new_move(Pokemon *p, Data *d)
{
    std::vector<moves *> level_moves;

    for (pokemon_moves *pm : p->moveset)
    {
        if (pm->level == p->level)
        {
            level_moves.push_back(find_move(pm, d));
        }
    }

    if (level_moves.size() == 0)
    {
        erase();
        mvprintw(0, 0, "%s has no new moves for level %d", p->name.c_str(), p->level);
        mvprintw(20, 0, "Press any key to return");
        refresh();
        getch();
        return;
    }

    erase();
    mvprintw(0, 0, "Here are moves for %s for level %d. Choose one to learn it!", p->name.c_str(), p->level);
    mvprintw(20, 0, "Press < to return");

    moves *m;
    type_names *type;
    for (size_t i = 0; i < level_moves.size(); i++)
    {
        m = level_moves.at(i);
        type = d->vtype_names.at(m->type_id - 1);
        mvprintw(i + 2, 5, "[%d] %s (%s)", (int)i + 1, m->identifier.c_str(), type->name.c_str());
    }
    refresh();

    char move = getch();
    int index = move - '0' - 1;
    if (move == '<')
    {
        return;
    }
    while (index >= (int)level_moves.size() && index < 0)
    {
        move = getch();
        index = move - '0' - 1;
        if (move == '<')
        {
            return;
        }
    }

    moves *new_move = level_moves.at(index);

    clear();
    mvprintw(0, 0, "Learning %s", new_move->identifier.c_str());
    mvprintw(1, 0, "Choose a move to replace it with. This move will be forgotten and this cannpt be undone!");
    mvprintw(3, 5, "[1] %s", p->chosen_moves.at(0)->identifier.c_str());
    if (p->chosen_moves.size() == 2)
    {
        mvprintw(4, 5, "[2] %s", p->chosen_moves.at(1)->identifier.c_str());
    }

    char choice = getch();
    bool input = false;
    moves *old_move;
    while (!input)
    {
        switch (choice)
        {
        case '1':
            old_move = p->chosen_moves.at(0);
            p->chosen_moves.erase(p->chosen_moves.begin());
            p->chosen_moves.push_back(new_move);
            input = true;
            break;
        case '2':
            old_move = p->chosen_moves.at(1);
            p->chosen_moves.pop_back();
            p->chosen_moves.push_back(new_move);
            input = true;
            break;
        }
    }
    mvprintw(10, 0, "%s forgotten and %s learned", old_move->identifier.c_str(), new_move->identifier.c_str());
    refresh();
    napms(2000);
}

int pokecenter(Character *pc, World *w, Data *d)
{
    erase();
    addstr("Welcome to the Pok\u00e9mon Center!\n");
    refresh();
    napms(2000);

    size_t i;
    Pokemon *p = pc->c_pokemon.at(0);
    attron(COLOR_PAIR(GREEN));
    for (i = 0; i < pc->c_pokemon.size(); i++)
    {
        p = pc->c_pokemon.at(i);
        if (p->cur_hp < p->max_hp)
        {
            p->cur_hp = p->max_hp;
            mvprintw(3 + i, 0, "%s fully healed", p->name.c_str());
            refresh();
            napms(1000);
        }
    }
    attroff(COLOR_PAIR(GREEN));

    mvprintw(10, 0, "All pok\u00e9mon are fully healed!");

    mvprintw(13, 0, "press p to swap the pokemon in your party");
    mvprintw(14, 0, "press m to check for new moves");

    mvaddstr(22, 0, "press < to go back to the map");
    refresh();
    char opt;
    char poke;
    int index;
    Pokemon *move_p;
    bool input = false;
    while (true)
    {
        opt = getch();
        switch (opt)
        {
        case 'p':
            choose_party(pc, w);
            return 0;
        case 'm':
            erase();
            mvprintw(0, 0, "Choose a pokemon to check for new moves");
            mvprintw(21, 0, "Press < to go back");
            for (i = 0; i < w->user_pokemon.size(); i++)
            {
                p = w->user_pokemon.at(i);
                if (i == 9)
                {
                    mvprintw(i + 3, 5, "[a] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else if (i == 10)
                {
                    mvprintw(i + 3, 5, "[b] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else if (i == 11)
                {
                    mvprintw(i + 3, 5, "[c] %s (%s) level: %d HP: %d/%d", p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
                else
                {
                    mvprintw(i + 3, 5, "[%ld] %s (%s) level: %d HP: %d/%d", i + 1, p->name.c_str(), p->p_types[0]->name.c_str(), p->level, p->cur_hp, p->max_hp);
                }
            }
            refresh();
            while (!input)
            {
                poke = getch();
                if (poke == 'a')
                    index = 9;
                else if (poke == 'b')
                    index = 10;
                else if (poke == 'b')
                    index = 11;
                else
                {
                    index = poke - '0' - 1;
                }
                if (index < (int)w->user_pokemon.size())
                {
                    move_p = w->user_pokemon.at(index);
                    new_move(move_p, d);
                    input = true;
                }
            }
            return 0;
        case '<':
            return 0;
        }
    }

    return -1;
}