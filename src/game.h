#ifndef GAME_HEADER
#define GAME_HEADER

#include "items.h"
#include "monster.h"
#include "mapgen.h"
#include "save.h"

//handles the movement of a player
void playermove(Region *reg, Player *pl, Pole dir, DisplayInfo* di);
void monstermove_one(Region *reg, Player *pl, Room *room, MonsterInRoom *monsterptr, Pole pole);
void monstermove(Region *reg, Player *pl, double diff);

void playerattack(Region *reg, Player *pl);

#endif