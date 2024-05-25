/*
gamebasic.h
Basic structures for the game and basic functions to manipulate them
*/

#ifndef GAMEBASIC_HEADER
#define GAMEBASIC_HEADER

#define _XOPEN_SOURCE 700
#include <ncursesw/ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>


#include "settings.h"

// Structure for display information and window pointers
typedef struct DisplayInfo
{
    int width;
    int height;
    WINDOW *box1;
    WINDOW *box2;
    WINDOW *box3;
}
DisplayInfo;

typedef enum
{
    RESERVED = -1, // Reserved space for a room which door was already generated
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
    WEAPON, HEAL, QUEST
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
    ITEM_WEAPON_SWORD = 25,
    ITEM_QUEST_QUIZZ = 26,
    ITEM_QUEST_TEDDYBEAR = 27,
    ITEM_QUEST_BALL = 28,
}
ItemIndex;

typedef enum 
{
    MONSTER_INVADER = 0,
    MONSTER_ALIEN = 1,
    MONSTER_SAUCER = 2,
}
MonsterIndex;

// Coordinates structure, with x and y
typedef struct Coordinates
{
    int32_t x;
    int32_t y;
}
Co;

// Structure for the items
typedef struct Item
{
    char *name;
    wchar_t symb;
    ItemType type;
    uint32_t stat;
}
Item;

// Structure for the monsters
typedef struct Monster
{
    wchar_t symb; // Monster symbol
    int16_t hp; // Monster health
    double atk; // Monster attack
    uint16_t xp_reward;
    // Value between 0 and 100 to determine the monster's movement pattern (higher is more agressive)
    uint16_t agression_value; 
    double baseatkdelay; // Delay between attacks
    double basemovedelay; // Delay between moves
}
Monster;

struct Room;
struct Door;

// Structure for the informations of the player
typedef struct Player
{
    char name[MAX_PLAYER_NAME_COUNT];
    int32_t hp;
    int32_t xp;
    int32_t atk;
    int32_t def;
    int32_t death_count;
    double atkdelay;

    Co loc;

    ItemIndex weapon;
    ItemIndex armor;
    ItemIndex inv[MAX_INVENTORY_SIZE];
    uint16_t inv_size;

    struct Room *currentroom;
}
Player;

// Structure for a door
typedef struct Door
{
    bool exists;
    uint16_t dist; // Distance from the left wall or top wall
    struct Room *to;
}
Door;


// Structure for the quest informations
typedef struct QuestInfo
{
    bool is_teddybear_generated;
    bool is_teddybear_found;
    bool is_ball_generated;
    bool is_ball_found;

    uint16_t monsters_killed;

    uint8_t quizz_done;
    uint8_t quizz_generated;
}
QuestInfo;


// This structure locates an item, storing the index instead of the full Item structure
typedef struct ItemInRoom
{
    bool exists;
    ItemIndex index;
    Co loc;
}
ItemInRoom;


// This structure locates an monster and its current state, storing the index instead of the full Monster structure
typedef struct MonsterInRoom
{
    bool exists;
    ItemIndex index;
    Co loc;
    int16_t hp;
    double atkdelay;
    double movedelay;
}
MonsterInRoom;


// Structure for data about room
typedef struct Room
{
    // Is the room generated
    bool is_generated;
    uint32_t index;

    // Coordinates of the top-left corner of the room
    Co corner;

    // Room width, walls included
    uint8_t width;
    // Room height, walls included
    uint8_t height;

    // Doors of the room
    Door door_north;
    Door door_east;
    Door door_south;
    Door door_west;

    // Items and monsters
    ItemInRoom item1;
    ItemInRoom item2;
    MonsterInRoom monster1;
    MonsterInRoom monster2;
}
Room;


// Structure for the map
typedef struct Region
{
    // Seed used by the RNG
    uint32_t seed;

    // Representation of the map : grid[x][y]
    int8_t **grid;
    uint16_t grid_width;
    uint16_t grid_height;

    // List of room pointers
    Room *roomlist[MAX_ROOM_COUNT];
    // number of allocated rooms
    uint16_t allocated_rooms;
    // number of generated rooms
    uint16_t generated_rooms;

    // Coordinated in grid of the zero
    Co zero;

    // Death timer
    float deathtimer;

    QuestInfo questinfo;
}
Region;

Co coordinates(int16_t x, int16_t y);
bool same_coordinates(Co co1, Co co2);

bool is_digit(int chr);
bool is_valid_playername_char(int chr);

uint32_t new_rand(Region* r);
int32_t randint(Region* reg, int32_t min, int32_t max);
bool randevent(Region *reg, uint32_t prob);


int8_t *get_from_grid(Region* reg, int32_t x, int32_t y);

void reg_memfree(Region *reg);

bool is_inside_room(Room *room, Co co);
void set_itemptr(Co co, Player *pl, Room *room, ItemInRoom **itemptr);
void set_monsterptr(Co co, Player *pl, Room *room, MonsterInRoom **monsterptr);

#endif