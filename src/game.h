#ifndef GAME_HEADER
#define GAME_HEADER

#include "items.h"
#include "monster.h"
#include "mapgen.h"
#include "save.h"

//handles the movement of a player
void playermove(Region *reg, Player *pl, Pole dir, DisplayInfo* di);

#endif