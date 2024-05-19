#include "game.h"
#include "display.h"


//Exits the program while calling endwin()
void pexit(DisplayInfo*, int);
void segfault();
void aborthandler();
//New game procedure
void NewGame(DisplayInfo *, Region*, Player*);
bool loadSave(DisplayInfo *di, Region *reg, Player *pl);
void Game(DisplayInfo*, Region*, Player*);