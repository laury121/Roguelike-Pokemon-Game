//Lauren Palan

#ifndef UI_H
#define UI_H

#include <ncurses.h>

class Map;
class Character;

void initUI(void);
void drawMap(Map* m);
int handleInput(Map* m, Character* pc, int& worldX, int& worldY);
void endUI();
void showTrainerList(Map* m, Character* pc);
void flyToMap(Map* m, Character* pc, int& worldX, int& worldY);

#endif