#include <cstdlib>
#include <cmath>
#include <climits>
#include <cstdio>
#include <ncurses.h>

#include "character.h"
#include "heap.h"
#include "world.h"

int printDist(int grid[21][80])
{
    printf("     ");
    for (int j = 0; j < 80; j++)
        printf(" %2d", j);
    printf("\n");

    for (int i = 0; i < 21; i++)
    {
        printf("%3d |", i);
        for (int j = 0; j < 80; j++)
        {
            if (grid[i][j] > 1000)
                printf(" --");
            else
                printf(" %02d", grid[i][j] % 100);
        }
        printf("\n");
    }

    return 0;
}

int calc_cost_hiker(Map *m, int row, int col)
{
    char terrain = m->grid[row][col];
    if (row == 0 || row == 20 || col == 0 || col == 79)
    {
        return INT_MAX;
    }
    if (terrain == '#')
    {
        return 10;
    }
    if (terrain == 'M')
    {
        return 50;
    }
    if (terrain == 'C')
    {
        return 50;
    }
    if (terrain == ':')
    {
        return 15;
    }
    if (terrain == '.')
    {
        return 10;
    }
    if (terrain == '%')
    {
        if (m->grid[row - 1][col] != '%' &&
            m->grid[row + 1][col] != '%' &&
            m->grid[row][col - 1] != '%' &&
            m->grid[row][col + 1] != '%')
        {
            return INT_MAX;
        }
        return 15;
    }
    if (terrain == '^')
    {
        if (m->grid[row - 1][col] != '^' &&
            m->grid[row + 1][col] != '^' &&
            m->grid[row][col - 1] != '^' &&
            m->grid[row][col + 1] != '^')
        {
            return INT_MAX;
        }
        return 15;
    }
    if (terrain == '~')
    {
        return INT_MAX;
    }
    printf(" %c \n", terrain);
    return 0;
}

int calc_cost_rival(Map *m, int row, int col)
{
    char terrain = m->grid[row][col];
    if (row == 0 || row == 20 || col == 0 || col == 79)
    {
        return INT_MAX;
    }
    if (terrain == '#')
    {
        return 10;
    }
    if (terrain == 'M')
    {
        return 50;
    }
    if (terrain == 'C')
    {
        return 50;
    }
    if (terrain == ':')
    {
        return 20;
    }
    if (terrain == '.')
    {
        return 10;
    }
    if (terrain == '%')
    {
        return INT_MAX;
    }
    if (terrain == '^')
    {
        return INT_MAX;
    }
    if (terrain == '~')
    {
        return INT_MAX;
    }
    printf(" %c \n", terrain);
    return 0;
}

