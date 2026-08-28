//Lauren Palan
//This is for generating the different terrain "maps"
//The terrain is represented as a 2D array of characters, where each character represents 
//a different type of terrain or feature. The main function generates the terrain and prints 
//it to the console. The terrain generation includes creating paths, placing buildings, and 
//adding various features like water, tall grass, trees, and boulders.

#include "main.h"

//main generation function
void generateTerrain(char terrain[HEIGHT][WIDTH], gate *gates, int x, int y) {
    initWalls(terrain);

    generateNSPath(terrain, gates);
    generateEWPath(terrain, gates);

    int tx = x - WORLD_SIZE / 2; //relative x coordinate to world center
    int ty = y - WORLD_SIZE / 2; //relative y coordinate to world center
    //for placing center and mart with probability
    int distance = abs(tx) + abs(ty);
    double probability;
    if (distance < 200) {
        probability = (-45.0 * distance / 200.0 + 50.0) / 100.0;
    } else {
        probability = 0.05; 
    }
    if (tx == 0 && ty == 0) {
        placeCM(terrain, CENTER);
        placeCM(terrain, MART);
    } else {
        if (rand() / (double)RAND_MAX < probability) {
            placeCM(terrain, CENTER);
        } 
        if (rand() / (double)RAND_MAX < probability) {
            placeCM(terrain, MART);
        }
    }

    char features[] = {WATER, TALL_GRASS, TREE, BOULDER};
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 4; i++) {
            placeFeature(terrain, features[i]);
        }
    }

}

//initialize walls
void initWalls(char terrain[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                terrain[y][x] = BOULDER;
            } else {
                terrain[y][x] = CLEARING; //default terrain
            }
        }
    }
}

//generate north to south path
void generateNSPath(char terrain[HEIGHT][WIDTH], gate *gates) {

    int startx, endx;
    if (gates->north != -1) {
        startx = gates->north;
    } else if (gates->south != -1) {
        startx = gates->south; 
    } else {
        //no gates exist
        startx = rand() % (WIDTH - 2) + 1; 
    }

    if (gates->south != -1) {
        endx = gates->south;
    } else {
        endx = rand() % (WIDTH - 2) + 1;
    }

    int x = startx;
    //if north gate exists, start row 0, otherwise start row 1 to avoid overwriting boulder wall
    int y = (gates->north != -1) ? 0 : 1;
    //if south gate exists, end at last row, otherwise end at second to last row
    int endy = (gates->south != -1) ? HEIGHT - 1 : HEIGHT - 2; 

    while (y < endy) {
        terrain[y][x] = PATH;

        //if on world edge, can't move, stay in place
        if (y == 0 || y == HEIGHT - 1) { 
            y++;
            continue;
        }

        if (x > endx) {
            x--;
        } else if (x < endx) {
            x++;
        }

        if (rand() % 4 ==0) {
            int change = (rand() % 2) ? 1 : -1;
            if (x + change > 0 && x + change < WIDTH - 1) {
                x += change;
            }
        }
        y++;
    }

}

//generate east to west path
void generateEWPath(char terrain[HEIGHT][WIDTH], gate *gates) {
    int starty, endy;
    //find start y coordinate
    if (gates->west !=-1) {
        starty = gates->west;
    } else if (gates->east != -1) {
        starty = gates->east;
    } else {
        starty = rand() % (HEIGHT - 2) + 1;
    }
    //find end y coordinate
    if (gates->east != 1) {
        endy = gates->east;
    } else {
        endy = rand() % (HEIGHT - 2) + 1;
    }
    
    int y = starty;
    //if west gate exists, start column 0, otherwise start column 1 to avoid overwriting boulder wall
    int x = (gates->west != -1) ? 0 : 1;
    //if east gate exists, end at last column, otherwise end at second to last column
    int endx = (gates->east != -1) ? WIDTH - 1 : WIDTH - 2;

    while (x < endx) {
        terrain[y][x] = PATH;

        //if on world edge, can't move, stay in place
        if (x == 0 || x == WIDTH - 1) { 
            x++;
            continue;
        }
        
        if (y > endy) {
            y--;
        } else if (y < endy) {
            y++;
        }

        if (rand() % 100 < 75) {
            int change = (rand() % 2) ? 1 : -1;
            if (y + change > 0 && y + change < HEIGHT - 1) {
                y += change;
            }
        }
        x++;
    }

}   

//prints the terrain to the console
void printTerrain(char terrain[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putchar(terrain[y][x]);
        }
        putchar('\n');
    }
}

//places CENTER or MART on the terrain next to the path
void placeCM(char terrain[HEIGHT][WIDTH], char building) {
    
    //set trys large to ensure placement while trying to find valid spot
    for (int trys = 0; trys < 1000; trys++) {
        int x = rand() % (WIDTH - 2) + 1;
        int y = rand() % (HEIGHT - 2) + 1;
        int horizontal = (rand() % 2); //0: horizontal, 1: vertical, for 2x1 building

        if (horizontal) { //2x1 building, (x,y) and (x+1,y)
            //check if room on terrain or boulder
            if (x+1 >= WIDTH - 1 || terrain[y][x] != CLEARING || terrain[y][x+1] != CLEARING) {
                continue;
            }

            //check if next to path
            if (!nextToPath(terrain, x, y) && !nextToPath(terrain, x+1, y)) {
                continue;
            }

            //room and next to path, place building
            terrain[y][x] = building;
            terrain[y][x+1] = building;
            break;

        } else { //1x2 building, (x,y) and (x,y+1)
            //check if room on terrain or boulder
            if (y+1 >= HEIGHT - 1 || terrain[y][x] != CLEARING || terrain[y+1][x] != CLEARING) {
                continue;
            }

            //check if next to path
            if (!nextToPath(terrain, x, y) && !nextToPath(terrain, x, y+1)) {
                continue;
            }

            //room and next to path, place building
            terrain[y][x] = building;
            terrain[y+1][x] = building;
            break;
        }
         
    }
}

//checks if a position is next to a path
int nextToPath(char terrain[HEIGHT][WIDTH], int x, int y) {
    //check all 4 directions
    if (y > 0 && terrain[y-1][x] == PATH) return 1; //up
    if (y < HEIGHT - 1 && terrain[y+1][x] == PATH) return 1; //down
    if (x > 0 && terrain[y][x-1] == PATH) return 1; //left
    if (x < WIDTH - 1 && terrain[y][x+1] == PATH) return 1; //right
    return 0;
}

//this places the different terrain features randomly
void placeFeature(char terrain[HEIGHT][WIDTH], char feature) {
    //random position to start inside outer walls
    int currx = rand() % (WIDTH - 2) + 1;
    int curry = rand() % (HEIGHT - 2) + 1;

    while (rand() % 100 < 97) { //97% chance to continue placing
        //check if position is clear
        if (terrain[curry][currx] != BOULDER && terrain[curry][currx] != PATH &&
            terrain[curry][currx] != CENTER && terrain[curry][currx] != MART &&
            terrain[curry][currx] != feature) {
            terrain[curry][currx] = feature;
        }
        
        //choose direction to move
        int direction = rand() % 4; //0: up, 1: down, 2: left, 3: right
        if (direction == 0 && curry > 1) curry--;
        else if (direction == 1 && curry < HEIGHT - 2) curry++;
        else if (direction == 2 && currx > 1) currx--;
        else if (direction == 3 && currx < WIDTH - 2) currx++;

       
    }
}