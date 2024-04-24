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
    //...
}