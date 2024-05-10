#include "display.h"


//Sleeps for the given number of microseconds
void us_sleep(uint64_t us)
{
    struct timespec ts;
    ts.tv_sec = us/(1000000);
    ts.tv_nsec = (us%1000000) * 1000;
    nanosleep(&ts, &ts);
}
//Erases the content of the window and recreates the box
void new_wclear(WINDOW *win)
{
    werase(win);
    box(win, 0, 0);
}

//Initialises the user interface
void initcurses(DisplayInfo *di)
{
    initscr(); 
    cbreak();
    noecho();
    curs_set(0);

    getmaxyx(stdscr, di->height, di->width);

    start_color();
    init_pair(CY_RED, COLOR_RED, COLOR_BLACK);
    init_pair(CY_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(CY_CYAN, COLOR_CYAN, COLOR_BLACK);
}

//Initializes the main menu interface
void init_mainmenu(DisplayInfo *di)
{
    // Load ASCII art
    char *ascii_art =
        "::::::::    ::::::::    ::::::::    ::::    ::::   :::::::::::  ::::::::        :::   :::   ::::::::   ::::    :::  :::::::::   ::::::::::  :::::::::  \n"
        ":+:    :+:  :+:     :+:  :+:    :+:  +:+:+: :+:+:+      :+:     :+:    :+:       :+:   :+:  :+:    :+:  :+:+:   :+:  :+:    :+:  :+:         :+:    :+: \n"
        "+:+         +:+     +:+  +:+         +:+ +:+:+ +:+      +:+     +:+               +:+ +:+   +:+    +:+  :+:+:+  +:+  +:+    +:+  +:+         +:+    +:+ \n"
        "+#+         +#+     +:+  +#++:++#++  +#+  +:+  +#+      +#+     +#+                +#++:    +#+    +:+  +#+ +:+ +#+  +#+    +:+  +#++:++#    +#++:++#:  \n"
        "+#+         +#+     +#+         +#+  +#+       +#+      +#+     +#+                 +#+     +#+    +#+  +#+  +#+#+#  +#+    +#+  +#+         +#+    +#+ \n"
        "#+#    #+#  #+#     #+#  #+#    #+#  #+#       #+#      #+#     #+#    #+#          #+#     #+#    #+#  #+#   #+# #  +#    #+#   #+#         #+#    #+# \n"
        " ########    ########    ########    ###       ###  ###########  ########           ###      ########   ###    ####  #########   ##########  ###    ### \n";

    // Calculate the number of lines in the ASCII art
    int num_lines = 12;
    int num_char_line = 151;
    // Calculate position to center ASCII horizontally
    int start_x = (di->width - num_char_line) / 2;

    // Generate a new box for ASCII ART
    di->box1 = newwin(num_lines + 2, num_char_line, 3, start_x);

    // Print ASCII art
    wattron(di->box1, COLOR_PAIR(CY_CYAN));
    mvwprintw(di->box1, 1, 1, "%s", ascii_art);
    wattroff(di->box1, COLOR_PAIR(CY_CYAN));
    wrefresh(di->box1);

    di->box2 = newwin(4*(di->height - 15)/5, 4*di->width/5, 18, di->width/10);
    keypad(di->box2, true);
    box(di->box2, 0, 0);
}

//Ends the main menu interface
void end_mainmenu(DisplayInfo *di)
{
    delwin(di->box1);
    delwin(di->box2);
    refresh();
}

//Initializes the game interface
void init_gameui(DisplayInfo *di)
{
    di->box1 = newwin(di->height-8, di->width-30, 0, 0); //top left panel
    di->box2 = newwin(8, di->width-30, di->height-8, 0); //bottom left panel
    di->box3 = newwin(di->height, 30, 0, di->width-30); //right panel
    keypad(di->box1, true);
    keypad(di->box2, true);
    keypad(di->box3, true);
    box(di->box1, 0, 0);
    box(di->box2, 0, 0);
    box(di->box3, 0, 0);
}
void end_gameui(DisplayInfo *di)
{
    wclear(di->box1);
    wclear(di->box2);
    wclear(di->box3);
    wrefresh(di->box1);
    wrefresh(di->box2);
    wrefresh(di->box3);
    delwin(di->box1);
    delwin(di->box2);
    delwin(di->box3);
    refresh();
}

void endcurses(DisplayInfo *di)
{
    //delwin(di->box1);
    //delwin(di->box2);
    //delwin(di->box3);
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


//Main menu loop. Returns an integer corresponding to the user's choice
int MainMenu(DisplayInfo *di)
{
    WINDOW *win = di->box2;
    uint8_t cursor = 0;
    int chr;
    new_wclear(win);
    mvwprintw(win, 1,2,"Bienvenue dans le jeu !");
    waddwstr(win, L" 👽");
    mvwaddwstr(win, 2,2, L"Appuyez sur entrée pour sélectionner ou sur les flèches pour vous déplacer");
    mvwprintw(win,3,5, "Nouvelle partie");
    mvwprintw(win,4,5, "Charger une partie");
    mvwprintw(win,5,5, "Quitter le jeu");

    mvwaddwstr(win, 7,2, L"En jeu, utilisez la touche C pour accéder aux contrôles");
    
    mvwaddwstr(win, cursor+3, 2, L"▶");
    while(true)
    {
        wrefresh(win);
        chr = wgetch(win);
        switch (chr)
        {
            case '\n':
            case '\r':
            case KEY_ENTER:
                return cursor;
            case KEY_DOWN:
                mvwprintw(win,cursor+3, 2, "  ");
                cursor = (cursor+1)%3;
                mvwaddwstr(win,cursor+3, 2, L"▶");
                break;
            case KEY_UP:
                mvwprintw(win,cursor+3, 2, "  ");
                cursor = (cursor+2)%3;
                mvwaddwstr(win,cursor+3, 2, L"▶");
                break;
            default:
                break;
        }
    }
}


//Updates the right panel
void right_panel_update(Region *reg, Player *pl, WINDOW *win)
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


void show_controls(DisplayInfo *di)
{
    WINDOW *win = di->box2;
    new_wclear(win);
    mvwaddwstr(win, 1, 2, L"ZQSD ou flèches directionnelles : se déplacer");
    mvwaddwstr(win, 2, 2, L"W : quitter le jeu");
    mvwaddwstr(win, 3, 2, L"Appuyez sur une touche pour reprendre la partie...");
    wgetch(win);
    new_wclear(win);
    wrefresh(win);
}


//Updates the display of the map
void update_map(DisplayInfo *di,Region *reg, Player *pl)
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
