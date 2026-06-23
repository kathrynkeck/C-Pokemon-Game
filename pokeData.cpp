#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>

#include "pokeData.h"

Data::~Data()
{
    for (pokemon *p : vpokemon)
    {
        delete p;
    }
    vpokemon.clear();

    for (moves *p : vmoves)
    {
        delete p;
    }
    vmoves.clear();

    for (pokemon_moves *p : vpokemon_moves)
    {
        delete p;
    }
    vpokemon_moves.clear();

    for (pokemon_species *p : vpokemon_species)
    {
        delete p;
    }
    vpokemon_species.clear();

    for (experience *p : vexperience)
    {
        delete p;
    }
    vexperience.clear();

    for (type_names *p : vtype_names)
    {
        delete p;
    }
    vtype_names.clear();

    for (pokemon_stats *p : vpokemon_stats)
    {
        delete p;
    }
    vpokemon_stats.clear();

    for (stats *p : vstats)
    {
        delete p;
    }
    vstats.clear();

    for (pokemon_types *p : vpokemon_types)
    {
        delete p;
    }
    vpokemon_types.clear();
}

int file_exist(std::string filename)
{
    std::ifstream file(filename);
    return file.is_open();
}

int Data::set_vpokemon()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/pokemon.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/pokemon.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/pokemon.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    pokemon *p;
    std::string id = "";
    std::string identifier = "";
    std::string species_id = "";
    std::string height = "";
    std::string weight = "";
    std::string base_experience = "";
    std::string order = "";
    std::string is_default = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        p = new pokemon();

        // id
        for (i = 0; line[i] != ','; i++)
        {
            id += line[i];
        }
        if (!id.empty())
        {
            p->id = atoi(id.c_str());
        }
        else
        {
            p->id = INT_MAX;
        }

        // identifier
        for (i = i + 1; line[i] != ','; i++)
        {
            identifier += line[i];
        }
        if (!identifier.empty())
        {
            p->identifier = identifier;
        }
        else
        {
            p->identifier = "";
        }

        // species_id
        for (i = i + 1; line[i] != ','; i++)
        {
            species_id += line[i];
        }
        if (!species_id.empty())
        {
            p->species_id = atoi(species_id.c_str());
        }
        else
        {
            p->species_id = INT_MAX;
        }

        // height
        for (i = i + 1; line[i] != ','; i++)
        {
            height += line[i];
        }
        if (!height.empty())
        {
            p->height = atoi(height.c_str());
        }
        else
        {
            p->height = INT_MAX;
        }

        // weight
        for (i = i + 1; line[i] != ','; i++)
        {
            weight += line[i];
        }
        if (!weight.empty())
        {
            p->weight = atoi(weight.c_str());
        }
        else
        {
            p->weight = INT_MAX;
        }

        // base_experience
        for (i = i + 1; line[i] != ','; i++)
        {
            base_experience += line[i];
        }
        if (!base_experience.empty())
        {
            p->base_experience = atoi(base_experience.c_str());
        }
        else
        {
            p->base_experience = INT_MAX;
        }

        // order
        for (i = i + 1; line[i] != ','; i++)
        {
            order += line[i];
        }
        if (!order.empty())
        {
            p->order = atoi(order.c_str());
        }
        else
        {
            p->order = INT_MAX;
        }

        // is_default
        for (i = i + 1; line[i]; i++)
        {
            is_default += line[i];
        }
        if (!is_default.empty())
        {
            p->is_default = atoi(is_default.c_str());
        }
        else
        {
            p->is_default = INT_MAX;
        }

        line = "";
        id = "";
        identifier = "";
        species_id = "";
        height = "";
        weight = "";
        base_experience = "";
        order = "";
        is_default = "";

        vpokemon.push_back(p);
    }

    return 0;
}

