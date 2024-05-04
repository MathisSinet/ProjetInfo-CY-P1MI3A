#include "display.h"
#include <signal.h>

//Exits the program while calling endwin()
void pexit(int);
void segfault();
//New game procedure
void NewGame(Region*, Player*);
void Game(Region*, Player*);