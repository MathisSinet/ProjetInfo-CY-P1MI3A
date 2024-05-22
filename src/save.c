#include "save.h"

//Saves the game to the given savefile
int save(FILE *savefile, Region *reg, Player *pl)
{
    PlayerSave playersave;
    RegionSave regionsave;
    RoomSave roomsave;
    Room *room;

    if (!savefile)
    {
        return 0;
    }

    //Copying of the Player structure

    strcpy(playersave.name, pl->name);
    playersave.hp = pl->hp;
    playersave.xp = pl->xp;
    playersave.atk = pl->atk;
    playersave.def = pl->def;
    playersave.atkdelay = pl->atkdelay;
    playersave.loc = pl->loc;
    playersave.weapon = pl->weapon;
    playersave.armor = pl->armor;
    for (int i=0; i<MAX_INVENTORY_SIZE; i++)
    {
        playersave.inv[i] = pl->inv[i];
    }
    playersave.inv_size = pl->inv_size;
    playersave.currentroom = pl->currentroom->index;

    //Copying of the Region structure

    regionsave.seed = reg->seed;
    regionsave.grid_width = reg->grid_width;
    regionsave.grid_height = reg->grid_height;
    regionsave.allocated_rooms = reg->allocated_rooms;
    regionsave.generated_rooms = reg->generated_rooms;
    regionsave.zero = reg->zero;
    regionsave.deathtimer = reg->deathtimer;
    regionsave.questinfo = reg->questinfo;

    //Locates the grid and room in the savefile

    regionsave.gridptr = sizeof(PlayerSave) + sizeof(RegionSave);
    regionsave.roomptr = regionsave.gridptr + regionsave.grid_width*regionsave.grid_height*sizeof(int8_t);

    //Writes the Player and Region structures to the savefile

    rewind(savefile);
    fwrite(&playersave, sizeof(PlayerSave), 1, savefile);
    fwrite(&regionsave, sizeof(RegionSave), 1, savefile);

    //Saves the grid

    for (int x=0; x<regionsave.grid_width; x++)
    {
        fwrite(reg->grid[x], sizeof(int8_t), regionsave.grid_height, savefile);
    }

    //Saves the rooms

    for (Room **roomptr = reg->roomlist; roomptr<reg->roomlist+reg->allocated_rooms; roomptr++)
    {
        room = *roomptr;
        roomsave.is_generated = room->is_generated;
        roomsave.index = room->index;
        roomsave.corner = room->corner;
        roomsave.width = room->width;
        roomsave.height = room->height;
        roomsave.item1 = room->item1;
        roomsave.item2 = room->item2;
        roomsave.monster1 = room->monster1;
        roomsave.monster2 = room->monster2;

        roomsave.door_north.exists = room->door_north.exists;
        roomsave.door_north.dist = room->door_north.dist;
        if(roomsave.door_north.exists){
            roomsave.door_north.to = room->door_north.to->index;
        }

        roomsave.door_east.exists = room->door_east.exists;
        roomsave.door_east.dist = room->door_east.dist;
        if(roomsave.door_east.exists){
            roomsave.door_east.to = room->door_east.to->index;
        }

        roomsave.door_south.exists = room->door_south.exists;
        roomsave.door_south.dist = room->door_south.dist;
        if(roomsave.door_south.exists){
            roomsave.door_south.to = room->door_south.to->index;
        }

        roomsave.door_west.exists = room->door_west.exists;
        roomsave.door_west.dist = room->door_west.dist;
        if(roomsave.door_west.exists){
            roomsave.door_west.to = room->door_west.to->index;
        }

        fwrite(&roomsave, sizeof(RoomSave), 1, savefile);
    }

    return 1;
}


