#include "main.h"

void pexit(int __status)
{
    endwin();
    exit(__status);
}

void NewGame(Region *reg, Player *pl)
{
    char buffer[20] = {'\0'};
    int x,y;

    clear();
    curs_set(1);
    
    //asks for player name
    for(int i=0; i<MAX_PLAYER_NAME_COUNT; i++){pl->name[i] = '\0';}
    mvprintw(0,0, "Entrez le nom du joueur : ");
    getyx(stdscr, y, x);
    getusrstr(y, x, pl->name, MAX_PLAYER_NAME_COUNT, &is_valid_playername_char);

    //asks for the seed
    mvprintw(1,0, "Entrez la seed : ");
    getyx(stdscr, y, x);
    getusrstr(y, x, buffer, 20, &is_digit);

    curs_set(0);

    if (!sscanf(buffer, "%u", &reg->seed))
    {
        reg->seed = rand();
    }

    initial_map(reg);
}

void Game(Region *reg, Player *pl)
{
    int ch;
    nodelay(stdscr, true);
    init_debug_print(reg, pl);
    while (true)
    {
        ch = getch();
        switch (ch)
        {
            case 'z':
            case KEY_UP:
                playermove(reg, pl, NORTH);
                break;
            case 'q':
            case KEY_LEFT:
                playermove(reg, pl, WEST);
                break;
            case 's':
            case KEY_DOWN:
                playermove(reg, pl, SOUTH);
                break;
            case 'd':
            case KEY_RIGHT:
                playermove(reg, pl, EAST);
                break;
        }
        if (ch != ERR)
        {
            init_debug_print(reg, pl);
        }
        if (reg->deathtimer <= 0)
        {
            //game over
            break;
        }
        if (ch == 'q')
        {
            break;
        }
    }
    nodelay(stdscr, false);
}

int main(int argc, char **argv)
{
    Region reg;
    Player pl;

    setlocale(LC_ALL, "");
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
            Game(&reg, &pl);
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