int Character::shortestPathHiker(Map *m, World *w)
{
    Heap *h = new Heap();
    heap_node nodes[21][80];
    heap_node *min;
    int visited[21][80] = {0};
    int in_heap[21][80] = {0};
    int cost;
    int i;
    int j;

    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            nodes[i][j].row = i;
            nodes[i][j].col = j;
            nodes[i][j].dist = INT_MAX;
        }
    }

    nodes[m->pc_row][m->pc_col].row = m->pc_row;
    nodes[m->pc_row][m->pc_col].col = m->pc_col;
    nodes[m->pc_row][m->pc_col].dist = 0;
    h->heap_insert(&nodes[m->pc_row][m->pc_col]);

    while (h->heap_peek_min() != NULL)
    {
        min = h->heap_remove_min();
        i = min->row;
        j = min->col;
        visited[i][j] = 1;
        w->hikerMap[i][j] = min->dist;
        // top left
        if (i > 0 && j > 0)
        {
            if (!visited[i - 1][j - 1])
            {
                cost = calc_cost_hiker(m, nodes[i - 1][j - 1].row, nodes[i - 1][j - 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i - 1, j - 1);
                    return -1;
                }
                if (!in_heap[i - 1][j - 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i - 1][j - 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i - 1][j - 1]);
                        in_heap[i - 1][j - 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i - 1][j - 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i - 1][j - 1], min->dist + cost);
                    }
                }
            }
        }
        // top
        if (i > 0)
        {
            if (!visited[i - 1][j])
            {
                cost = calc_cost_hiker(m, nodes[i - 1][j].row, nodes[i - 1][j].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i - 1, j);
                    return -1;
                }
                if (!in_heap[i - 1][j])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i - 1][j].dist = min->dist + cost;
                        h->heap_insert(&nodes[i - 1][j]);
                        in_heap[i - 1][j] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i - 1][j].dist)
                    {
                        h->heap_decrease_dist(&nodes[i - 1][j], min->dist + cost);
                    }
                }
            }
        }
        // top right
        if (i > 0 && j < 79)
        {
            if (!visited[i - 1][j + 1])
            {
                cost = calc_cost_hiker(m, nodes[i - 1][j + 1].row, nodes[i - 1][j + 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i - 1, j + 1);
                    return -1;
                }
                if (!in_heap[i - 1][j + 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i - 1][j + 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i - 1][j + 1]);
                        in_heap[i - 1][j + 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i - 1][j + 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i - 1][j + 1], min->dist + cost);
                    }
                }
            }
        }
        // left
        if (j > 0)
        {
            if (!visited[i][j - 1])
            {
                cost = calc_cost_hiker(m, nodes[i][j - 1].row, nodes[i][j - 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i, j - 1);
                    return -1;
                }
                if (!in_heap[i][j - 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i][j - 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i][j - 1]);
                        in_heap[i][j - 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i][j - 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i][j - 1], min->dist + cost);
                    }
                }
            }
        }
        // right
        if (j < 79)
        {
            if (!visited[i][j + 1])
            {
                cost = calc_cost_hiker(m, nodes[i][j + 1].row, nodes[i][j + 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i, j + 1);
                    return -1;
                }
                if (!in_heap[i][j + 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i][j + 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i][j + 1]);
                        in_heap[i][j + 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i][j + 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i][j + 1], min->dist + cost);
                    }
                }
            }
        }
        // bottom left
        if (i < 20 && j > 0)
        {
            if (!visited[i + 1][j - 1])
            {
                cost = calc_cost_hiker(m, nodes[i + 1][j - 1].row, nodes[i + 1][j - 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i + 1, j - 1);
                    return -1;
                }
                if (!in_heap[i + 1][j - 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i + 1][j - 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i + 1][j - 1]);
                        in_heap[i + 1][j - 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i + 1][j - 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i + 1][j - 1], min->dist + cost);
                    }
                }
            }
        }
        // bottom
        if (i < 20)
        {
            if (!visited[i + 1][j])
            {
                cost = calc_cost_hiker(m, nodes[i + 1][j].row, nodes[i + 1][j].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i + 1, j);
                    return -1;
                }
                if (!in_heap[i + 1][j])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i + 1][j].dist = min->dist + cost;
                        h->heap_insert(&nodes[i + 1][j]);
                        in_heap[i + 1][j] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i + 1][j].dist)
                    {
                        h->heap_decrease_dist(&nodes[i + 1][j], min->dist + cost);
                    }
                }
            }
        }
        // bottom right
        if (i < 20 && j < 79)
        {
            if (!visited[i + 1][j + 1])
            {
                cost = calc_cost_hiker(m, nodes[i + 1][j + 1].row, nodes[i + 1][j + 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0\n", i + 1, j + 1);
                    return -1;
                }
                if (!in_heap[i + 1][j + 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i + 1][j + 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i + 1][j + 1]);
                        in_heap[i + 1][j + 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i + 1][j + 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i + 1][j + 1], min->dist + cost);
                    }
                }
            }
        }
    }

    delete (h);

    return 0;
}

