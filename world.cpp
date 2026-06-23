#include <cstdlib>
#include <cstdio>
#include <climits>
#include "map.h"
#include "terrainGen.h"
#include "world.h"
#include <ncurses.h>
#include <cstring>
#include "pokemon.h"

World::World()
{
    int i;
    int j;
    pc_bag = new Bag();
    num_trainers = 0;
    for (i = 0; i < 401; i++)
    {
        for (j = 0; j < 401; j++)
        {
            maps[i][j] = NULL;
        }
    }
    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            hikerMap[i][j] = INT_MAX;
        }
    }
    for (i = 0; i < 21; i++)
    {
        for (j = 0; j < 80; j++)
        {
            rivalMap[i][j] = INT_MAX;
        }
    }
    map_x = 200;
    map_y = 200;
}

World::~World()
{
    int i;
    int j;
    for (i = 0; i < 401; i++)
    {
        for (j = 0; j < 401; j++)
        {
            if ((maps[i][j]))
            {
                delete (maps[i][j]);
                maps[i][j] = NULL;
            }
        }
    }
}

int World::get_map(int x, int y, Map **m)
{
    if (!(maps[x][y]))
    {
        return -1;
    }
    *m = maps[x][y];

    return 0;
}

int World::set_map(int x, int y)
{
    if ((maps[x][y]))
    {
        return 1;
    }

    Map *m = new Map();
    m->set_map_location(x, y);
    int gate;
    if ((y > 0) && (maps[x][y - 1]))
    { // north
        maps[x][y - 1]->get_sGate(&gate);
        m->set_nGate(gate);
    }
    if ((y < 400) && (maps[x][y + 1]))
    { // south
        maps[x][y + 1]->get_nGate(&gate);
        m->set_sGate(gate);
    }
    if ((x < 400) && (maps[x + 1][y]))
    { // east
        maps[x + 1][y]->get_wGate(&gate);
        m->set_eGate(gate);
    }
    if ((x > 0) && (maps[x - 1][y]))
    { // west
        maps[x - 1][y]->get_eGate(&gate);
        m->set_wGate(gate);
    }
    if (generateTerrain(m))
    {
        return -1;
    }
    maps[x][y] = m;

    return 0;
}

void defeated_pokemon(Character *pc, Pokemon *wild, bool is_trainer, Data *d)
{
    float exp;
    float a, b, l;
    int num_split = 0;
    int each_exp;

    if (is_trainer)
        a = 1.5;
    else
        a = 1;
    b = wild->base_exp;
    l = wild->level;

    exp = ((b * l) / 7.0f) * a;

    for (Pokemon *p : pc->c_pokemon)
    {
        if (p->cur_hp != 0)
        {
            num_split++;
        }
    }

    each_exp = exp / num_split;

    clear();
    mvprintw(1, 0, "%s defeated!", wild->name.c_str());
    refresh();
    napms(1000);

    for (Pokemon *p : pc->c_pokemon)
    {
        if (p->cur_hp != 0)
        {
            move(4, 0);
            clrtoeol();
            p->exp += each_exp;
            mvprintw(4, 5, "%s gains %d XP. XP: %d", p->name.c_str(), each_exp, p->exp);
            refresh();
            napms(500);
            if (p->exp >= p->next_level_exp)
            {
                p->level_up(d);
            }
            refresh();
            napms(2000);
        }
    }
}

bool hit_screen(Pokemon *attacker, Pokemon *defender, moves *move, int damage, bool is_critical, bool is_npc)
{
    bool r;
    erase();
    refresh();

    if (is_npc)
    {
        attron(COLOR_PAIR(BLUE));
        mvprintw(0, 0, "Trainer Battle!\n");
        attroff(COLOR_PAIR(BLUE));
    }
    else
    {
        attron(COLOR_PAIR(BLUE));
        mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
        attroff(COLOR_PAIR(BLUE));
    }
    mvprintw(2, 0, "%s used %s!", attacker->name.c_str(), move->identifier.c_str());
    refresh();
    napms(2000);
    if (is_critical)
    {
        attron(COLOR_PAIR(RED));
        mvprintw(3, 7, "A critical hit!");
        attroff(COLOR_PAIR(RED));
        refresh();
        napms(1000);
    }
    if (defender->cur_hp > damage)
    {
        defender->cur_hp -= damage;
        attron(COLOR_PAIR(YELLOW));
        mvprintw(5, 0, "%s's HP went down %d", defender->name.c_str(), damage);
        attroff(COLOR_PAIR(YELLOW));

        r = false;
    }
    else
    {
        mvprintw(5, 0, "%s's HP went down %d", defender->name.c_str(), defender->cur_hp);
        defender->cur_hp = 0;
        attron(COLOR_PAIR(PURPLE));
        mvprintw(6, 0, "%s fainted!", defender->name.c_str());
        attroff(COLOR_PAIR(PURPLE));

        r = true;
    }

    refresh();
    napms(2000);

    return r;
}

