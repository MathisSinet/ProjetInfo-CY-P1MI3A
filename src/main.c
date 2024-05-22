/*
main.c
Main file of the program
*/

#include "main.h"

// calls exit(__status) while ending curses mode
void pexit(DisplayInfo *di, int __status)
{
    endcurses(di);
    exit(__status);
}
// segmentation fault handler
void segfault()
{
    sleep(1);
    endwin();
    printf("---Erreur de segmentation---\n");
    exit(EXIT_FAILURE);
}
// abortion handler
void aborthandler()
{
    sleep(1);
    endwin();
    printf("---Aborted---\n");
    exit(EXIT_FAILURE);
}


// Function to create a new game
void NewGame(DisplayInfo *di, Region *reg, Player *pl)
{
    WINDOW *win = di->box2;

    char buffer[20] = {'\0'};
    int x,y;

    new_wclear(win);
    curs_set(1);
    
    //asks for player name
    //for(int i=0; i<MAX_PLAYER_NAME_COUNT; i++){pl->name[i] = '\0';}
    mvwprintw(win,1,2, "Entrez le nom du joueur : ");
    getyx(win, y, x);
    getusrstr(win, y, x, pl->name, MAX_PLAYER_NAME_COUNT, &is_valid_playername_char);

    //asks for the seed
    mvwprintw(win,2,2, "Entrez la seed : ");
    getyx(win, y, x);
    getusrstr(win, y, x, buffer, 20, &is_digit);

    curs_set(0);

    if (!sscanf(buffer, "%u", &reg->seed))
    {
        reg->seed = rand();
    }
    
    initial_map(reg, pl);
}


// Function to ask the user to load a save and load it if found
bool loadSave(DisplayInfo *di, Region *reg, Player *pl)
{
    WINDOW *win = di->box2;
    char buffer[MAX_PLAYER_NAME_COUNT] = {'\0'};
    int x,y;

    new_wclear(win);
    curs_set(1);

    mvwaddwstr(win, 4, 2, L"⚠️  Les noms de joueur sont sensibles à la casse !");
    for(int i=0; i<MAX_PLAYER_NAME_COUNT; i++){pl->name[i] = '\0';}
    mvwprintw(win, 1, 2, "Entrez le nom du joueur : ");
    getyx(win, y, x);
    getusrstr(win, y, x, buffer, MAX_PLAYER_NAME_COUNT, &is_valid_playername_char);

    curs_set(0);

    if (load(buffer, reg, pl))
    {
        return true;
    }

    mvwaddwstr(win, 2, 2, L"Désolé, la sauvegarde n'a pas pu être chargée");
    mvwaddstr(win, 3, 2, "Appuyez sur une touche pour revenir au menu principal");
    wgetch(win);
    return false;
}


//Game loop
void Game(DisplayInfo *di, Region *reg, Player *pl)
{
    int ch;
    float t1, t2, diff;
    update_map(di, reg, pl);
    wrefresh(di->box2); //temp
    right_panel_update(reg, pl, di->box3);
    nodelay(di->box1, true);
    t1 = clock();
    while (true)
    {
        ch = wgetch(di->box1);
        if (ch == 'x' || ch == 'X')
        {
            break;
        }
        switch (ch)
        {
            case 'z':
            case 'Z':
            case KEY_UP:
                playermove(reg, pl, NORTH, di);
                break;
            case 'q':
            case 'Q':
            case KEY_LEFT:
                playermove(reg, pl, WEST, di);
                break;
            case 's':
            case 'S':
            case KEY_DOWN:
                playermove(reg, pl, SOUTH, di);
                break;
            case 'd':
            case 'D':
            case KEY_RIGHT:
                playermove(reg, pl, EAST, di);
                break;

            case 'c':
            case 'C':
                show_controls(di);
                break;
            case 'w':
            case 'W':
                save_ui(di, reg, pl);
                break;
            case 'e':
            case 'E':
                manage_inventory(reg, pl, di);
                break;
            case ' ':
                playerattack(reg, pl);
                break;
        }
        if (true)
        {
            update_map(di, reg, pl);
            right_panel_update(reg, pl, di->box3);
        }
        if (pl->hp <= 0)
        {
            mvwprintw(di->box1,1,0,"Game over");
            wrefresh(di->box1);
            us_sleep(1000000);
            break;
        }
        if (reg->deathtimer <= 0.0)
        {
            //game over
            break;
        }

        t2 = clock();
        diff = (t2-t1)/CLOCKS_PER_SEC;
        t1 = clock();
        reg->deathtimer -= diff;
        pl->atkdelay -= diff;
        monstermove(reg, pl, diff);
        monsterattack(reg, pl, diff);
    }
    nodelay(di->box1, false);
    reg_memfree(reg);
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

    init_mainmenu(&di);
    while(true)
    {
        switch (MainMenu(&di))
        {
            case MAIN_MENU_NEW:
                NewGame(&di, &reg, &pl);
                end_mainmenu(&di);
                init_gameui(&di);
                Game(&di, &reg, &pl);
                end_gameui(&di);
                init_mainmenu(&di);
                break;
            case MAIN_MENU_LOAD:
                if (loadSave(&di, &reg, &pl))
                {
                    end_mainmenu(&di);
                    init_gameui(&di);
                    Game(&di, &reg, &pl);
                    end_gameui(&di);
                    init_mainmenu(&di);
                }
                break;
            case MAIN_MENU_QUIT:
                pexit(&di, EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
    
    
    endcurses(&di);
    
    return 0;
}
