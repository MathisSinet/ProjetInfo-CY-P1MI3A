#include "quest.h"

//Performs a quizz
/// @return true if quizz was completed else false
bool quizz(Player* pl, Region* reg, DisplayInfo* di, uint8_t quizz_id)
{
    WINDOW *win = di->box1;
    nodelay(win, false);
    new_wclear(win);
    mvwaddwstr(win, 1, 1, L"Vous avez trouvé un équipement informatique défaillant de la station !");
    mvwaddwstr(win, 2, 1, L"Retrouvez les valeurs affichées par ce programme pour corriger le code défaillant");

    wgetch(win);
    nodelay(win, true);
    return false;
}

bool check_quests(Region* reg, Player* pl)
{
    return false;
}