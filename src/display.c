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
    init_pair(PAIR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
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
    wattron(di->box1, COLOR_PAIR(PAIR_CYAN));
    mvwprintw(di->box1, 1, 1, "%s", ascii_art);
    wattroff(di->box1, COLOR_PAIR(PAIR_CYAN));
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
//Ends the game interface
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


//Let the user input a string of max_len on the window, stored in buffer
/// @param win window where the input takes place
/// @param y y coordinate of the first character
/// @param x x coordinate of the first character
/// @param buffer buffer where the input string will be stored
/// @param max_len maximum len of the buffer
/// @param validatefunc validate function of a character in the string
void getusrstr(WINDOW *win, int y, int x, char *buffer, int max_len, bool(*validatefunc)(int))
{
    int len=0;
    int chr;

    while(true)
    {
        chr = wgetch(win);

        if (len > 0 && (chr == KEY_ENTER || chr == '\n' || chr == '\r'))
        {
            buffer[len] = '\0';
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
    
    mvwaddwstr(win, cursor+3, 2, RARROW_SYMB);
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
                mvwaddwstr(win,cursor+3, 2, RARROW_SYMB);
                break;
            case KEY_UP:
                mvwprintw(win,cursor+3, 2, "  ");
                cursor = (cursor+2)%3;
                mvwaddwstr(win,cursor+3, 2, RARROW_SYMB);
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
            wattron(win, COLOR_PAIR(PAIR_RED));
            mvwadd_wch(win, 3, 3+2*i, WACS_BLOCK);
            wattroff(win, COLOR_PAIR(PAIR_RED));
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
            wattron(win, COLOR_PAIR(PAIR_BLUE));
            mvwadd_wch(win, 4, 4+2*i, WACS_BLOCK);
            wattroff(win, COLOR_PAIR(PAIR_BLUE));
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

    for (int i=0; i<pl->inv_size; i++)
    {
        item = getitem(pl->inv[i], itemname);
        mvwprintw(win, 10+i, 4, "%s %lc", item.name, item.symb);
    }

    wrefresh(win);
}


//Inventory
void manage_inventory(Region *reg, Player *pl, DisplayInfo *di)
{
    WINDOW *win = di->box3;
    int ch;
    unsigned int cursor = 0;

    if (pl->inv_size <= 0)
    {
        return;
    }

    mvwprintw(win, di->height-3, 2, "U : Utiliser l'objet");
    mvwprintw(win, di->height-3, 2, "E : Fermer l'inventaire");
    mvwprintw(win, di->height-2, 2, "A : Jeter l'objet");
    mvwaddwstr(win, 10+cursor, 2, RARROW_SYMB);

    while(true)
    {
        ch = wgetch(win);
        if (ch == 'e' || ch == 'E')
        {
            break;
        }
        if (ch == 'a' || ch == 'A')
        {
            for (int i=cursor; i<pl->inv_size-1; i++)
            {
                pl->inv[i] = pl->inv[i+1];
            }
            pl->inv_size--;
            if (pl->inv_size == 0)
            {
                break;
            }
            right_panel_update(reg, pl, win);
            mvwaddwstr(win, 10+cursor, 2, RARROW_SYMB);
            if (cursor == pl->inv_size)
            {
                mvwaddwstr(win, 10+cursor, 2, L"  ");
                cursor = (cursor+pl->inv_size-1) % pl->inv_size;
                mvwaddwstr(win, 10+cursor, 2, RARROW_SYMB);
            }
        }
        if (ch == KEY_DOWN || ch == 's' || ch == 'S')
        {
            mvwaddwstr(win, 10+cursor, 2, L"  ");
            cursor = (cursor+1) % pl->inv_size;
            mvwaddwstr(win, 10+cursor, 2, RARROW_SYMB);
        }
        if (ch == KEY_UP || ch == 'z' || ch == 'Z')
        {
            mvwaddwstr(win, 10+cursor, 2, L"  ");
            cursor = (cursor+pl->inv_size-1) % pl->inv_size;
            mvwaddwstr(win, 10+cursor, 2, RARROW_SYMB);
        }
    }

    right_panel_update(reg, pl, win);
}


//Shows the controls in the bottom window
void show_controls(DisplayInfo *di)
{
    WINDOW *win = di->box2;
    new_wclear(win);
    mvwaddwstr(win, 1, 2, L"ZQSD ou flèches directionnelles : se déplacer");
    mvwaddwstr(win, 2, 2, L"X : quitter le jeu sans sauvegarder");
    mvwaddwstr(win, 3, 2, L"W : sauvegarder le jeu");
    mvwaddwstr(win, 4, 2, L"E : ouvrir/fermer l'inventaire");
    mvwaddwstr(win, 5, 2, L"Appuyez sur une touche pour reprendre la partie...");
    wgetch(win);
    new_wclear(win);
    wrefresh(win);
}


//Save User Interface
void save_ui(DisplayInfo *di, Region *reg, Player *pl)
{
    WINDOW *win = di->box2;
    FILE *savefile;

    new_wclear(win);
    char path[MAX_PLAYER_NAME_COUNT+20];
    sprintf(path, "saves/%s", pl->name);
    if (true/*access(path, W_OK)*/)
    {
        mvwaddwstr(win, 1, 2, L"Voulez-vous sauvegarder ? O/N");
        if (wgetch(win) == 'o')
        {
            new_wclear(win);
            if ((savefile = fopen(path, "wb")))
            {
                save(savefile, reg, pl);
                fclose(savefile);
                mvwprintw(win, 1, 2, "Sauvegarde dans %s", path);
                mvwaddwstr(win, 2, 2, L"Appuyez sur une touche pour revenir au jeu");
            }
            else
            {
                mvwaddwstr(win, 2, 2, L"Une erreur s'est produite. Appuyez sur une touche pour revenir au jeu");
            }
            wgetch(win);
        }
    }

    new_wclear(win);
    wrefresh(win);
}

//Updates the display of the map
void update_map(DisplayInfo *di, Region *reg, Player *pl)
{
    int w,h;
    int row=1;
    int8_t *tile;
    WINDOW *win = di->box1;

    getmaxyx(win, h, w);
    h-=3; w-=2;
    new_wclear(win);
    mvwprintw(win, row++, 2, "x=%d, y=%d, grid width=%d, grid height=%d", pl->loc.x, pl->loc.y, reg->grid_width, reg->grid_height);
    
    for (int y=pl->loc.y-h/2; y<pl->loc.y+h/2; y++)
    {
        wmove(win, row++,1);
        for (int x=pl->loc.x-w/4; x<pl->loc.x+w/4; x++)
        {
            tile = get_from_grid(reg, x, y);
            if (!tile)
            {
                wprintw(win, "  ");
                continue;
            }
            if(x==pl->loc.x && y==pl->loc.y)
            {
                waddwstr(win, CHARACTER_SYMB);
                continue;
            }
            if (pl->currentroom->isitem)
            {
                if (x==pl->currentroom->itemloc.x && y==pl->currentroom->itemloc.y)
                {
                    wprintw(win, "%lc", getitem(pl->currentroom->item, NULL).symb);
                    continue;
                }
            }
            switch (*tile)
            {
            case VOID:
                wprintw(win, "  ");
                break;
            case RESERVED:
                wprintw(win, "RR");
                break;
            case WALL:
                waddwstr(win, L"🧱");
                break;
            case DOOR:
                waddwstr(win, L"🚪");
                break;
            default:
                break;
            }
        }
    }
}
