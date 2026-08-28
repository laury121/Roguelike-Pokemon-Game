//Lauren Palan

#ifndef TERRAIN_H
#define TERRAIN_H




#define WIDTH 80
#define HEIGHT 21

#define BOULDER '%'
#define PATH '#'
#define CENTER 'C'
#define MART 'M'
#define WATER '~'
#define TALL_GRASS ':'
#define CLEARING '.'
#define TREE '^'

typedef struct gate {
    int north;
    int south;
    int east;
    int west;
} gate;

void initWalls(char terrain[HEIGHT][WIDTH]);
void generateNSPath(char terrain[HEIGHT][WIDTH], gate *gates);
void generateEWPath(char terrain[HEIGHT][WIDTH], gate *gates);
void generateTerrain(char terrain[HEIGHT][WIDTH], gate *gates, int x, int y);
void printTerrain(char terrain[HEIGHT][WIDTH]);

int nextToPath(char terrain[HEIGHT][WIDTH], int x, int y);
void placeCM(char terrain[HEIGHT][WIDTH], char building);
void placeFeature(char terrain[HEIGHT][WIDTH], char feature);




#endif