int Data::set_vmoves()
{

    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/moves.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/moves.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/moves.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    moves *m;
    std::string id = "";
    std::string identifier = "";
    std::string generation_id = "";
    std::string type_id = "";
    std::string power = "";
    std::string pp = "";
    std::string accuracy = "";
    std::string priority = "";
    std::string target_id = "";
    std::string damage_class_id = "";
    std::string effect_id = "";
    std::string effect_chance = "";
    std::string contest_type_id = "";
    std::string contest_effect_id = "";
    std::string super_contest_effect_id = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        m = new moves();

        // id
        for (i = 0; line[i] != ','; i++)
        {
            id += line[i];
        }
        if (!id.empty())
        {
            m->id = atoi(id.c_str());
        }
        else
        {
            m->id = INT_MAX;
        }

        // identifier
        for (i = i + 1; line[i] != ','; i++)
        {
            identifier += line[i];
        }
        if (!identifier.empty())
        {
            m->identifier = identifier;
        }
        else
        {
            m->identifier = "";
        }

        // generation_id
        for (i = i + 1; line[i] != ','; i++)
        {
            generation_id += line[i];
        }
        if (!generation_id.empty())
        {
            m->generation_id = atoi(generation_id.c_str());
        }
        else
        {
            m->generation_id = INT_MAX;
        }

        // type_id
        for (i = i + 1; line[i] != ','; i++)
        {
            type_id += line[i];
        }
        if (!type_id.empty())
        {
            m->type_id = atoi(type_id.c_str());
        }
        else
        {
            m->type_id = INT_MAX;
        }

        // power
        for (i = i + 1; line[i] != ','; i++)
        {
            power += line[i];
        }
        if (!power.empty())
        {
            m->power = atoi(power.c_str());
        }
        else
        {
            m->power = INT_MAX;
        }

        // pp
        for (i = i + 1; line[i] != ','; i++)
        {
            pp += line[i];
        }
        if (!pp.empty())
        {
            m->pp = atoi(pp.c_str());
        }
        else
        {
            m->pp = INT_MAX;
        }

        // accuracy
        for (i = i + 1; line[i] != ','; i++)
        {
            accuracy += line[i];
        }
        if (!accuracy.empty())
        {
            m->accuracy = atoi(accuracy.c_str());
        }
        else
        {
            m->accuracy = INT_MAX;
        }

        // priority
        for (i = i + 1; line[i] != ','; i++)
        {
            priority += line[i];
        }
        if (!priority.empty())
        {
            m->priority = atoi(priority.c_str());
        }
        else
        {
            m->priority = INT_MAX;
        }

        // target_id
        for (i = i + 1; line[i] != ','; i++)
        {
            target_id += line[i];
        }
        if (!target_id.empty())
        {
            m->target_id = atoi(target_id.c_str());
        }
        else
        {
            m->target_id = INT_MAX;
        }

        // damage_class_id
        for (i = i + 1; line[i] != ','; i++)
        {
            damage_class_id += line[i];
        }
        if (!damage_class_id.empty())
        {
            m->damage_class_id = atoi(damage_class_id.c_str());
        }
        else
        {
            m->damage_class_id = INT_MAX;
        }

        // effect_id
        for (i = i + 1; line[i] != ','; i++)
        {
            effect_id += line[i];
        }
        if (!effect_id.empty())
        {
            m->effect_id = atoi(effect_id.c_str());
        }
        else
        {
            m->effect_id = INT_MAX;
        }

        // effect_chance
        for (i = i + 1; line[i] != ','; i++)
        {
            effect_chance += line[i];
        }
        if (!effect_chance.empty())
        {
            m->effect_chance = atoi(effect_chance.c_str());
        }
        else
        {
            m->effect_chance = INT_MAX;
        }

        // contest_type_id
        for (i = i + 1; line[i] != ','; i++)
        {
            contest_type_id += line[i];
        }
        if (!contest_type_id.empty())
        {
            m->contest_type_id = atoi(contest_type_id.c_str());
        }
        else
        {
            m->contest_type_id = INT_MAX;
        }

        // contest_effect_id
        for (i = i + 1; line[i] != ','; i++)
        {
            contest_effect_id += line[i];
        }
        if (!contest_effect_id.empty())
        {
            m->contest_effect_id = atoi(contest_effect_id.c_str());
        }
        else
        {
            m->contest_effect_id = INT_MAX;
        }

        // super_contest_effect_id
        for (i = i + 1; line[i]; i++)
        {
            super_contest_effect_id += line[i];
        }
        if (!super_contest_effect_id.empty())
        {
            m->super_contest_effect_id = atoi(super_contest_effect_id.c_str());
        }
        else
        {
            m->super_contest_effect_id = INT_MAX;
        }

        line = "";
        id = "";
        identifier = "";
        generation_id = "";
        type_id = "";
        power = "";
        pp = "";
        accuracy = "";
        priority = "";
        target_id = "";
        damage_class_id = "";
        effect_id = "";
        effect_chance = "";
        contest_type_id = "";
        contest_effect_id = "";
        super_contest_effect_id = "";

        vmoves.push_back(m);
    }
    return 0;
}

