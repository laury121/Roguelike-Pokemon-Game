
#include "buildings.h"
#include <ncurses.h>
#include <cstdlib>

void openPokeMart(Character& player) {
    int choice;

    while (true) {
        clear();

        mvprintw(0, 0, "Welcome to the Pokemart!");

        mvprintw(1, 30, "INVENTORY:");
        mvprintw(2, 30, "Money: $%d", player.money);
        mvprintw(3, 30, "Pokeballs: %d", player.pokeballs);
        mvprintw(4, 30, "Potions: %d", player.potions);
        mvprintw(5, 30, "Revives: %d", player.revives);

        mvprintw(3, 1, "1. Pokeball ($75)");
        mvprintw(4, 1, "2. Potion ($250)");
        mvprintw(5, 1, "3. Revive ($500)");
        mvprintw(6, 1, "4. Exit");

        refresh();

        int cost = 0;
        choice = getch();
        if (choice == '4') break;
        if (choice == '1') cost = 75;
        else if (choice == '2') cost = 250;
        else if (choice == '3') cost = 500;
        else continue; //ignore invalid input

        if (cost > 0 && player.money >= cost) {
            player.money -= cost;
            if (choice == '1') player.pokeballs++;
            else if (choice == '2') player.potions++;
            else if (choice == '3') player.revives++;
            mvprintw(8, 1, "Purchased!");
        } else {
            mvprintw(8, 1, "Not enough money.");
        }

        refresh();
        getch(); // pause so player sees result
    }
}

void openPokemonCenter(Character& player) {
    while (true) {
        clear();

        mvprintw(0, 0, "Pokémon Center");

        mvprintw(2, 1, "1. Heal Pokémon");
        mvprintw(3, 1, "2. View Party");
        mvprintw(4, 1, "3. Switch Lead");
        mvprintw(5, 1, "4. Exit");

        refresh();

        char c = getch();

        if (c == '1') {
            for (auto& p : player.party)
                p.currentHp = p.maxHp;

            mvprintw(7, 1, "Your Pokémon have been healed!");
            refresh();
            getch();
        }
        else if (c == '2') {
            viewParty(player);
        }
        else if (c == '3') {
            switchLead(player);
        }
        else if (c == '4') {
            break;
        }
    }
}

void viewParty(Character& player) {
    clear();

    mvprintw(3, 1, "Your Party:");

    for (int i = 0; i < player.party.size(); i++) {
        auto& p = player.party[i];

        mvprintw(4 + i, 2, "%d. %s HP: %d/%d",
            i + 1, p.base->identifier.c_str(), p.currentHp, p.maxHp);
    }

    refresh();
    getch();
}

void switchLead(Character& player) {
    int choice;

    clear();
    mvprintw(3, 1, "Choose lead Pokémon:");

    for (int i = 0; i < player.party.size(); i++) {
        mvprintw(4 + i, 2, "%d. %s", i + 1,
            player.party[i].base->identifier.c_str());
    }

    refresh();
    scanw("%d", &choice);

    if (choice > 0 && choice <= player.party.size()) {
        player.activeIndex = choice - 1;
    }
}

void enterBank(Character& player) {
    while (true) {
        clear();

        mvprintw(0, 0, "Welcome to the Bank!");
        mvprintw(1, 1, "You have: $%d", player.money);
        
        mvprintw(4, 0, "1. Risk Ladder");
        mvprintw(5, 0, "2. Slots");
        mvprintw(6, 0, "3. Guessing Game");
        mvprintw(7, 0, "4. Exit");

        refresh();

        char c = getch();

        if (c == '1') riskLadder(player);
        else if (c == '2') slotsGame(player);
        else if (c == '3') guessingGame(player);
        else if (c == '4') break;
    }
}

void guessingGame(Character& player) {
    clear();

    mvprintw(3, 1, "Guess a number (1-5): ");

    refresh();

    int guess = getch() - '0';
    int target = rand() % 5 + 1;

    if (guess == target) {
        player.money += 50;
        mvprintw(5, 1, "Correct! +$50");
        refresh();
        getch();
        return;
    } else {
        player.money += 10;
        mvprintw(5, 1, "Wrong! It was %d. +$10", target);
        refresh();
        getch();
        return;
    }
}

void riskLadder(Character& player) {
    int bet;
    int multiplier = 1;

    clear();
    mvprintw(0, 0, "Risk Ladder");

    mvprintw(2, 1, "Enter bet amount: ");
    refresh();
    scanw("%d", &bet);

    if (bet <= 0 || bet > player.money) return;
    player.money -= bet; //take money right away

    int choice;

    while (true) {
        clear();

        mvprintw(2, 1, "Current multiplier: x%d", multiplier);
        mvprintw(3, 1, "Potential winnings: $%d", bet * multiplier);

        mvprintw(5, 1, "[1] Climb higher");
        mvprintw(6, 1, "[2] Cash out");
        mvprintw(7, 1, "Choose: ");
        refresh();

        choice = getch();

        if (choice == '2') {
            int winnings = bet * multiplier;
            player.money += winnings;
            mvprintw(9, 1, "You cashed out $%d!", bet * multiplier);
            refresh();
            getch();
            return;
        }

        // risk step ( choice == 1)
        int chance = 70 - (multiplier * 10); // gets harder

        if (rand() % 100 < chance) {
            multiplier++;
        } else {
            mvprintw(9, 1, "You lost everything!");
            refresh();
            getch();
            return;
        }
    }
}

void slotsGame(Character& player) {
    int bet;

    clear();
    mvprintw(0, 0, "Slots Machine");
    mvprintw(2, 1, "Enter bet: ");
    refresh();
    scanw("%d", &bet);

    if (bet <= 0 || bet > player.money) return;
    player.money -= bet;

    const char* symbols[] = {"CHERRY", "STAR", "GEM", "SEVEN"};

    int a = rand() % 4;
    int b = rand() % 4;
    int c = rand() % 4;

    clear();
    mvprintw(3, 1, "Slots:");

    mvprintw(4, 1, "[ %s ] [ %s ] [ %s ]",
        symbols[a], symbols[b], symbols[c]);

    int payout;

    if (a == b && b == c) {
        payout = bet * 5;
        mvprintw(6, 1, "JACKPOT! +$%d", payout);
    }
    else if (a == b || b == c || a == c) {
        payout = bet * 2;
        mvprintw(6, 1, "Small win +$%d", payout);
    }
    else {
        payout = 0;
        mvprintw(6, 1, "You lost $%d", bet);
    }

    player.money += payout;

    refresh();
    getch();
}