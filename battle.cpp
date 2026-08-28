
#include "battle.h"
#include <iostream>
#include <cstdlib>

Battle::Battle(Character* p, Character* o, bool wild) {
    player = p;
    opponent = o;
    isWild = wild;
    battleOver = false;

    playerActive = player->getActivePokemon();
    opponentActive = opponent->getActivePokemon();
}

void Battle::drawBattleScreen() {
    clear();

    int leftCol = 2;
    int rightCol = 40;

    //opponent top right
    mvprintw(1, rightCol, "Enemy:");
    mvprintw(2, rightCol, "%s", opponentActive->base->identifier.c_str());

    mvprintw(3, rightCol, "HP: %d/%d", opponentActive->currentHp, opponentActive->maxHp);
    mvprintw(4, rightCol + 20, "Lv %d", opponentActive->level);

    //player bottom left
    mvprintw(8, leftCol, "You:");
    mvprintw(9, leftCol, "%s", playerActive->base->identifier.c_str());

    mvprintw(10, leftCol, "HP: %d/%d", playerActive->currentHp, playerActive->maxHp);
    mvprintw(11, leftCol + 20, "Lv %d", playerActive->level);

    //divider for menu
    for (int i = 0; i < 70; i++) {
        mvaddch(12, i, '-');
    }

    refresh();
}

void Battle::start() {
    mvprintw(13, 2, "Battle Begins!");
    refresh();
    getch();
    while (!battleOver) {
        Action playerAction = getPlayerAction();
        Action oppAction = getOpponentAction();
        executeTurn(playerAction, oppAction);
        checkFaint();
    }
    endBattle();
}

void Battle::executeTurn(Action p, Action o) {
    //non-move actions go first
    if (p.type != ACTION_MOVE && o.type == ACTION_MOVE) {
        executeAction(player, opponent, p);
        executeAction(opponent, player, o);
        return;
    }

    if (o.type != ACTION_MOVE && p.type == ACTION_MOVE) {
        executeAction(opponent, player, o);
        executeAction(player, opponent, p);
        return;
    }

    //if both moves, check priority
    if (p.move->priority > o.move->priority) {
        executeAction(player, opponent, p);
        executeAction(opponent, player, o);
    }
    else if (o.move->priority > p.move->priority) {
        executeAction(opponent, player, o);
        executeAction(player, opponent, p);
    }
    else {
        //if same priority, then speed
        if (playerActive->speed > opponentActive->speed) {
            executeAction(player, opponent, p);
            executeAction(opponent, player, o);
        } else {
            executeAction(opponent, player, o);
            executeAction(player, opponent, p);
        }
    }
}

