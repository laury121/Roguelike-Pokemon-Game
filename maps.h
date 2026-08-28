//Lauren Palan


#ifndef MAPS_H
#define MAPS_H

#include "terrain.h"
#include "characters.h"
#include <string.h>

#define WORLD_SIZE 401
#define WORLD_CENTER 200
#define MAX_TRAINERS 10



typedef struct map {
    char terrain[HEIGHT][WIDTH];
    gate gates;

    character *chars[MAX_TRAINERS + 1]; //+1 for PC
    int num_chars;
} map;

void initMap(void);
map *getMap(int x, int y);
int inBounds(int x, int y);
void freeWorld();

#endif