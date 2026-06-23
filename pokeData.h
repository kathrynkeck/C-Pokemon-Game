#ifndef POKEDATA_H
#define POKEDATA_H
#include <string>
#include <vector>

enum data_cat
{
    POKEMON,
    MOVES,
    POKEMON_MOVES,
    POKEMON_SPECIES,
    EXPERIENCE,
    TYPE_NAMES,
    POKEMON_STATS,
    STATS,
    POKEMON_TYPES
};

struct pokemon
{
public:
    int id;
    std::string identifier;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int is_default;
};

struct moves
{
public:
    int id;
    std::string identifier;
    int generation_id;
    int type_id;
    int power;
    int pp;
    int accuracy;
    int priority;
    int target_id;
    int damage_class_id;
    int effect_id;
    int effect_chance;
    int contest_type_id;
    int contest_effect_id;
    int super_contest_effect_id;
};

struct pokemon_moves
{
public:
    int pokemon_id;
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;
};

struct pokemon_species
{
public:
    int id;
    std::string identifier;
    int generation_id;
    int evolves_from_species_id;
    int evolution_chain_id;
    int color_id;
    int shape_id;
    int habitat_id;
    int gender_rate;
    int capture_rate;
    int base_happiness;
    int is_baby;
    int hatch_counter;
    int has_gender_differences;
    int growth_rate_id;
    int forms_switchable;
    int is_legendary;
    int is_mythical;
    int order;
    int conquest_order;
};

struct experience
{
public:
    int growth_rate_id;
    int level;
    int exp;
};

struct type_names
{
public:
    int type_id;
    int local_language_id;
    std::string name;
};

struct pokemon_stats
{
public:
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;
};

struct stats
{
public:
    int id;
    int damage_class_id;
    std::string identifier;
    int is_battle_only;
    int game_index;
};

struct pokemon_types
{
public:
    int pokemon_id;
    int type_id;
    int slot;
};

class Data
{
private:
    int pokemon_output();
    int moves_output();
    int pokemon_moves_output();
    int pokemon_species_output();
    int experience_output();
    int type_names_output();
    int pokemon_stats_output();
    int stats_output();
    int pokemon_types_output();
public:
    std::vector<pokemon *> vpokemon;
    std::vector<moves *> vmoves;
    std::vector<pokemon_moves *> vpokemon_moves;
    std::vector<pokemon_species *> vpokemon_species;
    std::vector<experience *> vexperience;
    std::vector<type_names *> vtype_names;
    std::vector<pokemon_stats *> vpokemon_stats;
    std::vector<stats *> vstats;
    std::vector<pokemon_types *> vpokemon_types;

    ~Data();
    int set_vpokemon();
    int set_vmoves();
    int set_vpokemon_moves();
    int set_vpokemon_species();
    int set_vexperience();
    int set_vtype_names();
    int set_vpokemon_stats();
    int set_vstats();
    int set_vpokemon_types();

    int output_list(int id);
};

#endif