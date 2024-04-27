/*
The part of the code not related to display.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "settings.h"

typedef enum
{
    RESERVED = -1, //reserved space for a roow which door was already generated
    VOID = 0,
    WALL = 1,
    DOOR = 2,
}
GridConst;

//Coordinates structure, with x and y
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
    Co loc;
}
Player;

//Structure for data about room
typedef struct Room
{
    //is the room generated
    bool is_generated; 

    //coordinates of the top-left corner of the room
    Co corner;

    //room width, walls included
    uint8_t width;
    //room height, walls included
    uint8_t height;

    uint8_t door_count;
    struct Room **door_north;
    struct Room **door_east;
    struct Room **door_south;
    struct Room **door_west;
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
Co coordinates(int16_t, int16_t);

//generates a random number with the region's seed
uint32_t new_rand(Region*);
//Generates a random number between min and max inclusive using the region's seed
uint32_t randint(Region*, uint32_t, uint32_t);

//allocates memory for a room, stores it in the region and returns a pointer to it
Room *allocate_room(Region*);
void init_room(Room*);

//initializes a new map
void initial_map(Region*);

