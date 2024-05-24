#include "quest.h"


// Checks if all quests are done
bool check_quests(Region* reg, Player* pl)
{
    return (
        reg->questinfo.is_ball_found &&
        reg->questinfo.is_teddybear_found &&
        reg->questinfo.monsters_killed >= GOAL_MONSTER_KILLS &&
        reg->questinfo.quizz_done == 3
    );
}


// Checks if the character is valid for a quizz 1 answer
bool isvalid_quizz1_answer_chr(int chr)
{
    return (is_digit(chr) || chr == '?');
}


// Performs a quizz
/// @return true if quizz was completed else false
bool quizz(Player* pl, Region* reg, DisplayInfo* di, uint8_t quizz_id)
{
    WINDOW *win = di->box1;
    bool success;

    nodelay(win, false);
    new_wclear(win);
    mvwaddwstr(win, 1, 2, L"Vous avez trouvé un équipement informatique défaillant de la station !");
    mvwaddwstr(win, 2, 2, L"Retrouvez les valeurs affichées par ce programme pour corriger le code défaillant :");

    switch(quizz_id)
    {
        case 1:
            success = quizz1(pl, reg, win);
            break;
        case 2:
            success = quizz2(pl, reg, win);
            break;
        case 3:
            success = quizz3(pl, reg, win);
            break;
        default:
            break;
    }

    if (success)
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
    return success;
}


// Generates the first quizz
// This is a quizz on structures and data initialization
/// @return true if the quizz was completed
bool quizz1(Player* pl, Region* reg, WINDOW* win)
{
    // Strings for the given answers and the expected answers
    char pl_answer1[5], pl_answer2[5], answer1[5], answer2[5];

    // Chooses random operators among '=', '+=' and '-='
    uint8_t style1 = randint(reg, 0, 2);
    uint8_t style2 = randint(reg, 0, 2);
    // Values of s.x and s.y are random
    uint16_t val1 = randint(reg, 0, 1000);
    uint16_t val2 = randint(reg, 0, 1000);

    char *equals[3] = {"=", "+=", "-="};
    uint16_t nb_success = 0; // Number of correct answers

    // Computes the expected answers
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

    // Prints the program
    mvwaddstr(win, 4, 2, "#include <stdio.h>");
    mvwaddstr(win, 6, 2, "typedef struct{");
    mvwaddstr(win, 7, 2, "    int x;");
    mvwaddstr(win, 8, 2, "    int y;");
    mvwaddstr(win, 9, 2, "}Mystruct;");
    mvwaddstr(win, 11, 2, "int main(){");
    mvwaddstr(win, 12, 2, "   Mystruct s;");
    mvwprintw(win, 13, 2, "   s.x %s %u;", equals[style1], val1);
    mvwprintw(win, 14, 2, "   s.y %s %u;", equals[style2], val2);
    mvwaddstr(win, 15, 2, "   printf(\"%d\", s.y);");
    mvwaddstr(win, 16, 2, "   printf(\"%d\", s.x);");
    mvwaddstr(win, 17, 2, "   return 0;");
    mvwaddstr(win, 18, 2, "}");

    mvwaddwstr(win, 20, 2, L"Si le programme tente d'afficher une valeur non initialisée, répondez '?'");

    // Asks for the answers
    curs_set(1);
    mvwaddstr(win, 21, 2, "Premier affichage : ");
    getusrstr(win, 21, 22, pl_answer1, 5, &isvalid_quizz1_answer_chr);
    mvwaddstr(win, 22, 2, "Second affichage : ");
    getusrstr(win, 22, 21, pl_answer2, 5, &isvalid_quizz1_answer_chr);
    curs_set(0);
    
    // Compares the given answers with the expected answers
    if (!strcmp(pl_answer1, answer1))
    {
        nb_success++;
        mvwprintw(win, 21, 30, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 21, 30, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer1);
    }
    if (!strcmp(pl_answer2, answer2))
    {
        nb_success++;
        mvwprintw(win, 22, 30, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 22, 30, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer2);
    }
    
    wmove(win, 24, 2);
    
    return nb_success == 2;
}


// Generates the second quizz
// This is a quizz on loops
/// @return true if the quizz was completed
bool quizz2(Player* pl, Region* reg, WINDOW* win)
{
    // Strings for the given answers and the expected answers
    char pl_answer[7], answer[7];

    // Chooses random types of loops
    uint8_t style1 = randint(reg, 0, 2);
    uint8_t style2 = randint(reg, 0, 2);
    // Values of N,M and x are random
    uint16_t N = randint(reg, 30, 80);
    uint16_t M = randint(reg, 30, 80);
    uint16_t x = randint(reg, 2, 7);

    uint16_t factor1, factor2;
    uint16_t success = 0; // If the quizz was completed

    // Prints the program
    mvwprintw(win, 4, 2, "#include <stdio.h>");
    mvwprintw(win, 6, 2, "int func(int N, int M){");
    mvwprintw(win, 7, 2, "   int res = 0, x = %u;", x);
    switch(style1)
    {
        case 0:
            mvwprintw(win, 8, 2, "   for (int i=0; i<N; i++){");
            factor1 = N;
            break;
        case 1:
            mvwprintw(win, 8, 2, "   for (int i=0; i<=N; i+=N/x){");
            factor1 = x+1;
            break;
        case 2:
            mvwprintw(win, 8, 2, "   for (int i=0; i<N/x; i++){");
            factor1 = N/x;
            break;
    }
    switch(style2)
    {
        case 0:
            mvwprintw(win, 9, 2, "      for (int j=0; i<M; j++){");
            factor2 = M;
            break;
        case 1:
            mvwprintw(win, 9, 2, "      for (int j=0; j<=M; j+=M/x){");
            factor2 = x+1;
            break;
        case 2:
            mvwprintw(win, 9, 2, "      for (int j=0; j<M/x; j++){");
            factor2 = M/x;
            break;
    }
    mvwprintw(win, 10, 2, "         res+=1;");
    mvwprintw(win, 11, 2, "     }");
    mvwprintw(win, 12, 2, "   }");
    mvwprintw(win, 13, 2, "   return res;");
    mvwprintw(win, 14, 2, "}");
    mvwprintw(win, 16, 2, "int main(){");
    mvwprintw(win, 17, 2, "   int N=%u, M=%u;", N, M);
    mvwaddstr(win, 18, 2, "   printf(\"%d\", func(N,M));");
    mvwprintw(win, 19, 2, "   return 0;");
    mvwprintw(win, 20, 2, "}");

    // Asks for the answer
    curs_set(1);
    mvwprintw(win, 22, 2, "Affichage : ");
    getusrstr(win, 22, 14, pl_answer, 5, &is_digit);
    curs_set(0);
    
    // Computes the expected answer
    snprintf(answer, 7, "%u", factor1*factor2);

    // Compares the given answer with the expected answer
    if (!strcmp(pl_answer, answer))
    {
        success = 1;
        mvwprintw(win, 22, 24, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 22, 24, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer);
    }
    
    wmove(win, 24, 2);
    
    return success;
}


