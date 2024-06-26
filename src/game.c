/*
game.c
Contains functions to allow the player and monsters to move and attack
Also contains the item pickup code
*/


#include "game.h"
#include "display.h"


/*__________PLAYER'S ACTIONS__________*/


// Tries to move the player in the corresponding direction
void playermove(Region *reg, Player *pl, Pole dir, DisplayInfo* di)
{
    Co newco = pl->loc;
    int dest_content;
    Item item;
    ItemInRoom *itemptr;

    // Stores in newco the new coordinate of the player
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

    // Content of the destination tile
    dest_content = *get_from_grid(reg, newco.x, newco.y);

    // Allows movement if the destination square is empty
    if (dest_content == VOID)
    {
        pl->loc = newco;
    }

    // Item pickup
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
                if (quizz(pl, reg, di, reg->questinfo.quizz_done+1))
                {
                    itemptr->exists = false;
                }
            }
            if(itemptr->index == ITEM_QUEST_TEDDYBEAR)
            {
                new_wclear(di->box2);
                char teddybear_descr[200] = "Vous avez trouvé un vieux nounours en peluche, celui de votre fille. Il semble avoir traversé des galaxies, tout comme vous. Vous sentez la chaleur des souvenirs familiaux vous envahir.";
                item_desc(di->box2, teddybear_descr);
                us_sleep(1500*1000);
                wgetch(di->box2);
                itemptr->exists = false;
                reg->questinfo.is_teddybear_found = true;
            }
            if(itemptr->index == ITEM_QUEST_BALL)
            {
                new_wclear(di->box2);
                char ball_descr[300] = "Vous avez trouvé un ballon de football. En voyant cet objet, des souvenirs vous reviennent en tête, vous pouvez voir votre fils jouer au football avec vous. Vous décidez donc de jongler un peu avec la balle.";
                item_desc(di->box2, ball_descr);
                us_sleep(1500*1000);
                wgetch(di->box2);
                itemptr->exists = false;
                reg->questinfo.is_ball_found = true;
            }
            break;
        }
    }

    // Code when the player enters a door
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


// Player's attack
void playerattack(Region *reg, Player *pl)
{
    // Room *room = pl->currentroom;
    MonsterInRoom *monsterptr;
    if (pl->atkdelay > 0.0)
    {
        return;
    }
    pl->atkdelay = PLAYER_BASE_ATKDELAY;
    for (int16_t x=pl->loc.x-2; x<=pl->loc.x+2; x++)
    {
        for (int16_t y=pl->loc.y-2; y<=pl->loc.y+2; y++)
        {
            set_monsterptr(coordinates(x,y), pl, pl->currentroom, &monsterptr);
            if (monsterptr)
            {
                monsterptr->hp -= pl->atk;
            }
        }
    }
}


/*__________MONSTERS' ACTIONS__________*/


// Moves one monster in the corresponding direction
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
        monsterptr->movedelay = 0.03; //if the movement fails, tries again in 30 ms
    }
    else
    {
        monsterptr->loc = newco;
    }
}


// Moves the given monster in a random valid direction
void monstermove_random(Region *reg, Player *pl, Room *room, MonsterInRoom *monsterptr)
{
    monstermove_one(reg, pl, room, monsterptr, rand()%4);
}

// Moves the given monster towards the player
void monstermove_towardsplayer(Region *reg, Player *pl, Room *room, MonsterInRoom *monsterptr)
{
    if (rand()%2)
    {
        if (pl->loc.x < monsterptr->loc.x)
        {
            monstermove_one(reg, pl, room, monsterptr, WEST);
        }
        else
        {
            monstermove_one(reg, pl, room, monsterptr, EAST);
        }
    }
    else
    {
        if (pl->loc.y < monsterptr->loc.y)
        {
            monstermove_one(reg, pl, room, monsterptr, NORTH);
        }
        else
        {
            monstermove_one(reg, pl, room, monsterptr, SOUTH);
        }
    }
}


// Moves the monsters in the current room
void monstermove(Region *reg, Player *pl, double diff)
{
    Room *room = pl->currentroom;
    Monster monster;
    if (room->monster1.exists)
    {
        monster = getmonster(room->monster1.index);
        room->monster1.movedelay -= diff;
        if (room->monster1.movedelay < 0.0)
        {
            room->monster1.movedelay += monster.basemovedelay;
            if (rand()%100 > monster.agression_value)
            {
                monstermove_random(reg, pl, room, &room->monster1);
            }
            else
            {
                monstermove_towardsplayer(reg, pl, room, &room->monster1);
            }
            if (room->monster1.hp <= 0)
            {
                room->monster1.exists = false;
                pl->xp += monster.xp_reward;
                reg->questinfo.monsters_killed++;
            }
        }
    }

    if (room->monster2.exists)
    {
        monster = getmonster(room->monster2.index);
        room->monster2.movedelay -= diff;
        if (room->monster2.movedelay < 0.0)
        {
            room->monster2.movedelay += monster.basemovedelay;
            if (rand()%100 > monster.agression_value)
            {
                monstermove_random(reg, pl, room, &room->monster2);
            }
            else
            {
                monstermove_towardsplayer(reg, pl, room, &room->monster2);
            }
            if (room->monster2.hp <= 0)
            {
                room->monster2.exists = false;
                pl->xp += monster.xp_reward;
                reg->questinfo.monsters_killed++;
            }
        }
    }
}


// Makes the given monster try to attack the player
void monsterattack_one(Region *reg, Player *pl, MonsterInRoom *monster, double diff)
{
    Monster monster_data;
    double damage;
    int damage2;
    int damage_roll;

    monster_data = getmonster(monster->index);
    monster->atkdelay -= diff;
    if (monster->atkdelay > 0.0)
    {
        return;
    }
    monster->atkdelay = monster_data.baseatkdelay;
    for (int16_t x=monster->loc.x-1; x<=monster->loc.x+1; x++)
    {
        for (int16_t y=monster->loc.y-1; y<=monster->loc.y+1; y++)
        {
            if (same_coordinates(coordinates(x,y), pl->loc))
            {
                damage = monster_data.atk * (1.0 - 0.8*((double)pl->def/100));
                damage2 = (int)(100*damage);
                damage_roll = rand()%100;
                if (damage_roll >= damage2%100)
                {
                    pl->hp -= damage2/100;
                }
                else
                {
                    pl->hp -= damage2/100 + 1;
                }
            }
        }
            
    }
}


// Makes the monsters in the current room try to attack the player
void monsterattack(Region *reg, Player *pl, double diff)
{
    if (pl->currentroom->monster1.exists && pl->currentroom->monster1.hp > 0)
    {
        monsterattack_one(reg, pl, &pl->currentroom->monster1, diff);
    }
    if (pl->currentroom->monster2.exists && pl->currentroom->monster2.hp > 0)
    {
        monsterattack_one(reg, pl, &pl->currentroom->monster2, diff);
    }
}


/*__________WIN/DEATH__________*/


// Displays the death screen and run the death routine
void death(Region *reg, Player *pl, DisplayInfo *di, int cause_of_death)
{
    end_gameui(di);

    pl->death_count++;
    death_screen(di, pl, cause_of_death);

    pl->hp = PLAYER_BASE_HP;
    pl->xp = 0;
    pl->loc = coordinates(0, 0);
    pl->weapon = ITEM_BASE_WEAPON;

    pl->currentroom = reg->roomlist[0];

    init_gameui(di);
    nodelay(di->box1, true);
}


// Displays the win screen
void win(Region *reg, Player *pl, DisplayInfo *di)
{
    end_gameui(di);
    win_screen(di, pl);

    init_gameui(di);
}