#include "mapgen.h"

/*__________MAP GENERATION FUNCTIONS__________*/


//Allocates a room on the region's room list
//The newly-created room has only its index initialized
//This function does not check if the max number of rooms is exceded
Room *allocate_room(Region* r)
{
    int index = r->allocated_rooms++;
    r->roomlist[index] = malloc(sizeof(Room));
    if (r->roomlist[index] == NULL)
    {
        perror("Room allocation failed\n");
        exit(EXIT_FAILURE);
    }
    r->roomlist[index]->index = index;
    return r->roomlist[index];
}

//Initializes a non-generated room
void init_room(Room* room)
{
    room->is_generated = false;

    room->door_north.exists = false;
    room->door_north.to = NULL;
    room->door_east.exists = false;
    room->door_east.to = NULL;
    room->door_south.exists = false;
    room->door_south.to = NULL;
    room->door_west.exists = false;
    room->door_west.to = NULL;

    
    room->item1.exists = false;
    room->item2.exists = false;
    room->item1.loc = coordinates(room->corner.x-2*MAX_ROOM_WIDTH, room->corner.y-2*MAX_ROOM_HEIGHT);
    room->item2.loc = coordinates(room->corner.x-2*MAX_ROOM_WIDTH, room->corner.y-2*MAX_ROOM_HEIGHT);

    room->monster1.exists = false;
    room->monster2.exists = false;
    room->monster1.loc = coordinates(room->corner.x-2*MAX_ROOM_WIDTH, room->corner.y-2*MAX_ROOM_HEIGHT);
    room->monster2.loc = coordinates(room->corner.x-2*MAX_ROOM_WIDTH, room->corner.y-2*MAX_ROOM_HEIGHT);
}

//Returns true if the region of the grid given by a corner, width and height is free
//i.e. it contains only VOID
bool is_free_box(Region *reg, Co corner, uint16_t width, uint16_t height)
{
    int8_t* tile;

    for (int x=corner.x; x<corner.x+width; x++)
    {
        for (int y=corner.y; y<corner.y+height; y++)
        {
            tile = get_from_grid(reg, x, y);
            if (!tile)
            {
                perror("Invalid memory read in is_free_box");
                abort();
            }
            if (*tile != VOID)
            {
                return false;
            }
        }
    }
    return true;
}


//Declares space in the region for a future room to be generated when the player
//enters a door
//This function uses MIN_ROOM_WIDTH and MIN_ROOM_HEIGHT
void reserve_box(Region *reg, Co corner)
{
    for (int x=corner.x; x<corner.x+MIN_ROOM_WIDTH; x++)
    {
        for (int y=corner.y; y<corner.y+MIN_ROOM_HEIGHT; y++)
        {
            if (*get_from_grid(reg, x, y) == VOID)
            {
                *get_from_grid(reg, x, y) = RESERVED;
            }
        }
    }
}


//Removes the reservation of a room previously done by reserve_room
//so that the space is VOID again to create the room
void unreserve_box(Region* reg, Co corner)
{
    for (int x=corner.x; x<corner.x+MIN_ROOM_WIDTH; x++)
    {
        for (int y=corner.y; y<corner.y+MIN_ROOM_HEIGHT; y++)
        {
            if (*get_from_grid(reg, x, y) == RESERVED)
            {
                *get_from_grid(reg, x, y) = VOID;
            }
        }
    }
}

