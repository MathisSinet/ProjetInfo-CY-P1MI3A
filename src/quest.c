#include "quest.h"

void quizz(Player* pl, Region* reg, DisplayInfo* di)
{
    mvwprintw(di->box2, 1, 2, "Test");
    wrefresh(di->box2);
    wgetch(di->box2);
}