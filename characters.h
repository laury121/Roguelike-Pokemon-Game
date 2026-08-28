//Lauren Palan

#ifndef CHARACTERS_H
#define CHARACTERS_H

#include "terrain.h"
#include <stdlib.h>

typedef struct map map;

typedef enum {
    PC,
    HIKER,
    RIVAL,
    PACER,
    WANDERER,
    SENTRY,
    EXPLORER
} trainer_type;

typedef struct character{
    int x, y;
    char symbol;
    trainer_type type;
    int next_turn;
    int direction;
} character;


character *createPC(map *m);
character *createTrainer(map *m, trainer_type type);
void spawnTrainers(map *m, int numTrainers);
void moveNPC(map *m, character *pc);
void printNewMap(map *m);

#endif