int Character::shortestPathRival(Map *m, World *w)
{
    Heap *h = new Heap();
    heap_node nodes[21][80];
    heap_node *min;
    int visited[21][80] = {0};
    int in_heap[21][80] = {0};
    int cost;
    int i;
    int j;

    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            nodes[i][j].row = i;
            nodes[i][j].col = j;
            nodes[i][j].dist = INT_MAX;
        }
    }

    nodes[m->pc_row][m->pc_col].row = m->pc_row;
    nodes[m->pc_row][m->pc_col].col = m->pc_col;
    nodes[m->pc_row][m->pc_col].dist = 0;
    h->heap_insert(&nodes[m->pc_row][m->pc_col]);

    while (h->heap_peek_min() != NULL)
    {
        min = h->heap_remove_min();
        i = min->row;
        j = min->col;
        visited[i][j] = 1;
        w->rivalMap[i][j] = min->dist;
        // top left
        if (i > 0 && j > 0)
        {
            if (!visited[i - 1][j - 1])
            {
                cost = calc_cost_rival(m, nodes[i - 1][j - 1].row, nodes[i - 1][j - 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i - 1, j - 1);
                    return -1;
                }
                if (!in_heap[i - 1][j - 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i - 1][j - 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i - 1][j - 1]);
                        in_heap[i - 1][j - 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i - 1][j - 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i - 1][j - 1], min->dist + cost);
                    }
                }
            }
        }
        // top
        if (i > 0)
        {
            if (!visited[i - 1][j])
            {
                cost = calc_cost_rival(m, nodes[i - 1][j].row, nodes[i - 1][j].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i - 1, j);
                    return -1;
                }
                if (!in_heap[i - 1][j])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i - 1][j].dist = min->dist + cost;
                        h->heap_insert(&nodes[i - 1][j]);
                        in_heap[i - 1][j] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i - 1][j].dist)
                    {
                        h->heap_decrease_dist(&nodes[i - 1][j], min->dist + cost);
                    }
                }
            }
        }
        // top right
        if (i > 0 && j < 79)
        {
            if (!visited[i - 1][j + 1])
            {
                cost = calc_cost_rival(m, nodes[i - 1][j + 1].row, nodes[i - 1][j + 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i - 1, j + 1);
                    return -1;
                }
                if (!in_heap[i - 1][j + 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i - 1][j + 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i - 1][j + 1]);
                        in_heap[i - 1][j + 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i - 1][j + 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i - 1][j + 1], min->dist + cost);
                    }
                }
            }
        }
        // left
        if (j > 0)
        {
            if (!visited[i][j - 1])
            {
                cost = calc_cost_rival(m, nodes[i][j - 1].row, nodes[i][j - 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i, j - 1);
                    return -1;
                }
                if (!in_heap[i][j - 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i][j - 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i][j - 1]);
                        in_heap[i][j - 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i][j - 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i][j - 1], min->dist + cost);
                    }
                }
            }
        }
        // right
        if (j < 79)
        {
            if (!visited[i][j + 1])
            {
                cost = calc_cost_rival(m, nodes[i][j + 1].row, nodes[i][j + 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i, j + 1);
                    return -1;
                }
                if (!in_heap[i][j + 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i][j + 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i][j + 1]);
                        in_heap[i][j + 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i][j + 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i][j + 1], min->dist + cost);
                    }
                }
            }
        }
        // bottom left
        if (i < 20 && j > 0)
        {
            if (!visited[i + 1][j - 1])
            {
                cost = calc_cost_rival(m, nodes[i + 1][j - 1].row, nodes[i + 1][j - 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i + 1, j - 1);
                    return -1;
                }
                if (!in_heap[i + 1][j - 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i + 1][j - 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i + 1][j - 1]);
                        in_heap[i + 1][j - 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i + 1][j - 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i + 1][j - 1], min->dist + cost);
                    }
                }
            }
        }
        // bottom
        if (i < 20)
        {
            if (!visited[i + 1][j])
            {
                cost = calc_cost_rival(m, nodes[i + 1][j].row, nodes[i + 1][j].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0", i + 1, j);
                    return -1;
                }
                if (!in_heap[i + 1][j])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i + 1][j].dist = min->dist + cost;
                        h->heap_insert(&nodes[i + 1][j]);
                        in_heap[i + 1][j] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i + 1][j].dist)
                    {
                        h->heap_decrease_dist(&nodes[i + 1][j], min->dist + cost);
                    }
                }
            }
        }
        // bottom right
        if (i < 20 && j < 79)
        {
            if (!visited[i + 1][j + 1])
            {
                cost = calc_cost_rival(m, nodes[i + 1][j + 1].row, nodes[i + 1][j + 1].col);
                if (cost == 0)
                {
                    printf("Cost of row: %d, col: %d returned 0\n", i + 1, j + 1);
                    return -1;
                }
                if (!in_heap[i + 1][j + 1])
                {
                    if (cost != INT_MAX)
                    {
                        nodes[i + 1][j + 1].dist = min->dist + cost;
                        h->heap_insert(&nodes[i + 1][j + 1]);
                        in_heap[i + 1][j + 1] = 1;
                    }
                }
                else
                {
                    if (cost != INT_MAX && min->dist + cost < nodes[i + 1][j + 1].dist)
                    {
                        h->heap_decrease_dist(&nodes[i + 1][j + 1], min->dist + cost);
                    }
                }
            }
        }
    }

    delete (h);

    return 0;
}

int Character::pc_init(Map *m, World *w)
{
    int start;
    int i;
    while (1)
    {
        if (rand() % 2 == 0)
        {
            start = (rand() % 76) + 2;
            for (i = 1; i < 19; i++)
            {
                if (m->grid[i][start] == '#')
                {
                    row = i;
                    col = start;
                    break;
                }
            }
        }
        else
        {
            start = (rand() % 17) + 2;
            for (i = 1; i < 79; i++)
            {
                if (m->grid[start][i] == '#')
                {
                    row = start;
                    col = i;
                    break;
                }
            }
        }
        if (row != 0 && col != 0)
        {
            break;
        }
    }
    priority = 1;

    shortestPathHiker(m, w);
    shortestPathRival(m, w);

    return 0;
}