void Battle::executeAction(Character* user, Character* target, Action action) {
    GeneratedPokemon* atk = (user == player) ? playerActive : opponentActive;
    GeneratedPokemon* def = (user == player) ? opponentActive : playerActive;

    switch (action.type) {

        case ACTION_MOVE: {
            if (atk->isFainted()) return;

            mvprintw(13, 2, "%s used %s!",
                atk->base->identifier.c_str(),
                action.move->identifier.c_str());
            refresh();
            getch();

            if (!tryHit(action.move->accuracy)) {
                mvprintw(13, 2, "Move missed!");
                refresh();
                getch();
                return;
            }

            int dmg = calculateDamage(atk, def, action.move);
            def->takeDamage(dmg);

            mvprintw(13, 2, "It dealt %d damage!", dmg);
            refresh();
            getch();

            break;
        }

        case ACTION_ITEM: {
            if (user == player) {
                GeneratedPokemon* targetPoke = playerActive;
                if (action.item == 0) {
                    if (player->potions <= 0) {
                        mvprintw(13, 2, "No potions left!");
                    }
                    else if (targetPoke->currentHp == targetPoke->maxHp) {
                        mvprintw(13, 2, "Already at full HP!");
                    }
                    else {
                        player->potions--;
                        targetPoke->heal(20);
                        mvprintw(13, 2, "Used Potion! Healed 20 HP.");
                    }
                }
                else if (action.item == 1) {
                    if (player->revives <= 0) {
                        mvprintw(13, 2, "No revives left!");
                    }
                    else if (!targetPoke->isFainted()) {
                        mvprintw(13, 2, "Cannot revive a healthy Pokemon!");
                    }
                    else {
                        player->revives--;
                        targetPoke->revive();
                        mvprintw(13, 2, "Pokemon revived!");
                    }
                }
                else if (action.item == 2) {
                    if (!isWild) {
                        mvprintw(13, 2, "Can't catch this Pokemon!");
                    } else if (player->pokeballs <= 0) {
                        mvprintw(13, 2, "No pokeballs left!");
                    } else {
                        player->pokeballs--;

                        mvprintw(13, 2, "Throwing Pokeball...");
                        refresh();
                        getch();

                        int chance = 50 + (def->maxHp - def->currentHp);

                        if (rand() % 100 < chance) {
                            if (player->party.size() < 6) {
                                player->party.push_back(*def);
                                mvprintw(13, 2, "Gotcha! Pokemon caught!");
                            } else {
                                mvprintw(13, 2, "Party full!");
                            }
                            battleOver = true;
                        } else {
                            mvprintw(13, 2, "It broke free!");
                        }

                        
                    }
            }
            getch();
            break;
        }

        case ACTION_SWITCH: {
            user->activeIndex = action.switchIndex;
            if (user == player) {
                playerActive = &player->party[action.switchIndex];
            } else {
                opponentActive = &opponent->party[action.switchIndex];
            }
            refresh();
            getch();
            break;
        }

        case ACTION_RUN: {
            if (isWild && rand() % 2 == 0) {
                mvprintw(13, 2, "Escaped!");
                battleOver = true;
            } else {
                mvprintw(13, 2, "Couldn't escape!");
                refresh();
                getch();
            }
            break;
        }
    }
    }
}

Action Battle::getPlayerAction() {
    while (true) {
        Action a;

        int choice = playerMenu();

        if (choice == 0) {
            int m = moveMenu();
            a.type = ACTION_MOVE;
            a.move = playerActive->moves[m];
            return a;
        }
        else if (choice == 1) {
            int item = bagMenu();
            if (item == 3) continue;
            a.type = ACTION_ITEM;
            a.item = item;
            return a;
        }
        else if (choice == 2) {
            a.type = ACTION_RUN;
            return a;
        }
        else {
            int s = switchMenu();
            a.type = ACTION_SWITCH;
            a.switchIndex = s;
            return a;
        }
    }
    
}

Action Battle::getOpponentAction() {
    Action a;

    if (opponentActive->isFainted()) {
        a.type = ACTION_SWITCH;
        a.switchIndex = opponent->getNextPokemonIndex();
        return a;
    }

    int r = rand() % opponentActive->moves.size();

    a.type = ACTION_MOVE;
    a.move = opponentActive->moves[r];

    return a;
}


bool Battle::tryHit(int accuracy) {
    return (rand() % 100) < accuracy;
}

int Battle::calculateDamage(GeneratedPokemon* attacker, GeneratedPokemon* defender, const Move* move) {
    int level = attacker->level;
    int power = std::max(1, move->power);
    
    //base damage
    int base = ((2 * level / 5 + 2) * power * attacker->attack / defender->defense) / 50 + 2;
    //critical hit
    double critical = (rand() % 256 < attacker->speed / 2) ? 1.5 : 1.0;
    //random factor (85% - 100%)
    double random = (85 + rand() % 16) / 100.0;
    
    int damage = std::max(1, (int)(base * critical * random));

    return damage;
}

