#include "pokemon.h"
#include "pokeData.h"
#include "terrainGen.h"

#include <vector>
#include <iostream>
#include <ncurses.h>

moves *finds_move(pokemon_moves *pm, Data *d)
{
    if (d->vmoves.at(pm->move_id - 1)->id != pm->move_id)
    {
        fprintf(stderr, "correct move not found\n");
        return NULL;
    }

    return d->vmoves.at(pm->move_id);
}

Pokemon::Pokemon(int distance, Data *d)
{
    int i;
    std::vector<pokemon_moves *> level_moves;
    int min_level;
    pokemon_moves *tmp;

    // set pokemon id and name
    i = rand() % d->vpokemon.size();
    pokemon_id = d->vpokemon.at(i)->id;
    while (pokemon_id > 898)
    {
        i = rand() % d->vpokemon.size();
        pokemon_id = d->vpokemon.at(i)->id;
    }
    species_id = d->vpokemon.at(i)->species_id;
    name = d->vpokemon.at(i)->identifier;
    base_exp = d->vpokemon.at(i)->base_experience;

    // set gender
    if (rand() % 2 == 0)
    {
        gender = true; // male
    }
    else
    {
        gender = false; // female
    }

    // decide shiny
    if (rand() % 8192 == 0)
    {
        is_shiny = true;
    }
    else
    {
        is_shiny = false;
    }

    // decide level
    if (distance <= 200)
    {
        level = (rand() % (distance < 2 ? 1 : distance / 2)) + 1;
    }
    else if (distance == 400)
    {
        level = 100;
    }
    else
    {
        level = (rand() % (100 - ((distance - 200) / 2))) + (distance - 200) / 2;
    }

    int loc;
    loc = pokemon_id - 1;

    hp_base = d->vpokemon_stats.at((loc * 6))->base_stat;
    attack_base = d->vpokemon_stats.at((loc * 6) + 1)->base_stat;
    defense_base = d->vpokemon_stats.at((loc * 6) + 2)->base_stat;
    speed_base = d->vpokemon_stats.at((loc * 6) + 3)->base_stat;
    special_attack_base = d->vpokemon_stats.at((loc * 6) + 4)->base_stat;
    special_defense_base = d->vpokemon_stats.at((loc * 6) + 5)->base_stat;

    // set stats
    hp_iv = rand() % 16;
    attack_iv = rand() % 16;
    defense_iv = rand() % 16;
    speed_iv = rand() % 16;
    special_attack_iv = rand() % 16;
    special_defense_iv = rand() % 16;

    max_hp = ((((hp_base + hp_iv) * 2) * level) / 100) + level + 10;
    cur_hp = max_hp;

    attack = ((((attack_base + attack_iv) * 2) * level) / 100) + 5;
    defense = ((((defense_base + defense_iv) * 2) * level) / 100) + 5;
    speed = ((((speed_base + speed_iv) * 2) * level) / 100) + 5;
    special_attack = ((((special_attack_base + special_attack_iv) * 2) * level) / 100) + 5;
    special_defense = ((((special_defense_base + special_defense_iv) * 2) * level) / 100) + 5;

    // set moves
    for (pokemon_moves *m : d->vpokemon_moves)
    {
        if (m->pokemon_id == species_id && m->pokemon_move_method_id == 1)
        {
            moveset.push_back(m);
        }
    }

    min_level = 100;
    for (pokemon_moves *n : moveset)
    {
        if (n->level <= level)
        {
            level_moves.push_back(n);
            if (n->level < min_level)
                min_level = n->level;
        }
    }

    if (level_moves.size() == 0)
    {
        level = min_level;
        for (pokemon_moves *n : moveset)
        {
            if (n->level <= level)
                level_moves.push_back(n);
        }
    }

    if (level_moves.size() <= 2)
    {
        for (pokemon_moves *n : level_moves)
        {
            chosen_moves.push_back(finds_move(n, d));
        }
    }
    else
    {
        chosen_moves.push_back(finds_move(level_moves.at(rand() % level_moves.size()), d));
        tmp = level_moves.at(rand() % level_moves.size());
        if (tmp->move_id != chosen_moves.at(0)->id)
        {
            chosen_moves.push_back(finds_move(tmp, d));
        }
    }

    p_types[0] = nullptr;
    p_types[1] = nullptr;
    bool id_seen = false;
    for (pokemon_types *pt : d->vpokemon_types)
    {
        if (pt->pokemon_id == pokemon_id)
        {
            id_seen = true;
            if (p_types[0] == nullptr)
            {
                if (pt->type_id <= 18)
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1);
                }
                else
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1001 + 18);
                }
            }
            else
            {
                if (pt->type_id <= 18)
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1);
                }
                else
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1001 + 18);
                }
            }
        }
        else if (id_seen)
        {
            break;
        }
    }

    species = d->vpokemon_species.at(pokemon_id - 1);
    exp = d->vexperience.at((species->growth_rate_id - 1) * 100 + level - 1)->exp;
    next_level_exp = d->vexperience.at((species->growth_rate_id - 1) * 100 + level)->exp;
}

