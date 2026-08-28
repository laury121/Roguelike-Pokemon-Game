//Lauren Palan
//This is for connecting multiple terrain maps together.
//And saving the randomly generated maps in a 2D array of pointers to maps, 
//representing the entire world of maps.

#include "maps.h"


//2D array of pointers to maps, representing the entire world of maps
static map *world[WORLD_SIZE][WORLD_SIZE];

//initialize the world of maps to NULL
void initMap(void) {
    for (int y = 0; y < WORLD_SIZE; y++) {
        for (int x = 0; x < WORLD_SIZE; x++) {
            world[y][x] = NULL;
        }
    }
}

//bounds check
static int inBoundsWorld(int x, int y) {
    return x >= 0 && x < WORLD_SIZE && 
        y >= 0 && y < WORLD_SIZE;
}



//returns pointer to map at given coordinates, creating it if it doesn't exist yet
map *getMap(int x, int y) {
    //check if coordinates are in bounds of world
    if (!inBoundsWorld(x, y)) {
        return NULL;
    }

    //if map exists, return it
    if (world[y][x] != NULL) return world[y][x];

    //if map doesn't exist yet, create it
    map *newMap = malloc(sizeof(map));
    if (newMap == NULL) return NULL; //malloc failed
    
    //initialize gates to -1
    memset(&newMap->gates, -1, sizeof(gate)); 
    //check neighbors for gates and update accordingly
    //checkNeighbor(x, y, &newMap->gates); 
    
    //if no neighboring gates, randomly generate gates
    if (y == 0) {
       newMap->gates.north = -1; //no gate if on world edge
    } else if (world[y - 1][x]) {
        //if north neighbor exists, set north gate to match south gate of neighbor
        newMap->gates.north = world[y - 1][x]->gates.south;
    } else {
        newMap->gates.north = rand() % (WIDTH - 2) + 1;
    }

    if (y == WORLD_SIZE - 1) {
        newMap->gates.south = -1;
    } else if (world[y + 1][x]) {
        newMap->gates.south = world[y + 1][x]->gates.north;
    } else {
        newMap->gates.south = rand() % (WIDTH - 2) + 1;
    }

    if (x == 0) {
        newMap->gates.west = -1;
    } else if (world[y][x - 1]) {
       newMap->gates.west = world[y][x - 1]->gates.east;
    } else {
    newMap->gates.west = rand() % (HEIGHT - 2) + 1;
    }
    
    if (x == WORLD_SIZE - 1) {
        newMap->gates.east = -1;
    } else if (world[y][x + 1]) {
        newMap->gates.east = world[y][x + 1]->gates.west;
    } else {
        newMap->gates.east = rand() % (HEIGHT - 2) + 1;
    }

    
    //generate terrain for new map
    generateTerrain(newMap->terrain, &newMap->gates, x, y); 
    //store new map
    world[y][x] = newMap;

    return newMap;
}

void freeWorld() {
    for (int y = 0; y < WORLD_SIZE; y++) {
        for (int x = 0; x < WORLD_SIZE; x++) {
            if (world[y][x]) {
                free(world[y][x]);
                world[y][x] = NULL;
            }
        }
    }
}