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

uint32_t randint(Region *r, uint32_t min, uint32_t max)
{
    return new_rand(r) % (min-max+1) + min;
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
    room->door_north = NULL;
    room->door_east = NULL;
    room->door_south = NULL;
    room->door_west = NULL;
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

    //First room initialisation
    Room *firstRoom = allocate_room(reg);
    firstRoom->width = 11;
    firstRoom->height = 11;
    firstRoom->door_count = 4;
    //...
}