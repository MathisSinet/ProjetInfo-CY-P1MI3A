/*
main.h
Main file of the program
*/

#include "game.h"
#include "display.h"


void pexit(DisplayInfo*, int);
void segfault();
void aborthandler();

void NewGame(DisplayInfo *, Region*, Player*);
bool loadSave(DisplayInfo *di, Region *reg, Player *pl);
void Game(DisplayInfo*, Region*, Player*);