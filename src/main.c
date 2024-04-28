#include "main.h"

void pexit(int __status)
{
    endwin();
    exit(__status);
}

void NewGame(Region *reg, Player *pl)
{
    char buffer[50] = {'\0'};
    int len;
    int x,y;
    int chr;

    clear();
    refresh();
    mvprintw(0,0, "Entrez la seed : ");
    getyx(stdscr, y, x);
    len=0;
    
    curs_set(1);
    while(true)
    {
        chr = getch();

        if (chr == KEY_ENTER || chr == '\n' || chr == '\r')
        {
            break;
        }

        if (chr >= '0' && chr <= '9')
        {
            buffer[len++] = chr;
            mvaddnstr(y, x, buffer, len);
        }

        if (len >= 48)
        {
            break;
        }
    }
    curs_set(0);

    if (!sscanf(buffer, "%u", &reg->seed))
    {
        reg->seed = rand();
    }


    initial_map(reg);
    init_debug_print(reg);
}

int main(int argc, char **argv)
{
    Region reg;
    Player pl;

    setlocale(LC_ALL, "fr_FR.UTF-8");
    srand(time(NULL));
    rand();

    initscr(); //initialises curses mode
    cbreak();
    noecho();
    keypad(stdscr, true);
    curs_set(0);

    switch (MainMenu())
    {
        case MAIN_MENU_NEW:
            NewGame(&reg, &pl);
            break;
        case MAIN_MENU_QUIT:
            pexit(EXIT_SUCCESS);
            break;
        default:
            break;
    }
    
    endwin();
    
    return 0;
}