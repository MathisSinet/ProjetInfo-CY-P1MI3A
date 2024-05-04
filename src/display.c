#include "display.h"

void HelloWorld()
{
    printw("Hello world!\n");
    refresh();
    getch();
}


void getusrstr(int y, int x, char *buffer, int max_len, bool(*validatefunc)(int))
{
    int len=0;
    int chr;

    while(true)
    {
        chr = getch();

        if (len > 0 && (chr == KEY_ENTER || chr == '\n' || chr == '\r'))
        {
            break;
        }

        if (len > 0 && (chr == KEY_BACKSPACE || chr == KEY_DC))
        {
            buffer[--len] = '\0';
            mvaddstr(y, x+len, " ");
            move(y, x+len);
        }

        if ((*validatefunc)(chr) && len < max_len-1)
        {
            buffer[len++] = (char) chr;
            mvaddnstr(y, x, buffer, len);
        }
    }
}


int MainMenu()
{
    uint8_t cursor = 0;
    int chr;
    clear();
    mvprintw(0,0,"Bienvenue dans le jeu !");
    addwstr(L" 👽");
    mvaddwstr(1,0, L"Appuyez sur entrée pour sélectionner ou sur les flèches pour vous déplacer");
    mvprintw(2,0, "  : Nouvelle partie");
    mvprintw(3,0, "  : Charger une partie");
    mvprintw(4,0, "  : Quitter le jeu");
    mvprintw(cursor+2, 0, "X");
    while(true)
    {
        refresh();
        chr = getch();
        switch (chr)
        {
            case '\n':
            case '\r':
            case KEY_ENTER:
                return cursor;
            case KEY_DOWN:
                mvprintw(cursor+2, 0, " ");
                cursor = (cursor+1)%3;
                mvprintw(cursor+2, 0, "X");
                break;
            case KEY_UP:
                mvprintw(cursor+2, 0, " ");
                cursor = (cursor+2)%3;
                mvprintw(cursor+2, 0, "X");
                break;
            default:
                mvprintw(5,0,"%d",chr);
        }
    }
}

void init_debug_print(Region *reg, Player *pl)
{
    int w,h;
    int row=0;
    getmaxyx(stdscr, h, w);
    clear();
    mvprintw(row++, 0, "x=%d, y=%d", pl->loc.x, pl->loc.y);
    for (int y=pl->loc.y-h/2; y<pl->loc.y+h/2; y++)
    {
        move(row++,0);
        for (int x=pl->loc.x-w/2; x<pl->loc.x+w/2; x++)
        {
            if (
                reg->zero.x + x < 0 || 
                reg->zero.x + x >= reg->grid_width || 
                reg->zero.y + y < 0 || 
                reg->zero.y + y >reg->grid_height)
            {
                printw(" ");
                continue;
            }
            if(x==pl->loc.x && y==pl->loc.y)
            {
                printw("C");
                continue;
            }
            switch (*get_from_grid(reg, x, y))
            {
            case VOID:
                printw(" ");
                break;
            case RESERVED:
                printw("R");
                break;
            case WALL:
                printw("W");
                break;
            case DOOR:
                printw("D");
                break;
            default:
                break;
            }
        }
    }
}