// Generates the third quizz
// This is a quiz on structures in memory
/// @return true if the quizz was completed
bool quizz3(Player* pl, Region* reg, WINDOW* win)
{
    // Strings for the given answers and the expected answers
    char pl_answer1[7], pl_answer2[7], answer1[7], answer2[7];
    // Number of correct answers
    uint16_t nb_success = 0;
    // Random numbers used to determine the contents of the structure
    int16_t rand_type, rand_qty;
    // Current member type and quantity
    int current_type, current_qty;

    // Data used by the quizz generator
    char *types[12] = {"char", "short", "int", "float", "long", "double", "char*", "short*", "int*", "float*", "long*", "double*"};
    uint16_t types_alignof[12] = {1, 2, 4, 4, 8, 8, 8, 8, 8, 8, 8};
    uint16_t types_weights[12] = {10, 10, 10, 4, 6, 4, 5, 5, 5, 2, 2, 2};
    uint16_t total_types_weights = 0;
    for (int i=0; i<12; i++){total_types_weights += types_weights[i];};

    uint16_t qty_weights[5] = {20, 10, 10, 8, 8};
    uint16_t total_qty_weights = 0;
    for (int i=0; i<5; i++){total_qty_weights += qty_weights[i];};

    uint16_t structure_sizeof = 0;
    uint16_t structure_alignof = 0;

    // Prints the program
    mvwprintw(win, 4, 2, "#include <stdio.h>");
    mvwprintw(win, 6, 2, "typedef struct{");
    for (int i=0; i<5; i++)
    {
        // Decides the type and quantity of the member of the structure
        rand_type = randint(reg, 1, total_types_weights);
        current_type=-1;
        do
        {
            current_type++;
            rand_type -= types_weights[current_type];
        }
        while (rand_type > 0);
        rand_qty = randint(reg, 1, total_qty_weights);
        current_qty=-1;
        do
        {
            current_qty++;
            rand_qty -= qty_weights[current_qty];
        }
        while (rand_qty > 0);

        // Updates the sizeof and alignof of the structure
        while(structure_sizeof % types_alignof[current_type])
        {
            structure_sizeof++;
        }
        if (types_alignof[current_type] > structure_alignof)
        {
            structure_alignof = types_alignof[current_type];
        }
        structure_sizeof += types_alignof[current_type] * (current_qty+1);

        // Prints the member of the structure
        mvwprintw(win, 7+i, 2, "   %s m%c", types[current_type], 'a'+i);
        if (current_qty+1 > 1)
        {
            wprintw(win, "[%d]", current_qty+1);
        }
        wprintw(win, ";");
    }
    while(structure_sizeof % structure_alignof)
    {
        structure_sizeof++;
    }

    mvwprintw(win, 13, 2, "}Mystruct;");
    mvwprintw(win, 15, 2, "int main(){");
    mvwaddstr(win, 16, 2, "   printf(\"%ld\", sizeof(Mystruct));");
    mvwaddstr(win, 17, 2, "   printf(\"%ld\", _Alignof(Mystruct));");
    mvwprintw(win, 18, 2, "   return 0;");
    mvwprintw(win, 19, 2, "}");

    // Computes the expected answers
    snprintf(answer1, 7, "%u", structure_sizeof);
    snprintf(answer2, 7, "%u", structure_alignof);

    // Asks for the answers
    curs_set(1);
    mvwprintw(win, 21, 2, "Premier affichage : ");
    getusrstr(win, 21, 22, pl_answer1, 5, &isvalid_quizz1_answer_chr);
    mvwprintw(win, 22, 2, "Second affichage : ");
    getusrstr(win, 22, 21, pl_answer2, 5, &isvalid_quizz1_answer_chr);
    curs_set(0);

    // Compares the given answers with the expected answers
    if (!strcmp(pl_answer1, answer1))
    {
        nb_success++;
        mvwprintw(win, 21, 32, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 21, 32, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer1);
    }
    if (!strcmp(pl_answer2, answer2))
    {
        nb_success++;
        mvwprintw(win, 22, 32, "Correct !");
    }
    else
    {
        mvwaddwstr(win, 22, 32, L"Incorrect, la bonne réponse était ");
        waddstr(win, answer2);
    }
    
    wmove(win, 24, 2);
    
    return nb_success == 2;
}