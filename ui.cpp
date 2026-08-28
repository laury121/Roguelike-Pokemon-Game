//Lauren Palan

#include "ui.h"
#include "maps.h"
#include "characters.h"
#include "pathFinding.h"
#include "pokemon.h"
#include "battle.h"
#include "buildings.h"


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

void drawMap(Map* m) {
    clear(); //clear screen

    mvprintw(0, 0, "Use numpad or hjklf to move, 5 or space to wait, Q to quit"); //instructions

    //draw terrain
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            mvaddch(y+1, x, m->terrain[y][x]);
        } 
    } 
    
    //characters
    for (int i = 0; i < m->num_chars; i++) {
        Character* c = m->chars[i];
        mvaddch(c->y+1, c->x, c->symbol);
     }

    refresh(); //update screen
}

//handle user input
//sorry its big
int handleInput(Map* m, Character* pc, int& worldX, int& worldY) {
    char ch = getch();
    mvprintw(HEIGHT + 2, 0, "key pressed: %c   ", ch); //debugging
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
        case 'f': flyToMap(m, pc, worldX, worldY); return 0; //fly to map
        default: return 0; //ignore other input
    }

    int newX = pc->x + dx;
    int newY = pc->y + dy;

    //check bounds
    if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) {
        return 0; //out of bounds, ignore move
    }
    
    //handle gate transitions
    //north
    if (newY == 0 && newX == m->gates.north) {
        removeCharacter(m, pc); //remove PC from current map
        worldY--; //change world position
        m = getMap(worldX, worldY); //load new map
        pc->x = m->gates.south; //place PC at corresponding south gate of new map
        pc->y = HEIGHT - 2; //place PC just inside south gate of new map
        m->chars[m->num_chars++] = pc; //add PC to new map's character list
        return 0;
    }
    //south
    if (newY == HEIGHT - 1 && newX == m->gates.south) {
        removeCharacter(m, pc);
        worldY++;
        m = getMap(worldX, worldY);
        pc->x = m->gates.north;
        pc->y = 1;
        m->chars[m->num_chars++] = pc;
        return 0;
    }
    //west
    if (newX == 0 && newY == m->gates.west) {
        removeCharacter(m, pc);
        worldX--;
        m = getMap(worldX, worldY);
        pc->x = WIDTH - 2;
        pc->y = m->gates.east;
        m->chars[m->num_chars++] = pc;
        return 0;
    }
    //east
    if (newX == WIDTH - 1 && newY == m->gates.east) {
        removeCharacter(m, pc);
        worldX++;
        m = getMap(worldX, worldY);
        pc->x = 1;
        pc->y = m->gates.west;
        m->chars[m->num_chars++] = pc;
        return 0;
    }

    //block impassable terrain
    if (getCost(m->terrain[newY][newX]) == INFINITE_COST) {
        mvprintw(HEIGHT + 2, 0, "cant move there");
        return 0; //impassable, ignore move
    }
    //check if occupied by trainer
    for (int i = 0; i < m->num_chars; i++) {
        Character* c = m->chars[i];
        if (c != pc && c->x == newX && c->y == newY) {
            Battle b(pc, c, false);
            b.start();
            return 0; //occupied, ignore move
        }
    }

    //move PC
    pc->x = newX;
    pc->y = newY;

    // detect buildings
    char tile = m->terrain[newY][newX];
    static char prevTile = '\0'; //to track previous since C/M/B are 2x1
    if (tile == MART && prevTile != MART) {
        openPokeMart(*pc);
    }
    else if (tile == CENTER && prevTile != CENTER) {
        openPokemonCenter(*pc);
    }
    else if (tile == BANK && prevTile != BANK) {
        enterBank(*pc);
    }
    prevTile = tile;
    // detect tall grass
    if (tile == TALL_GRASS) {
        tryEncounter(m, pc, worldX, worldY);
    }


    return 0;

}

void showTrainerList(Map* m, Character* pc) {
    int start = 0;
    int ch;

    while (1) {
        clear();
        mvprintw(0, 0, "Trainers on this map (use arrow keys to scroll, esc to exit):");

        int line = 1;   //start printing below header
        int count = 0;  //num trainers displayed
        for (int i = 0; i < m->num_chars; i++) {
            Character* c = m->chars[i];

            if (c == pc) continue; //skip PC
            if (count < start) {
                count++;
                continue; //skip until we reach start index
            }

            int dx = c->x - pc->x;
            int dy = c->y - pc->y;
            const char *ns = (dy < 0) ? "north" : "south";
            const char *ew = (dx < 0) ? "west" : "east";

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

void flyToMap(Map* m, Character* pc, int& worldX, int& worldY) {
    echo();
    nocbreak();
    char input[20];
    mvprintw(HEIGHT + 2, 0, "Enter coordinates to fly to (x y): ");
    getstr(input);
    int x, y;
    
    //check input
    if (sscanf(input, "%d %d", &x, &y) != 2) {
        mvprintw(HEIGHT + 3, 0, "Invalid input. Press any key to continue.");
        getch();
        move(HEIGHT + 2, 0);
        clrtoeol();
        move(HEIGHT + 3, 0);
        clrtoeol();
        noecho();
        cbreak();
        return;
    }

    //check bounds
    if (x < -WORLD_CENTER || x >= WORLD_CENTER || y < -WORLD_CENTER || y >= WORLD_CENTER) {
        mvprintw(HEIGHT + 3, 0, "Coordinates out of bounds. Press any key to continue.");
        getch();
        move(HEIGHT + 2, 0);
        clrtoeol();
        move(HEIGHT + 3, 0);
        clrtoeol();
        noecho();
        cbreak();
        return;
    }

    //fly to new map
    removeCharacter(m, pc);
    worldX = x + WORLD_CENTER;
    worldY = y + WORLD_CENTER;
    m = getMap(worldX, worldY);
    pc->x = WIDTH / 2; //place PC in center of new map
    pc->y = HEIGHT / 2;
    m->chars[m->num_chars++] = pc;

    move(HEIGHT + 2, 0);
    clrtoeol();
    noecho();
    cbreak();
}