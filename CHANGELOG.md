# Changelog
# Created by: Lauren Palan

# Version 1.01

## started 01/29/2026
    - wrote assignment description on README
    - initialized main, terrain, and header
    - wrote makefile for such files

## added 02/01/2026
    - defined terrain size and symbols in terrain header
    - wrote functions for generating paths and outer walls with everything else clearing

## added 02/03/2026
    - generating 2x1 pokemart and center touching the path
    - placing other symbols function
## fixed
    - segmentation fault bug

## edited 02/04/2026
    - finished readme
    - changed some of my comments throughout 


# Version 1.02

## added 02/09/2026
    - file description on terrain.c
    - maps.c/maps.h
## updated
    - made sure all loops ran x,y instead of i,j
    - updated makefile to reflect new added files

## added 02/10/2026
    - umbrella header for future additional files for main
## updated
    - rewrote main to be input collecting loop
    - getMap() to enforce world edges
    - generateTerrain() and pathGeneration()s to take gates parameter
    - moved gates struct to terrain.h instead of in maps.h
    - center and mart probability in generateTerrain()


# Version 1.03

## added 02/12/2026
    - basic outline files pathfinding.c and pathfinding.h

## added 02/16/2026
    - dijkstras algorithm 

## added 02/20/2026
    - 

# Version 1.04

## added 03/01/2026
    - characters header file
    - character and num chars fields to map struct in maps.h

# Version 1.05

## added 03/20/2026
    - ui header file
    - ui.c file

## fixed 03/23/2026
    - circular dependencies in header files bug
    - opening/printing the map and stuff
## added 
    - trainer list stuff
    - README

# Version 1.06

## changed 03/27/2026
    - converted file name from .c to .cpp
    - updated makefile for cpp

## fixed 03/29/2026
    - started just running make and fixing errors as they arrise:
        > changed malloc(sizeof()) in maps and character to (maps *) malloc(...) and (character *)
        > in ui changed char *ns = (dy < 0) ? north : south; to const char, and same for *ew
    
    - changed structs in headers to classes
    - updated function declarations and original malloc changes to reflect classes and subclasses

    - fixed gate printing issue 

## added 03/31/2026
    - moving through gates in ui.cpp handle input
    - removing character for pc changing map changing in characters.cpp

    - updated free world to not delete pc
    - added isNPC() check to character class 
    - added removePC() to make sure the pointer isnt pointing to any map
    - delete pc seperate from free world

    - flying maps to input handling and fly function
    - printing map world coords

    - npc queue to map class 


# Version 1.07

## added 04/06/2026
    - wrote parser for pokemon csv
    - wrote get database function
    - pokemon.h for structs

## added 04/07/2026
    - made csv parser for all files to use instead of each file getting new parser
    - rewrote pokemon csv parser to just input table data to struct
    - wrote the other parsing functions


# Version 1.09

## added 04/21/2026
    - battle.cpp and battle.h
    - current and max Hp to generated pokemon struct
    - also isFainted, takeDamage, and heal to generated pokemon struct

## added 04/22/2026
    - inventory and pokemon party to character class
    - is fainted, take damage, heal, revive to generated pokemon struct
## fixed
    - the battle screen layout

# Version 1.10

## added 04/28/2026
    - new building type B for banks
    - building detection in input handeling
    - buildings.cpp for entering buildings: openPokeMart, openPokemonCenter, and enterBank

## fix
    - inventory consumption, before when you use balls, heals, etc. player inventory never decreased
    - bag menu back button inside battles
    - revive rules inside battles
    - executing battle actions more clear to the player as to what is happening

## added 04/29/2026
    - minigames into the bank for earning money
    - free healing, viewing party, and switching lead pokemon into pokemon centers

## fix
    - different screens allignments and stuff