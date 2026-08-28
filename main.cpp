//Lauren Palan
//main driver for all files
//stuff



#include "main.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <filesystem>

//#include <unistd.h>  

std::vector<Pokemon> pokemonList;
std::vector<PokemonMoves> pokemonMovesList;
std::vector<Move> moveList;
std::vector<PokemonStats> pokemonStatsList;



int main() {

    printf("Welcome to the Pokemon Game!\n");
    std::srand(std::time(nullptr)); 
    
    //load pokemon
    std::string path = getDatabasePath();
    pokemonList = pokemonCSV(path + "/pokemon.csv");
    pokemonMovesList = pokemonMovesCSV(path + "/pokemon_moves.csv");
    moveList = movesCSV(path + "/moves.csv");
    pokemonStatsList = pokemonStatsCSV(path + "/pokemon_stats.csv");

    //initialize world of maps
    initMap();
    int worldX = WORLD_CENTER;
    int worldY = WORLD_CENTER;
    Map* m = getMap(worldX, worldY);

    //spawn PC and trainers
    Character* pc = createPC(m);
    //m->chars[m->num_chars++] = pc; //add PC to map's character list
    spawnTrainers(m, 10);
    
    //main game loop
    initUI();
    bool quit = false;
    while (!quit) {
        m = getMap(worldX, worldY); //reload current map in case we changed maps
        drawMap(m); //redraw map each turn to update character positions
        mvprintw(HEIGHT + 1, 0, "Map coordinates: (%d, %d)   ", worldX - WORLD_CENTER, worldY - WORLD_CENTER); //display current map coordinates
        
        refresh();
        quit = handleInput(m, pc, worldX, worldY); //handle user input and update PC position, also handles map transitions
        moveNPC(m, pc); //move NPCs towards PC each turn using pathfinding
    }

    endUI(); 
    //free memory
    removePC(pc);
    freeWorld();
    delete pc; //free PC character
    printf("Thanks for playing!\n");

    
    return 0;
}


















