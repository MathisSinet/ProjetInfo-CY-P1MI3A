#include "main.h"

void pexit(int __status)
{
    endwin();
    exit(__status);
}

void NewGame(Region *reg, Player *pl)
{
    char buffer[20] = {'\0'};
    int len;
    int x,y;
    int chr;

    clear();
    refresh();
    mvprintw(0,0, "Entrez le nom du joueur : ");
    getyx(stdscr, y, x);
    len=0;

    for(int i=0; i<MAX_PLAYER_NAME_COUNT; i++){pl->name[i] = '\0';}
    
    curs_set(1);
    while(true)
    {
        chr = getch();

        if (len > 0 && (chr == KEY_ENTER || chr == '\n' || chr == '\r'))
        {
            break;
        }

        if (len > 0 && (chr == KEY_BACKSPACE || chr == KEY_DC))
        {
            pl->name[--len] = '\0';
            mvaddstr(y, x+len, " ");
            move(y, x+len);
        }

        if (is_valid_playername_char(chr) && len < MAX_PLAYER_NAME_COUNT-1)
        {
            pl->name[len++] = (char) chr;
            mvaddnstr(y, x, pl->name, len);
        }
    }

    mvprintw(1,0, "Entrez la seed : ");
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

        if (len > 0 && (chr == KEY_BACKSPACE || chr == KEY_DC))
        {
            buffer[--len] = '\0';
            mvaddstr(y, x+len, " ");
            move(y, x+len);
        }

        if (chr >= '0' && chr <= '9' && len < 19)
        {
            buffer[len++] = chr;
            mvaddnstr(y, x, buffer, len);
        }
    }

    //code for player name

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