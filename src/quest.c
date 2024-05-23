#include "quest.h"

bool isvalid_quizz1_answer_chr(int chr)
{
    return (is_digit(chr) || chr == '?');
}


//Performs a quizz
/// @return true if quizz was completed else false
bool quizz(Player* pl, Region* reg, DisplayInfo* di, uint8_t quizz_id)
{
    WINDOW *win = di->box1;
    bool sucess;

    nodelay(win, false);
    new_wclear(win);
    mvwaddwstr(win, 1, 2, L"Vous avez trouvé un équipement informatique défaillant de la station !");
    mvwaddwstr(win, 2, 2, L"Retrouvez les valeurs affichées par ce programme pour corriger le code défaillant :");

    switch(quizz_id)
    {
        case 1:
            sucess = quizz1(pl, reg, win);
            break;
        default:
            break;
    }

    if (sucess)
    {
        reg->questinfo.quizz_done++;
        waddwstr(win, L"Vous avez réussi !");
    }
    else
    {
        waddwstr(win, L"Vous avez échoué !");
        pl->hp--;
    }

    wgetch(win);
    nodelay(win, true);
    return sucess;
}

bool check_quests(Region* reg, Player* pl)
{
    return false;
}

bool quizz1(Player* pl, Region* reg, WINDOW* win)
{
    char pl_answer1[5], pl_answer2[5], answer1[5], answer2[5];
    uint8_t style1 = randint(reg, 0, 2);
    uint8_t style2 = randint(reg, 0, 2);
    uint16_t val1 = randint(reg, 0, 1000);
    uint16_t val2 = randint(reg, 0, 1000);
    char *equals[3] = {"=", "+=", "-="};
    uint16_t nb_success = 0;

    if (style2 == 0)
    {
        snprintf(answer1, 5, "%u", val2);
    }
    else
    {
        strncpy(answer1, "?", 2);
    }
    if (style1 == 0)
    {
        snprintf(answer2, 5, "%u", val1);
    }
    else
    {
        strncpy(answer2, "?", 2);
    }

    mvwprintw(win, 4, 2, "typedef struct{");
    mvwprintw(win, 5, 2, "    int x;");
    mvwprintw(win, 6, 2, "    int y;");
    mvwprintw(win, 7, 2, "}Mystruct;");
    mvwprintw(win, 9, 2, "int main(){");
    mvwprintw(win, 10, 2, "   Mystruct s;");
    mvwprintw(win, 11, 2, "   s.x %s %u", equals[style1], val1);
    mvwprintw(win, 12, 2, "   s.y %s %u", equals[style2], val2);
    mvwaddstr(win, 13, 2, "   printf(\"%d\", s.y)");
    mvwaddstr(win, 14, 2, "   printf(\"%d\", s.x)");
    mvwprintw(win, 15, 2, "   return 0;");
    mvwprintw(win, 16, 2, "}");

    mvwaddwstr(win, 18, 2, L"Si le programme tente d'afficher une valeur non initialisée, répondez '?'");

    curs_set(1);
    mvwprintw(win, 20, 2, "Premier affichage : ");
    getusrstr(win, 20, 22, pl_answer1, 5, &isvalid_quizz1_answer_chr);
    mvwprintw(win, 21, 2, "Second affichage : ");
    getusrstr(win, 21, 21, pl_answer2, 5, &isvalid_quizz1_answer_chr);
    curs_set(0);
    
    if (!strcmp(pl_answer1, answer1))
    {
        nb_success++;
        mvwprintw(win, 20, 30, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 20, 30, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer1);
    }
    if (!strcmp(pl_answer2, answer2))
    {
        nb_success++;
        mvwprintw(win, 21, 30, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 21, 30, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer2);
    }
    
    wmove(win, 23, 2);
    
    return nb_success == 2;
}

bool quizz2(Player* pl, Region* reg, WINDOW* win)
{
    char pl_answer1[5], pl_answer2[5], answer1[5], answer2[5];
    uint8_t style1 = randint(reg, 0, 2);
    uint8_t style2 = randint(reg, 0, 2);
    uint16_t val1 = randint(reg, 0, 1000);
    uint16_t val2 = randint(reg, 0, 1000);
    char *equals[3] = {"=", "+=", "-="};
    uint16_t nb_success = 0;

    if (style2 == 0)
    {
        snprintf(answer1, 5, "%u", val2);
    }
    else
    {
        strncpy(answer1, "?", 2);
    }
    if (style1 == 0)
    {
        snprintf(answer2, 5, "%u", val1);
    }
    else
    {
        strncpy(answer2, "?", 2);
    }

    mvwprintw(win, 4, 2, "int func(int N, int M){");
    mvwprintw(win, 5, 2, "    int res = 0;");
    ///mvwprintw(win, 6, 2, "    for (int i=0; i<N; i++){");
    ///mvwprintw(win, 7, 2, "      for (int j=0; j<M; j+=M/5){;");
    mvwprintw(win, 9, 2, "          res+=1");
    mvwprintw(win, 10, 2, "     }");
    mvwprintw(win, 11, 2, "   }");
    mvwprintw(win, 12, 2, "   return res;");
    mvwaddstr(win, 13, 2, "}");
    mvwprintw(win, 15, 2, "int main(){");
    mvwprintw(win, 16, 2, "   int N, M;");
    ///mvwprintw(win, 17, 2, "   printf(\"%d"\, func(N,M))");

    mvwaddwstr(win, 19, 2, L"Si le programme tente d'afficher une valeur non initialisée, répondez '?'");

    curs_set(1);
    mvwprintw(win, 20, 2, "Donner la complexité du programme : ");
    getusrstr(win, 20, 22, pl_answer1, 5, &isvalid_quizz1_answer_chr);
    mvwprintw(win, 21, 2, "Donner la valeur de res : ");
    getusrstr(win, 21, 21, pl_answer2, 5, &isvalid_quizz1_answer_chr);
    curs_set(0);
    
    if (!strcmp(pl_answer1, answer1))
    {
        nb_success++;
        mvwprintw(win, 20, 30, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 20, 30, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer1);
    }
    if (!strcmp(pl_answer2, answer2))
    {
        nb_success++;
        mvwprintw(win, 21, 30, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 21, 30, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer2);
    }
    
    wmove(win, 23, 2);
    
    return nb_success == 2;
}