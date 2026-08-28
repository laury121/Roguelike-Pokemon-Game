

#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <ncurses.h>
#include <algorithm>
#include <random>

class Map;
class Character;

struct Pokemon {
    int id;
    std::string identifier;
    int species_id;
    int height;
    int weight;
    int base_experience;
    int order;
    int is_default;
};

struct Move {
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

struct PokemonMoves {
    int pokemon_id;
    int version_group_id;
    int move_id;
    int pokemon_move_method_id;
    int level;
    int order;
};

struct PokemonSpecies {
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

struct Experience {
    int growth_rate_id;
    int level;
    int experience;
};

struct TypeNames {
    int type_id;
    int local_language_id;
    std::string name;
};

struct PokemonStats {
    int pokemon_id;
    int stat_id;
    int base_stat;
    int effort;
};

struct Stats {
    int id;
    int damage_class_id;
    std::string identifier;
    int is_battle_only;
    int game_index;
};

struct PokemonTypes {
    int pokemon_id;
    int type_id;
    int slot;
};

struct GeneratedPokemon {
    const Pokemon* base;
    int id;
    int level;
    int ivHp, ivAttack, ivDefense, ivSpAttack, ivSpDefense, ivSpeed;
    int hp, attack, defense, spAttack, spDefense, speed;
    std::vector<const Move*> moves;
    bool isShiny;
    bool isMale;


    int currentHp = hp; 
    int maxHp = 100;

    bool isFainted() const {
        return currentHp <= 0;
    }
    void takeDamage(int damage) {
        currentHp -= damage;
        if (currentHp < 0) currentHp = 0;
    }
    void heal(int ammount) {
        currentHp += ammount;
        if (currentHp > maxHp) currentHp = maxHp;
    }
    void revive() {
        if (currentHp <= 0) {
            currentHp = maxHp / 2;
            if (currentHp == 0) currentHp = 1;
        }
    }
};

void tryEncounter(Map* m, Character* pc, int worldX, int worldY);
GeneratedPokemon generatePokemon(int worldX, int worldY);
std::vector<const Move*> getMoves(int pokemonId, int level);
void getBaseStats(int pokemonId, int& hp, int& attack, int& defense, int& spAttack, int& spDefense, int& speed);
int calculateLevel(int worldX, int worldY);


#endif