/*
The part of the code not related to display.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "settings.h"

#define RESERVE_WIDTH 3
#define RESERVE_HEIGHT 3

typedef enum
{
    RESERVED = -1, //reserved space for a roow which door was already generated
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

//Coordinates structure, with x and y
typedef struct Coordinates
{
    int32_t x;
    int32_t y;
}Co;

//Structure for the items
typedef struct Item
{
    
}
Item;

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
    struct Room **currentroom;
}
Player;

//Structure for a door
typedef struct Door
{
    bool exists;
    uint16_t dist;
    struct Room **to;
}
Door;

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
    Door door_north;
    Door door_east;
    Door door_south;
    Door door_west;
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
int32_t randint(Region*, int32_t, int32_t);

int8_t *get_from_grid(Region*, int32_t, int32_t);

//allocates memory for a room, stores it in the region and returns a pointer to it
Room *allocate_room(Region*);
//initialises a non-generated room with no doors 
void init_room(Room*);

//used by reserve_room
bool is_free_box(Region*, Co, uint16_t, uint16_t);
void reserve_box(Region*, Co);
//reserves space for a room (1:success, 0:failure)
int reserve_room(Region*, Room*, Pole);

//creates the walls of the room in the grid
void wall_room(Region*, Room*);

//initializes a new map
void initial_map(Region*);