int Data::set_vpokemon_moves()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/pokemon_moves.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/pokemon_moves.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/pokemon_moves.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    pokemon_moves *p;
    std::string pokemon_id = "";
    std::string version_group_id = "";
    std::string move_id = "";
    std::string pokemon_move_method_id = "";
    std::string level = "";
    std::string order = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        p = new pokemon_moves();

        // pokemon_id
        for (i = 0; line[i] != ','; i++)
        {
            pokemon_id += line[i];
        }
        if (!pokemon_id.empty())
        {
            p->pokemon_id = atoi(pokemon_id.c_str());
        }
        else
        {
            p->pokemon_id = INT_MAX;
        }

        // version_group_id
        for (i = i + 1; line[i] != ','; i++)
        {
            version_group_id += line[i];
        }
        if (!version_group_id.empty())
        {
            p->version_group_id = atoi(version_group_id.c_str());
        }
        else
        {
            p->version_group_id = INT_MAX;
        }

        // move_id
        for (i = i + 1; line[i] != ','; i++)
        {
            move_id += line[i];
        }
        if (!move_id.empty())
        {
            p->move_id = atoi(move_id.c_str());
        }
        else
        {
            p->move_id = INT_MAX;
        }

        // pokemon_move_method_id
        for (i = i + 1; line[i] != ','; i++)
        {
            pokemon_move_method_id += line[i];
        }
        if (!pokemon_move_method_id.empty())
        {
            p->pokemon_move_method_id = atoi(pokemon_move_method_id.c_str());
        }
        else
        {
            p->pokemon_move_method_id = INT_MAX;
        }

        // level
        for (i = i + 1; line[i] != ','; i++)
        {
            level += line[i];
        }
        if (!level.empty())
        {
            p->level = atoi(level.c_str());
        }
        else
        {
            p->level = INT_MAX;
        }

        // order
        for (i = i + 1; line[i]; i++)
        {
            order += line[i];
        }
        if (!order.empty())
        {
            p->order = atoi(order.c_str());
        }
        else
        {
            p->order = INT_MAX;
        }

        line = "";
        pokemon_id = "";
        version_group_id = "";
        move_id = "";
        pokemon_move_method_id = "";
        level = "";
        order = "";

        vpokemon_moves.push_back(p);
    }

    return 0;
}