int choose_pokemon(Character *pc)
{
    size_t i;
    char ch;
    int num_avail = 0;
    mvaddstr(9, 0, "Choose a Pok\u00e9mon to send out:");
    for (i = 0; i < pc->c_pokemon.size(); i++)
    {
        if (pc->c_pokemon.at(i)->cur_hp > 0)
        {
            num_avail++;
            mvprintw(i + 10, 7, "[%d] %s HP: %d/%d", (int)i + 1, pc->c_pokemon.at(i)->name.c_str(), pc->c_pokemon.at(i)->cur_hp, pc->c_pokemon.at(i)->max_hp);
        }
    }
    if (num_avail == 0)
    {
        erase();
        mvprintw(9, 0, "All your pok\u00e9mon are knocked-out!");
        refresh();
        napms(2000);
        attron(COLOR_PAIR(ALL_WHITE));
        wbkgd(stdscr, COLOR_PAIR(ALL_WHITE));
        refresh();
        napms(500);
        attroff(COLOR_PAIR(ALL_WHITE));
        attron(COLOR_PAIR(ALL_BLACK));
        wbkgd(stdscr, COLOR_PAIR(ALL_BLACK));
        refresh();
        attroff(COLOR_PAIR(ALL_BLACK));
        attrset(COLOR_PAIR(WHITE));
        return 0;
    }
    refresh();
    bool poke_choice = false;
    while (!poke_choice)
    {
        ch = getch();
        switch (ch)
        {
        case '1':
            if (pc->c_pokemon.size() >= 1)
            {
                if (pc->c_pokemon.at(0)->cur_hp == 0)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(21, 0, "%s is knocked out! Choose a different Pok\u00e9mon.", pc->c_pokemon.at(0)->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                return 1;
            }
            break;
        case '2':
            if (pc->c_pokemon.size() >= 2)
            {
                if (pc->c_pokemon.at(1)->cur_hp == 0)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(21, 0, "%s is knocked out! Choose a different Pok\u00e9mon.", pc->c_pokemon.at(1)->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                return 2;
            }
            break;
        case '3':
            if (pc->c_pokemon.size() >= 3)
            {
                if (pc->c_pokemon.at(2)->cur_hp == 0)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(21, 0, "%s is knocked out! Choose a different Pok\u00e9mon.", pc->c_pokemon.at(2)->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                return 3;
            }
            break;
        case '4':
            if (pc->c_pokemon.size() >= 4)
            {
                if (pc->c_pokemon.at(3)->cur_hp == 0)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(21, 0, "%s is knocked out! Choose a different Pok\u00e9mon.", pc->c_pokemon.at(3)->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                return 4;
            }
            break;
        case '5':
            if (pc->c_pokemon.size() >= 5)
            {
                if (pc->c_pokemon.at(4)->cur_hp == 0)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(21, 0, "%s is knocked out! Choose a different Pok\u00e9mon.", pc->c_pokemon.at(4)->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                return 5;
            }
            break;
        case '6':
            if (pc->c_pokemon.size() >= 6)
            {
                if (pc->c_pokemon.at(5)->cur_hp == 0)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(21, 0, "%s is knocked out! Choose a different Pok\u00e9mon.", pc->c_pokemon.at(5)->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                return 6;
            }
            break;
        }
    }
    return -1;
}

int calc_damage(Pokemon *attacker, Pokemon *defender, moves *move, bool is_npc)
{
    double damage, critical, random, stab;

    if (rand() % 256 < (attacker->speed_base / 2))
    {
        critical = 1.5;
    }
    else
    {
        critical = 1;
    }
    random = (rand() % 26 + 85) / 100.0f;
    if (move->type_id == attacker->p_types[0]->type_id || (attacker->p_types[1] != nullptr && move->type_id == attacker->p_types[1]->type_id))
    {
        stab = 1.5;
    }
    else
    {
        stab = 1;
    }
    int power;
    if (move->power != INT_MAX)
    {
        power = move->power;
    }
    else
    {
        power = 15;
    }
    damage = ((((((2.0f * (float)attacker->level) / 5.0f) + 2.0f) * power * ((float)attacker->attack / defender->defense)) / 50.0f) + 2.0f);
    damage *= critical * random * stab * 1;

    if ((int)damage == 0)
        damage = 1;

    bool is_critical;
    if (critical == 1.5)
        is_critical = true;
    else
        is_critical = false;
    if (hit_screen(attacker, defender, move, (int)damage, is_critical, is_npc))
    {
        return -1;
    }
    return 0;
}

int pokemon_turn(Pokemon *yours, Pokemon *wild, bool is_npc)
{
    moves *move = wild->chosen_moves.at(rand() % wild->chosen_moves.size());
    if (rand() % 100 <= move->accuracy)
    {
        if (calc_damage(wild, yours, move, is_npc) == -1)
            return -1;
    }
    else
    {
        erase();
        if (is_npc)
        {
            attron(COLOR_PAIR(BLUE));
            mvprintw(0, 0, "Trainer Battle!\n");
            attroff(COLOR_PAIR(BLUE));
        }
        else
        {
            attron(COLOR_PAIR(BLUE));
            mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
            attroff(COLOR_PAIR(BLUE));
        }

        mvprintw(2, 0, "%s used %s!", wild->name.c_str(), move->identifier.c_str());
        refresh();
        napms(1000);
        attron(COLOR_PAIR(GREEN));
        mvprintw(3, 0, "%s's attack missed!", wild->name.c_str());
        attroff(COLOR_PAIR(GREEN));
        refresh();
        napms(1000);
    }

    return 0;
}

moves *choose_move(Pokemon *yours, Pokemon *wild, bool is_npc)
{
    size_t i;
    char ch;
    erase();
    if (is_npc)
    {
        attron(COLOR_PAIR(BLUE));
        mvprintw(0, 0, "Trainer Battle!\n");
        attroff(COLOR_PAIR(BLUE));
    }
    else
    {
        attron(COLOR_PAIR(BLUE));
        mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
        attroff(COLOR_PAIR(BLUE));
    }

    mvaddstr(4, 0, "Choose a move:");

    for (i = 0; i < yours->chosen_moves.size(); i++)
    {
        mvprintw(i + 5, 7, "[%d] %s", int(i + 1), yours->chosen_moves.at(i)->identifier.c_str());
    }
    refresh();

    moves *move;
    bool input = false;
    while (!input)
    {
        ch = getch();
        switch (ch)
        {
        case '1':
            move = yours->chosen_moves.at(0);
            input = true;
            break;
        case '2':
            move = yours->chosen_moves.at(1);
            input = true;
            break;
        }
    }

    return move;
}

int player_turn(Pokemon *yours, Pokemon *wild, bool is_npc)
{
    bool p_first;
    moves *my_move = choose_move(yours, wild, is_npc);
    moves *their_move = wild->chosen_moves.at(rand() % wild->chosen_moves.size());

    if (my_move->priority > their_move->priority)
        p_first = true;
    else if (my_move->priority < their_move->priority)
        p_first = false;
    else if (yours->speed > wild->speed)
        p_first = true;
    else if (yours->speed < wild->speed)
        p_first = false;
    else
    {
        if (rand() % 2 == 0)
            p_first = true;
        else
            p_first = false;
    }

    if (p_first)
    {
        if (rand() % 100 <= my_move->accuracy)
        {
            if (calc_damage(yours, wild, my_move, is_npc) == -1)
                return -1;
        }
        else
        {
            erase();
            if (is_npc)
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Trainer Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }
            else
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }

            mvprintw(2, 0, "%s used %s!", yours->name.c_str(), my_move->identifier.c_str());
            refresh();
            napms(1000);
            attron(COLOR_PAIR(GREEN));
            mvprintw(3, 0, "%s's attack missed!", yours->name.c_str());
            attroff(COLOR_PAIR(GREEN));
            refresh();
            napms(1000);
        }
        if (rand() % 100 <= their_move->accuracy)
        {
            if (calc_damage(wild, yours, their_move, is_npc) == -1)
                return 1;
        }
        else
        {
            erase();
            if (is_npc)
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Trainer Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }
            else
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }

            mvprintw(2, 0, "%s used %s!", wild->name.c_str(), their_move->identifier.c_str());
            refresh();
            napms(1000);
            attron(COLOR_PAIR(GREEN));
            mvprintw(3, 0, "%s's attack missed!", wild->name.c_str());
            attroff(COLOR_PAIR(GREEN));
            refresh();
            napms(1000);
        }
    }
    else
    {
        if (rand() % 100 <= their_move->accuracy)
        {
            if (calc_damage(wild, yours, their_move, is_npc) == -1)
                return 1;
        }
        else
        {
            erase();
            if (is_npc)
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Trainer Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }
            else
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }

            mvprintw(2, 0, "%s used %s!", wild->name.c_str(), their_move->identifier.c_str());
            refresh();
            napms(1000);
            attron(COLOR_PAIR(GREEN));
            mvprintw(3, 0, "%s's attack missed!", wild->name.c_str());
            attroff(COLOR_PAIR(GREEN));
            refresh();
            napms(1000);
        }
        if (rand() % 100 <= my_move->accuracy)
        {
            if (calc_damage(yours, wild, my_move, is_npc) == -1)
                return -1;
        }
        else
        {
            erase();
            if (is_npc)
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Trainer Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }
            else
            {
                attron(COLOR_PAIR(BLUE));
                mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
                attroff(COLOR_PAIR(BLUE));
            }

            mvprintw(2, 0, "%s used %s!", yours->name.c_str(), my_move->identifier.c_str());
            refresh();
            napms(1000);
            attron(COLOR_PAIR(GREEN));
            mvprintw(3, 0, "%s's attack missed!", yours->name.c_str());
            attroff(COLOR_PAIR(GREEN));
            refresh();
            napms(1000);
        }
    }

    return 0;
}

