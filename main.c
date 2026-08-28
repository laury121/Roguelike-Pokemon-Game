//Lauren Palan
//main driver for all files
//right now it prints the terrain and prompts user for input to move map to map



#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

#include <unistd.h>  
//#include <windows.h>


int main() {
    printf("Welcome to the Pokemon Game!\n");
    srand(time(NULL)); 
    
    //initialize world of maps
    initMap();
    int x = WORLD_CENTER;
    int y = WORLD_CENTER;
    map *m = getMap(x, y);

    //spawn PC and trainers
    character *pc = createPC(m);
    spawnTrainers(m, 10);
    
    //main game loop
    initUI();
    int quit = 0;
    while (!quit) {
        drawMap(m);
        quit = handleInput(m, pc);
        moveNPC(m, pc);
    }

    endUI(); 
    
    freeWorld();
    
    return 0;
}


