int Data::set_vpokemon_species()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/pokemon_species.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/pokemon_species.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/pokemon_species.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    pokemon_species *s;
    std::string id = "";
    std::string identifier = "";
    std::string generation_id = "";
    std::string evolves_from_species_id = "";
    std::string evolution_chain_id = "";
    std::string color_id = "";
    std::string shape_id = "";
    std::string habitat_id = "";
    std::string gender_rate = "";
    std::string capture_rate = "";
    std::string base_happiness = "";
    std::string is_baby = "";
    std::string hatch_counter = "";
    std::string has_gender_differences = "";
    std::string growth_rate_id = "";
    std::string forms_switchable = "";
    std::string is_legendary = "";
    std::string is_mythical = "";
    std::string order = "";
    std::string conquest_order = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        s = new pokemon_species();

        // id
        for (i = 0; line[i] != ','; i++)
        {
            id += line[i];
        }
        if (!id.empty())
        {
            s->id = atoi(id.c_str());
        }
        else
        {
            s->id = INT_MAX;
        }

        // identifier
        for (i = i + 1; line[i] != ','; i++)
        {
            identifier += line[i];
        }
        if (!identifier.empty())
        {
            s->identifier = identifier;
        }
        else
        {
            s->identifier = "";
        }

        // generation_id
        for (i = i + 1; line[i] != ','; i++)
        {
            generation_id += line[i];
        }
        if (!generation_id.empty())
        {
            s->generation_id = atoi(generation_id.c_str());
        }
        else
        {
            s->generation_id = INT_MAX;
        }

        // evolves_from_species_id
        for (i = i + 1; line[i] != ','; i++)
        {
            evolves_from_species_id += line[i];
        }
        if (!evolves_from_species_id.empty())
        {
            s->evolves_from_species_id = atoi(evolves_from_species_id.c_str());
        }
        else
        {
            s->evolves_from_species_id = INT_MAX;
        }

        // evolution_chain_id
        for (i = i + 1; line[i] != ','; i++)
        {
            evolution_chain_id += line[i];
        }
        if (!evolution_chain_id.empty())
        {
            s->evolution_chain_id = atoi(evolution_chain_id.c_str());
        }
        else
        {
            s->evolution_chain_id = INT_MAX;
        }

        // color_id
        for (i = i + 1; line[i] != ','; i++)
        {
            color_id += line[i];
        }
        if (!color_id.empty())
        {
            s->color_id = atoi(color_id.c_str());
        }
        else
        {
            s->color_id = INT_MAX;
        }

        // shape_id
        for (i = i + 1; line[i] != ','; i++)
        {
            shape_id += line[i];
        }
        if (!shape_id.empty())
        {
            s->shape_id = atoi(shape_id.c_str());
        }
        else
        {
            s->shape_id = INT_MAX;
        }

        // habitat_id
        for (i = i + 1; line[i] != ','; i++)
        {
            habitat_id += line[i];
        }
        if (!habitat_id.empty())
        {
            s->habitat_id = atoi(habitat_id.c_str());
        }
        else
        {
            s->habitat_id = INT_MAX;
        }

        // gender_rate
        for (i = i + 1; line[i] != ','; i++)
        {
            gender_rate += line[i];
        }
        if (!gender_rate.empty())
        {
            s->gender_rate = atoi(gender_rate.c_str());
        }
        else
        {
            s->gender_rate = INT_MAX;
        }

        // capture_rate
        for (i = i + 1; line[i] != ','; i++)
        {
            capture_rate += line[i];
        }
        if (!capture_rate.empty())
        {
            s->capture_rate = atoi(capture_rate.c_str());
        }
        else
        {
            s->capture_rate = INT_MAX;
        }

        // base_happiness
        for (i = i + 1; line[i] != ','; i++)
        {
            base_happiness += line[i];
        }
        if (!base_happiness.empty())
        {
            s->base_happiness = atoi(base_happiness.c_str());
        }
        else
        {
            s->base_happiness = INT_MAX;
        }

        // is_baby
        for (i = i + 1; line[i] != ','; i++)
        {
            is_baby += line[i];
        }
        if (!is_baby.empty())
        {
            s->is_baby = atoi(is_baby.c_str());
        }
        else
        {
            s->is_baby = INT_MAX;
        }

        // hatch_counter
        for (i = i + 1; line[i] != ','; i++)
        {
            hatch_counter += line[i];
        }
        if (!hatch_counter.empty())
        {
            s->hatch_counter = atoi(hatch_counter.c_str());
        }
        else
        {
            s->hatch_counter = INT_MAX;
        }

        // has_gender_differences
        for (i = i + 1; line[i] != ','; i++)
        {
            has_gender_differences += line[i];
        }
        if (!has_gender_differences.empty())
        {
            s->has_gender_differences = atoi(has_gender_differences.c_str());
        }
        else
        {
            s->has_gender_differences = INT_MAX;
        }

        // growth_rate_id
        for (i = i + 1; line[i] != ','; i++)
        {
            growth_rate_id += line[i];
        }
        if (!growth_rate_id.empty())
        {
            s->growth_rate_id = atoi(growth_rate_id.c_str());
        }
        else
        {
            s->growth_rate_id = INT_MAX;
        }

        // forms_switchable
        for (i = i + 1; line[i] != ','; i++)
        {
            forms_switchable += line[i];
        }
        if (!forms_switchable.empty())
        {
            s->forms_switchable = atoi(forms_switchable.c_str());
        }
        else
        {
            s->forms_switchable = INT_MAX;
        }

        // is_legendary
        for (i = i + 1; line[i] != ','; i++)
        {
            is_legendary += line[i];
        }
        if (!is_legendary.empty())
        {
            s->is_legendary = atoi(is_legendary.c_str());
        }
        else
        {
            s->is_legendary = INT_MAX;
        }

        // is_mythical
        for (i = i + 1; line[i] != ','; i++)
        {
            is_mythical += line[i];
        }
        if (!is_mythical.empty())
        {
            s->is_mythical = atoi(is_mythical.c_str());
        }
        else
        {
            s->is_mythical = INT_MAX;
        }

        // order
        for (i = i + 1; line[i] != ','; i++)
        {
            order += line[i];
        }
        if (!order.empty())
        {
            s->order = atoi(order.c_str());
        }
        else
        {
            s->order = INT_MAX;
        }

        // has_gender_differences
        for (i = i + 1; line[i]; i++)
        {
            conquest_order += line[i];
        }
        if (!conquest_order.empty())
        {
            s->conquest_order = atoi(conquest_order.c_str());
        }
        else
        {
            s->conquest_order = INT_MAX;
        }

        line = "";
        id = "";
        identifier = "";
        generation_id = "";
        evolves_from_species_id = "";
        evolution_chain_id = "";
        color_id = "";
        shape_id = "";
        habitat_id = "";
        gender_rate = "";
        capture_rate = "";
        base_happiness = "";
        is_baby = "";
        hatch_counter = "";
        has_gender_differences = "";
        growth_rate_id = "";
        forms_switchable = "";
        is_legendary = "";
        is_mythical = "";
        order = "";
        conquest_order = "";

        vpokemon_species.push_back(s);
    }

    return 0;
}

