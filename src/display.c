/*
display.c
Display features of the game
*/

#include "display.h"


// Sleeps for the given number of microseconds
void us_sleep(uint64_t us)
{
    struct timespec ts;
    ts.tv_sec = us/(1000000);
    ts.tv_nsec = (us%1000000) * 1000;
    nanosleep(&ts, &ts);
}

// Erases the content of the window and recreates the box
void new_wclear(WINDOW *win)
{
    werase(win);
    box(win, 0, 0);
}

// Initialises the user interface
void initcurses(DisplayInfo *di)
{
    initscr(); 
    cbreak();
    noecho();
    curs_set(0);

    getmaxyx(stdscr, di->height, di->width);

    if (di->height < MIN_TERMINAL_HEIGHT || di->width < MIN_TERMINAL_WIDTH)
    {
        mvprintw(0,0, "Vous devez avoir un terminal de taille minimale %dx%d pour utiliser ce programme", MIN_TERMINAL_WIDTH,
            MIN_TERMINAL_HEIGHT);
        mvprintw(2,0, "Votre terminal a pour taille %dx%d", di->width, di->height);
        getch();
        abort();
    }

    start_color();
    init_color(COLOR_GREY, 500, 500, 500);
    init_pair(PAIR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(PAIR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(PAIR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(PAIR_GREY, COLOR_GREY, COLOR_BLACK);
    init_pair(PAIR_GREEN, COLOR_GREEN, COLOR_BLACK);
}

// Initializes the main menu interface
void init_mainmenu(DisplayInfo *di)
{
    int y,i;
    // Load ASCII art
    char *ascii_art =
        " ::::::::    ::::::::    ::::::::    ::::    ::::   :::::::::::  ::::::::        :::   :::   ::::::::   ::::    :::  :::::::::   ::::::::::  ::::::::: \n"
        ":+:    :+:  :+:     :+:  :+:    :+:  +:+:+: :+:+:+      :+:     :+:    :+:       :+:   :+:  :+:    :+:  :+:+:   :+:  :+:    :+:  :+:         :+:    :+:\n"
        "+:+         +:+     +:+  +:+         +:+ +:+:+ +:+      +:+     +:+               +:+ +:+   +:+    +:+  :+:+:+  +:+  +:+    +:+  +:+         +:+    +:+\n"
        "+#+         +#+     +:+  +#++:++#++  +#+  +:+  +#+      +#+     +#+                +#++:    +#+    +:+  +#+ +:+ +#+  +#+    +:+  +#++:++#    +#++:++#: \n"
        "+#+         +#+     +#+         +#+  +#+       +#+      +#+     +#+                 +#+     +#+    +#+  +#+  +#+#+#  +#+    +#+  +#+         +#+    +#+\n"
        "#+#    #+#  #+#     #+#  #+#    #+#  #+#       #+#      #+#     #+#    #+#          #+#     #+#    #+#  #+#   #+# #  +#    #+#   #+#         #+#    #+#\n"
        " ########    ########    ########    ###       ###  ###########  ########           ###      ########   ###    ####  #########   ##########  ###    ###\n";

    // Calculate the number of lines in the ASCII art
    int num_lines = 7;
    int num_char_line = 151;
    // Calculate position to center ASCII horizontally
    int start_x = (di->width - num_char_line) / 2;

    // Generate a new box for ASCII ART
    di->box1 = newwin(num_lines+1, di->width, 3, 0);

    // Print ASCII art
    wattron(di->box1, COLOR_PAIR(PAIR_CYAN));
    i = 0;
    y = 1;
    wmove(di->box1, 1, start_x);
    while (ascii_art[i] && di->width+2 > num_char_line)
    {
        if (ascii_art[i] != '\n')
        {
            wprintw(di->box1, "%c", ascii_art[i]);
        }
        else
        {
            wmove(di->box1, ++y, start_x);
        }
        i++;
    }
    wattroff(di->box1, COLOR_PAIR(PAIR_CYAN));
    wrefresh(di->box1);

    // Bottom box
    di->box2 = newwin(4*(di->height - 15)/5, 4*di->width/5, 18, di->width/10);
    keypad(di->box2, true);
    box(di->box2, 0, 0);
}

// Ends the main menu interface
void end_mainmenu(DisplayInfo *di)
{
    delwin(di->box1);
    delwin(di->box2);
    refresh();
}

// Initializes the game interface
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
// Ends the game interface
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

        if (len > 0 && (chr == KEY_BACKSPACE || chr == KEY_DC || chr == 127))
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


// Main menu loop. Returns an integer corresponding to the user's choice
int MainMenu(DisplayInfo *di)
{
    WINDOW *win = di->box2;
    uint8_t cursor = 0;
    int chr;

    new_wclear(win);
    mvwprintw(win, 1, 2, "Bienvenue dans le jeu !");
    waddwstr(win, L" 👽");
    mvwaddwstr(win, 2, 2, L"Appuyez sur entrée pour sélectionner ou sur les flèches pour vous déplacer");
    mvwprintw(win, 3, 5, "Nouvelle partie");
    mvwprintw(win, 4, 5, "Charger une partie");
    mvwprintw(win, 5, 5, "Quitter le jeu");

    mvwaddwstr(win, 7 ,2, L"En jeu, utilisez la touche C pour accéder aux contrôles");
    
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


// Updates the right panel
void right_panel_update(Region *reg, Player *pl, WINDOW *win)
{
    char itemname[15];
    Item item;

    new_wclear(win);

    // Player name
    wattron(win, A_BOLD);
    mvwaddstr(win, 1,2, pl->name);
    wattroff(win, A_BOLD);

    // HP bar
    mvwaddstr(win, 3, 2, "PV ");
    for (int i=1; i<=10; i++)
    {
        if (pl->hp >= i)
        {
            wattron(win, COLOR_PAIR(PAIR_RED));
            mvwadd_wch(win, 3, 4+2*i, WACS_BLOCK);
            wattroff(win, COLOR_PAIR(PAIR_RED));
        }
        else
        {
            mvwadd_wch(win, 3, 4+2*i, WACS_BLOCK);
        }
    }

    // Defense bar
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

    item = getitem(pl->weapon, itemname);
    // Attack load bar
    mvwprintw(win, 5, 2, "%lc", item.symb);
    for (int i=1; i<=10; i++)
    {
        if (pl->atkdelay <= PLAYER_BASE_ATKDELAY - i*PLAYER_BASE_ATKDELAY/10)
        {
            wattron(win, COLOR_PAIR(PAIR_GREEN));
            mvwadd_wch(win, 5, 4+2*i, WACS_BLOCK);
            wattroff(win, COLOR_PAIR(PAIR_GREEN));
        }
        else
        {
            mvwadd_wch(win, 5, 4+2*i, WACS_BLOCK);
        }
    }

    // Attack and score
    mvwprintw(win, 6, 2, "Attaque : %d", pl->atk);
    mvwprintw(win, 7, 2, "Score : %d", pl->xp);
    
    // Weapon
    mvwprintw(win, 9, 2, "Arme : ");
    wprintw(win, "%s %lc", item.name, item.symb);
    mvwprintw(win, 10, 2, "Inventaire :");

    // Inventory
    for (int i=0; i<MAX_INVENTORY_SIZE; i++)
    {
        if (i < pl->inv_size)
        {
            item = getitem(pl->inv[i], itemname);
            mvwprintw(win, 11+i, 4, "%s %lc", item.name, item.symb);
        }
        else
        {
            mvwprintw(win, 11+i, 4, "[vide]");
        }
    }

    wrefresh(win);
}


// Updates the bottom
void bottom_panel_update(Region *reg, Player *pl, WINDOW *win)
{
    new_wclear(win);
    wattron(win, A_BOLD);
    mvwprintw(win, 1, 2, "Objectifs :");
    wattroff(win, A_BOLD);
    mvwaddwstr(win, 2, 2, L"Objets à récupérer : 🧸");
    waddstr(win, reg->questinfo.is_teddybear_found ? "[X]" : "[ ]");
    waddwstr(win, L"  ⚽");
    waddstr(win, reg->questinfo.is_ball_found ? "[X]" : "[ ]");
    mvwaddwstr(win, 3, 2, L"Quiz à terminer : ");
    wprintw(win, "%d/3", reg->questinfo.quizz_done);
    mvwaddwstr(win, 4, 2, L"Monstres à éliminer : ");
    wprintw(win, "%d/%d", reg->questinfo.monsters_killed, GOAL_MONSTER_KILLS);

    wrefresh(win);
}


// Inventory managment function
void manage_inventory(Region *reg, Player *pl, DisplayInfo *di)
{
    WINDOW *win = di->box3;
    int ch;
    unsigned int cursor = 0;
    Item item;
    ItemIndex itemindex;
    bool drop;

    // This function does nothing if the inventory is empty
    if (pl->inv_size <= 0)
    {
        return;
    }

    mvwprintw(win, di->height-4, 2, "U : Utiliser l'objet");
    mvwprintw(win, di->height-3, 2, "E : Fermer l'inventaire");
    mvwprintw(win, di->height-2, 2, "A : Jeter l'objet");
    mvwaddwstr(win, 11+cursor, 2, RARROW_SYMB);

    while(true)
    {
        ch = wgetch(win);
        drop = false;
        // Close inventory
        if (ch == 'e' || ch == 'E')
        {
            break;
        }
        // Use item
        if (ch == 'u' || ch == 'U')
        {
            item = getitem(pl->inv[cursor], NULL);
            if (pl->inv[cursor] == ITEM_SHIELD)
            {
                pl->def += 10;
                pl->def = pl->def > 100 ? 100 : pl->def;
                drop = true;
            }
            else if (item.type == HEAL)
            {
                pl->hp += item.stat;
                pl->hp = pl->hp > PLAYER_BASE_HP ? PLAYER_BASE_HP : pl->hp;
                drop = true;
            }
            if (item.type == WEAPON)
            {
                itemindex = pl->weapon;
                pl->weapon = pl->inv[cursor];
                if (itemindex != ITEM_BASE_WEAPON)
                {
                    pl->inv[cursor] = itemindex;
                }
                else
                {
                    drop = true;
                }
                pl->atk = item.stat;
                //Display update
                right_panel_update(reg, pl, win);
                mvwprintw(win, di->height-4, 2, "U : Utiliser l'objet");
                mvwprintw(win, di->height-3, 2, "E : Fermer l'inventaire");
                mvwprintw(win, di->height-2, 2, "A : Jeter l'objet");
            }
        }
        // Drop item
        if (ch == 'a' || ch == 'A' || drop)
        {
            // Removes the item from the inventory
            for (int i=cursor; i<pl->inv_size-1; i++)
            {
                pl->inv[i] = pl->inv[i+1];
            }
            pl->inv_size--;
            // If empty inventory, stop the inventory manager
            if (pl->inv_size == 0)
            {
                break;
            }
            // Display update
            right_panel_update(reg, pl, win);
            mvwprintw(win, di->height-4, 2, "U : Utiliser l'objet");
            mvwprintw(win, di->height-3, 2, "E : Fermer l'inventaire");
            mvwprintw(win, di->height-2, 2, "A : Jeter l'objet");
            mvwaddwstr(win, 11+cursor, 2, RARROW_SYMB);
            if (cursor == pl->inv_size)
            {
                mvwaddwstr(win, 11+cursor, 2, L"  ");
                cursor = (cursor+pl->inv_size-1) % pl->inv_size;
                mvwaddwstr(win, 11+cursor, 2, RARROW_SYMB);
            }
        }
        if (ch == KEY_DOWN || ch == 's' || ch == 'S')
        {
            mvwaddwstr(win, 11+cursor, 2, L"  ");
            cursor = (cursor+1) % pl->inv_size;
            mvwaddwstr(win, 11+cursor, 2, RARROW_SYMB);
        }
        if (ch == KEY_UP || ch == 'z' || ch == 'Z')
        {
            mvwaddwstr(win, 11+cursor, 2, L"  ");
            cursor = (cursor+pl->inv_size-1) % pl->inv_size;
            mvwaddwstr(win, 11+cursor, 2, RARROW_SYMB);
        }
    }

    right_panel_update(reg, pl, win);
}


// Shows the controls in the bottom window
void show_controls(DisplayInfo *di)
{
    WINDOW *win = di->box2;
    new_wclear(win);
    mvwaddwstr(win, 1, 2, L"ZQSD ou flèches directionnelles : se déplacer");
    mvwaddwstr(win, 2, 2, L"X : quitter / W : sauvegarder");
    mvwaddwstr(win, 3, 2, L"Espace : attaquer");
    mvwaddwstr(win, 4, 2, L"E : ouvrir/fermer l'inventaire");
    mvwaddwstr(win, 5, 2, L"Appuyez sur une touche pour reprendre la partie...");
    wgetch(win);
    new_wclear(win);
    wrefresh(win);
}


// Save User Interface
void save_ui(DisplayInfo *di, Region *reg, Player *pl)
{
    WINDOW *win = di->box2;
    FILE *savefile;

    new_wclear(win);
    // Sets the path
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
                mvwaddwstr(win, 2, 2, L"Appuyez sur une touche pour revenir au jeu...");
            }
            else
            {
                mvwaddwstr(win, 2, 2, L"Une erreur s'est produite. Appuyez sur une touche pour revenir au jeu...");
            }
            wgetch(win);
        }
    }

    new_wclear(win);
    wrefresh(win);
}


// Updates the display of the map
void update_map(DisplayInfo *di, Region *reg, Player *pl)
{
    int w,h;
    int row=1;
    int8_t *tile;
    WINDOW *win = di->box1;

    ItemInRoom *itemptr;
    MonsterInRoom *monsterptr;

    getmaxyx(win, h, w);
    h-=3; w-=2;
    new_wclear(win);
    reg->deathtimer < DEFAULT_DEATH_TIMER / 10 ? wattron(win, COLOR_PAIR(PAIR_RED)) : 0;
    mvwprintw(win, row++, 2, "%.f secondes avant ", reg->deathtimer);
    waddwstr(win, L"épuisement de l'oxygène de la station");
    reg->deathtimer < DEFAULT_DEATH_TIMER / 10 ? wattroff(win, COLOR_PAIR(PAIR_RED)) : 0;
    
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
            if(same_coordinates(coordinates(x,y), pl->loc))
            {
                waddwstr(win, CHARACTER_SYMB);
                continue;
            }

            set_itemptr(coordinates(x,y), pl, pl->currentroom, &itemptr);
            if (itemptr)
            {
                wprintw(win, "%lc", getitem(itemptr->index, NULL).symb);
                if (itemptr->index == ITEM_SHIELD || itemptr->index == ITEM_WEAPON_SWORD)
                {
                    wprintw(win, " ");
                }
                continue;
            }

            set_monsterptr(coordinates(x,y), pl, pl->currentroom, &monsterptr);
            if (monsterptr)
            {
                if (monsterptr->hp > 0)
                {
                    wprintw(win, "%lc", getmonster(monsterptr->index, NULL).symb);
                }
                else
                {
                    waddwstr(win, L"💥");
                }
                
                continue;
            }

            switch (*tile)
            {
            case VOID:
                wprintw(win, "  ");
                break;
            case RESERVED:
                wattron(win, COLOR_PAIR(PAIR_GREY));
                wprintw(win, "RR");
                wattroff(win, COLOR_PAIR(PAIR_GREY));
                break;
            case WALL:
                waddwstr(win, WALL_SYMB);
                break;
            case DOOR:
                waddwstr(win, DOOR_SYMB);
                break;
            default:
                break;
            }
        }
    }
}

