#include "display.h"
#include <signal.h>

//Exits the program while calling endwin()
void pexit(DisplayInfo*, int);
void segfault();
//New game procedure
void NewGame(DisplayInfo *, Region*, Player*);
void Game(DisplayInfo*, Region*, Player*);