void Battle::checkFaint() {
    if (playerActive->isFainted()) {
        mvprintw(13, 2, "Your Pokemon Fainted!");
        if (player->hasUsablePokemon()) {
            playerActive = player->getNextPokemon();
            mvprintw(13, 2, "You send new pokemon!");
        } else {
            mvprintw(13, 2, "You lost the battle!");
            battleOver = true;
        }
    }

    if (opponentActive->isFainted()) {
        mvprintw(13, 2, "Opponent Pokemon Fainted!");
        if (opponent->hasUsablePokemon()) {
            opponentActive = opponent->getNextPokemon();
            mvprintw(13, 2, "Opponent sends new pokemon!");
        } else {
            mvprintw(13, 2, "You won the battle!");
            battleOver = true;
        }
    }
}

int Battle::playerMenu() {
    const char* options[] = {"Fight", "Bag", "Run", "Pokemon"};
    int choice = 0;
    int ch;

    while (true) {
        drawBattleScreen();

        mvprintw(14, 2, "Choose an action:");

        for (int i = 0; i < 4; i++) {
            if (i == choice) {
                attron(A_REVERSE);
                mvprintw(15 + i, 4, options[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(15 + i, 4, options[i]);
            }
        }

        refresh();
        ch = getch();

        switch (ch) {
            case KEY_UP:
                choice = (choice - 1 + 4) % 4;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % 4;
                break;
            case '\n':
                return choice;
        }
    }
}

int Battle::moveMenu() {
    int choice = 0;
    int ch;

    int numMoves = playerActive->moves.size();

    while (true) {
        drawBattleScreen();

        mvprintw(14, 2, "Choose a move:");

        for (int i = 0; i < numMoves; i++) {
            std::string name = playerActive->moves[i]->identifier;

            if (i == choice) {
                attron(A_REVERSE);
                mvprintw(15 + i, 4, name.c_str());
                attroff(A_REVERSE);
            } else {
                mvprintw(15 + i, 4, name.c_str());
            }
        }

        refresh();
        ch = getch();

        switch (ch) {
            case KEY_UP:
                choice = (choice - 1 + numMoves) % numMoves;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % numMoves;
                break;
            case '\n':
                return choice;
        }
    }
}

int Battle::switchMenu() {
    int choice = 0;
    int ch;

    int n = player->party.size();

    while (true) {
        drawBattleScreen();
        mvprintw(14, 2, "Choose Pokemon:");

        for (int i = 0; i < n; i++) {
            std::string name = player->party[i].base->identifier;

            if (i == choice) {
                attron(A_REVERSE);
                mvprintw(15 + i, 4, name.c_str());
                attroff(A_REVERSE);
            } else {
                mvprintw(15 + i, 4, name.c_str());
            }
        }

        refresh();
        ch = getch();

        switch (ch) {
            case KEY_UP:
                choice = (choice - 1 + n) % n;
                break;
            case KEY_DOWN:
                choice = (choice + 1) % n;
                break;
            case '\n':
                return choice;
        }
    }
}

void Battle::switchPokemon(Character* c) {
    GeneratedPokemon* next = c->getNextPokemon();
    if (next) {
        if (c == player) {
            playerActive = next;
        } else {
            opponentActive = next;
        }
    }
}

bool Battle::canRun() const {
    return isWild;
}

int Battle::bagMenu() {
    const char* options[] = {"Potion", "Revive", "Pokeball", "Back"};
    int choice = 0;
    int ch;

    while (true) {
        drawBattleScreen();
        mvprintw(14, 2, "Bag:");
        mvprintw(15, 2, "Potions: %d  Revives: %d  Balls: %d",
                player->potions, player->revives, player->pokeballs);

        for (int i = 0; i < 4; i++) {
            if (i == choice) {
                attron(A_REVERSE);
                mvprintw(16 + i, 4, options[i]);
                attroff(A_REVERSE);
            } else {
                mvprintw(16 + i, 4, options[i]);
            }
        }

        refresh();
        ch = getch();

        switch (ch) {
            case KEY_UP: choice = (choice - 1 + 4) % 4; break;
            case KEY_DOWN: choice = (choice + 1) % 4; break;
            case '\n': return choice;
        }
    }
}

void Battle::endBattle() {
    mvprintw(13, 2, "Battle Ends!");
}