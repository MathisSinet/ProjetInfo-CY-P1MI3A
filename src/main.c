#include "main.h"

void pexit(DisplayInfo *di, int __status)
{
    endcurses(di);
    exit(__status);
}
void segfault()
{
    endwin();
    printf("---Erreur de segmentation---\n");
    exit(EXIT_FAILURE);
}
void aborthandler()
{
    sleep(1);
    endwin();
    printf("---Aborted---\n");
    exit(EXIT_FAILURE);
}

void NewGame(DisplayInfo *di, Region *reg, Player *pl)
{
    char buffer[20] = {'\0'};
    int x,y;

    new_wclear(di->box1);
    curs_set(1);
    
    //asks for player name
    for(int i=0; i<MAX_PLAYER_NAME_COUNT; i++){pl->name[i] = '\0';}
    mvwprintw(di->box1,1,2, "Entrez le nom du joueur : ");
    getyx(di->box1, y, x);
    getusrstr(di->box1, y, x, pl->name, MAX_PLAYER_NAME_COUNT, &is_valid_playername_char);

    //asks for the seed
    mvwprintw(di->box1,2,2, "Entrez la seed : ");
    getyx(di->box1, y, x);
    getusrstr(di->box1, y, x, buffer, 20, &is_digit);

    curs_set(0);

    if (!sscanf(buffer, "%u", &reg->seed))
    {
        reg->seed = rand();
    }

    initial_map(reg, pl);
}

void Game(DisplayInfo *di, Region *reg, Player *pl)
{
    int ch;
    init_debug_print(di, reg, pl);
    nodelay(di->box1, true);
    while (true)
    {
        ch = wgetch(di->box1);
        if (ch == 'w')
        {
            break;
        }
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
            init_debug_print(di, reg, pl);
        }
        if (reg->deathtimer <= 0)
        {
            //game over
            break;
        }
    }
    nodelay(di->box1, false);
}

int main(int argc, char **argv)
{
    Region reg;
    Player pl;
    DisplayInfo di;

    setlocale(LC_ALL, "");
    srand(time(NULL));
    rand();
    signal(SIGSEGV, &segfault);
    signal(SIGABRT, &aborthandler);


    initcurses(&di);

    switch (MainMenu(&di))
    {
        case MAIN_MENU_NEW:
            NewGame(&di, &reg, &pl);
            Game(&di, &reg, &pl);
            break;
        case MAIN_MENU_QUIT:
            pexit(&di, EXIT_SUCCESS);
            break;
        default:
            break;
    }
    
    endcurses(&di);
    
    return 0;
}