int Character::rival_init(Map *m, World *w)
{
    int r, c;

    while (1)
    {
        r = (rand() % 19) + 1;
        c = (rand() % 78) + 1;
        if (w->rivalMap[r][c] != INT_MAX && m->charMap[r][c] == NULL)
        {
            break;
        }
    }
    row = r;
    col = c;
    priority = 2;
    return 0;
}

int Character::hiker_init(Map *m, World *w)
{
    int r, c;

    while (1)
    {
        r = (rand() % 19) + 1;
        c = (rand() % 78) + 1;
        if (w->hikerMap[r][c] != INT_MAX && m->charMap[r][c] == NULL)
        {
            break;
        }
    }
    row = r;
    col = c;
    priority = 3;
    return 0;
}

int Character::pacer_init(Map *m, World *w)
{
    int r, c;
    int num = rand() % 4;

    if (num == 0)
    {
        dir = 'n';
    }
    else if (num == 1)
    {
        dir = 'w';
    }
    else if (num == 2)
    {
        dir = 's';
    }
    else
    {
        dir = 'e';
    }

    while (1)
    {
        r = (rand() % 19) + 1;
        c = (rand() % 78) + 1;
        if (m->charMap[r][c] == NULL &&
            m->grid[r][c] != '%' &&
            m->grid[r][c] != '^' &&
            m->grid[r][c] != '~')
        {
            break;
        }
    }
    row = r;
    col = c;
    priority = 4;

    return 0;
}

int Character::wanderer_init(Map *m, World *w)
{
    int r, c;
    int num = rand() % 4;

    if (num == 0)
    {
        dir = 'n';
    }
    else if (num == 1)
    {
        dir = 'w';
    }
    else if (num == 2)
    {
        dir = 's';
    }
    else
    {
        dir = 'e';
    }

    while (1)
    {
        r = (rand() % 19) + 1;
        c = (rand() % 78) + 1;
        if (m->charMap[r][c] == NULL &&
            m->grid[r][c] != '%' &&
            m->grid[r][c] != '^' &&
            m->grid[r][c] != '~')
        {
            break;
        }
    }
    row = r;
    col = c;
    priority = 5;

    return 0;
}

int Character::sentry_init(Map *m, World *w)
{
    int r, c;
    while (1)
    {
        r = (rand() % 19) + 1;
        c = (rand() % 78) + 1;
        if (m->charMap[r][c] == NULL &&
            m->grid[r][c] != '%' &&
            m->grid[r][c] != '^' &&
            m->grid[r][c] != '~')
        {
            break;
        }
    }
    row = r;
    col = c;
    priority = 0;

    return 0;
}

int Character::explorer_init(Map *m, World *w)
{
    int r, c;
    int num = rand() % 4;

    if (num == 0)
    {
        dir = 'n';
    }
    else if (num == 1)
    {
        dir = 'w';
    }
    else if (num == 2)
    {
        dir = 's';
    }
    else
    {
        dir = 'e';
    }

    while (1)
    {
        r = (rand() % 19) + 1;
        c = (rand() % 78) + 1;
        if (m->charMap[r][c] == NULL &&
            m->grid[r][c] != '%' &&
            m->grid[r][c] != '^' &&
            m->grid[r][c] != '~')
        {
            break;
        }
    }
    row = r;
    col = c;
    priority = 6;

    return 0;
}

Character::Character(char type, Map *m, World *w, Data *d)
{
    int i;
    int distance;

    if (type == '@')
    {
        pc_init(m, w);
        m->pc_row = row;
        m->pc_col = col;
    }
    else if (type == 'r')
    {
        rival_init(m, w);
    }
    else if (type == 'h')
    {
        hiker_init(m, w);
    }
    else if (type == 'p')
    {
        pacer_init(m, w);
    }
    else if (type == 'w')
    {
        wanderer_init(m, w);
    }
    else if (type == 's')
    {
        sentry_init(m, w);
    }
    else if (type == 'e')
    {
        explorer_init(m, w);
    }
    this->type = type;
    m->charMap[row][col] = this;
    nextCol = col;
    nextRow = row;
    defeated = 0;

    distance = abs(m->xLoc - 200) + abs(m->yLoc - 200);

    if (type != '@')
    {
        Pokemon *p1 = new Pokemon(distance, d);
        c_pokemon.push_back(p1);
        for (i = 0; i < 5 && (rand() % 10) < 6; i++)
        {
            Pokemon *p = new Pokemon(distance, d);
            c_pokemon.push_back(p);
        }
    }
}

