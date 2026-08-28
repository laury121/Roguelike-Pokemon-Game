//Lauren Palan

#include "ui.h"


void initUI(void) {
    initscr(); //initialize ncurses
    cbreak(); //disable line buffering
    noecho(); //don't echo user input
    keypad(stdscr, TRUE); //enable arrow keys
    curs_set(0); //hide cursor
}

void endUI() {
    endwin(); //end ncurses mode
}

void drawMap(map *m) {
    clear(); //clear screen

    mvprintw(0, 0, "Use numpad or hjkl to move, 5 or space to wait, Q to quit"); //instructions

    //draw terrain
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            mvaddch(y+1, x, m->terrain[y][x]);
        } 
    } 
    
    //characters
    for (int i = 0; i < m->num_chars; i++) {
        character *c = m->chars[i];
        mvaddch(c->y+1, c->x, c->symbol);
     }
    
    refresh(); //update screen
}

//handle user input
int handleInput(map *m, character *pc) {
    char ch = getch();
    mvprintw(0, 0, "key pressed: %c   ", ch); //debugging
    refresh();

    int dx = 0;
    int dy = 0;

    switch (ch) {
        case '7': case 'y': dx = -1; dy = -1; break; //up-left
        case '8': case 'k': dy = -1; break; //up
        case '9': case 'u': dx = 1; dy = -1; break; //up-right
        case '6': case 'l': dx = 1; break; //right
        case '3': case 'n': dx = 1; dy = 1; break; //down-right
        case '2': case 'j': dy = 1; break; //down
        case '1': case 'b': dx = -1; dy = 1; break; //down-left
        case '4': case 'h': dx = -1; break; //left
        case '5': case ' ': case '.': return 0; //wait
        case 'Q': return 1; //quit
        case 't': showTrainerList(m, pc); return 0; //show trainer list
        default: return 0; //ignore other input
    }

    int newX = pc->x + dx;
    int newY = pc->y + dy;

    //check bounds
    if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) {
        return 0; //out of bounds, ignore move
    }
    //block gates
    if (m->gates.north != -1 && newY == 0 && newX == m->gates.north) {
        mvprintw(0,0, "cant go through gate");
        return 0;
    }
    //block impassable terrain
    if (getCost(m->terrain[newY][newX]) == INFINITE_COST) {
        mvprintw(0,0, "cant move there");
        return 0; //impassable, ignore move
    }
    //check if occupied by trainer
    for (int i = 0; i < m->num_chars; i++) {
        character *c = m->chars[i];
        if (c != pc && c->x == newX && c->y == newY) {
            mvprintw(0,0, "pokemon battle placeholder");
            return 0; //occupied, ignore move
        }
    }

    //move PC
    pc->x = newX;
    pc->y = newY;
    return 0;

}

void showTrainerList(map *m, character *pc) {
    int start = 0;
    int ch;

    while (1) {
        clear();
        mvprintw(0, 0, "Trainers on this map (use arrow keys to scroll, esc to exit):");

        int line = 1;   //start printing below header
        int count = 0;  //num trainers displayed
        for (int i = 0; i < m->num_chars; i++) {
            character *c = m->chars[i];

            if (c == pc) continue; //skip PC
            if (count < start) {
                count++;
                continue; //skip until we reach start index
            }

            int dx = c->x - pc->x;
            int dy = c->y - pc->y;
            char *ns = (dy < 0) ? "north" : "south";
            char *ew = (dx < 0) ? "west" : "east";

            mvprintw(line, 0, "%c %d %s and %d %s", c->symbol, abs(dy), ns, abs(dx), ew);
            line++;
            count++;

            //stop for full screen
            if (line >= HEIGHT) break;
        }

        refresh();
        ch = getch();
        if (ch == 27) break; //esc to exit
        else if (ch == KEY_DOWN && start < m->num_chars - 1) start++;
        else if (ch == KEY_UP && start > 0) start--;

    }
}