int Data::set_vexperience()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/experience.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/experience.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/experience.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    experience *e;
    std::string growth_rate_id = "";
    std::string level = "";
    std::string exp = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        e = new experience();

        // growth_rate_id
        for (i = 0; line[i] != ','; i++)
        {
            growth_rate_id += line[i];
        }
        if (!growth_rate_id.empty())
        {
            e->growth_rate_id = atoi(growth_rate_id.c_str());
        }
        else
        {
            e->growth_rate_id = INT_MAX;
        }

        // level
        for (i = i + 1; line[i] != ','; i++)
        {
            level += line[i];
        }
        if (!level.empty())
        {
            e->level = atoi(level.c_str());
        }
        else
        {
            e->level = INT_MAX;
        }

        // experience
        for (i = i + 1; line[i]; i++)
        {
            exp += line[i];
        }
        if (!exp.empty())
        {
            e->exp = atoi(exp.c_str());
        }
        else
        {
            e->exp = INT_MAX;
        }

        line = "";
        growth_rate_id = "";
        level = "";
        exp = "";

        vexperience.push_back(e);
    }

    return 0;
}

int Data::set_vtype_names()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/type_names.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/type_names.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/type_names.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    type_names *t;
    std::string type_id = "";
    std::string local_language_id = "";
    std::string name = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        t = new type_names();

        // type_id
        for (i = 0; line[i] != ','; i++)
        {
            type_id += line[i];
        }
        if (!type_id.empty())
        {
            t->type_id = atoi(type_id.c_str());
        }
        else
        {
            t->type_id = INT_MAX;
        }

        // local_language_id
        for (i = i + 1; line[i] != ','; i++)
        {
            local_language_id += line[i];
        }
        if (!local_language_id.empty())
        {
            t->local_language_id = atoi(local_language_id.c_str());
        }
        else
        {
            t->local_language_id = INT_MAX;
        }

        // name
        for (i = i + 1; line[i]; i++)
        {
            name += line[i];
        }
        if (!name.empty())
        {
            t->name = name;
        }
        else
        {
            t->name = "";
        }

        line = "";
        type_id = "";
        local_language_id = "";
        name = "";

        if (t->local_language_id == 9)
        {
            vtype_names.push_back(t);
        }
    }

    return 0;
}

