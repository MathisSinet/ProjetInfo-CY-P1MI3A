/*
The part of the code not related to display.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "settings.h"

typedef struct Coordinates
{
    int16_t x;
    int16_t y;
}Co;

//Structure for the items
typedef struct Item
{
    
}
Item;

//Structure for the informations of the player
typedef struct Player
{
    char name[MAX_PLAYER_NAME_COUNT];
    int32_t hp;
    int32_t xp;
    int32_t atk;
    int32_t def;
}
Player;

typedef struct Room
{
    bool is_generated; //is the room generated

    //coordinates of the top-left corner of the room
    Co corner;

    uint8_t width;
    uint8_t height;

    uint8_t door_count;
}
Room;

typedef struct Region
{
    uint64_t seed;
    int8_t **grid;
    Room *roomlist;
    uint16_t allocated_rooms;

    Co zero; //coordinated in grid of the zero
}
Region;

void allocate_room(Region*, uint16_t, uint16_t);