int Character::rival_next_move(Map *m, World *w, int *cost)
{
    int nr = row;
    int nc = col;
    int min = INT_MAX;

    if (w->rivalMap[row - 1][col - 1] < min &&
        (m->charMap[row - 1][col - 1] == NULL || m->charMap[row - 1][col - 1]->type == '@'))
    {
        min = w->rivalMap[row - 1][col - 1];
        nr = row - 1;
        nc = col - 1;
    }
    if (w->rivalMap[row - 1][col] < min &&
        (m->charMap[row - 1][col] == NULL || m->charMap[row - 1][col]->type == '@'))
    {
        min = w->rivalMap[row - 1][col];
        nr = row - 1;
        nc = col;
    }
    if (w->rivalMap[row - 1][col + 1] < min &&
        (m->charMap[row - 1][col + 1] == NULL || m->charMap[row - 1][col + 1]->type == '@'))
    {
        min = w->rivalMap[row - 1][col + 1];
        nr = row - 1;
        nc = col + 1;
    }
    if (w->rivalMap[row][col - 1] < min &&
        (m->charMap[row][col - 1] == NULL || m->charMap[row][col - 1]->type == '@'))
    {
        min = w->rivalMap[row][col - 1];
        nr = row;
        nc = col - 1;
    }
    if (w->rivalMap[row][col + 1] < min &&
        (m->charMap[row][col + 1] == NULL || m->charMap[row][col + 1]->type == '@'))
    {
        min = w->rivalMap[row][col + 1];
        nr = row;
        nc = col + 1;
    }
    if (w->rivalMap[row + 1][col - 1] < min &&
        (m->charMap[row + 1][col - 1] == NULL || m->charMap[row + 1][col - 1]->type == '@'))
    {
        min = w->rivalMap[row + 1][col - 1];
        nr = row + 1;
        nc = col - 1;
    }
    if (w->rivalMap[row + 1][col] < min &&
        (m->charMap[row + 1][col] == NULL || m->charMap[row + 1][col]->type == '@'))
    {
        min = w->rivalMap[row + 1][col];
        nr = row + 1;
        nc = col;
    }
    if (w->rivalMap[row + 1][col + 1] < min &&
        (m->charMap[row + 1][col + 1] == NULL || m->charMap[row + 1][col + 1]->type == '@'))
    {
        min = w->rivalMap[row + 1][col + 1];
        nr = row + 1;
        nc = col + 1;
    }

    nextRow = nr;
    nextCol = nc;

    *cost = calc_cost_rival(m, nr, nc);

    return 0;
}

int Character::hiker_next_move(Map *m, World *w, int *cost)
{
    int nr = row;
    int nc = col;
    int min = INT_MAX;

    if (w->hikerMap[row - 1][col - 1] < min &&
        (m->charMap[row - 1][col - 1] == NULL || m->charMap[row - 1][col - 1]->type == '@'))
    {
        min = w->hikerMap[row - 1][col - 1];
        nr = row - 1;
        nc = col - 1;
    }
    if (w->hikerMap[row - 1][col] < min &&
        (m->charMap[row - 1][col] == NULL || m->charMap[row - 1][col]->type == '@'))
    {
        min = w->hikerMap[row - 1][col];
        nr = row - 1;
        nc = col;
    }
    if (w->hikerMap[row - 1][col + 1] < min &&
        (m->charMap[row - 1][col + 1] == NULL || m->charMap[row - 1][col + 1]->type == '@'))
    {
        min = w->hikerMap[row - 1][col + 1];
        nr = row - 1;
        nc = col + 1;
    }
    if (w->hikerMap[row][col - 1] < min &&
        (m->charMap[row][col - 1] == NULL || m->charMap[row][col - 1]->type == '@'))
    {
        min = w->hikerMap[row][col - 1];
        nr = row;
        nc = col - 1;
    }
    if (w->hikerMap[row][col + 1] < min &&
        (m->charMap[row][col + 1] == NULL || m->charMap[row][col + 1]->type == '@'))
    {
        min = w->hikerMap[row][col + 1];
        nr = row;
        nc = col + 1;
    }
    if (w->hikerMap[row + 1][col - 1] < min &&
        (m->charMap[row + 1][col - 1] == NULL || m->charMap[row + 1][col - 1]->type == '@'))
    {
        min = w->hikerMap[row + 1][col - 1];
        nr = row + 1;
        nc = col - 1;
    }
    if (w->hikerMap[row + 1][col] < min &&
        (m->charMap[row + 1][col] == NULL || m->charMap[row + 1][col]->type == '@'))
    {
        min = w->hikerMap[row + 1][col];
        nr = row + 1;
        nc = col;
    }
    if (w->hikerMap[row + 1][col + 1] < min &&
        (m->charMap[row + 1][col + 1] == NULL || m->charMap[row + 1][col + 1]->type == '@'))
    {
        min = w->hikerMap[row + 1][col + 1];
        nr = row + 1;
        nc = col + 1;
    }

    nextRow = nr;
    nextCol = nc;

    *cost = calc_cost_hiker(m, nr, nc);

    return 0;
}

