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