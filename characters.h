//Lauren Palan

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "terrain.h"
#include "pokemon.h"

class Map;

enum TrainerType {
    PCTYPE,
    HIKER,
    RIVAL,
    PACER,
    WANDERER,
    SENTRY,
    EXPLORER
};

class Character{
public:
    int x, y;
    char symbol;
    TrainerType type;
    int next_turn;
    int direction;

    std::vector<GeneratedPokemon> party;
    int activeIndex = 0;
    int potions = 0;
    int revives = 0;
    int pokeballs = 0;
    int money = 100;
    GeneratedPokemon* getActivePokemon();
    bool hasUsablePokemon();
    GeneratedPokemon* getNextPokemon();
    int getNextPokemonIndex();

    virtual bool isNPC() const {return true;} //default to NPC

    Character(int x = 0, int y = 0, char symbol = '?', TrainerType type = PCTYPE, int next_turn = 0, int direction = 0)
        : x(x), y(y), symbol(symbol), type(type), next_turn(next_turn), direction(direction) {}
    
    virtual ~Character() {}
};

class PC : public Character {
public:
    PC(int x = 0, int y = 0) : Character(x, y, '@', PCTYPE) {}
    bool isNPC() const override {return false;} //PC is not an NPC
};

class NPC : public Character {
public:
    NPC(int x = 0, int y = 0, char symbol = 'h', TrainerType type = HIKER) //just to default, will be overwritten later
    : Character(x, y, symbol, type) {}
};


Character *createPC(Map *m);
Character *createTrainer(Map *m, TrainerType type);
void spawnTrainers(Map *m, int numTrainers);
void moveNPC(Map *m, Character *pc);
void printNewMap(Map *m);
void removeCharacter(Map *m, Character *c);
void removePC(Character *c);



#endif