/*
DISPLAY CODE FOR THE GAME
*/

#define _XOPEN_SOURCE 700
#include <ncursesw/ncurses.h>
#include <locale.h>
#ifndef GAMEBASIC
    #include "game.h"
#endif
#include "save.h"

#define MAIN_MENU_NEW 0
#define MAIN_MENU_LOAD 1
#define MAIN_MENU_QUIT 2

//Use nodelay(stdscr, true) to prevent delay using getch (returns ERR if no key is pressed)

typedef enum
{
    CY_RED = 1,
    CY_BLUE = 2,
    CY_CYAN = 3
}
ColorPair;

typedef struct DisplayInfo
{
    int width;
    int height;
    WINDOW *box1;
    WINDOW *box2;
    WINDOW *box3;
}
DisplayInfo;

void us_sleep(uint64_t us);
void new_wclear(WINDOW *win);

void initcurses(DisplayInfo *di);

void init_mainmenu(DisplayInfo *di);
void end_mainmenu(DisplayInfo *di);
void init_gameui(DisplayInfo *di);
void end_gameui(DisplayInfo *di);

void endcurses(DisplayInfo *di);

//Dynamic way to asks for the user string in curses
void getusrstr(WINDOW* win, int y, int x, char* buffer, int max_len, bool(*validatefunc)(int));
//Main Menu Loop
int MainMenu(DisplayInfo *di);

void right_panel_update(Region *reg, Player *pl, WINDOW *win);
void show_controls(DisplayInfo *di);
void save_ui(DisplayInfo *di, Region *reg, Player *pl);
void update_map(DisplayInfo *di, Region* reg, Player* pl);