int World::trainer_battle(Character *npc, Character *pc, Data *d)
{
    Pokemon *mine, *theirs;
    int i, j, poke_choice;
    int num_p_awake = 0;
    size_t trainer_num_p = npc->c_pokemon.size();
    erase();
    mine = pc->c_pokemon.at(0);
    for (i = 1; i < (int)pc->c_pokemon.size() && mine->cur_hp == 0; i++)
    {
        mine = pc->c_pokemon.at(i);
    }
    if (mine->cur_hp == 0)
    {
        clear();
        mvprintw(12, 0, "All your pok\u00e9mon are knocked-out!");
        refresh();
        napms(2000);
        attron(COLOR_PAIR(ALL_WHITE));
        wbkgd(stdscr, COLOR_PAIR(ALL_WHITE));
        refresh();
        napms(500);
        attroff(COLOR_PAIR(ALL_WHITE));
        attron(COLOR_PAIR(ALL_BLACK));
        wbkgd(stdscr, COLOR_PAIR(ALL_BLACK));
        refresh();
        attroff(COLOR_PAIR(ALL_BLACK));
        return 0;
    }
    attron(COLOR_PAIR(BLUE));
    mvprintw(0, 0, "Trainer Battle!\n");
    attroff(COLOR_PAIR(BLUE));
    mvprintw(1, 0, "%c has challenged you to a Pok\u00e9mon battle!", npc->type);

    for (i = 0; i < (int)trainer_num_p; i++)
    {
        if (npc->c_pokemon.at(i)->cur_hp > 0)
            num_p_awake++;
    }
    mvprintw(2, 5, "%c has %d active pokemon", npc->type, num_p_awake);
    refresh();
    napms(1000);
    theirs = npc->c_pokemon.at(rand() % trainer_num_p);
    mvprintw(5, 0, "%c sends out %s", npc->type, theirs->name.c_str());

    mvprintw(6, 5, "Pokemon name: %s (%s), level: %d, HP: %d/%d", theirs->name.c_str(),
             theirs->gender ? "Male" : "Female", theirs->level, theirs->cur_hp, theirs->max_hp);

    refresh();
    napms(1000);

    mvprintw(12, 0, "Go %s!", mine->name.c_str());
    mvprintw(13, 5, "level: %d, HP: %d/%d", mine->level, mine->cur_hp, mine->max_hp);

    refresh();
    napms(500);

    mvaddstr(19, 0, "[1] fight [2] open bag [3] switch pok\u00e9mon");
    refresh();
    char choice = getch();
    bool fighting = true;
    int result;
    while (fighting)
    {
        switch (choice)
        {
        case '1':
            result = player_turn(mine, theirs, true);
            if (result == -1)
            {
                num_p_awake--;
                defeated_pokemon(pc, theirs, true, d);
                if (num_p_awake > 0)
                {
                    for (i = 1; i < (int)trainer_num_p && theirs->cur_hp == 0; i++)
                    {
                        theirs = npc->c_pokemon.at(i);
                    }
                }
                else
                {
                    erase();
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(0, 0, "Trainer Battle!\n");
                    attroff(COLOR_PAIR(BLUE));
                    attron(COLOR_PAIR(GREEN));
                    mvprintw(12, 0, "%c defeated!", npc->type);
                    attroff(COLOR_PAIR(GREEN));
                    refresh();
                    napms(2000);
                    npc->defeated = 1;
                    return 0;
                }
            }
            if (result == 1)
            {
                poke_choice = choose_pokemon(pc);
                if (poke_choice == 0)
                    return -1;
                mine = pc->c_pokemon.at(poke_choice - 1);
                erase();
            }
            break;
        case '2':
            npc_battle_bag(pc);
            pokemon_turn(mine, theirs, true);
            break;
        case '3':
            for (j = 7; j < 24; j++)
            {
                move(j, 0);
                clrtoeol();
            }

            poke_choice = choose_pokemon(pc);
            if (poke_choice == 0)
                return -1;
            mine = pc->c_pokemon.at(poke_choice - 1);
            erase();
            attron(COLOR_PAIR(BLUE));
            mvprintw(0, 0, "Trainer Battle!\n");
            attroff(COLOR_PAIR(BLUE));
            mvprintw(1, 0, "%c has challenged you to a Pok\u00e9mon battle!", npc->type);
            mvprintw(2, 5, "%c has %d active pokemon", npc->type, num_p_awake);
            mvprintw(5, 0, "%c sends out %s", npc->type, theirs->name.c_str());
            mvprintw(6, 5, "Pokemon name: %s (%s), level: %d, HP: %d/%d", theirs->name.c_str(),
                     theirs->gender ? "Male" : "Female", theirs->level, theirs->cur_hp, theirs->max_hp);
            mvprintw(12, 0, "Go %s!", mine->name.c_str());
            mvprintw(13, 5, "level: %d, HP: %d/%d", mine->level, mine->cur_hp, mine->max_hp);
            refresh();
            napms(2000);
            if (pokemon_turn(mine, theirs, true) == -1)
            {
                poke_choice = choose_pokemon(pc);
                if (poke_choice == 0)
                    return -1;
                mine = pc->c_pokemon.at(poke_choice - 1);
            }
            break;
        }
        erase();
        attron(COLOR_PAIR(BLUE));
        mvprintw(0, 0, "Trainer Battle!\n");
        attroff(COLOR_PAIR(BLUE));
        mvprintw(1, 0, "%c has challenged you to a Pok\u00e9mon battle!", npc->type);
        mvprintw(2, 5, "%c has %d active pokemon", npc->type, num_p_awake);
        mvprintw(5, 0, "%c sends out %s", npc->type, theirs->name.c_str());
        mvprintw(6, 5, "Pokemon name: %s (%s), level: %d, HP: %d/%d", theirs->name.c_str(),
                 theirs->gender ? "Male" : "Female", theirs->level, theirs->cur_hp, theirs->max_hp);
        mvprintw(12, 0, "Go %s!", mine->name.c_str());
        mvprintw(13, 5, "level: %d, HP: %d/%d", mine->level, mine->cur_hp, mine->max_hp);
        mvaddstr(19, 0, "[1] fight [2] open bag [3] switch pok\u00e9mon");
        refresh();
        choice = getch();
    }

    return 0;
}