int Data::set_vpokemon_stats()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/pokemon_stats.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/pokemon_stats.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/pokemon_stats.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    pokemon_stats *p;
    std::string pokemon_id = "";
    std::string stat_id = "";
    std::string base_stat = "";
    std::string effort = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        p = new pokemon_stats();

        // pokemon_id
        for (i = 0; line[i] != ','; i++)
        {
            pokemon_id += line[i];
        }
        if (!pokemon_id.empty())
        {
            p->pokemon_id = atoi(pokemon_id.c_str());
        }
        else
        {
            p->pokemon_id = INT_MAX;
        }

        // stat_id
        for (i = i + 1; line[i] != ','; i++)
        {
            stat_id += line[i];
        }
        if (!stat_id.empty())
        {
            p->stat_id = atoi(stat_id.c_str());
        }
        else
        {
            p->stat_id = INT_MAX;
        }

        // base_stat
        for (i = i + 1; line[i] != ','; i++)
        {
            base_stat += line[i];
        }
        if (!base_stat.empty())
        {
            p->base_stat = atoi(base_stat.c_str());
        }
        else
        {
            p->base_stat = INT_MAX;
        }

        // effort
        for (i = i + 1; line[i]; i++)
        {
            effort += line[i];
        }
        if (!effort.empty())
        {
            p->effort = atoi(effort.c_str());
        }
        else
        {
            p->effort = INT_MAX;
        }

        line = "";
        pokemon_id = "";
        stat_id = "";
        base_stat = "";
        effort = "";

        vpokemon_stats.push_back(p);
    }

    return 0;
}

int Data::set_vstats()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/stats.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/stats.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/stats.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    stats *s;
    std::string id = "";
    std::string damage_class_id = "";
    std::string identifier = "";
    std::string is_battle_only = "";
    std::string game_index = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        s = new stats();

        // id
        for (i = 0; line[i] != ','; i++)
        {
            id += line[i];
        }
        if (!id.empty())
        {
            s->id = atoi(id.c_str());
        }
        else
        {
            s->id = INT_MAX;
        }

        // damage_class_id
        for (i = i + 1; line[i] != ','; i++)
        {
            damage_class_id += line[i];
        }
        if (!damage_class_id.empty())
        {
            s->damage_class_id = atoi(damage_class_id.c_str());
        }
        else
        {
            s->damage_class_id = INT_MAX;
        }

        // identifier
        for (i = i + 1; line[i] != ','; i++)
        {
            identifier += line[i];
        }
        if (!identifier.empty())
        {
            s->identifier = identifier;
        }
        else
        {
            s->identifier = "";
        }

        // is_battle_only
        for (i = i + 1; line[i] != ','; i++)
        {
            is_battle_only += line[i];
        }
        if (!is_battle_only.empty())
        {
            s->is_battle_only = atoi(is_battle_only.c_str());
        }
        else
        {
            s->is_battle_only = INT_MAX;
        }

        // game_index
        for (i = i + 1; line[i]; i++)
        {
            game_index += line[i];
        }
        if (!game_index.empty())
        {
            s->game_index = atoi(game_index.c_str());
        }
        else
        {
            s->game_index = INT_MAX;
        }

        line = "";
        id = "";
        damage_class_id = "";
        identifier = "";
        is_battle_only = "";
        game_index = "";

        vstats.push_back(s);
    }

    return 0;
}

int Data::set_vpokemon_types()
{
    // check file path
    std::string path = "";
    if (file_exist("/share/cs327/pokedex/pokedex/data/csv/pokemon_types.csv"))
    {
        path = "/share/cs327/pokedex/pokedex/data/csv/pokemon_types.csv";
    }
    else
    {
        char *home = std::getenv("HOME");
        if (home)
        {
            std::string loc2 = std::string(home) + "/.poke327/pokedex/pokedex/data/csv/pokemon_types.csv";
            if (file_exist(loc2))
            {
                path = loc2;
            }
        }
    }

    if (path.empty())
    {
        std::cerr << "Database file not found" << std::endl;
    }

    std::string line;
    std::ifstream csv(path);
    int i;
    pokemon_types *p;
    std::string pokemon_id = "";
    std::string type_id = "";
    std::string slot = "";

    getline(csv, line);

    while (getline(csv, line))
    {
        p = new pokemon_types();

        // pokemon_id
        for (i = 0; line[i] != ','; i++)
        {
            pokemon_id += line[i];
        }
        if (!pokemon_id.empty())
        {
            p->pokemon_id = atoi(pokemon_id.c_str());
        }
        else
        {
            p->pokemon_id = INT_MAX;
        }

        // type_id
        for (i = i + 1; line[i] != ','; i++)
        {
            type_id += line[i];
        }
        if (!type_id.empty())
        {
            p->type_id = atoi(type_id.c_str());
        }
        else
        {
            p->type_id = INT_MAX;
        }

        // slot
        for (i = i + 1; line[i]; i++)
        {
            slot += line[i];
        }
        if (!slot.empty())
        {
            p->slot = atoi(slot.c_str());
        }
        else
        {
            p->slot = INT_MAX;
        }

        line = "";
        pokemon_id = "";
        type_id = "";
        slot = "";

        vpokemon_types.push_back(p);
    }

    return 0;
}