//This function tries to reserve a room in the grid, from a given room and a given door pole
//return value : true if successful, false if failure
//If successful, the reserved room has the entry door marked as existing and pointing to the previous room
//The exit door in the previous room points to the new room
//The corner of the new room matches the corner of the reservation
int reserve_room(Region* reg, Room* room, Pole pole)
{
    Co corner;
    Room *newroom;

    if (reg->allocated_rooms == MAX_ROOM_COUNT) 
    {
        return 0;
    }

    switch (pole)
    {
    case NORTH:
        corner.x = room->corner.x + room->door_north.dist - MIN_ROOM_WIDTH/2;
        corner.y = room->corner.y - MIN_ROOM_HEIGHT + 1;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, corner, MIN_ROOM_WIDTH, MIN_ROOM_HEIGHT-1))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_north.to = newroom;
        newroom->corner = corner;
        newroom->door_south.to = room;
        newroom->door_south.exists = true;
        break;

    case EAST:
        corner.x = room->corner.x + room->width - 1;
        corner.y = room->corner.y + room->door_east.dist - MIN_ROOM_HEIGHT/2;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, coordinates(corner.x+1, corner.y), MIN_ROOM_WIDTH-1, MIN_ROOM_HEIGHT))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_east.to = newroom;
        newroom->corner = corner;
        newroom->door_west.to = room;
        newroom->door_west.exists = true;
        break;

    case SOUTH:
        corner.x = room->corner.x + room->door_south.dist - MIN_ROOM_WIDTH/2;
        corner.y = room->corner.y + room->height - 1;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, coordinates(corner.x, corner.y+1), MIN_ROOM_WIDTH, MIN_ROOM_HEIGHT-1))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_south.to = newroom;
        newroom->corner = corner;
        newroom->door_north.to = room;
        newroom->door_north.exists = true;
        break;

    case WEST:
        corner.x = room->corner.x - MIN_ROOM_HEIGHT + 1;
        corner.y = room->corner.y + room->door_west.dist - MIN_ROOM_HEIGHT/2;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, corner, MIN_ROOM_WIDTH-1, MIN_ROOM_HEIGHT))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_west.to = newroom;
        newroom->corner = corner;
        newroom->door_east.to = room;
        newroom->door_east.exists = true;
        break;
    
    default:
        break;
    }

    return 1;
}


//Creates the walls of the room in the grid, while also placing the doors
void wall_room(Region *reg, Room *room)
{
    //upper wall
    for (int dx=0; dx<room->width; dx++)
    {
        if (room->door_north.exists && dx == room->door_north.dist)
        {
            *get_from_grid(reg, room->corner.x + dx, room->corner.y) = DOOR;
        }
        else
        {
            *get_from_grid(reg, room->corner.x + dx, room->corner.y) = WALL;
        }
    }
    //lower wall
    for (int dx=0; dx<room->width; dx++)
    {
        if (room->door_south.exists && dx == room->door_south.dist)
        {
            *get_from_grid(reg, room->corner.x + dx, room->corner.y + room->height-1) = DOOR;
        }
        else
        {
            *get_from_grid(reg, room->corner.x + dx, room->corner.y + room->height-1) = WALL;
        }
    }
    //eastern wall
    for (int dy=0; dy<room->height; dy++)
    {
        if (room->door_east.exists && dy == room->door_east.dist)
        {
            *get_from_grid(reg, room->corner.x + room->width-1,  room->corner.y + dy) = DOOR;
        }
        else
        {
            *get_from_grid(reg, room->corner.x + room->width-1, room->corner.y + dy) = WALL;
        }
    }
    //western wall
    for (int dy=0; dy<room->height; dy++)
    {
        if (room->door_west.exists && dy == room->door_west.dist)
        {
            *get_from_grid(reg, room->corner.x, room->corner.y + dy) = DOOR;
        }
        else
        {
            *get_from_grid(reg, room->corner.x, room->corner.y + dy) = WALL;
        }
    }
}


//Doubles the allocated memory for the grid in the given direction
void extend_grid(Region* reg, Pole dir)
{
    int8_t **newgrid;
    int8_t *newcol;

    switch(dir)
    {
        case NORTH:
            reg->grid_height *= 2;
            reg->zero = coordinates(reg->zero.x, reg->zero.y + reg->grid_height / 2);
            for (int8_t **column = reg->grid; column < reg->grid+reg->grid_width; column++)
            {
                newcol = realloc(*column, reg->grid_height * sizeof(int8_t));
                if (!newcol)
                {
                    perror("Map allocation failed\n");
                    abort();
                }
                *column = newcol;
                for (int8_t *p1=*column, *p2=*column+reg->grid_height/2; p2 < *column+reg->grid_height; p1++,p2++)
                {
                    *p2 = *p1;
                    *p1 = VOID;
                }
            }
            break;
        case SOUTH:
            reg->grid_height *= 2;
            for (int8_t **column = reg->grid; column < reg->grid+reg->grid_width; column++)
            {
                newcol = realloc(*column, reg->grid_height * sizeof(int8_t));
                if (!newcol)
                {
                    perror("Map allocation failed\n");
                    abort();
                }
                *column = newcol;
                for (int8_t *p = *column + reg->grid_height/2; p<*column+reg->grid_height; p++)
                {
                    *p = VOID;
                }
            }
            break;
        case EAST:
            reg->grid_width *= 2;
            newgrid = realloc(reg->grid, reg->grid_width * sizeof(int8_t*));
            if (!newgrid)
            {
                perror("Map allocation failed : grid extension to the right\n");
                abort();
            }
            reg->grid = newgrid;
            for (int8_t **column = reg->grid+reg->grid_width/2; column < reg->grid+reg->grid_width; column++)
            {
                *column = calloc(reg->grid_height, sizeof(int8_t));
                if(!*column)
                {
                    perror("Map allocation failed\n");
                    abort();
                }
            }
            break;
        case WEST:
            reg->grid_width *= 2;
            reg->zero = coordinates(reg->zero.x + reg->grid_width/2, reg->zero.y);
            newgrid = realloc(reg->grid, reg->grid_width * sizeof(int8_t*));
            if (!newgrid)
            {
                perror("Map allocation failed\n");
                abort();
            }
            reg->grid = newgrid;
            for (int8_t **col1=reg->grid, **col2=reg->grid+reg->grid_width/2; col2<reg->grid+reg->grid_width; col1++,col2++)
            {
                *col2 = *col1;
                *col1 = calloc(reg->grid_height, sizeof(int8_t));
                if(!*col1)
                {
                    perror("Map allocation failed\n");
                    abort();
                }
            }
            break;
    }
}