int World::set_num_trainers(int nt)
{
    num_trainers = nt;

    return 0;
}

int World::pop_map(Map *m, Data *d)
{
    m->charMap[m->pc_row][m->pc_col]->shortestPathHiker(m, this);
    m->charMap[m->pc_row][m->pc_col]->shortestPathRival(m, this);
    int i;
    if (num_trainers > 0)
    {
        Character *hiker = new Character('h', m, this, d);
        m->add_character(hiker, this);
    }

    if (num_trainers > 1)
    {
        Character *rival = new Character('r', m, this, d);
        m->add_character(rival, this);
    }

    for (i = 2; i < num_trainers; i++)
    {
        int num = rand() % 6;
        Character *c;

        if (num == 0)
        {
            c = new Character('h', m, this, d);
        }
        else if (num == 1)
        {
            c = new Character('r', m, this, d);
        }
        else if (num == 2)
        {
            c = new Character('p', m, this, d);
        }
        else if (num == 3)
        {
            c = new Character('w', m, this, d);
        }
        else if (num == 4)
        {
            c = new Character('s', m, this, d);
        }
        else if (num == 5)
        {
            c = new Character('e', m, this, d);
        }
        if (c->type != 's')
        {
            m->add_character(c, this);
        }
    }
    return 0;
}