//Tries to load a save with the given name
/// @param name Player name, which is the same as the savefile name
/// @param reg Region pointer where the save will be loaded
/// @param pl Player pointer where the save will be loaded
/// @return 1 : success ; 0 : failure
int load(char *name, Region *reg, Player *pl)
{
    char path[MAX_PLAYER_NAME_COUNT+20]; //file path
    FILE *savefile;
    
    Room *room;

    //Tries to open the savefile

    sprintf(path, "saves/%s", name);
    savefile = fopen(path, "rb");
    if (!savefile)
    {
        return 0;
    }
    
    PlayerSave playersave;
    RegionSave regionsave;
    RoomSave roomsave;

    //Reads the Player and Region structures

    rewind(savefile);
    fread(&playersave, sizeof(PlayerSave), 1, savefile);
    fread(&regionsave, sizeof(RegionSave), 1, savefile);
    reg->seed = regionsave.seed;
    reg->grid_width = regionsave.grid_width;
    reg->grid_height = regionsave.grid_height;
    reg->allocated_rooms = regionsave.allocated_rooms;
    reg->generated_rooms = regionsave.generated_rooms;
    reg->zero = regionsave.zero;
    reg->deathtimer = regionsave.deathtimer;
    reg->questinfo = regionsave.questinfo;

    //Loads the grid

    //Column allocation
    reg->grid = malloc(reg->grid_width * sizeof(int8_t*));
    if (!reg->grid)
    {
        perror("Map allocation failed\n");
        abort();
    }
    //Row allocation
    for (int8_t **column = reg->grid; column < reg->grid+reg->grid_width; column++)
    {
        *column = malloc(reg->grid_height * sizeof(int8_t));
        if(!*column)
        {
            perror("Map allocation failed\n");
            abort();
        }
        fread(*column, sizeof(int8_t), reg->grid_height, savefile);
    }

    //Reads the rooms
    //room.door_POLE.to is a temporary pointer as not all rooms have a pointer assigned to them yet

    for (Room **roomptr = reg->roomlist; roomptr < reg->roomlist+MAX_ROOM_COUNT; roomptr++)
    {
        if (roomptr < reg->roomlist + reg->allocated_rooms)
        {
            fread(&roomsave, sizeof(RoomSave), 1, savefile);
            *roomptr = malloc(sizeof(Room));
            room = *roomptr;
            if (!room)
            {
                perror("Room allocation failed\n");
                abort();
            }
            
            room->is_generated = roomsave.is_generated;
            room->index = roomsave.index;
            room->corner = roomsave.corner;
            room->width = roomsave.width;
            room->height = roomsave.height;
            room->item1 = roomsave.item1;
            room->item2 = roomsave.item2;
            room->monster1 = roomsave.monster1;
            room->monster2 = roomsave.monster2;

            room->door_north.exists = roomsave.door_north.exists;
            room->door_north.dist = roomsave.door_north.dist;
            room->door_north.to = (Room*)(reg->roomlist + roomsave.door_north.to);

            room->door_east.exists = roomsave.door_east.exists;
            room->door_east.dist = roomsave.door_east.dist;
            room->door_east.to = (Room*)(reg->roomlist + roomsave.door_east.to);

            room->door_south.exists = roomsave.door_south.exists;
            room->door_south.dist = roomsave.door_south.dist;
            room->door_south.to = (Room*)(reg->roomlist + roomsave.door_south.to);

            room->door_west.exists = roomsave.door_west.exists;
            room->door_west.dist = roomsave.door_west.dist;
            room->door_west.to = (Room*)(reg->roomlist + roomsave.door_west.to);
        }
        else{
            *roomptr = NULL;
        }
    }

    //Now that all rooms have an assigned pointer, we update the pointers of the doors

    for (Room **roomptr = reg->roomlist; roomptr < reg->roomlist+reg->allocated_rooms; roomptr++)
    {
        room = *roomptr;

        if (room->door_north.exists)
        {
            room->door_north.to = *(Room**)(room->door_north.to);
        }
        if (room->door_east.exists)
        {
            room->door_east.to = *(Room**)(room->door_east.to);
        }
        if (room->door_south.exists)
        {
            room->door_south.to = *(Room**)(room->door_south.to);
        }
        if (room->door_west.exists)
        {
            room->door_west.to = *(Room**)(room->door_west.to);
        }
    }

    //We now finish by reading the Player structure

    strcpy(pl->name, name);
    pl->hp = playersave.hp,
    pl->xp = playersave.xp;
    pl->atk = playersave.atk;
    pl->def = playersave.def;
    pl->atkdelay = playersave.atkdelay;
    pl->loc = playersave.loc;
    pl->weapon = playersave.weapon;
    pl->armor = playersave.armor;
    for (int i=0; i<MAX_INVENTORY_SIZE; i++)
    {
        pl->inv[i] = playersave.inv[i];
    }
    pl->inv_size = playersave.inv_size;
    pl->currentroom = reg->roomlist[playersave.currentroom];
    
    return 1;
}