// Displays the lore items description on the screen
void item_desc(WINDOW *win, char* chaine)
{
    // Adjust the cursor position to start just below the top left corner of the box
    int i=0, y, x;
    int wheight, wwidth;
    getmaxyx(win, wheight, wwidth);

    y = 1; x = 2;
    wmove(win, y, x);
    // Print the text line by line with a delay
    while(chaine[i] != '\0')
    {
        if(chaine[i] == '\n' || (chaine[i] == ' ' && x > wwidth-12))
        {
            y++; x=2;
            wmove(win, y, x);
        }
        else
        {
            // Print the character
            wprintw(win, "%c", chaine[i]);
            x++;
        }
        wrefresh(win);
        i++;
    }
    mvwprintw(win, wheight-2, 2, "Appyuez sur une touche pour continuer...");
    wrefresh(win);
}


// Displays the lore screen
void lore_screen(DisplayInfo *di, WINDOW* lore_box)
{
    new_wclear(lore_box);
    // Adjust the cursor position to start just below the top left corner of the box
    int i=0, y, x;
    int wheight, wwidth;
    getmaxyx(lore_box, wheight, wwidth);

    y = 1; x = 2;
    wmove(lore_box, y, x);

    char chaine[1000] = "Nous sommes en 2180. Vous incarnez un pionnier de l'espace envoyé par la NASA à bord de sa station spatiale "
                        "parcourant l'immensité du vide, lorsque vous arrivez sur une planète encore inexplorée, votre base est attaquée par des êtres d'origine "
                        "inconnue. Votre mission est de réaliser des tâches de maintenance de votre station tout en combattant des menaces biologiques mystérieuses, "
                        "afin de rester en vie. \nLa station est un avant-poste crucial pour l'exploration de l'espace et la recherche scientifique. Ainsi, toute "
                        "défaillance pourrait compromettre les données précieuses collectées et mettre fin à cette mission vitale. \nDans cette lutte pour la survie et "
                        "la préservation de votre vaisseau, des objets singuliers vous rappellent des souvenirs de votre vie passée sur Terre, les récupérer renforcera "
                        "votre humanité. ";
    // Print the text line by line with a delay
    nodelay(lore_box, true);
    while(chaine[i] != '\0')
    {
        if(chaine[i] == '\n' || (chaine[i] == ' ' && x > wwidth-12))
        {
            y++; x=2;
            wmove(lore_box, y, x);
        }
        else
        {
            // Print the character
            wprintw(lore_box, "%c", chaine[i]);
            x++;
        }
        if (wgetch(lore_box) != ERR)
        {
            // If a key is pressed, break the loop
            break;
        }
        wrefresh(lore_box);
        usleep(15000);
        i++;
    }
    nodelay(lore_box, false);
    wmove(lore_box, wheight-2, 2);
    wprintw(lore_box, "Appuyez sur une touche pour continuer...");
    wgetch(lore_box);
}


