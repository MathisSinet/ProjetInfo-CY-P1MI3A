#include "display.h"

void us_sleep(uint64_t us)
{
    struct timespec ts;
    ts.tv_sec = us/(1000000);
    ts.tv_nsec = (us%1000000) * 1000;
    nanosleep(&ts, &ts);
}

void initcurses(DisplayInfo *di)
{
    initscr(); //initialises curses mode
    cbreak();
    noecho();
    curs_set(0);

    getmaxyx(stdscr, di->height, di->width);
    di->box1 = newwin(di->height-8, di->width-30, 0, 0);
    di->box2 = newwin(8, di->width-30, di->height-8, 0);
    di->box3 = newwin(di->height, 30, 0, di->width-30);

    keypad(di->box1, true);
    start_color();
    init_pair(CY_RED, COLOR_RED, COLOR_BLACK);
    init_pair(CY_BLUE, COLOR_BLUE, COLOR_BLACK);

    box(di->box1, 0, 0);
    box(di->box2, 0, 0);
    box(di->box3, 0, 0);
}

void new_wclear(WINDOW *win)
{
    werase(win);
    box(win, 0, 0);
}

void endcurses(DisplayInfo *di)
{
    delwin(di->box1);
    delwin(di->box2);
    delwin(di->box3);
    endwin();
}

void getusrstr(WINDOW *win, int y, int x, char *buffer, int max_len, bool(*validatefunc)(int))
{
    int len=0;
    int chr;

    while(true)
    {
        chr = wgetch(win);

        if (len > 0 && (chr == KEY_ENTER || chr == '\n' || chr == '\r'))
        {
            break;
        }

        if (len > 0 && (chr == KEY_BACKSPACE || chr == KEY_DC))
        {
            buffer[--len] = '\0';
            mvwaddstr(win, y, x+len, " ");
            wmove(win, y, x+len);
        }

        if ((*validatefunc)(chr) && len < max_len-1)
        {
            buffer[len++] = (char) chr;
            mvwaddnstr(win, y, x, buffer, len);
        }
    }
}


void display_ui(Region *reg, Player *pl, WINDOW *win)
{
    char itemname[15];
    Item item;

    new_wclear(win);
    wattron(win, A_BOLD);
    mvwaddstr(win, 1,2, pl->name);
    wattroff(win, A_BOLD);

    mvwaddstr(win, 3, 2, "PV ");
    
    for (int i=1; i<=10; i++)
    {
        if (pl->hp >= i)
        {
            wattron(win, COLOR_PAIR(CY_RED));
            mvwadd_wch(win, 3, 3+2*i, WACS_BLOCK);
            wattroff(win, COLOR_PAIR(CY_RED));
        }
        else
        {
            mvwadd_wch(win, 3, 3+2*i, WACS_BLOCK);
        }
    }
    mvwaddstr(win, 4, 2, "DEF ");
    for (int i=1; i<=10; i++)
    {
        if (pl->def >= i*10)
        {
            wattron(win, COLOR_PAIR(CY_BLUE));
            mvwadd_wch(win, 4, 4+2*i, WACS_BLOCK);
            wattroff(win, COLOR_PAIR(CY_BLUE));
        }
        else
        {
            mvwadd_wch(win, 4, 4+2*i, WACS_BLOCK);
        }
    }

    mvwprintw(win, 5, 2, "Attaque : %d", pl->atk);
    mvwprintw(win, 6, 2, "Score : %d", pl->xp);
    
    mvwprintw(win, 8, 2, "Arme : ");
    item = getitem(pl->weapon, itemname);
    wprintw(win, "%s %lc", item.name, item.symb);
    mvwprintw(win, 9, 2, "Inventaire :");

    wrefresh(win);
}


int MainMenu(DisplayInfo *di)
{
    uint8_t cursor = 0;
    int chr;
    erase();
    mvwprintw(di->box1, 1,2,"Bienvenue dans le jeu !");
    waddwstr(di->box1, L" 👽");
    mvwaddwstr(di->box1, 2,2, L"Appuyez sur entrée pour sélectionner ou sur les flèches pour vous déplacer");
    mvwprintw(di->box1,3,5, "Nouvelle partie");
    mvwprintw(di->box1,4,5, "Charger une partie");
    mvwprintw(di->box1,5,5, "Quitter le jeu");
    //mvwadd_wch(di->box1,cursor+3, 2, WACS_RARROW);
    mvwaddwstr(di->box1,cursor+3, 2, L"▶️");
    while(true)
    {
        wrefresh(di->box1);
        chr = wgetch(di->box1);
        switch (chr)
        {
            case '\n':
            case '\r':
            case KEY_ENTER:
                return cursor;
            case KEY_DOWN:
                mvwprintw(di->box1,cursor+3, 2, " ");
                cursor = (cursor+1)%3;
                //mvwadd_wch(di->box1,cursor+3, 2, WACS_RARROW);
                mvwaddwstr(di->box1,cursor+3, 2, L"▶️");
                break;
            case KEY_UP:
                mvwprintw(di->box1,cursor+3, 2, " ");
                cursor = (cursor+2)%3;
                //mvwadd_wch(di->box1,cursor+3, 2, WACS_RARROW);
                mvwaddwstr(di->box1,cursor+3, 2, L"▶️");
                break;
            default:
                mvwprintw(di->box1,6,2,"%d",chr);
        }
    }
}

void init_debug_print(DisplayInfo *di,Region *reg, Player *pl)
{
    int w,h;
    int row=1;
    getmaxyx(di->box1, h, w);
    h-=3; w-=2;
    new_wclear(di->box1);
    mvwprintw(di->box1, row++, 2, "x=%d, y=%d, grid width=%d, grid height=%d", pl->loc.x, pl->loc.y, reg->grid_width, reg->grid_height);
    for (int y=pl->loc.y-h/2; y<pl->loc.y+h/2; y++)
    {
        wmove(di->box1, row++,1);
        for (int x=pl->loc.x-w/4; x<pl->loc.x+w/4; x++)
        {
            if (
                reg->zero.x + x < 0 || 
                reg->zero.x + x >= reg->grid_width || 
                reg->zero.y + y < 0 || 
                reg->zero.y + y >reg->grid_height)
            {
                wprintw(di->box1, "  ");
                continue;
            }
            if(x==pl->loc.x && y==pl->loc.y)
            {
                waddwstr(di->box1, L"ඞ ");
                continue;
            }
            if (pl->currentroom->isitem)
            {
                if (x==pl->currentroom->itemloc.x && y==pl->currentroom->itemloc.y)
                {
                    wprintw(di->box1, "%lc", getitem(pl->currentroom->item, NULL).symb);
                    continue;
                }
            }
            switch (*get_from_grid(reg, x, y))
            {
            case VOID:
                wprintw(di->box1, "  ");
                break;
            case RESERVED:
                wprintw(di->box1, "RR");
                break;
            case WALL:
                waddwstr(di->box1, L"🧱");
                break;
            case DOOR:
                waddwstr(di->box1, L"🚪");
                break;
            default:
                break;
            }
        }
    }
}
