#include "settings.h"

//Structure for the items
typedef struct Item
{
    
}
Item;

//Structure for the informations of the player
typedef struct Player
{
    char name[MAX_PLAYER_NAME_COUNT];
    uint32_t hp;
    uint32_t xp;
    uint32_t atk;
    uint32_t def;
}
Player;

typedef struct Room
{
    bool is_generated; //is the room generated

    //coordinates of the top-left corner of the room
    uint16_t x; 
    uint16_t y;

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
}
Region;

void allocate_room(Region*, uint16_t, uint16_t);