int Character::defeated_rival_next_move(Map *m, World *w, int *cost)
{

    // move like an explorer until they are in tall grass, then move like a wanderer
    int nr, nc;
    int num;
    int i;
    char terrain = m->grid[row][col];

    for (i = 0; i < 20; i++)
    {
        if (dir == 'n')
        {
            nr = row - 1;
            nc = col;
        }
        else if (dir == 's')
        {
            nr = row + 1;
            nc = col;
        }
        else if (dir == 'w')
        {
            nr = row;
            nc = col - 1;
        }
        else
        {
            nr = row;
            nc = col + 1;
        }
        if (nr > 0 &&
            nr < 20 &&
            nc > 0 &&
            nc < 79 &&
            (m->charMap[nr][nc] == NULL) &&
            m->grid[nr][nc] != '%' &&
            m->grid[nr][nc] != '^' &&
            m->grid[nr][nc] != '~' &&
            ((m->grid[nr][nc] == ':' && m->grid[row][col] == ':') ||
             (m->grid[row][col] != ':')))
        {
            break;
        }
        num = rand() % 4;
        if (num == 0)
        {
            dir = 'n';
        }
        else if (num == 1)
        {
            dir = 's';
        }
        else if (num == 2)
        {
            dir = 'e';
        }
        else
        {
            dir = 'w';
        }
    }
    if (i == 20)
    {
        nr = row;
        nc = col;
    }

    nextRow = nr;
    nextCol = nc;

    if (terrain == '#')
    {
        *cost = 10;
    }
    else if (terrain == 'M')
    {
        *cost = 50;
    }
    else if (terrain == 'C')
    {
        *cost = 50;
    }
    else if (terrain == ':')
    {
        *cost = 20;
    }
    else if (terrain == '.')
    {
        *cost = 10;
    }
    else
    {
        fprintf(stderr, "defeated_rival_next_move fail");
        return -1;
    }

    return 0;
}

int Character::defeated_hiker_next_move(Map *m, World *w, int *cost)
{
    // move like an explorer until they are in tall grass, then move like a wanderer
    int nr, nc;
    int num;
    int i;

    for (i = 0; i < 20; i++)
    {
        if (dir == 'n')
        {
            nr = row - 1;
            nc = col;
        }
        else if (dir == 's')
        {
            nr = row + 1;
            nc = col;
        }
        else if (dir == 'w')
        {
            nr = row;
            nc = col - 1;
        }
        else
        {
            nr = row;
            nc = col + 1;
        }

        if (nr > 0 &&
            nr < 20 &&
            nc > 0 &&
            nc < 79 &&
            (m->charMap[nr][nc] == NULL) &&
            calc_cost_hiker(m, nr, nc) != INT_MAX &&
            ((m->grid[nr][nc] == ':' && m->grid[row][col] == ':') ||
             (m->grid[row][col] != ':')))
        {
            break;
        }
        num = rand() % 4;
        if (num == 0)
        {
            dir = 'n';
        }
        else if (num == 1)
        {
            dir = 's';
        }
        else if (num == 2)
        {
            dir = 'e';
        }
        else
        {
            dir = 'w';
        }
    }
    if (i == 20)
    {
        nr = row;
        nc = col;
    }

    nextRow = nr;
    nextCol = nc;

    *cost = calc_cost_hiker(m, nr, nc);

    if (*cost == INT_MAX)
    {
        fprintf(stderr, "defeated hiker next move failed\n");
        return -1;
    }

    return 0;
}

