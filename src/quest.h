#ifndef QUEST_HEADER
#define QUEST_HEADER

#include "gamebasic.h"
#include "display.h"

bool quizz(Player* pl, Region* reg, DisplayInfo* di, uint8_t quizz_id);
bool check_quests(Region* reg, Player* pl);

#endif