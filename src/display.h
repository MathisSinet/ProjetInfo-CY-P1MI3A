#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include "game.h"

#define MAIN_MENU_NEW 0
#define MAIN_MENU_LOAD 1
#define MAIN_MENU_QUIT 2

//Use nodelay(stdscr, true) to prevent delay using getch (returns ERR if no key is pressed)

//test
void HelloWorld();
//Dynamic way to asks for the user string in curses
void getusrstr(int, int, char*, int, bool(*)(int));
//Main Menu Loop
int MainMenu();

void init_debug_print(Region*);