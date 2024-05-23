#include "quest.h"

//Performs a quizz
/// @return true if quizz was completed else false
bool quizz(Player* pl, Region* reg, DisplayInfo* di, uint8_t quizz_id)
{
    WINDOW *win = di->box1;
    nodelay(win, false);
    new_wclear(win);
    mvwaddwstr(win, 1, 1, L"Vous avez trouvé un équipement informatique défaillant de la station !");
    mvwaddwstr(win, 2, 1, L"Retrouvez les valeurs affichées par ce programme pour corriger le code défaillant :");

    wgetch(win);
    nodelay(win, true);
    return false;
}

bool check_quests(Region* reg, Player* pl)
{
    return false;
}

bool quizz1(Player* pl, Region* reg, WINDOW* win)
{
    char pl_answer1[5], pl_answer2[5], answer1[5], answer2[5];
    mvwaddwstr(win, 4, 1, L"typedef struct{");
    mvwaddwstr(win, 5, 1, L"    int x;");
    mvwaddwstr(win, 6, 1, L"    int y;");
    mvwaddwstr(win, 7, 1, L"}Mystruct");
    mvwaddwstr(win, 9, 1, L"int main(){");
    mvwaddwstr(win, 10, 1, L"   Mystruct s;");
    mvwaddwstr(win, 11, 1, L"   s.x += 1");
    mvwaddwstr(win, 12, 1, L"   s.y -= 2");
    mvwaddwstr(win, 13, 1, L"   printf(\"%d %d\", s.x, s.y)");
    mvwaddwstr(win, 14, 1, L"   return 0;");
    ///
    if (!strcmp(pl_answer1, answer1) && !strcmp(pl_answer2, answer2))
    {
        return true;
    }
    return false;
}