int Character::pacer_next_move(Map *m, World *w, int *cost)
{
    int nr, nc;
    if (dir == 'n')
    {
        nr = row - 1;
        nc = col;
    }
    else if (dir == 's')
    {
        nr = row + 1;
        nc = col;
    }
    else if (dir == 'w')
    {
        nr = row;
        nc = col - 1;
    }
    else
    {
        nr = row;
        nc = col + 1;
    }

    if (nr < 1 || nr > 19 || nc < 1 || nc > 78 ||
        (m->charMap[nr][nc] != NULL && m->charMap[nr][nc]->type != '@') ||
        m->grid[nr][nc] == '%' ||
        m->grid[nr][nc] == '^' ||
        m->grid[nr][nc] == '~')
    {
        if (dir == 'n')
        {
            dir = 's';
        }
        else if (dir == 's')
        {
            dir = 'n';
        }
        else if (dir == 'w')
        {
            dir = 'e';
        }
        else
        {
            dir = 'w';
        }
        nr = row;
        nc = col;
    }

    nextRow = nr;
    nextCol = nc;

    if (m->grid[nr][nc] == '#')
    {
        *cost = 10;
    }
    else if (m->grid[nr][nc] == 'M')
    {
        *cost = 50;
    }
    else if (m->grid[nr][nc] == 'C')
    {
        *cost = 50;
    }
    else if (m->grid[nr][nc] == ':')
    {
        *cost = 20;
    }
    else if (m->grid[nr][nc] == '.')
    {
        *cost = 10;
    }
    else
    {
        fprintf(stderr, "pacer_next_move fail");
        return -1;
    }

    return 0;
}

int Character::wanderer_next_move(Map *m, World *w, int *cost)
{
    int nr, nc;
    int num;
    char terrain = m->grid[row][col];
    int i;

    for (i = 0; i < 20; i++)
    {
        if (dir == 'n')
        {
            nr = row - 1;
            nc = col;
        }
        else if (dir == 's')
        {
            nr = row + 1;
            nc = col;
        }
        else if (dir == 'w')
        {
            nr = row;
            nc = col - 1;
        }
        else
        {
            nr = row;
            nc = col + 1;
        }
        if (nr > 0 &&
            nr < 20 &&
            nc > 0 &&
            nc < 79 &&
            (m->charMap[nr][nc] == NULL || m->charMap[nr][nc]->type == '@') &&
            m->grid[nr][nc] == terrain)
        {
            break;
        }
        num = rand() % 4;
        if (num == 0)
        {
            dir = 'n';
        }
        else if (num == 1)
        {
            dir = 's';
        }
        else if (num == 2)
        {
            dir = 'e';
        }
        else
        {
            dir = 'w';
        }
    }
    if (i == 20)
    {
        nr = row;
        nc = col;
    }

    nextRow = nr;
    nextCol = nc;

    if (terrain == '#')
    {
        *cost = 10;
    }
    else if (terrain == 'M')
    {
        *cost = 50;
    }
    else if (terrain == 'C')
    {
        *cost = 50;
    }
    else if (terrain == ':')
    {
        *cost = 20;
    }
    else if (terrain == '.')
    {
        *cost = 10;
    }
    else
    {
        fprintf(stderr, "wanderer_next_move fail");
        return -1;
    }

    return 0;
}

int Character::explorer_next_move(Map *m, World *w, int *cost)
{
    int nr, nc;
    int num;
    int i;

    for (i = 0; i < 20; i++)
    {
        if (dir == 'n')
        {
            nr = row - 1;
            nc = col;
        }
        else if (dir == 's')
        {
            nr = row + 1;
            nc = col;
        }
        else if (dir == 'w')
        {
            nr = row;
            nc = col - 1;
        }
        else
        {
            nr = row;
            nc = col + 1;
        }
        if (nr > 0 &&
            nr < 20 &&
            nc > 0 &&
            nc < 79 &&
            (m->charMap[nr][nc] == NULL || m->charMap[nr][nc]->type == '@') &&
            m->grid[nr][nc] != '%' &&
            m->grid[nr][nc] != '^' &&
            m->grid[nr][nc] != '~')
        {
            break;
        }
        num = rand() % 4;
        if (num == 0)
        {
            dir = 'n';
        }
        else if (num == 1)
        {
            dir = 's';
        }
        else if (num == 2)
        {
            dir = 'e';
        }
        else
        {
            dir = 'w';
        }
    }
    if (i == 20)
    {
        nr = row;
        nc = col;
    }

    nextRow = nr;
    nextCol = nc;

    if (m->grid[nr][nc] == '#')
    {
        *cost = 10;
    }
    else if (m->grid[nr][nc] == 'M')
    {
        *cost = 50;
    }
    else if (m->grid[nr][nc] == 'C')
    {
        *cost = 50;
    }
    else if (m->grid[nr][nc] == ':')
    {
        *cost = 20;
    }
    else if (m->grid[nr][nc] == '.')
    {
        *cost = 10;
    }
    else
    {
        fprintf(stderr, "explorer_next_move fail");
        return -1;
    }

    return 0;
}

