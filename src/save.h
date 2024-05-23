/*
save.h
This header contains stuctures and functions used by the game saving system
*/

#include "gamebasic.h"

/*
This section contains redefinition of structures found in  file gamebasic.h
that remove pointers
*/

//Structure used to save the Player structure
struct PlayerSave
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
    uint32_t currentroom;
};

//Structure used to save the Door structure
struct DoorSave
{
    bool exists;
    uint16_t dist; 
    uint32_t to;
};

//Structure used to save the Room structure
struct RoomSave
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

    struct DoorSave door_north;
    struct DoorSave door_east;
    struct DoorSave door_south;
    struct DoorSave door_west;

    //items and monsters
    ItemInRoom item1;
    ItemInRoom item2;
    MonsterInRoom monster1;
    MonsterInRoom monster2;
};

//Structure used to save the Region structure
struct RegionSave
{
    uint32_t seed;

    //representation of the map : grid[x][y]
    uint64_t gridptr;
    uint16_t grid_width;
    uint16_t grid_height;

    //list of room pointers
    uint64_t roomptr;
    //number of allocated rooms
    uint16_t allocated_rooms;
    uint16_t generated_rooms;

    //coordinated in grid of the zero
    Co zero;

    //death timer
    float deathtimer;

    QuestInfo questinfo;
};

//Structure used to save the Player structure
typedef struct PlayerSave PlayerSave;
//Structure used to save the Door structure
typedef struct DoorSave DoorSave;
//Structure used to save the Room structure
typedef struct RoomSave RoomSave;
//Structure used to save the Region structure
typedef struct RegionSave RegionSave;


int save(FILE *savefile, Region *reg, Player *pl);
int load(char *name, Region *reg, Player *pl);