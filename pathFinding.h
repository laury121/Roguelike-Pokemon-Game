//Lauren Palan

#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "terrain.h"
#include "characters.h"

#define INFINITE_COST 999 //represents unreachable


typedef struct NPC{
    int row, col;
    int type;
} NPC;

int getCost(char terrain);

void calculateShortestPath(int startx, int starty, int endx, int endy, char terrain[HEIGHT][WIDTH],
                    int distance[HEIGHT][WIDTH], int previous[HEIGHT][WIDTH]);
void updateNPCPosition(character *npc, char terrain[HEIGHT][WIDTH], int distance[HEIGHT][WIDTH], 
                    int previous[HEIGHT][WIDTH], int targetx, int targety);
void printPath(int previous[HEIGHT][WIDTH], int startx, int starty);

#endif 