// Displays the win screen
void win_screen(DisplayInfo *di, Player *pl)
{
    int y,i;
    char buffer[100];
    char *ascii_art =
":::     :::  :::::::::::  ::::::::  :::::::::::  ::::::::  :::::::::::  :::::::::   ::::::::::\n"
":+:     :+:      :+:     :+:    :+:     :+:     :+:    :+:     :+:      :+:    :+:  :+:       \n"
"+:+     +:+      +:+     +:+            +:+     +:+    +:+     +:+      +:+    +:+  +:+       \n"
"+#+     +:+      +#+     +#+            +#+     +#+    +:+     +#+      +#++:++#:   +#++:++#  \n"
" +#+   +#+       +#+     +#+            +#+     +#+    +#+     +#+      +#+    +#+  +#+       \n"
"  #+#+#+#        #+#     #+#    #+#     #+#     #+#    #+#     #+#      #+#    #+#  #+#       \n"
"    ###      ###########  ########      ###      ########  ###########  ###    ###  ##########\n";

    // Calculate the number of lines in the ASCII art
    int num_lines = 7;
    int num_char_line = 94;
    // Calculate position to center ASCII horizontally
    int start_x = (di->width - num_char_line) / 2;

    // Generate a new box for ASCII ART
    WINDOW* win_box = newwin(2*num_lines + 6, di->width, 3, 0);

    // Print ASCII art
    wattron(win_box, COLOR_PAIR(PAIR_GREEN));
    i = 0;
    y = 1;
    wmove(win_box, 1, start_x);
    while (ascii_art[i] && di->width+2 > num_char_line)
    {
        if (ascii_art[i] != '\n')
        {
            wprintw(win_box, "%c", ascii_art[i]);
        }
        else
        {
            wmove(win_box, ++y, start_x);
        }
        i++;
    }
    wattroff(win_box, COLOR_PAIR(PAIR_GREEN));
    wrefresh(win_box);

    // Displays a sentence depending on the cause of the player win
    y = 2*num_lines + 2;

    mvwaddwstr(win_box, y, (di->width-48)/2, L"Félicitations, vous avez triomphé des monstres !");
    mvwaddwstr(win_box, y+1, (di->width-76)/2, L"La station est réparée, vous continuez sain et sauf votre recherche spatiale");
    snprintf(buffer, 100, "XP obtenue : %d", pl->xp);
    mvwaddstr(win_box, y+2, (di->width-strlen(buffer)-1)/2, buffer);
    snprintf(buffer, 100, "Nombre de mort(s) : %d", pl->death_count);
    mvwaddstr(win_box, y+3, (di->width-strlen(buffer)-1)/2, buffer);
    refresh();

    wgetch(win_box);

    delwin(win_box);
}