int Data::pokemon_output()
{
    size_t i;

    for (i = 0; i < vpokemon.size(); i++)
    {
        if (vpokemon.at(i)->id != INT_MAX)
        {
            std::cout << vpokemon.at(i)->id << ",";
        }
        else
        {
            std::cout << ",";
        }

        std::cout << vpokemon.at(i)->identifier << ",";
        if (vpokemon.at(i)->species_id != INT_MAX)
        {
            std::cout << vpokemon.at(i)->species_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon.at(i)->height != INT_MAX)
        {
            std::cout << vpokemon.at(i)->height << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon.at(i)->weight != INT_MAX)
        {
            std::cout << vpokemon.at(i)->weight << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon.at(i)->base_experience != INT_MAX)
        {
            std::cout << vpokemon.at(i)->base_experience << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon.at(i)->order != INT_MAX)
        {
            std::cout << vpokemon.at(i)->order << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon.at(i)->is_default != INT_MAX)
        {
            std::cout << vpokemon.at(i)->is_default;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::pokemon_species_output()
{
    size_t i;

    for (i = 0; i < vpokemon_species.size(); i++)
    {
        if (vpokemon_species.at(i)->id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->id << ",";
        }
        else
        {
            std::cout << ",";
        }

        std::cout << vpokemon_species.at(i)->identifier << ",";
        if (vpokemon_species.at(i)->generation_id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->generation_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->evolves_from_species_id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->evolves_from_species_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->evolution_chain_id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->evolution_chain_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->color_id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->color_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->shape_id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->shape_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->habitat_id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->habitat_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->gender_rate != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->gender_rate << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->capture_rate != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->capture_rate << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->base_happiness != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->base_happiness << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->is_baby != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->is_baby << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->hatch_counter != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->hatch_counter << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->has_gender_differences != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->has_gender_differences << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->growth_rate_id != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->growth_rate_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->forms_switchable != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->forms_switchable << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->is_legendary != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->is_legendary << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->is_mythical != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->is_mythical << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->order != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->order << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_species.at(i)->conquest_order != INT_MAX)
        {
            std::cout << vpokemon_species.at(i)->conquest_order;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::pokemon_moves_output()
{
    size_t i;

    for (i = 0; i < vpokemon_moves.size(); i++)
    {
        if (vpokemon_moves.at(i)->pokemon_id != INT_MAX)
        {
            std::cout << vpokemon_moves.at(i)->pokemon_id << ",";
        }
        else
        {
            std::cout << ",";
        }
        if (vpokemon_moves.at(i)->version_group_id != INT_MAX)
        {
            std::cout << vpokemon_moves.at(i)->version_group_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_moves.at(i)->move_id != INT_MAX)
        {
            std::cout << vpokemon_moves.at(i)->move_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_moves.at(i)->pokemon_move_method_id != INT_MAX)
        {
            std::cout << vpokemon_moves.at(i)->pokemon_move_method_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_moves.at(i)->level != INT_MAX)
        {
            std::cout << vpokemon_moves.at(i)->level << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_moves.at(i)->order != INT_MAX)
        {
            std::cout << vpokemon_moves.at(i)->order;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::moves_output()
{
    size_t i;

    for (i = 0; i < vmoves.size(); i++)
    {
        if (vmoves.at(i)->id != INT_MAX)
        {
            std::cout << vmoves.at(i)->id << ",";
        }
        else
        {
            std::cout << ",";
        }

        std::cout << vmoves.at(i)->identifier << ",";
        if (vmoves.at(i)->generation_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->generation_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->type_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->type_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->power != INT_MAX)
        {
            std::cout << vmoves.at(i)->power << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->pp != INT_MAX)
        {
            std::cout << vmoves.at(i)->pp << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->accuracy != INT_MAX)
        {
            std::cout << vmoves.at(i)->accuracy << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->priority != INT_MAX)
        {
            std::cout << vmoves.at(i)->priority << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->target_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->target_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->damage_class_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->damage_class_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->effect_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->effect_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->effect_chance != INT_MAX)
        {
            std::cout << vmoves.at(i)->effect_chance << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->contest_type_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->contest_type_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->contest_effect_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->contest_effect_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vmoves.at(i)->super_contest_effect_id != INT_MAX)
        {
            std::cout << vmoves.at(i)->super_contest_effect_id;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::experience_output()
{
    size_t i;

    for (i = 0; i < vexperience.size(); i++)
    {
        if (vexperience.at(i)->growth_rate_id != INT_MAX)
        {
            std::cout << vexperience.at(i)->growth_rate_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vexperience.at(i)->level != INT_MAX)
        {
            std::cout << vexperience.at(i)->level << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vexperience.at(i)->exp != INT_MAX)
        {
            std::cout << vexperience.at(i)->exp;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::type_names_output()
{
    size_t i;

    for (i = 0; i < vtype_names.size(); i++)
    {
        if (vtype_names.at(i)->type_id != INT_MAX)
        {
            std::cout << vtype_names.at(i)->type_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vtype_names.at(i)->local_language_id != INT_MAX)
        {
            std::cout << vtype_names.at(i)->local_language_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        std::cout << vtype_names.at(i)->name;

        std::cout << std::endl;
    }

    return 0;
}

int Data::pokemon_stats_output()
{
    size_t i;

    for (i = 0; i < vpokemon_stats.size(); i++)
    {
        if (vpokemon_stats.at(i)->pokemon_id != INT_MAX)
        {
            std::cout << vpokemon_stats.at(i)->pokemon_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_stats.at(i)->stat_id != INT_MAX)
        {
            std::cout << vpokemon_stats.at(i)->stat_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_stats.at(i)->base_stat != INT_MAX)
        {
            std::cout << vpokemon_stats.at(i)->base_stat << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_stats.at(i)->effort != INT_MAX)
        {
            std::cout << vpokemon_stats.at(i)->effort;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::stats_output()
{
    size_t i;

    for (i = 0; i < vstats.size(); i++)
    {
        if (vstats.at(i)->id != INT_MAX)
        {
            std::cout << vstats.at(i)->id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vstats.at(i)->damage_class_id != INT_MAX)
        {
            std::cout << vstats.at(i)->damage_class_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        std::cout << vstats.at(i)->identifier << ",";

        if (vstats.at(i)->is_battle_only != INT_MAX)
        {
            std::cout << vstats.at(i)->is_battle_only << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vstats.at(i)->game_index != INT_MAX)
        {
            std::cout << vstats.at(i)->game_index;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::pokemon_types_output()
{
    size_t i;

    for (i = 0; i < vpokemon_types.size(); i++)
    {
        if (vpokemon_types.at(i)->pokemon_id != INT_MAX)
        {
            std::cout << vpokemon_types.at(i)->pokemon_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_types.at(i)->type_id != INT_MAX)
        {
            std::cout << vpokemon_types.at(i)->type_id << ",";
        }
        else
        {
            std::cout << ",";
        }

        if (vpokemon_types.at(i)->slot != INT_MAX)
        {
            std::cout << vpokemon_types.at(i)->slot;
        }

        std::cout << std::endl;
    }

    return 0;
}

int Data::output_list(int type)
{
    switch (type)
    {
    case POKEMON:
        pokemon_output();
        break;
    case MOVES:
        moves_output();
        break;
    case POKEMON_MOVES:
        pokemon_moves_output();
        break;
    case POKEMON_SPECIES:
        pokemon_species_output();
        break;
    case EXPERIENCE:
        experience_output();
        break;
    case TYPE_NAMES:
        type_names_output();
        break;
    case POKEMON_STATS:
        pokemon_stats_output();
        break;
    case STATS:
        stats_output();
        break;
    case POKEMON_TYPES:
        pokemon_types_output();
        break;
    }

    return 0;
}