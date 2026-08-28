//Lauren Palan

#ifndef UI_H
#define UI_H

#include "maps.h"
#include "characters.h"
#include "pathFinding.h"
#include <ncurses.h>

void initUI(void);
void drawMap(map *m);
int handleInput(map *m, character *pc);
void endUI();
void showTrainerList(map *m, character *pc);

#endif