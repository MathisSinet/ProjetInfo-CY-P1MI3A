#include "save.h"

int save(FILE *savefile, Region *reg, Player *pl)
{
    PlayerSave playersave;
    RegionSave regionsave;
    RoomSave roomsave;
    Room *room;

    strcpy(playersave.name, pl->name);
    playersave.hp = pl->hp;
    playersave.xp = pl->xp;
    playersave.atk = pl->atk;
    playersave.def = pl->def;
    playersave.loc = pl->loc;
    playersave.weapon = pl->weapon;
    playersave.armor = pl->armor;
    for (int i=0; i<MAX_INVENTORY_SIZE; i++)
    {
        playersave.inv[i] = pl->inv[i];
    }
    playersave.inv_size = pl->inv_size;
    playersave.currentroom = pl->currentroom->index;

    regionsave.seed = reg->seed;
    regionsave.grid_width = reg->grid_width;
    regionsave.grid_height = reg->grid_height;
    regionsave.allocated_rooms = reg->allocated_rooms;
    regionsave.zero = reg->zero;
    regionsave.deathtimer = reg->deathtimer;

    regionsave.gridptr = sizeof(PlayerSave) + sizeof(RegionSave);
    regionsave.roomptr = regionsave.gridptr + regionsave.grid_width*regionsave.grid_height*sizeof(int8_t);

    rewind(savefile);
    fwrite(&playersave, sizeof(PlayerSave), 1, savefile);
    fwrite(&regionsave, sizeof(RegionSave), 1, savefile);

    for (int x=0; x<regionsave.grid_width; x++)
    {
        fwrite(reg->grid[x], sizeof(int8_t), regionsave.grid_height, savefile);
    }

    for (Room **roomptr = reg->roomlist; roomptr<reg->roomlist+reg->allocated_rooms; roomptr++)
    {
        room = *roomptr;
        roomsave.is_generated = room->is_generated;
        roomsave.index = room->index;
        roomsave.corner = room->corner;
        roomsave.width = room->width;
        roomsave.height = room->height;
        roomsave.isitem = room->isitem;
        roomsave.item = room->item;
        roomsave.itemloc = room->itemloc;

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

int load(char *name, Region *reg, Player *pl)
{
    char path[MAX_PLAYER_NAME_COUNT+20];
    FILE *savefile;
    
    Room *room;

    sprintf(path, "saves/%s", name);
    savefile = fopen(path, "rb");
    if (!savefile)
    {
        return 0;
    }
    
    PlayerSave playersave;
    RegionSave regionsave;
    RoomSave roomsave;

    rewind(savefile);
    fread(&playersave, sizeof(PlayerSave), 1, savefile);
    fread(&regionsave, sizeof(RegionSave), 1, savefile);
    reg->seed = regionsave.seed;
    reg->grid_width = regionsave.grid_width;
    reg->grid_height = regionsave.grid_height;
    reg->allocated_rooms = regionsave.allocated_rooms;
    reg->zero = regionsave.zero;
    reg->deathtimer = regionsave.deathtimer;

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
            room->isitem = roomsave.isitem;
            room->item = roomsave.item;
            room->itemloc = roomsave.itemloc;

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


    strcpy(pl->name, name);
    pl->hp = playersave.hp,
    pl->xp = playersave.xp;
    pl->atk = playersave.atk;
    pl->def = playersave.def;
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