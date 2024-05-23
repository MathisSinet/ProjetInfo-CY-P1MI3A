#ifndef QUEST_HEADER
#define QUEST_HEADER

#include "gamebasic.h"
#include "display.h"

bool isvalid_quizz1_answer_chr(int chr);

bool quizz(Player* pl, Region* reg, DisplayInfo* di, uint8_t quizz_id);
bool check_quests(Region* reg, Player* pl);
bool quizz1(Player* pl, Region* reg, WINDOW* win);

#endif