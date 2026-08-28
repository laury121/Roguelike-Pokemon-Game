//Lauren Palan


#ifndef MAPS_H
#define MAPS_H

#include "terrain.h"
#include "characters.h"
#include <cstring>
#include <cstdlib>

#define WORLD_SIZE 401
#define WORLD_CENTER 200
#define MAX_TRAINERS 10



class Map {
public:
    char terrain[HEIGHT][WIDTH];
    Gate gates;

    Character* chars[MAX_TRAINERS + 1]; //+1 for PC
    int num_chars;

    Character* npcQueue[MAX_TRAINERS]; //queue of NPCs for turn order, sorted by next_turn
    int npcQueueSize;

    //constructor
    Map() : num_chars(0), npcQueueSize(0) {
        memset(terrain, 0, sizeof(terrain));
        memset(chars, 0, sizeof(chars));
        memset(npcQueue, 0, sizeof(npcQueue));
    }

    void addNPCToQueue(Character* npc) {
        chars[npcQueueSize] = npc;
        npcQueue[npcQueueSize++] = npc;
    }

    //deconstructor
    ~Map() {
        //free characters
        for (int i = 0; i < num_chars; i++) {
            if(chars[i]->isNPC()) delete chars[i];
        }
    }
};

void initMap(void);
Map* getMap(int x, int y);
int inBounds(int x, int y);
void freeWorld();

#endif