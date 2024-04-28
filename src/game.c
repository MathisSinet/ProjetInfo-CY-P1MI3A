#include "game.h"

Co coordinates(int16_t x, int16_t y)
{
    Co ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

uint32_t new_rand(Region* r)
{
    //before implementation of custom number generator, use rand
    r->seed = RAND_A * r->seed + RAND_C;
    return r->seed;
}

int32_t randint(Region *r, int32_t min, int32_t max)
{
    return new_rand(r) % (max-min+1) + min;
}

int8_t* get_from_grid(Region* reg, int32_t x, int32_t y)
{
    //ATTENTION SEGFAULT
    //fprintf(stderr, "\nCalling get_from_grid... ");
    //fprintf(stderr, "x=%d,y=%d... ",x,y);
    //fprintf(stderr, "zero x=%d,y=%d",reg->zero.x, reg->zero.y);
    return reg->grid[reg->zero.x + x] + reg->zero.y + y;
}

Room *allocate_room(Region* r)
{
    int index = r->allocated_rooms++;
    r->roomlist[index] = malloc(sizeof(Room));
    if (r->roomlist[index] == NULL)
    {
        perror("Room allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return r->roomlist[index];
}
void init_room(Room* room)
{
    room->is_generated = false;
    room->door_count = 0;
    room->door_north.exists = false;
    room->door_north.to = NULL;
    room->door_east.exists = false;
    room->door_east.to = NULL;
    room->door_south.exists = false;
    room->door_south.to = NULL;
    room->door_west.exists = false;
    room->door_west.to = NULL;
}

bool is_free_box(Region *reg, Co corner, uint16_t width, uint16_t height)
{
    for (int x=corner.x; x<corner.x+width; x++)
    {
        for (int y=corner.y; y<corner.y+height; y++)
        {
            if (*get_from_grid(reg, x, y) != VOID)
            {
                return false;
            }
        }
    }
    return true;
}
void reserve_box(Region *reg, Co corner)
{
    for (int x=corner.x; x<corner.x+RESERVE_WIDTH; x++)
    {
        for (int y=corner.y; y<corner.y+RESERVE_HEIGHT; y++)
        {
            if (*get_from_grid(reg, x, y) == VOID)
            {
                *get_from_grid(reg, x, y) = RESERVED;
            }
        }
    }
}

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
        corner.x = room->corner.x + room->door_north.dist - RESERVE_WIDTH/2;
        corner.y = room->corner.y - RESERVE_HEIGHT + 1;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, corner, RESERVE_WIDTH, RESERVE_HEIGHT-1))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_north.to = &newroom;
        newroom->door_south.to = &room;
        newroom->door_south.exists = true;
        break;

    case EAST:
        corner.x = room->corner.x + room->width - 1;
        corner.y = room->corner.y + room->door_east.dist - RESERVE_HEIGHT/2;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, coordinates(corner.x+1, corner.y), RESERVE_WIDTH-1, RESERVE_HEIGHT))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_east.to = &newroom;
        newroom->door_west.to = &room;
        newroom->door_west.exists = true;
        break;

    case SOUTH:
        corner.x = room->corner.x + room->door_south.dist - RESERVE_WIDTH/2;
        corner.y = room->corner.y + room->height - 1;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, coordinates(corner.x, corner.y+1), RESERVE_WIDTH, RESERVE_HEIGHT-1))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_south.to = &newroom;
        newroom->door_north.to = &room;
        newroom->door_north.exists = true;
        break;

    case WEST:
        corner.x = room->corner.x - RESERVE_HEIGHT + 1;
        corner.y = room->corner.y + room->door_west.dist - RESERVE_HEIGHT/2;
        //fprintf(stderr, "room corner %d,%d, dist:%d\n", room->corner.x, room->corner.y, room->door_north.dist);
        if (!is_free_box(reg, corner, RESERVE_WIDTH-1, RESERVE_HEIGHT))
        {
            return 0;
        }
        reserve_box(reg, corner);
        newroom = allocate_room(reg);
        init_room(newroom);
        room->door_west.to = &newroom;
        newroom->door_east.to = &room;
        newroom->door_east.exists = true;
        break;
    
    default:
        break;
    }

    return 1;
}

void wall_room(Region *reg, Room *room)
{
    //upper wall
    for (int dx=0; dx<room->width; dx++)
    {
        if (dx == room->door_north.dist)
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
        if (dx == room->door_south.dist)
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
        if (dy == room->door_east.dist)
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
        if (dy == room->door_west.dist)
        {
            *get_from_grid(reg, room->corner.x, room->corner.y + dy) = DOOR;
        }
        else
        {
            *get_from_grid(reg, room->corner.x, room->corner.y + dy) = WALL;
        }
    }
}

void initial_map(Region* reg)
{
    //Room list initialisation
    reg->allocated_rooms = 0;
    for (Room **p = reg->roomlist; p < reg->roomlist+MAX_ROOM_COUNT; p++)
    {
        *p = NULL;
    }

    reg->grid_width = 10 * MAX_ROOM_WIDTH;
    reg->grid_height = 10 * MAX_ROOM_HEIGHT;
    reg->zero = coordinates(reg->grid_width / 2, reg->grid_height / 2);

    //Column allocation
    reg->grid = malloc(reg->grid_width * sizeof(int8_t*));
    if (!reg->grid)
    {
        perror("Map allocation failed\n");
        exit(EXIT_FAILURE);
    }
    //Row allocation
    for (int8_t **column = reg->grid; column < reg->grid+reg->grid_width; column++)
    {
        *column = calloc(reg->grid_height, sizeof(int8_t));
        if(!*column)
        {
            perror("Map allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    //First room initialization
    Room *firstRoom = allocate_room(reg);
    firstRoom->width = 13;
    firstRoom->height = 7;
    firstRoom->corner.x = -6;
    firstRoom->corner.y = -3;
    firstRoom->door_count = 4;
    
    firstRoom->door_north.exists = true;
    firstRoom->door_north.dist = randint(reg, 1, 11);
    reserve_room(reg, firstRoom, NORTH);

    firstRoom->door_east.exists = true;
    firstRoom->door_east.dist = randint(reg, 1, 5);
    reserve_room(reg, firstRoom, EAST);

    firstRoom->door_south.exists = true;
    firstRoom->door_south.dist = randint(reg, 1, 11);
    reserve_room(reg, firstRoom, SOUTH);

    firstRoom->door_west.exists = true;
    firstRoom->door_west.dist = randint(reg, 1, 5);
    reserve_room(reg, firstRoom, WEST);

    wall_room(reg, firstRoom);
}