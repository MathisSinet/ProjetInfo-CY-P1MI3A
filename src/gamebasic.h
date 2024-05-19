#ifndef GAMEBASIC_HEADER
#define GAMEBASIC_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "settings.h"

typedef enum
{
    RESERVED = -1, //reserved space for a room which door was already generated
    VOID = 0,
    WALL = 1,
    DOOR = 2,
}
GridConst;

typedef enum
{
    NORTH, EAST, SOUTH, WEST,
}
Pole;

typedef enum
{
    WEAPON, ARMOR, HEAL
}
ItemType;

typedef enum 
{
    ITEM_HEAL1 = 0,
    ITEM_SHIELD = 1,
    ITEM_BASE_WEAPON = 20,
    ITEM_WEAPON_STICKS = 21,
    ITEM_WEAPON_BOXING = 22,
    ITEM_WEAPON_KEY = 23,
    ITEM_WEAPON_KNIFE = 24,
    ITEM_WEAPON_SWORD = 25
    
}
ItemIndex;

//Coordinates structure, with x and y
typedef struct Coordinates
{
    int32_t x;
    int32_t y;
}
Co;

//Structure for the items
typedef struct Item
{
    char *name;
    wchar_t symb;
    ItemType type;
    uint32_t stat;
}
Item;

typedef struct Monster
{
    uint16_t hp;
    uint16_t atk;
    uint64_t baseatkdelay;
    uint64_t atkdelay;
}
Monster;

struct Room;
struct Door;

//Structure for the informations of the player
typedef struct Player
{
    char name[MAX_PLAYER_NAME_COUNT];
    int32_t hp;
    int32_t xp;
    int32_t atk;
    int32_t def;
    Co loc;
    ItemIndex weapon;
    ItemIndex armor;
    ItemIndex inv[MAX_INVENTORY_SIZE];
    uint16_t inv_size;
    struct Room *currentroom;
}
Player;

//Structure for a door
typedef struct Door
{
    bool exists;
    uint16_t dist; //distance from the left wall or top wall
    struct Room *to;
}
Door;

//Structure for data about room
typedef struct Room
{
    //is the room generated
    bool is_generated;
    uint32_t index;

    //coordinates of the top-left corner of the room
    Co corner;

    //room width, walls included
    uint8_t width;
    //room height, walls included
    uint8_t height;

    Door door_north;
    Door door_east;
    Door door_south;
    Door door_west;

    bool isitem; //If the room contains an item
    ItemIndex item; //Item in the room
    Co itemloc; //Coordinates of the item (relative to the zero)
}
Room;

//structure for the map
typedef struct Region
{
    //seed used by the RNG
    uint32_t seed;

    //representation of the map : grid[x][y]
    int8_t **grid;
    uint16_t grid_width;
    uint16_t grid_height;

    //list of room pointers
    Room *roomlist[MAX_ROOM_COUNT];
    //number of allocated rooms
    uint16_t allocated_rooms;

    //coordinated in grid of the zero
    Co zero;

    //death timer
    int deathtimer;
}
Region;

//Coordinates builder
Co coordinates(int16_t x, int16_t y);

bool is_digit(int chr);
bool is_valid_playername_char(int chr);
//generates a random number with the region's seed
uint32_t new_rand(Region* r);
//Generates a random number between min and max included using the region's seed
int32_t randint(Region* reg, int32_t min, int32_t max);
bool randevent(Region *reg, uint32_t prob);


int8_t *get_from_grid(Region* reg, int32_t x, int32_t y);

void reg_memfree(Region *reg);

#endif