//Lauren Palan

#include "characters.h"
#include "maps.h"
#include "pathFinding.h"
#include <stdlib.h>
#include <stdio.h>


character *createPC(map *m) {
    character *pc = malloc(sizeof(character));
    if (pc == NULL) return NULL; //malloc failed

    pc->type = PC;
    pc->symbol = '@';
    pc->next_turn = 0;
    pc->direction = 0;

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
    return pc;
}

character *createTrainer(map *m, trainer_type type) {
    character *t = malloc(sizeof(character));
    if (t == NULL) return NULL; //malloc failed

    t->type = type;
    t->next_turn = 0;
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

    return t;
}

void spawnTrainers(map *m, int numTrainers) {
    //m->num_chars = 0; //reset character count
    //createPC(m); //add PC first

    //at least 1 hiker and 1 rival
    createTrainer(m, HIKER);
    createTrainer(m, RIVAL);

    //spawn remaining trainers randomly
    for (int i = 0; i < numTrainers - 2; i++) {
        trainer_type type = (trainer_type)(rand() % 6 + 1); //random type excluding PC
        createTrainer(m, type);
    }

}

void printNewMap(map *m) {
    char display[HEIGHT][WIDTH];

    //copy terrain to display
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            display[y][x] = m->terrain[y][x];
        }
    }

    //add characters to display
    for (int i = 0; i < m->num_chars; i++) {
        character *c = m->chars[i];
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

void moveNPC(map *m, character *pc) {
    int distance[HEIGHT][WIDTH];
    int previous[HEIGHT][WIDTH];

    for (int i = 1; i < m->num_chars; i++) {
        character *c = m->chars[i];
        if (c->type == PC) continue; //skip PC

        updateNPCPosition(c, m->terrain, distance, previous, pc->x, pc->y);
    }
}