int Character::next_move(Map *m, World *w, int *cost)
{

    if (type == 'r')
    {
        if (defeated)
        {
            defeated_rival_next_move(m, w, cost);
        }
        else
        {
            rival_next_move(m, w, cost);
        }
    }
    else if (type == 'h')
    {
        if (defeated)
        {
            defeated_hiker_next_move(m, w, cost);
        }
        else
        {
            hiker_next_move(m, w, cost);
        }
    }
    else if (type == 'p')
    {
        pacer_next_move(m, w, cost);
    }
    else if (type == 'w')
    {
        wanderer_next_move(m, w, cost);
    }
    else if (type == 'e')
    {
        explorer_next_move(m, w, cost);
    }

    return 0;
}

int Character::pc_next_move(Map *m, World *w, int *cost, int direction)
{
    int nrow, ncol;
    nrow = row;
    ncol = col;
    Map *newM;
    if (direction == UP_LEFT)
    {
        nrow = row - 1;
        ncol = col - 1;
    }
    else if (direction == UP)
    {
        nrow = row - 1;
        ncol = col;
    }
    else if (direction == UP_RIGHT)
    {
        nrow = row - 1;
        ncol = col + 1;
    }
    else if (direction == LEFT)
    {
        nrow = row;
        ncol = col - 1;
    }
    else if (direction == RIGHT)
    {
        nrow = row;
        ncol = col + 1;
    }
    else if (direction == DOWN_LEFT)
    {
        nrow = row + 1;
        ncol = col - 1;
    }
    else if (direction == DOWN)
    {
        nrow = row + 1;
        ncol = col;
    }
    else if (direction == DOWN_RIGHT)
    {
        nrow = row + 1;
        ncol = col + 1;
    }

    // if attempting to move to unreachable cell or gate, stay in place
    if (m->grid[nrow][ncol] == '%' ||
        m->grid[nrow][ncol] == '^' ||
        m->grid[nrow][ncol] == '~')
    {
        nrow = row;
        ncol = col;
    }

    // on a gate
    if (m->grid[nrow][ncol] == '#' &&
        (nrow == 0 ||
         nrow == 20 ||
         ncol == 0 || ncol == 79))
    {
        // move through north gate
        if (nrow == 0)
        {
            w->map_y--;
            if (!w->maps[w->map_x][w->map_y])
            {
                w->set_map(w->map_x, w->map_y);
            }

            w->get_map(w->map_x, w->map_y, &newM);

            newM->pc_row = 19;
            newM->pc_col = ncol;
        }
        // move through south gate
        else if (nrow == 20)
        {
            w->map_y++;
            if (!w->maps[w->map_x][w->map_y])
            {
                w->set_map(w->map_x, w->map_y);
            }

            w->get_map(w->map_x, w->map_y, &newM);

            newM->pc_row = 1;
            newM->pc_col = ncol;
        }
        // move through west gate
        else if (ncol == 0)
        {
            w->map_x--;
            if (!w->maps[w->map_x][w->map_y])
            {
                w->set_map(w->map_x, w->map_y);
            }
            w->get_map(w->map_x, w->map_y, &newM);

            newM->pc_row = nrow;
            newM->pc_col = 78;
        }
        // move through east gate
        else if (ncol == 79)
        {
            w->map_x++;
            if (!w->maps[w->map_x][w->map_y])
            {
                w->set_map(w->map_x, w->map_y);
            }
            w->get_map(w->map_x, w->map_y, &newM);

            newM->pc_row = nrow;
            newM->pc_col = 1;
        }

        nextRow = newM->pc_row;
        nextCol = newM->pc_col;
        *cost = 10;
        return 1;
    }
    else
    {
        nextRow = nrow;
        nextCol = ncol;

        char nspot = m->grid[nrow][ncol];

        if (nspot == '#' ||
            nspot == 'M' ||
            nspot == 'C' ||
            nspot == '.')
        {
            *cost = 10;
        }
        else if (nspot == ':')
        {
            *cost = 20;
        }
    }

    return 0;
}
