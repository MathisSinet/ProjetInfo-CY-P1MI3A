/*
game.h
Contains functions to allow the player and monsters to move and attack
Also contains the item pickup code
*/

#ifndef GAME_HEADER
#define GAME_HEADER

#include "items.h"
#include "monster.h"
#include "mapgen.h"
#include "save.h"
#include "quest.h"

// Handles the movement of a player
void playermove(Region *reg, Player *pl, Pole dir, DisplayInfo* di);
void monstermove_one(Region *reg, Player *pl, Room *room, MonsterInRoom *monsterptr, Pole pole);
void monstermove_random(Region *reg, Player *pl, Room *room, MonsterInRoom *monsterptr);
void monstermove_towardsplayer(Region *reg, Player *pl, Room *room, MonsterInRoom *monsterptr);
void monstermove(Region *reg, Player *pl, double diff);

void playerattack(Region *reg, Player *pl);
void monsterattack_one(Region *reg, Player *pl, MonsterInRoom *monster, double diff);
void monsterattack(Region *reg, Player *pl, double diff);

void death(Region *reg, Player *pl, DisplayInfo* di, int cause_of_death);
void win(Region *reg, Player *pl, DisplayInfo* di);

#endif