// Displays the game over screen
void death_screen(DisplayInfo *di, Player *pl, int cause_of_death)
{
    int y,i;
    char buffer[100];
    char *ascii_art =
        " ::::::::       :::      ::::    ::::   ::::::::::       ::::::::   :::     :::  ::::::::::  ::::::::: \n"
        ":+:    :+:    :+: :+:    +:+:+: :+:+:+  :+:             :+:    :+:  :+:     :+:  :+:         :+:    :+:\n"
        "+:+          +:+   +:+   +:+ +:+:+ +:+  +:+             +:+    +:+  +:+     +:+  +:+         +:+    +:+\n"
        ":#:         +#++:++#++:  +#+  +:+  +#+  +#++:++#        +#+    +:+  +#+     +:+  +#++:++#    +#++:++#: \n"
        "+#+   +#+#  +#+     +#+  +#+       +#+  +#+             +#+    +#+   +#+   +#+   +#+         +#+    +#+\n"
        "#+#    #+#  #+#     #+#  #+#       #+#  #+#             #+#    #+#    #+#+#+#    #+#         #+#    #+#\n"
        " ########   ###     ###  ###       ###  ##########       ########       ###      ##########  ###    ###\n";

    // Calculate the number of lines in the ASCII art
    int num_lines = 7;
    int num_char_line = 103;
    // Calculate position to center ASCII horizontally
    int start_x = (di->width - num_char_line) / 2;

    // Generate a new box for ASCII ART
    WINDOW* gameover_box = newwin(2*num_lines + 6, di->width, 3, 0);

    // Print ASCII art
    wattron(gameover_box, COLOR_PAIR(PAIR_RED));
    i = 0;
    y = 1;
    wmove(gameover_box, 1, start_x);
    while (ascii_art[i] && di->width+2 > num_char_line)
    {
        if (ascii_art[i] != '\n')
        {
            wprintw(gameover_box, "%c", ascii_art[i]);
        }
        else
        {
            wmove(gameover_box, ++y, start_x);
        }
        i++;
    }
    wattroff(gameover_box, COLOR_PAIR(PAIR_RED));
    wrefresh(gameover_box);

    // Displays a sentence depending on the cause of the player death
    y = 2*num_lines + 2;

    switch (cause_of_death)
    {
    case 1:
        mvwaddwstr(gameover_box, y, (di->width-92)/2, L"Votre courage a été vain, les monstres ont triomphé... Appuyez sur une touche pour continuer");
        snprintf(buffer, 100, "XP obtenue : %d", pl->xp);
        mvwaddstr(gameover_box, y+2, (di->width-strlen(buffer)-1)/2, buffer);
        snprintf(buffer, 100, "Nombre de mort(s) : %d", pl->death_count);
        mvwaddstr(gameover_box, y+3, (di->width-strlen(buffer)-1)/2, buffer);
        wrefresh(gameover_box);
        break;
    case 2:
        mvwaddwstr(gameover_box, y, (di->width-66)/2, L"Vous n'avez plus d'oxygène.. Appuyez sur une touche pour continuer");
        snprintf(buffer, 100, "XP obtenue : %d", pl->xp);
        mvwaddstr(gameover_box, y+2, (di->width-strlen(buffer)-1)/2, buffer);
        snprintf(buffer, 100, "Nombre de mort(s) : %d", pl->death_count);
        mvwaddstr(gameover_box, y+3, (di->width-strlen(buffer)-1)/2, buffer);
        wrefresh(gameover_box);
        break;
    }

    sleep(2);
    wgetch(gameover_box);

    wclear(gameover_box);
    wrefresh(gameover_box);
    delwin(gameover_box);
    refresh();
}