int World::starter_pokemon(Character *pc, Data *d)
{
    char choice;
    Pokemon *s1 = new Pokemon(0, 1, d);
    Pokemon *s2 = new Pokemon(0, 4, d);
    Pokemon *s3 = new Pokemon(0, 7, d);
    erase();
    mvprintw(0, 0, "Choose a Pok\u00e9mon to start your adventure!");

    mvprintw(3, 0, "[1] %s (%s)", s1->name.c_str(), s1->p_types[0]->name.c_str());
    mvprintw(5, 0, "[2] %s (%s)", s2->name.c_str(), s2->p_types[0]->name.c_str());
    mvprintw(7, 0, "[3] %s (%s)", s3->name.c_str(), s3->p_types[0]->name.c_str());
    refresh();

    bool acceptable = false;

    while (!acceptable)
    {
        choice = getch();
        switch (choice)
        {
        case '1':
            pc->c_pokemon.push_back(s1);
            user_pokemon.push_back(s1);
            acceptable = true;
            delete s2;
            delete s3;
            break;
        case '2':
            pc->c_pokemon.push_back(s2);
            user_pokemon.push_back(s2);
            acceptable = true;
            delete s1;
            delete s3;
            break;
        case '3':
            pc->c_pokemon.push_back(s3);
            user_pokemon.push_back(s3);
            acceptable = true;
            delete s2;
            delete s1;
            break;
        default:
            mvprintw(10, 0, "Press 1, 2, or 3 to chose your Pok\u00e9mon!");
            refresh();
            break;
        }
    }

    return 0;
}

int World::pokemon_battle(int distance, Data *d, Character *pc)
{
    size_t i;
    int j;
    bool battle;
    char ch;
    int result, poke_choice;
    erase();
    refresh();
    Pokemon *wild = new Pokemon(distance, d);

    Pokemon *pc_p = pc->c_pokemon.at(0);
    for (i = 1; i < pc->c_pokemon.size() && pc_p->cur_hp == 0; i++)
    {
        pc_p = pc->c_pokemon.at(i);
    }
    if (pc_p->cur_hp == 0)
    {
        clear();
        mvprintw(12, 0, "All your pok\u00e9mon are knocked-out!");
        refresh();
        napms(2000);
        attron(COLOR_PAIR(ALL_WHITE));
        wbkgd(stdscr, COLOR_PAIR(ALL_WHITE));
        refresh();
        napms(500);
        attroff(COLOR_PAIR(ALL_WHITE));
        attron(COLOR_PAIR(ALL_BLACK));
        wbkgd(stdscr, COLOR_PAIR(ALL_BLACK));
        refresh();
        attroff(COLOR_PAIR(ALL_BLACK));
        return 0;
    }

    battle = true;
    while (battle)
    {
        erase();

        attron(COLOR_PAIR(BLUE));
        mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
        attroff(COLOR_PAIR(BLUE));

        if (!wild->is_shiny)
        {
            mvprintw(1, 0, "A wild %s has appeared", wild->name.c_str());
        }
        else
        {
            mvprintw(1, 0, "A wild shiny %s has appeared", wild->name.c_str());
        }
        mvprintw(3, 0, "Pokemon name: %s (%s), level: %d, HP: %d/%d", wild->name.c_str(), wild->gender ? "Male" : "Female", wild->level, wild->cur_hp, wild->max_hp);

        mvprintw(7, 0, "Go %s!", pc_p->name.c_str());
        mvprintw(8, 7, "level: %d, HP: %d/%d", pc_p->level, pc_p->cur_hp, pc_p->max_hp);

        mvaddstr(19, 0, "[1] fight [2] open bag [3] run [4] switch pok\u00e9mon");
        refresh();
        ch = getch();
        switch (ch)
        {
        case '1':
            result = player_turn(pc_p, wild, false);
            if (result == -1)
            {
                defeated_pokemon(pc, wild, false, d);
                return 0;
            }
            else if (result == 1)
            {
                poke_choice = choose_pokemon(pc);
                if (poke_choice == 0)
                    return -1;
                pc_p = pc->c_pokemon.at(poke_choice - 1);
            }
            break;
        case '2':
            if (battle_bag_interface(pc, wild) == -1)
                return 0;
            if (pokemon_turn(pc_p, wild, false) == -1)
            {
                poke_choice = choose_pokemon(pc);
                if (poke_choice == 0)
                    return -1;
                pc_p = pc->c_pokemon.at(poke_choice - 1);
            }
            break;
        case '3':
            if (rand() % 10 < 6)
            {
                move(19, 0);
                clrtoeol();
                attron(COLOR_PAIR(GREEN));
                mvaddstr(19, 0, "You escaped!");
                attroff(COLOR_PAIR(GREEN));
                refresh();
                getch();
                return 0;
            }
            else
            {
                move(19, 0);
                clrtoeol();
                attron(COLOR_PAIR(RED));
                mvaddstr(19, 0, "You couldn't escape!");
                attroff(COLOR_PAIR(RED));
                refresh();
                napms(1000);
                if (pokemon_turn(pc_p, wild, false) == -1)
                {
                    poke_choice = choose_pokemon(pc);
                    if (poke_choice == 0)
                        return -1;
                    pc_p = pc->c_pokemon.at(poke_choice - 1);
                }
            }
            break;
        case '4':
            for (j = 7; j < 24; j++)
            {
                move(j, 0);
                clrtoeol();
            }

            poke_choice = choose_pokemon(pc);
            if (poke_choice == 0)
                return -1;
            pc_p = pc->c_pokemon.at(poke_choice - 1);
            erase();

            if (!wild->is_shiny)
            {
                mvprintw(1, 0, "A wild %s has appeared", wild->name.c_str());
            }
            else
            {
                mvprintw(1, 0, "A wild shiny %s has appeared", wild->name.c_str());
            }
            mvprintw(3, 0, "Pokemon name: %s (%s), level: %d, HP: %d/%d", wild->name.c_str(), wild->gender ? "Male" : "Female", wild->level, wild->cur_hp, wild->max_hp);

            mvprintw(7, 0, "Go %s!", pc_p->name.c_str());
            mvprintw(8, 7, "level: %d, HP: %d/%d", pc_p->level, pc_p->cur_hp, pc_p->max_hp);

            refresh();
            napms(2000);

            if (pokemon_turn(pc_p, wild, false) == -1)
            {
                poke_choice = choose_pokemon(pc);
                if (poke_choice == 0)
                    return -1;
                pc_p = pc->c_pokemon.at(poke_choice - 1);
            }
            break;
        }
    }
    return 0;
}

