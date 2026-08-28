
#ifndef BATTLE_H
#define BATTLE_H

#include "pokemon.h"
#include "characters.h"

enum ActionType {
    ACTION_MOVE,
    ACTION_ITEM,
    ACTION_SWITCH,
    ACTION_RUN
};

struct Action {
    ActionType type;
    const Move* move;
    int item;//0 for potion, 1 for revive, 2 for pokeball, etc
    int switchIndex;
};

class Battle {
private:
    Character* player;
    Character* opponent;
    GeneratedPokemon* playerActive;
    GeneratedPokemon* opponentActive;
    bool isWild;
    bool battleOver;

public:
    Battle(Character* p, Character* o, bool wild);

    void start();
    Action getPlayerAction();
    Action getOpponentAction();
    void executeAction(Character* user, Character* target, Action action);
    void executeTurn(Action playerAction, Action oppAction);

    int playerMenu();
    int moveMenu();
    int bagMenu();
    int switchMenu();
    void drawBattleScreen();

    
    int calculateDamage(GeneratedPokemon* attacker, GeneratedPokemon* defender, const Move* move);
    bool tryHit(int accuracy);

    void checkFaint();
    void switchPokemon(Character* c);
    bool canRun() const;

    

    void endBattle();
};

#endif