//Initializes the Region and Player structures for a new game
void initial_map(Region* reg, Player *pl)
{
    //Room list initialisation
    reg->allocated_rooms = 0;
    for (Room **p = reg->roomlist; p < reg->roomlist+MAX_ROOM_COUNT; p++)
    {
        *p = NULL;
    }


    reg->questinfo.is_ball_generated = false;
    reg->questinfo.is_teddybear_generated = false;

    reg->grid_width = 10 * MAX_ROOM_WIDTH;
    reg->grid_height = 10 * MAX_ROOM_HEIGHT;
    reg->zero = coordinates(reg->grid_width / 2, reg->grid_height / 2);

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
        *column = calloc(reg->grid_height, sizeof(int8_t));
        if(!*column)
        {
            perror("Map allocation failed\n");
            abort();
        }
    }

    //First room initialization
    Room *firstRoom = allocate_room(reg);
    init_room(firstRoom);
    firstRoom->width = INIT_ROOM_WIDTH;
    firstRoom->height = INIT_ROOM_HEIGHT;
    firstRoom->corner.x = -INIT_ROOM_WIDTH/2;
    firstRoom->corner.y = -INIT_ROOM_HEIGHT/2;
    
    firstRoom->door_north.exists = true;
    firstRoom->door_north.dist = randint(reg, MIN_ROOM_WIDTH/2+1, INIT_ROOM_WIDTH-(MIN_ROOM_WIDTH/2)-2);
    reserve_room(reg, firstRoom, NORTH);

    firstRoom->door_east.exists = true;
    firstRoom->door_east.dist = randint(reg, MIN_ROOM_HEIGHT/2+1, INIT_ROOM_HEIGHT-(MIN_ROOM_HEIGHT/2)-2);
    reserve_room(reg, firstRoom, EAST);

    firstRoom->door_south.exists = true;
    firstRoom->door_south.dist = randint(reg, MIN_ROOM_WIDTH/2+1, INIT_ROOM_WIDTH-(MIN_ROOM_WIDTH/2)-2);
    reserve_room(reg, firstRoom, SOUTH);

    firstRoom->door_west.exists = true;
    firstRoom->door_west.dist = randint(reg, MIN_ROOM_HEIGHT/2+1, INIT_ROOM_HEIGHT-(MIN_ROOM_HEIGHT/2)-2);
    reserve_room(reg, firstRoom, WEST);

    firstRoom->is_generated = true;
    wall_room(reg, firstRoom);
    
    //Player initialization
    reg->deathtimer = DEFAULT_DEATH_TIMER;

    pl->currentroom = firstRoom;
    pl->loc = coordinates(0,0);
    pl->xp = 0;
    pl->hp = PLAYER_BASE_HP;
    pl->atk = 3;
    pl->def = 10;
    pl->inv_size = 0;
    pl->atkdelay = 0.0;

    pl->weapon = ITEM_BASE_WEAPON;
}


