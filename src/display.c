#include "display.h"

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

    box(di->box1, 0, 0);
    box(di->box2, 0, 0);
    box(di->box3, 0, 0);
}

void new_wclear(WINDOW *win)
{
    wclear(win);
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


int MainMenu(DisplayInfo *di)
{
    uint8_t cursor = 0;
    int chr;
    clear();
    mvwprintw(di->box1, 1,2,"Bienvenue dans le jeu !");
    waddwstr(di->box1, L" 👽");
    mvwaddwstr(di->box1, 2,2, L"Appuyez sur entrée pour sélectionner ou sur les flèches pour vous déplacer");
    mvwprintw(di->box1,3,2, "  : Nouvelle partie");
    mvwprintw(di->box1,4,2, "  : Charger une partie");
    mvwprintw(di->box1,5,2, "  : Quitter le jeu");
    mvwprintw(di->box1, cursor+3, 2, "X");
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
                mvwprintw(di->box1,cursor+3, 2, "X");
                break;
            case KEY_UP:
                mvwprintw(di->box1,cursor+3, 2, " ");
                cursor = (cursor+2)%3;
                mvwprintw(di->box1,cursor+3, 2, "X");
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
    mvwprintw(di->box1, row++, 2, "x=%d, y=%d", pl->loc.x, pl->loc.y);
    for (int y=pl->loc.y-h/2; y<pl->loc.y+h/2; y++)
    {
        wmove(di->box1, row++,1);
        for (int x=pl->loc.x-w/2; x<pl->loc.x+w/2; x++)
        {
            if (
                reg->zero.x + x < 0 || 
                reg->zero.x + x >= reg->grid_width || 
                reg->zero.y + y < 0 || 
                reg->zero.y + y >reg->grid_height)
            {
                wprintw(di->box1, " ");
                continue;
            }
            if(x==pl->loc.x && y==pl->loc.y)
            {
                wprintw(di->box1, "C");
                continue;
            }
            switch (*get_from_grid(reg, x, y))
            {
            case VOID:
                wprintw(di->box1, " ");
                break;
            case RESERVED:
                wprintw(di->box1, "R");
                break;
            case WALL:
                wprintw(di->box1, "W");
                break;
            case DOOR:
                wprintw(di->box1, "D");
                break;
            default:
                break;
            }
        }
    }
}