#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>

#include "pokeData.h"

class Pokemon
{
public:
    int hp_iv;
    int attack_iv;
    int defense_iv;
    int speed_iv;
    int special_attack_iv;
    int special_defense_iv;
    int hp_base;
    int attack_base;
    int defense_base;
    int speed_base;
    int special_attack_base;
    int special_defense_base;
    std::string name;
    int level;
    int pokemon_id;
    int species_id;
    bool gender;
    bool is_shiny;
    int max_hp;
    int cur_hp;
    int attack;
    int defense;
    int speed;
    int special_attack;
    int special_defense;
    int base_exp;
    std::vector<moves *> chosen_moves;
    std::vector<pokemon_moves *> moveset;
    type_names *p_types[2];
    pokemon_species *species;
    int exp;
    int next_level_exp;

    Pokemon(int, Data *);
    Pokemon(int, int, Data *);
    ~Pokemon();
    void level_up(Data *d);
};

#endif