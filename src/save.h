#ifndef GAMEBASIC
    #include "game.h"
#endif

struct PlayerSave
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
    uint32_t currentroom;
};

struct DoorSave
{
    bool exists;
    uint16_t dist; 
    uint32_t to;
};

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

    //item
    bool isitem;
    ItemIndex item;
    Co itemloc;
};

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

    //coordinated in grid of the zero
    Co zero;

    //death timer
    int deathtimer;
};

typedef struct PlayerSave PlayerSave;
typedef struct DoorSave DoorSave;
typedef struct RoomSave RoomSave;
typedef struct RegionSave RegionSave;

int save(FILE *savefile, Region *reg, Player *pl);
int load(char *name, Region *reg, Player *pl);