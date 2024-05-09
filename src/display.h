#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include "game.h"

#define MAIN_MENU_NEW 0
#define MAIN_MENU_LOAD 1
#define MAIN_MENU_QUIT 2

#define CY_RED 1
#define CY_BLUE 2

//Use nodelay(stdscr, true) to prevent delay using getch (returns ERR if no key is pressed)

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

void initcurses(DisplayInfo *di);
void endcurses(DisplayInfo *di);
void new_wclear(WINDOW *win);
//Dynamic way to asks for the user string in curses
void getusrstr(WINDOW*, int, int, char*, int, bool(*)(int));
//Main Menu Loop
int MainMenu(DisplayInfo *di);

void display_ui(Region *reg, Player *pl, WINDOW *win);
void init_debug_print(DisplayInfo *di, Region* reg, Player* pl);