int use_potion(Character *pc, Bag *bag)
{
    size_t i;
    char ch;
    bool quit;

    erase();
    mvprintw(0, 0, "Use a potion on a pok\u00e9mon to restore up to 20 HP.");
    mvprintw(1, 0, "You have %d potions", bag->num_potions);
    mvprintw(3, 0, "Choose a pok\u00e9mon to give a potion:");
    for (i = 0; i < pc->c_pokemon.size(); i++)
    {
        mvprintw(i + 4, 7, "[%d] %s HP: %d/%d", (int)i + 1, pc->c_pokemon.at(i)->name.c_str(), pc->c_pokemon.at(i)->cur_hp, pc->c_pokemon.at(i)->max_hp);
    }

    mvprintw(23, 0, "Press escape when you're done.");
    refresh();

    quit = false;
    Pokemon *cur;
    while (!quit)
    {
        ch = getch();
        switch (ch)
        {
        case '1':
            if (pc->c_pokemon.size() >= 1)
            {
                cur = pc->c_pokemon.at(0);
                if (cur->cur_hp == cur->max_hp)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(19, 0, "%s already has full HP!", cur->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                else if (cur->cur_hp > cur->max_hp - 20)
                {
                    cur->cur_hp = cur->max_hp;
                }
                else
                {
                    cur->cur_hp += 20;
                }
                mvprintw(4, 7, "[1] %s HP: %d/%d", cur->name.c_str(), cur->cur_hp, cur->max_hp);
                bag->num_potions--;
                mvprintw(1, 0, "You have %d potions", bag->num_potions);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You gave a potion to %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '2':
            if (pc->c_pokemon.size() >= 2)
            {
                cur = pc->c_pokemon.at(1);
                if (cur->cur_hp == cur->max_hp)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(19, 0, "%s already has full HP!", cur->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                else if (cur->cur_hp > cur->max_hp - 20)
                {
                    cur->cur_hp = cur->max_hp;
                }
                else
                {
                    cur->cur_hp += 20;
                }
                mvprintw(5, 7, "[2] %s HP: %d/%d", cur->name.c_str(), cur->cur_hp, cur->max_hp);
                bag->num_potions--;
                mvprintw(1, 0, "You have %d potions", bag->num_potions);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You gave a potion to %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '3':
            if (pc->c_pokemon.size() >= 3)
            {
                cur = pc->c_pokemon.at(2);
                if (cur->cur_hp == cur->max_hp)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(19, 0, "%s already has full HP!", cur->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                else if (cur->cur_hp > cur->max_hp - 20)
                {
                    cur->cur_hp = cur->max_hp;
                }
                else
                {
                    cur->cur_hp += 20;
                }
                mvprintw(6, 7, "[3] %s HP: %d/%d", cur->name.c_str(), cur->cur_hp, cur->max_hp);
                bag->num_potions--;
                mvprintw(1, 0, "You have %d potions", bag->num_potions);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You gave a potion to %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '4':
            if (pc->c_pokemon.size() >= 4)
            {
                cur = pc->c_pokemon.at(3);
                if (cur->cur_hp == cur->max_hp)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(19, 0, "%s already has full HP!", cur->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                else if (cur->cur_hp > cur->max_hp - 20)
                {
                    cur->cur_hp = cur->max_hp;
                }
                else
                {
                    cur->cur_hp += 20;
                }
                mvprintw(7, 7, "[4] %s HP: %d/%d", cur->name.c_str(), cur->cur_hp, cur->max_hp);
                bag->num_potions--;
                mvprintw(1, 0, "You have %d potions", bag->num_potions);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You gave a potion to %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '5':
            if (pc->c_pokemon.size() >= 5)
            {
                cur = pc->c_pokemon.at(4);
                if (cur->cur_hp == cur->max_hp)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(19, 0, "%s already has full HP!", cur->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                else if (cur->cur_hp > cur->max_hp - 20)
                {
                    cur->cur_hp = cur->max_hp;
                }
                else
                {
                    cur->cur_hp += 20;
                }
                mvprintw(8, 7, "[5] %s HP: %d/%d", cur->name.c_str(), cur->cur_hp, cur->max_hp);
                bag->num_potions--;
                mvprintw(1, 0, "You have %d potions", bag->num_potions);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You gave a potion to %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '6':
            if (pc->c_pokemon.size() >= 6)
            {
                cur = pc->c_pokemon.at(5);
                if (cur->cur_hp == cur->max_hp)
                {
                    attron(COLOR_PAIR(BLUE));
                    mvprintw(19, 0, "%s already has full HP!", cur->name.c_str());
                    attroff(COLOR_PAIR(BLUE));
                    refresh();
                    break;
                }
                else if (cur->cur_hp > cur->max_hp - 20)
                {
                    cur->cur_hp = cur->max_hp;
                }
                else
                {
                    cur->cur_hp += 20;
                }
                mvprintw(9, 7, "[6] %s HP: %d/%d", cur->name.c_str(), cur->cur_hp, cur->max_hp);
                bag->num_potions--;
                mvprintw(1, 0, "You have %d potions", bag->num_potions);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You gave a potion to %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case 27:
            quit = true;
            break;
        }
    }

    return 0;
}

int use_revive(Character *pc, Bag *bag)
{
    size_t i;
    char ch;
    bool quit;

    erase();
    mvprintw(0, 0, "Use a revive on a knocked-out pok\u00e9mon to restore half HP.");
    mvprintw(1, 0, "You have %d revives", bag->num_revives);
    mvprintw(3, 0, "Choose a pok\u00e9mon to revive:");
    for (i = 0; i < pc->c_pokemon.size(); i++)
    {
        if (pc->c_pokemon.at(i)->cur_hp == 0)
        {
            mvprintw(i + 4, 7, "[%d] %s HP: %d/%d", (int)i + 1, pc->c_pokemon.at(i)->name.c_str(), pc->c_pokemon.at(i)->cur_hp, pc->c_pokemon.at(i)->max_hp);
        }
    }

    mvprintw(23, 0, "Press escape when you're done.");
    refresh();

    quit = false;
    Pokemon *cur;
    while (!quit)
    {
        ch = getch();
        switch (ch)
        {
        case '1':
            if (pc->c_pokemon.size() >= 1 && pc->c_pokemon.at(0)->cur_hp == 0)
            {
                cur = pc->c_pokemon.at(0);
                cur->cur_hp = cur->max_hp / 2;
                move(4, 0);
                clrtoeol();
                bag->num_revives--;
                mvprintw(1, 0, "You have %d revives", bag->num_revives);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You revived %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '2':
            if (pc->c_pokemon.size() >= 2 && pc->c_pokemon.at(1)->cur_hp == 0)
            {
                cur = pc->c_pokemon.at(1);
                cur->cur_hp = cur->max_hp / 2;
                move(5, 0);
                clrtoeol();
                bag->num_revives--;
                mvprintw(1, 0, "You have %d revives", bag->num_revives);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You revived %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '3':
            if (pc->c_pokemon.size() >= 3 && pc->c_pokemon.at(2)->cur_hp == 0)
            {
                cur = pc->c_pokemon.at(2);
                cur->cur_hp = cur->max_hp / 2;
                move(6, 0);
                clrtoeol();
                bag->num_revives--;
                mvprintw(1, 0, "You have %d revives", bag->num_revives);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You revived %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '4':
            if (pc->c_pokemon.size() >= 4 && pc->c_pokemon.at(3)->cur_hp == 0)
            {
                cur = pc->c_pokemon.at(3);
                cur->cur_hp = cur->max_hp / 2;
                move(7, 0);
                clrtoeol();
                bag->num_revives--;
                mvprintw(1, 0, "You have %d revives", bag->num_revives);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You revived %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '5':
            if (pc->c_pokemon.size() >= 5 && pc->c_pokemon.at(4)->cur_hp == 0)
            {
                cur = pc->c_pokemon.at(4);
                cur->cur_hp = cur->max_hp / 2;
                move(8, 0);
                clrtoeol();
                bag->num_revives--;
                mvprintw(1, 0, "You have %d revives", bag->num_revives);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You revived %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case '6':
            if (pc->c_pokemon.size() >= 6 && pc->c_pokemon.at(5)->cur_hp == 0)
            {
                cur = pc->c_pokemon.at(5);
                cur->cur_hp = cur->max_hp / 2;
                move(9, 0);
                clrtoeol();
                bag->num_revives--;
                mvprintw(1, 0, "You have %d revives", bag->num_revives);
                attron(COLOR_PAIR(BLUE));
                mvprintw(19, 0, "You revived %s!", cur->name.c_str());
                attroff(COLOR_PAIR(BLUE));
                refresh();
            }
            break;
        case 27:
            quit = true;
            break;
        }
    }

    return 0;
}

int World::map_bag_interface(Character *pc)
{
    char ch;
    bool opt;
    opt = false;
    while (!opt)
    {
        erase();
        mvprintw(0, 0, "Your bag items:");
        mvprintw(1, 7, "Pok\u00e9balls: %d", pc_bag->num_pokeballs);
        mvprintw(2, 7, "Potions: %d", pc_bag->num_potions);
        mvprintw(3, 7, "Revives: %d", pc_bag->num_revives);
        mvprintw(6, 0, "Press (1) to use a potion. Press (2) to use a revive.");
        mvprintw(23, 0, "press escape to go back");
        refresh();
        ch = getch();
        switch (ch)
        {
        case '1':
            use_potion(pc, pc_bag);
            break;
        case '2':
            use_revive(pc, pc_bag);
            break;
        case 27:
            opt = true;
            break;
        }
    }

    return 0;
}

bool World::catch_pokemon(Pokemon *wild, Character *pc)
{
    erase();
    attron(COLOR_PAIR(BLUE));
    mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
    attroff(COLOR_PAIR(BLUE));

    if (user_pokemon.size() == 12)
    {
        pc_bag->num_pokeballs++;
    }

    int a = (((3 * wild->max_hp) - (2 * wild->cur_hp)) * wild->species->capture_rate) / (3 * wild->max_hp);
    if (a < 1)
        a = 1;

    if (rand() % 156 < a && user_pokemon.size() < 12)
    {
        if (pc->c_pokemon.size() < 6)
        {
            pc->c_pokemon.push_back(wild);
        }
        user_pokemon.push_back(wild);
        attron(COLOR_PAIR(GREEN));
        mvprintw(2, 0, "You caught %s!", wild->name.c_str());
        attroff(COLOR_PAIR(GREEN));
        refresh();
        napms(2000);
        return true;
    }
    attron(COLOR_PAIR(RED));
    mvprintw(2, 0, "Wild %s escaped!", wild->name.c_str());
    attroff(COLOR_PAIR(RED));
    refresh();
    napms(1000);
    move(2, 0);
    clrtoeol();
    if (rand() % 2 == 0)
    {
        mvprintw(2, 0, "Wild %s fled!", wild->name.c_str());
        refresh();
        napms(1000);
        return true;
    }
    else
    {
        refresh();
        napms(1000);
        return false;
    }
}

int World::battle_bag_interface(Character *pc, Pokemon *wild)
{
    char ch;
    bool opt;
    opt = false;
    erase();
    attron(COLOR_PAIR(BLUE));
    mvprintw(0, 0, "Pok\u00e9mon Battle!\n");
    attroff(COLOR_PAIR(BLUE));
    while (!opt)
    {
        mvprintw(1, 0, "Your bag items:");
        mvprintw(2, 7, "Pok\u00e9balls: %d", pc_bag->num_pokeballs);
        mvprintw(3, 7, "Potions: %d", pc_bag->num_potions);
        mvprintw(4, 7, "Revives: %d", pc_bag->num_revives);
        mvprintw(10, 0, "Press (1) to use a pok\u00e9ball. Press (2) to use a potion. Press (3) to use a revive.");
        refresh();
        ch = getch();
        switch (ch)
        {
        case '2':
            use_potion(pc, pc_bag);
            opt = true;
            break;
        case '3':
            use_revive(pc, pc_bag);
            opt = true;
            break;
        case '1':
            if (pc_bag->num_pokeballs == 0)
            {
                mvprintw(20, 0, "You don't have any pok\u00e9balls!");
                break;
            }
            pc_bag->num_pokeballs--;
            if (catch_pokemon(wild, pc))
                return -1;

            opt = true;
            break;
        }
    }
    return 0;
}

int World::npc_battle_bag(Character *pc)
{
    char ch;
    bool opt;
    opt = false;
    erase();
    attron(COLOR_PAIR(BLUE));
    mvprintw(0, 0, "Trainer Battle!\n");
    attroff(COLOR_PAIR(BLUE));
    while (!opt)
    {
        mvprintw(1, 0, "Your bag items:");
        mvprintw(2, 7, "Pok\u00e9balls: %d", pc_bag->num_pokeballs);
        mvprintw(3, 7, "Potions: %d", pc_bag->num_potions);
        mvprintw(4, 7, "Revives: %d", pc_bag->num_revives);
        mvprintw(10, 0, "Press (1) to use a potion. Press (2) to use a revive.");
        refresh();
        ch = getch();
        switch (ch)
        {
        case '1':
            use_potion(pc, pc_bag);
            opt = true;
            break;
        case '2':
            use_revive(pc, pc_bag);
            opt = true;
            break;
        }
    }
    return 0;
}