//Places side doors for a room generated from the north or the south
void place_ew_side_doors(Region *reg, Room *room)
{
    if (room->height > MIN_ROOM_HEIGHT+3)
    {
        if (randevent(reg, SIDE_DOORS_PROBABILITY))
        {
            room->door_west.dist = randint(reg, MIN_ROOM_HEIGHT/2+1, room->height-(MIN_ROOM_HEIGHT/2)-2);
            if (is_free_box(
                reg,
                coordinates(room->corner.x - MAX_ROOM_WIDTH, room->corner.y + room->door_west.dist - MIN_ROOM_HEIGHT/2),
                MAX_ROOM_WIDTH-1,
                MIN_ROOM_HEIGHT
                )
            )
            {
                room->door_west.exists = reserve_room(reg, room, WEST);
            }
        }
        if (randevent(reg, SIDE_DOORS_PROBABILITY))
        {
            room->door_east.dist = randint(reg, MIN_ROOM_HEIGHT/2+1, room->height-(MIN_ROOM_HEIGHT/2)-2);
            if (is_free_box(
                reg,
                coordinates(room->corner.x + room->width, room->corner.y + room->door_east.dist - MIN_ROOM_HEIGHT/2),
                MAX_ROOM_WIDTH-1,
                MIN_ROOM_HEIGHT
                )
            )
            {
                room->door_east.exists = reserve_room(reg, room, EAST);
            }
        }
    }
}


//Places side doors for a room generated from the west or the east
void place_ns_side_doors(Region *reg, Room *room)
{
    if (room->width > MIN_ROOM_WIDTH+3)
    {
        if (randevent(reg, SIDE_DOORS_PROBABILITY))
        {
            room->door_north.dist = randint(reg, MIN_ROOM_WIDTH/2+1, room->width-(MIN_ROOM_WIDTH/2)-2);
            if (is_free_box(
                reg,
                coordinates(room->corner.x + room->door_north.dist - MIN_ROOM_WIDTH/2, room->corner.y - MAX_ROOM_HEIGHT),
                MAX_ROOM_HEIGHT,
                MAX_ROOM_HEIGHT-1
                )
            )
            {
                room->door_north.exists = reserve_room(reg, room, NORTH);
            }
        }
        if (randevent(reg, SIDE_DOORS_PROBABILITY))
        {
            room->door_south.dist = randint(reg, MIN_ROOM_WIDTH/2+1, room->width-(MIN_ROOM_WIDTH/2)-2);
            if (is_free_box(
                reg,
                coordinates(room->corner.x + room->door_south.dist - MIN_ROOM_WIDTH/2, room->corner.y + room->height),
                MAX_ROOM_HEIGHT,
                MAX_ROOM_HEIGHT-1
                )
            )
            {
                room->door_south.exists = reserve_room(reg, room, SOUTH);
            }
        }
    }
}


//Returns true if the given room (top-left corner, width, height) is creatable, while allowing 'room' to overlap
bool newroom_valid_space(Region *reg, Room *room, Co corner, uint16_t width, uint16_t height)
{
    for (int x=corner.x; x<corner.x+width; x++)
    {
        for (int y=corner.y; y<corner.y+height; y++)
        {
            if (x >= room->corner.x && x < room->corner.x + room->width
                && y >= room->corner.y && y < room->corner.y + room->height)
            {
                continue;
            }
            if (*get_from_grid(reg, x, y) != VOID)
            {
                return false;
            }
        }
    }
    return true;
}


