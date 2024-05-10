/*
The part of the code not related to display.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "settings.h"

#define ITEM_HEAL1_SYMB L'🩹'

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
    ITEM_BASE_WEAPON = 20
}
ItemIndex;

//Coordinates structure, with x and y
typedef struct Coordinates
{
    int32_t x;
    int32_t y;
}Co;

//Structure for the items
typedef struct Item
{
    char *name;
    wchar_t symb;
    ItemType type;
    float stat1;
    float stat2;
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
    ItemIndex weapon;
    ItemIndex armor;
    ItemIndex inv[MAX_INVENTORY_SIZE];
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

    uint8_t door_count;
    Door door_north;
    Door door_east;
    Door door_south;
    Door door_west;

    //item
    bool isitem;
    uint8_t item;
    Co itemloc;
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


int8_t *get_from_grid(Region* reg, int32_t x, int32_t y);
Item getitem(ItemIndex index, char *name);

//allocates memory for a room, stores it in the region and returns a pointer to it
Room *allocate_room(Region* r);
//initializes a non-generated room with no doors 
void init_room(Room* room);

//used by reserve_room
bool is_free_box(Region* reg, Co corner, uint16_t width, uint16_t height);
void reserve_box(Region* reg, Co corner);
void unreserve_box(Region* reg, Co corner);
//reserves space for a room (1:success, 0:failure)
int reserve_room(Region* reg, Room* room, Pole pole);

//creates the walls of the room in the grid
void wall_room(Region* reg, Room* room);
//extends allocated memory for the grid
void extend_grid(Region* reg, Pole dir);

//initializes a new map
void initial_map(Region* reg, Player *pl);


void place_ew_side_doors(Region *reg, Room *room);
void place_ns_side_doors(Region *reg, Room *room);

bool newroom_valid_space(Region *reg, Room *source, Co corner, uint16_t width, uint16_t height);
//generates a room when a player enters it for the first time
void generate_room(Region *reg, Room* from, Pole dir);
void fill_room(Region *reg, Room *room);
//handles the movement of a player
void playermove(Region *reg, Player *pl, Pole dir);