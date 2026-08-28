//Lauren Palan

#include "pathFinding.h"

#include <stdio.h>

//check if position is in bounds
static int inBoundsTerrain(int x, int y) {
    return (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT);
}

//getting terrain movement cost
int getCost(char terrain) {
    switch (terrain) {
        case BOULDER: return INFINITE_COST;
        case PATH: return 10;
        case CENTER: return 10;
        case MART: return 10;
        case BANK: return 10;
        case CLEARING: return 10;
        case TALL_GRASS: return 20;
        case WATER: return INFINITE_COST;
        case TREE: return INFINITE_COST;
        default: return INFINITE_COST; //treat unknown terrain as impassable
    }
}

//Dijkstra's algorithm 
void shortestPath(int startx, int starty, int endx, int endy, 
                    char terrain[HEIGHT][WIDTH], int distance[HEIGHT][WIDTH], int previous[HEIGHT][WIDTH]) {
    
    //initialize distance and previous arrays
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            distance[i][j] = INFINITE_COST;
            previous[i][j] = -1;
        }
    }
    distance[starty][startx] = 0;

    //priority queue for unvisited nodes
    struct Node{
        int x, y;
        int dist;
    };

    struct Node pqueue[HEIGHT * WIDTH];
    int pqSize = 0;

    //add starting node to queue
    pqueue[pqSize++] = (Node){startx, starty, 0};

    //dijkstras main loop
    while (pqSize > 0) {
        //find node with smallest distance
        int minIndex = 0;
        for (int i = 1; i < pqSize; i++) {
            if (pqueue[i].dist < pqueue[minIndex].dist) {
                minIndex = i;
            }
        }

        Node current = pqueue[minIndex];

        //remove current node from queue
        for (int i = minIndex; i < pqSize - 1; i++) {
            pqueue[i] = pqueue[i + 1];
        }
        pqSize--;

        //if we reached the destination, stop
        if (current.x == endx && current.y == endy) {
            break;
        }

        //check neighbors
        int dx[] = {0, 1, 0, -1, 1, -1, 1, -1};
        int dy[] = {-1, 0, 1, 0, 1, 1, -1, -1};

        for (int i = 0; i < 8; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (inBoundsTerrain(newX, newY) && getCost(terrain[newY][newX]) != INFINITE_COST) {
                int newDist = current.dist + getCost(terrain[newY][newX]);
                if (newDist < distance[newY][newX]) {
                    distance[newY][newX] = newDist;
                    previous[newY][newX] = current.y * WIDTH + current.x; //store previous node as single int
                    pqueue[pqSize++] = (Node){newX, newY, newDist}; //add neighbor to queue
                }
            }
        }   
    }
}   

//update npc position based on shortest path
void updateNPCPosition(Character* npc, char terrain[HEIGHT][WIDTH],
                    int distance[HEIGHT][WIDTH], int previous[HEIGHT][WIDTH],
                    int targetx, int targety) {

    //calculate shortest path from npc to target
    shortestPath(npc->x, npc->y, targetx, targety, terrain, distance, previous);

    //if npc is already at target, do nothing
    if (npc->x == targetx && npc->y == targety) return;


    //find next step towards target
    int nextX = npc->x;
    int nextY = npc->y;

    while (previous[nextY][nextX] != -1 && inBoundsTerrain(nextX, nextY)) {
        int prevX = previous[nextY][nextX] % WIDTH;
        int prevY = previous[nextY][nextX] / WIDTH;

        if (prevX == npc->x && prevY == npc->y) break; //found next step

        nextX = prevX;
        nextY = prevY;
    }

    //update npc position
    npc->x = nextX;
    npc->y = nextY;
}

//print path from start to end using previous array
void printPath(int previous[HEIGHT][WIDTH], int startx, int starty) {
    int x = startx;
    int y = starty;

    while (previous[y][x] != -1) {
        printf("(%d, %d) -> ", x, y);
        int prevX = previous[y][x] % WIDTH;
        int prevY = previous[y][x] / WIDTH;
        x = prevX;
        y = prevY;
    }
    printf("(%d, %d)\n", x, y); //print destination
}