//Generates the room when a players enters it for the first time
void generate_room(Region *reg, Room* from, Pole dir)
{
    Room *newroom;
    int width, height, diff;
    width = randint(reg, MIN_ROOM_WIDTH+4, MAX_ROOM_WIDTH);
    height = randint(reg, MIN_ROOM_HEIGHT+4, MAX_ROOM_HEIGHT);
    switch (dir)
    {
    case NORTH:
        //grid extension?
        if (reg->zero.y + from->corner.y < 3 * MAX_ROOM_HEIGHT)
        {
            extend_grid(reg, NORTH);
        }

        newroom = from->door_north.to;
        unreserve_box(reg, newroom->corner);
        diff = randint(reg, 0, width - MIN_ROOM_WIDTH);
        newroom->width = width;
        newroom->height = height;
        newroom->corner.x -= diff;
        while (!newroom_valid_space(reg, from, newroom->corner, diff+MIN_ROOM_WIDTH, MIN_ROOM_HEIGHT)) //first resizing
        {
            diff--;
            newroom->corner.x++;
            newroom->width--;
        }
        while (!newroom_valid_space(reg, from, newroom->corner, newroom->width, MIN_ROOM_HEIGHT)) //second resizing
        {
            newroom->width--;
        }
        newroom->corner.y = from->corner.y - height + 1;
        while (!newroom_valid_space(reg, from, newroom->corner, newroom->width, newroom->height)) //third resizing
        {
            newroom->corner.y++;
            newroom->height--;
        }

        newroom->door_south.dist = diff + MIN_ROOM_WIDTH/2;

        //attempts to place doors

        if (newroom->width > MIN_ROOM_WIDTH+2)
        {
            newroom->door_north.dist = randint(reg, 2, newroom->width-3);
            newroom->door_north.exists = reserve_room(reg, newroom, NORTH);
        }
        place_ew_side_doors(reg, newroom);

        newroom->is_generated = true;
        wall_room(reg, newroom);
        fill_room(reg, newroom, NORTH);
        break;


    case EAST:
        //grid extension?
        if (reg->zero.x + from->corner.x + from->width > reg->grid_width - 3*MAX_ROOM_WIDTH)
        {
            extend_grid(reg, EAST);
        }

        newroom = from->door_east.to;
        unreserve_box(reg, newroom->corner);
        diff = randint(reg, 0, height - MIN_ROOM_HEIGHT);
        newroom->width = width;
        newroom->height = height;
        newroom->corner.y -= diff;
        while (!newroom_valid_space(reg, from, newroom->corner, MIN_ROOM_WIDTH, diff+MIN_ROOM_HEIGHT))
        {
            diff--;
            newroom->corner.y++;
            newroom->height--;
        }
        while (!newroom_valid_space(reg, from, newroom->corner, MIN_ROOM_WIDTH, newroom->height))
        {
            newroom->height--;
        }
        newroom->corner.x = from->corner.x + from->width - 1;
        while (!newroom_valid_space(reg, from, newroom->corner, newroom->width, newroom->height))
        {
            newroom->width--;
        }

        newroom->door_west.dist = diff + MIN_ROOM_WIDTH/2;

        if (newroom->height > MIN_ROOM_HEIGHT+2)
        {
            newroom->door_east.dist = randint(reg, 2, newroom->height-3);
            newroom->door_east.exists = reserve_room(reg, newroom, EAST);
        }
        place_ns_side_doors(reg, newroom);


        newroom->is_generated = true;
        wall_room(reg, newroom);
        fill_room(reg, newroom, EAST);
        break;

    case SOUTH:
        //grid extension?
        if (reg->zero.y + from->corner.y + from->height > reg->grid_height - 3*MAX_ROOM_HEIGHT)
        {
            extend_grid(reg, SOUTH);
        }


        newroom = from->door_south.to;
        unreserve_box(reg, newroom->corner);
        diff = randint(reg, 0, width - MIN_ROOM_WIDTH);
        newroom->width = width;
        newroom->height = height;
        newroom->corner.x -= diff;
        while (!newroom_valid_space(reg, from, newroom->corner, diff+MIN_ROOM_WIDTH, MIN_ROOM_HEIGHT))
        {
            diff--;
            newroom->corner.x++;
            newroom->width--;
        }
        while (!newroom_valid_space(reg, from, newroom->corner, newroom->width, MIN_ROOM_HEIGHT))
        {
            newroom->width--;
        }
        newroom->corner.y = from->corner.y + from->height - 1;
        while (!newroom_valid_space(reg, from, newroom->corner, newroom->width, newroom->height))
        {
            newroom->height--;
        }

        newroom->door_north.dist = diff + MIN_ROOM_WIDTH/2;

        if (newroom->width > MIN_ROOM_WIDTH+2)
        {
            newroom->door_south.dist = randint(reg, 2, newroom->width-3);
            newroom->door_south.exists = reserve_room(reg, newroom, SOUTH);
        }
        place_ew_side_doors(reg, newroom);

        newroom->is_generated = true;
        wall_room(reg, newroom);
        fill_room(reg, newroom, SOUTH);
        break;

    case WEST:
        //grid extension?
        if (reg->zero.x + from->corner.x < 3 * MAX_ROOM_WIDTH)
        {
            extend_grid(reg, WEST);
        }


        newroom = from->door_west.to;
        unreserve_box(reg, newroom->corner);
        diff = randint(reg, 0, height - MIN_ROOM_HEIGHT);
        newroom->width = width;
        newroom->height = height;
        newroom->corner.y -= diff;
        while (!newroom_valid_space(reg, from, newroom->corner, MIN_ROOM_WIDTH, diff+MIN_ROOM_HEIGHT))
        {
            diff--;
            newroom->corner.y++;
            newroom->height--;
        }
        while (!newroom_valid_space(reg, from, newroom->corner, MIN_ROOM_WIDTH, newroom->height))
        {
            newroom->height--;
        }
        newroom->corner.x = from->corner.x - width + 1;
        while (!newroom_valid_space(reg, from, newroom->corner, newroom->width, newroom->height))
        {
            newroom->corner.x++;
            newroom->width--;
        }

        newroom->door_east.dist = diff + MIN_ROOM_WIDTH/2;

        if (newroom->height > MIN_ROOM_HEIGHT+2)
        {
            newroom->door_west.dist = randint(reg, 2, newroom->height-3);
            newroom->door_west.exists = reserve_room(reg, newroom, WEST);
        }
        place_ns_side_doors(reg, newroom);

        newroom->is_generated = true;
        wall_room(reg, newroom);
        fill_room(reg, newroom, WEST);
        break;
    }
}