Pokemon::Pokemon(int distance, int id, Data *d)
{
    std::vector<pokemon_moves *> level_moves;
    int min_level;
    pokemon_moves *tmp;

    // set pokemon id and name
    pokemon_id = d->vpokemon.at(id - 1)->id;
    species_id = d->vpokemon.at(id - 1)->species_id;
    name = d->vpokemon.at(id - 1)->identifier;
    base_exp = d->vpokemon.at(id - 1)->base_experience;

    // set gender
    if (rand() % 2 == 0)
    {
        gender = true; // male
    }
    else
    {
        gender = false; // female
    }

    // decide shiny
    if (rand() % 8192 == 0)
    {
        is_shiny = true;
    }
    else
    {
        is_shiny = false;
    }

    // decide level
    if (distance <= 200)
    {
        level = (rand() % (distance < 2 ? 1 : distance / 2)) + 1;
    }
    else if (distance == 400)
    {
        level = 100;
    }
    else
    {
        level = (rand() % (100 - ((distance - 200) / 2))) + (distance - 200) / 2;
    }

    int loc;
    loc = pokemon_id - 1;

    hp_base = d->vpokemon_stats.at((loc * 6))->base_stat;
    attack_base = d->vpokemon_stats.at((loc * 6) + 1)->base_stat;
    defense_base = d->vpokemon_stats.at((loc * 6) + 2)->base_stat;
    speed_base = d->vpokemon_stats.at((loc * 6) + 3)->base_stat;
    special_attack_base = d->vpokemon_stats.at((loc * 6) + 4)->base_stat;
    special_defense_base = d->vpokemon_stats.at((loc * 6) + 5)->base_stat;

    // set stats
    hp_iv = rand() % 16;
    attack_iv = rand() % 16;
    defense_iv = rand() % 16;
    speed_iv = rand() % 16;
    special_attack_iv = rand() % 16;
    special_defense_iv = rand() % 16;

    max_hp = ((((hp_base + hp_iv) * 2) * level) / 100) + level + 10;
    cur_hp = max_hp;

    attack = ((((attack_base + attack_iv) * 2) * level) / 100) + 5;
    defense = ((((defense_base + defense_iv) * 2) * level) / 100) + 5;
    speed = ((((speed_base + speed_iv) * 2) * level) / 100) + 5;
    special_attack = ((((special_attack_base + special_attack_iv) * 2) * level) / 100) + 5;
    special_defense = ((((special_defense_base + special_defense_iv) * 2) * level) / 100) + 5;

    // set moves
    for (pokemon_moves *m : d->vpokemon_moves)
    {
        if (m->pokemon_id == species_id && m->pokemon_move_method_id == 1)
        {
            moveset.push_back(m);
        }
    }

    min_level = 100;
    for (pokemon_moves *n : moveset)
    {
        if (n->level <= level)
        {
            level_moves.push_back(n);
            if (n->level < min_level)
                min_level = n->level;
        }
    }

    if (level_moves.size() == 0)
    {
        level = min_level;
        for (pokemon_moves *n : moveset)
        {
            if (n->level <= level)
                level_moves.push_back(n);
        }
    }

    if (level_moves.size() <= 2)
    {
        for (pokemon_moves *n : level_moves)
        {
            chosen_moves.push_back(finds_move(n, d));
        }
    }
    else
    {
        chosen_moves.push_back(finds_move(level_moves.at(rand() % level_moves.size()), d));
        tmp = level_moves.at(rand() % level_moves.size());
        if (tmp->move_id != chosen_moves.at(0)->id)
        {
            chosen_moves.push_back(finds_move(tmp, d));
        }
    }

    p_types[0] = nullptr;
    p_types[1] = nullptr;
    bool id_seen = false;
    for (pokemon_types *pt : d->vpokemon_types)
    {
        if (pt->pokemon_id == pokemon_id)
        {
            id_seen = true;
            if (p_types[0] == nullptr)
            {
                if (pt->type_id <= 18)
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1);
                }
                else
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1001 + 18);
                }
            }
            else
            {
                if (pt->type_id <= 18)
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1);
                }
                else
                {
                    p_types[0] = d->vtype_names.at(pt->type_id - 1001 + 18);
                }
            }
        }
        else if (id_seen)
        {
            break;
        }
    }

    species = d->vpokemon_species.at(pokemon_id - 1);
    exp = d->vexperience.at((species->growth_rate_id - 1) * 100 + level - 1)->exp;
    next_level_exp = d->vexperience.at((species->growth_rate_id - 1) * 100 + level)->exp;
}


Pokemon::~Pokemon()
{
    for (pokemon_moves *p : moveset)
    {
        delete p;
    }
    moveset.clear();
}

void Pokemon::level_up(Data *d){
    level++;

    attack = ((((attack_base + attack_iv) * 2) * level) / 100) + 5;
    defense = ((((defense_base + defense_iv) * 2) * level) / 100) + 5;
    speed = ((((speed_base + speed_iv) * 2) * level) / 100) + 5;
    special_attack = ((((special_attack_base + special_attack_iv) * 2) * level) / 100) + 5;
    special_defense = ((((special_defense_base + special_defense_iv) * 2) * level) / 100) + 5;

    next_level_exp = d->vexperience.at((species->growth_rate_id - 1) * 100 + level)->exp;
    max_hp = ((((hp_base + hp_iv) * 2) * level) / 100) + level + 10;

    attron(COLOR_PAIR(GREEN));
    mvprintw(6, 0, "%s leveled up!", name.c_str());
    attroff(COLOR_PAIR(GREEN));
    mvprintw(7, 5, "%s is now level %d with HP %d/%d", name.c_str(), level, cur_hp, max_hp);
}