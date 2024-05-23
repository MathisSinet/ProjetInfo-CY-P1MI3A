/*
display.h
Display features of the game
*/

//Minimum terminal size : 70x30
//Recommended terminal size : 100x35


#ifndef DISPLAY_HEADER
#define DISPLAY_HEADER

#include <locale.h>
#include "game.h"

#define MAIN_MENU_NEW 0
#define MAIN_MENU_LOAD 1
#define MAIN_MENU_QUIT 2

#define RARROW_SYMB L"▶"
#define CHARACTER_SYMB L"ඞ "
#define WALL_SYMB L"🧱"
#define DOOR_SYMB L"🚪"

#define COLOR_GREY 585

//Use nodelay(stdscr, true) to prevent delay using getch (returns ERR if no key is pressed)

//Constants for color pairs
typedef enum
{
    PAIR_RED = 1,
    PAIR_BLUE = 2,
    PAIR_CYAN = 3,
    PAIR_GREY = 4,
    PAIR_GREEN = 5
}
ColorPair;

void us_sleep(uint64_t us);
void new_wclear(WINDOW *win);

void initcurses(DisplayInfo *di);

void init_mainmenu(DisplayInfo *di);
void end_mainmenu(DisplayInfo *di);
void init_gameui(DisplayInfo *di);
void end_gameui(DisplayInfo *di);
void item_desc(DisplayInfo *di, char* chaine);
void lore_screen(DisplayInfo *di, WINDOW* lore_box);
void death_screen(DisplayInfo *di, Player *pl, int cause_of_death);
void win_screen(DisplayInfo *di, Player *pl);


//Dynamic way to asks for the user string in curses
void getusrstr(WINDOW* win, int y, int x, char* buffer, int max_len, bool(*validatefunc)(int));
//Main Menu Loop
int MainMenu(DisplayInfo *di);

void right_panel_update(Region *reg, Player *pl, WINDOW *win);
void bottom_panel_update(Region *reg, Player *pl, WINDOW *win);
void manage_inventory(Region *reg, Player *pl, DisplayInfo *di);

void show_controls(DisplayInfo *di);

void save_ui(DisplayInfo *di, Region *reg, Player *pl);
void update_map(DisplayInfo *di, Region *reg, Player *pl);

#endif