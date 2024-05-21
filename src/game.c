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
        case WEAPON:
            if(pl->weapon == ITEM_BASE_WEAPON)
            {
                pl->weapon = itemptr->index;
                pl->atk = item.stat;
                itemptr->exists = false;
                break;
            }
        case HEAL:
            if (pl->inv_size != MAX_INVENTORY_SIZE)
            {
                pl->inv[pl->inv_size++] = itemptr->index;
                itemptr->exists = false;
            }
            break;
        

        case QUEST:
            if(itemptr->index == ITEM_QUEST_QUIZZ)
            {
                quizz(pl, reg, di);
                itemptr->exists = false;
            }
            if(itemptr->index == ITEM_QUEST_TEDDYBEAR)
            {
                mvwprintw(di->box2, 1, 2, "Vous avez trouvé l'ours en peluche !");
                wrefresh(di->box2);
                wgetch(di->box2);
                itemptr->exists = false;
            }
            if(itemptr->index == ITEM_QUEST_BALL)
            {
                mvwprintw(di->box2, 1, 2, "Vous avez trouvé le ballon !");
                wrefresh(di->box2);
                wgetch(di->box2);
                itemptr->exists = false;
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


void monstermove_one(Region *reg, Player *pl, Room *room, MonsterInRoom *monsterptr, Pole pole)
{
    Co newco = monsterptr->loc;
    switch(pole)
    {
        case NORTH:
            if (monsterptr->loc.y-1 > room->corner.y)
            {
                newco.y--;
            }
            break;
        case EAST:
            if (monsterptr->loc.x+1 < room->corner.x + room->width-1)
            {
                newco.x++;
            }
            break;
        case SOUTH:
            if (monsterptr->loc.y+1 < room->corner.y + room->height-1)
            {
                newco.y++;
            }
            break;
        case WEST:
            if (monsterptr->loc.x-1 > room->corner.x)
            {
                newco.x--;
            }
            break;
    }
    if (same_coordinates(newco, room->monster1.loc) || same_coordinates(newco, room->monster2.loc) ||
        same_coordinates(newco, room->item1.loc) || same_coordinates(newco, room->item2.loc)
        || same_coordinates(newco, pl->loc))
    {
        monsterptr->movedelay = 0.04;
    }
    else
    {
        monsterptr->loc = newco;
    }
}


//Function for the movement of the monsters
void monstermove(Region *reg, Player *pl, double diff)
{
    Room *room = pl->currentroom;
    Monster monster;
    if (room->monster1.exists)
    {
        monster = getmonster(room->monster1.index, NULL);
        room->monster1.movedelay -= diff;
        if (room->monster1.movedelay < 0.0)
        {
            room->monster1.movedelay += monster.basemovedelay;
            if (rand()%100 < 33)
            {
                monstermove_one(reg, pl, room, &pl->currentroom->monster1, rand()%4);
            }
            else
            {
                if (rand()%2)
                {
                    if (pl->loc.x < room->monster1.loc.x)
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster1, WEST);
                    }
                    else
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster1, EAST);
                    }
                }
                else
                {
                    if (pl->loc.y < room->monster1.loc.y)
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster1, NORTH);
                    }
                    else
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster1, SOUTH);
                    }
                }
            }
        }
    }

    if (room->monster2.exists)
    {
        monster = getmonster(room->monster2.index, NULL);
        room->monster2.movedelay -= diff;
        if (room->monster2.movedelay < 0.0)
        {
            room->monster2.movedelay += monster.basemovedelay;
            if (rand()%100 < 33)
            {
                monstermove_one(reg, pl, room, &pl->currentroom->monster2, rand()%4);
            }
            else
            {
                if (rand()%2)
                {
                    if (pl->loc.x < room->monster2.loc.x)
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster2, WEST);
                    }
                    else
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster2, EAST);
                    }
                }
                else
                {
                    if (pl->loc.y < room->monster2.loc.y)
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster2, NORTH);
                    }
                    else
                    {
                        monstermove_one(reg, pl, room, &pl->currentroom->monster2, SOUTH);
                    }
                }
            }
        }
    }
}


//Player's attack
void playerattack(Region *reg, Player *pl)
{

}