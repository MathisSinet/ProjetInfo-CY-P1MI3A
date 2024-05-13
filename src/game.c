#include "game.h"


/*__________PLAYER'S ACTIONS__________*/


//Tries to move the player in the corresponding direction
void playermove(Region *reg, Player *pl, Pole dir)
{
    Co newco = pl->loc;
    int dest_content;
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

    dest_content = *get_from_grid(reg, newco.x, newco.y);
    if (dest_content == VOID)
    {
        pl->loc = newco;
    }
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