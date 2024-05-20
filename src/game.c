#include "game.h"
#include "quest.h"
#include "display.h"


/*__________PLAYER'S ACTIONS__________*/


//Tries to move the player in the corresponding direction
void playermove(Region *reg, Player *pl, Pole dir, DisplayInfo* di)
{
    Co newco = pl->loc;
    int dest_content;
    Item item;
    ItemInRoom *itemptr;

    //stores in newco the new coordinate of the player
    switch (dir)
    {
    case NORTH:
        newco.y--;
        break;
    case EAST:
        newco.x++;
        break;
    case SOUTH:
        newco.y++;
        break;
    case WEST:
        newco.x--;
    }

    //content of the destination tile
    dest_content = *get_from_grid(reg, newco.x, newco.y);

    //item pickup
    set_itemptr(pl->loc, pl, pl->currentroom, &itemptr);
    if (itemptr)
    {
        item = getitem(itemptr->index, NULL);
        switch (item.type)
        {
        case HEAL:
            if (pl->inv_size != MAX_INVENTORY_SIZE)
            {
                pl->inv[pl->inv_size++] = itemptr->index;
                itemptr->exists = false;
            }
            break;

        case WEAPON:
            pl->weapon = itemptr->index;
            pl->atk = item.stat;
            itemptr->exists = false;
            break;

        case QUEST:
            if(itemptr->index == ITEM_QUEST_QUIZZ)
            {
                quizz(pl, reg, di);
                pl->currentroom->isitem = false;
            }
            if(itemptr->index == ITEM_QUEST_TEDDYBEAR)
            {
                mvwprintw(di->box2, 1, 2, "Vous avez trouvé l'ours en peluche !");
                wrefresh(di->box2);
                wgetch(di->box2);
                pl->currentroom->isitem = false;
            }
            if(itemptr->index == ITEM_QUEST_BALL)
            {
                mvwprintw(di->box2, 1, 2, "Vous avez trouvé le ballon !");
                wrefresh(di->box2);
                wgetch(di->box2);
                pl->currentroom->isitem = false;
            }
            break;
        }
    }

    //allows movement if the destination square is empty
    if (dest_content == VOID)
    {
        pl->loc = newco;
    }

    //code when the player enters a door
    if (dest_content == DOOR)
    {
        switch (dir)
        {
        case NORTH:
            if (!pl->currentroom->door_north.to->is_generated)
            {
                generate_room(reg, pl->currentroom, dir);
            }
            pl->currentroom = pl->currentroom->door_north.to;
            pl->loc.y -= 2;
            break;
        case EAST:
            if (!pl->currentroom->door_east.to->is_generated)
            {
                generate_room(reg, pl->currentroom, dir);
            }
            pl->currentroom = pl->currentroom->door_east.to;
            pl->loc.x += 2;
            break;
        case SOUTH:
            if (!pl->currentroom->door_south.to->is_generated)
            {
                generate_room(reg, pl->currentroom, dir);
            }
            pl->currentroom = pl->currentroom->door_south.to;
            pl->loc.y += 2;
            break;
        case WEST:
            if (!pl->currentroom->door_west.to->is_generated)
            {
                generate_room(reg, pl->currentroom, dir);
            }
            pl->currentroom = pl->currentroom->door_west.to;
            pl->loc.x -= 2;
            break;
        
        default:
            break;
        }
    }
}