//Fills the room with potential item and monster
void fill_room(Region *reg, Room *room, Pole pole)
{
    bool iswest = pole == WEST;
    bool iseast = pole == EAST;
    bool isnorth = pole == NORTH;
    bool issouth = pole == SOUTH;

    if (room->width > 4 && room->height > 4)
    {
        room->item1.loc = coordinates(
            room->corner.x + randint(reg, 1+iseast, room->width-2-iswest),
            room->corner.y + randint(reg, 1+issouth, room->height-2-isnorth)
        );
        fill_item(reg, room, &room->item1);
    }
    if (room->width > 6 && room->height > 6)
    {
        do{
        room->monster1.loc = coordinates(
            room->corner.x + randint(reg, 1+2*iseast, room->width-2-2*iswest),
            room->corner.y + randint(reg, 1+2*issouth, room->height-2-2*isnorth)
        );
        } while (same_coordinates(room->item1.loc, room->monster1.loc));
        fill_monster(reg, room, &room->monster1);
    }
    if (room->width > 8 && room->height > 8 && room->item1.exists && randevent(reg, 500))
    {
        do{
        room->item2.loc = coordinates(
            room->corner.x + randint(reg, 1+2*iseast, room->width-2-2*iswest),
            room->corner.y + randint(reg, 1+2*issouth, room->height-2-2*isnorth)
        );
        } while (same_coordinates(room->item2.loc, room->item1.loc) ||
            same_coordinates(room->item2.loc, room->monster1.loc));
        fill_item(reg, room, &room->item2);
    }
    if (room->width > 9 && room->height > 9 && room->monster1.exists && !room->item2.exists && randevent(reg, 900))
    {
        do{
        room->monster2.loc = coordinates(
            room->corner.x + randint(reg, 1+4*iseast, room->width-2-4*iswest),
            room->corner.y + randint(reg, 1+4*issouth, room->height-2-4*isnorth)
        );
        } while (same_coordinates(room->item2.loc, room->item1.loc) ||
            same_coordinates(room->item2.loc, room->monster1.loc));
        fill_monster(reg, room, &room->monster2);
    }
    

}


//Fills the room with potential item
void fill_item(Region *reg, Room *room, ItemInRoom *item){
    if (randevent(reg, 120))
    {
    item->exists = true;
    item->index = ITEM_SHIELD;
    }
    if (randevent(reg, 200))
    {
    item->exists = true;
    item->index = ITEM_HEAL1;
    return;
    }
    if (randevent(reg, 300))
    {
    item->exists = true;
    item->index = ITEM_QUEST_QUIZZ;
    }
    if (randevent(reg, 500))
    {
    item->exists = true;
    item->index = ITEM_WEAPON_STICKS;
    }
    if (randevent(reg, 400))
    {
    item->exists = true;
    item->index = ITEM_WEAPON_BOXING;
    }
    if (randevent(reg, 300))
    {
    item->exists = true;
    item->index = ITEM_WEAPON_KEY;
    }
    if (randevent(reg, 150))
    {
    item->exists = true;
    item->index = ITEM_WEAPON_KNIFE;
    }
    if (randevent(reg, 50))
    {
    item->exists = true;
    item->index = ITEM_WEAPON_SWORD;
    }
}

//Fills the room with potential monster
void fill_monster(Region *reg, Room *room, MonsterInRoom *monster)
{
    Monster monster_s;
    if (randevent(reg, 800))
    {
        monster->exists = true;
        monster->index = MONSTER_ALIEN;
        monster_s = getmonster(MONSTER_ALIEN, NULL);
        monster->hp = monster_s.hp;
        monster->movedelay = 1.5*monster_s.basemovedelay;
        monster->atkdelay = monster_s.baseatkdelay;
    }

}