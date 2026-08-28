//Lauren Palan

#include "characters.h"
#include "pathfinding.h"
#include "maps.h"
#include "pokemon.h"
#include <cstdlib>
#include <cstdio>


Character* createPC(Map* m) {
    Character* pc = new PC();
    if (pc == NULL) return NULL;

    while (1) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

        if (m->terrain[y][x] == PATH) {
            pc->x = x;
            pc->y = y;

            if (m->num_chars < MAX_TRAINERS + 1) {
                m->chars[m->num_chars++] = pc; //add PC to map's character list
            }

            break;
        }
    }

    //give starting stuff
    GeneratedPokemon starter = generatePokemon(0, 0);
    starter.currentHp = starter.maxHp;
    pc->party.push_back(starter);
    pc->activeIndex = 0;
    pc->potions = 3;
    pc->revives = 1;
    pc->pokeballs = 3;

    return pc;
}

Character* createTrainer(Map* m, TrainerType type) {
    Character* t = new NPC(0, 0, '?', type);

    t->direction = rand() % 8; //random initial direction

    //set symbol based on type
    switch (type) {
        case HIKER: t->symbol = 'h'; break;
        case RIVAL: t->symbol = 'r'; break;
        case PACER: t->symbol = 'p'; break;
        case WANDERER: t->symbol = 'w'; break;
        case SENTRY: t->symbol = 's'; break;
        case EXPLORER: t->symbol = 'e'; break;
        default: t->symbol = '?'; break; //should never happen
    }

    //find valid position
    while (1) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

        //check valid terrain
        if (getCost(m->terrain[y][x]) == INFINITE_COST) {
            continue; //invalid terrain, try again
        }

        //check position is occupied
        int occupied = 0;
        for (int i = 0; i < m->num_chars; i++) {
            if (m->chars[i]->x == x && m->chars[i]->y == y) {
                occupied = 1;
                break;
            }
        }

        if (occupied) continue;

        t->x = x;
        t->y = y;

        if (m->num_chars < MAX_TRAINERS + 1) {
            m->chars[m->num_chars++] = t; //add trainer to map's character list
        }
        break;
    }

    //give pokemon to trainer
    int numPokemon = 1 + rand() % 3;
    for (int i = 0; i < numPokemon; i++) {
        GeneratedPokemon p = generatePokemon(0, 0);
        p.currentHp = p.maxHp;
        t->party.push_back(p);
    }
    t->activeIndex = 0;

    return t;
}

void spawnTrainers(Map* m, int numTrainers) {

    //at least 1 hiker and 1 rival
    createTrainer(m, HIKER);
    createTrainer(m, RIVAL);

    //spawn remaining trainers randomly
    for (int i = 0; i < numTrainers - 2; i++) {
        TrainerType type = (TrainerType)(rand() % 6 + 1); //random type excluding PC
        createTrainer(m, type);
    }

}

void printNewMap(Map* m) {
    char display[HEIGHT][WIDTH];

    //copy terrain to display
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            display[y][x] = m->terrain[y][x];
        }
    }

    //add characters to display
    for (int i = 0; i < m->num_chars; i++) {
        Character* c = m->chars[i];
        display[c->y][c->x] = c->symbol;
    }

    //print display
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(display[y][x]);
        }
        putchar('\n');
    }
}

void moveNPC(Map* m, Character* pc) {
    int distance[HEIGHT][WIDTH];
    int previous[HEIGHT][WIDTH];

    for (int i = 1; i < m->num_chars; i++) {
        Character* c = m->chars[i];
        if (c->type == PCTYPE) continue; //skip PC

        updateNPCPosition(c, m->terrain, distance, previous, pc->x, pc->y);
    }
}

void removeCharacter(Map* m, Character* c) {
    //find character in map's character list
    for (int i = 0; i < m->num_chars; i++) {
        if (m->chars[i] == c) {
            //remove character from list and shift remaining characters down
            for (int j = i; j < m->num_chars - 1; j++) {
                m->chars[j] = m->chars[j + 1];
            }
            //set last character to nullptr and decrease count
            m->chars[m->num_chars - 1] = nullptr;
            m->num_chars--;
            break;
        }
    }
}

void removePC(Character* c) {
    for(int y = 0; y < WORLD_SIZE; y++) {
        for(int x = 0; x < WORLD_SIZE; x++) {
            Map* m = getMap(x, y);
            if (m) {
                for (int i = 0; i < m->num_chars; i++) {
                    if (m->chars[i] == c) {
                        //remove PC from list and shift remaining characters down
                        for (int j = i; j < m->num_chars - 1; j++) {
                            m->chars[j] = m->chars[j + 1];
                        }
                        //set last character to nullptr and decrease count
                        m->chars[m->num_chars - 1] = nullptr;
                        m->num_chars--;
                        return;
                    }
                }
            }
        }
    }
}

GeneratedPokemon* Character::getActivePokemon() {
    if (party.empty()) return nullptr;
    return &party[activeIndex];
}

bool Character::hasUsablePokemon() {
    for (auto& p : party) {
        if (!p.isFainted()) return true;
    }
    return false;
}

GeneratedPokemon* Character::getNextPokemon() {
    for (auto& p : party) {
        if (!p.isFainted()) return &p;
    }
    return nullptr;
}

int Character::getNextPokemonIndex() {
    for (size_t i = 0; i < party.size(); i++) {
        if (!party[i].isFainted()) return i;
    }
    return -1;
}