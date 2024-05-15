#include "gamebasic.h"

/*__________BASIC FUNCTIONS__________*/

//Coordinates builder
Co coordinates(int16_t x, int16_t y)
{
    Co ret;
    ret.x = x;
    ret.y = y;
    return ret;
}

//Returns true if the given chr is a digit
bool is_digit(int chr)
{
    return (chr >= '0' && chr <= '9');
}
//Returns true if the given chr is a valid player name char
bool is_valid_playername_char(int chr)
{
    return (
        is_digit(chr) ||
        (chr >= 'a' && chr <= 'z') ||
        (chr >= 'A' && chr <= 'Z') ||
        chr == '_'
    );
}

//Generates a random uint32 with the region's seed
uint32_t new_rand(Region* r)
{
    r->seed = RAND_A * r->seed + RAND_C;
    return r->seed;
}

//Generates a random integer between min and max included using the region's seed
int32_t randint(Region *r, int32_t min, int32_t max)
{
    return new_rand(r) % (max-min+1) + min;
}

//Has a prob per thousand chance of returning true
bool randevent(Region *reg, uint32_t prob)
{
    return randint(reg, 1, 1000) <= prob;
}

//returns a pointer to the tile on the grid with given coordinates
//returns NULL if the coordinates are invalid
int8_t* get_from_grid(Region* reg, int32_t x, int32_t y)
{
    int32_t nx = reg->zero.x + x;
    int32_t ny = reg->zero.y + y;
    if(nx<0 || ny<0 || nx>reg->grid_width || ny>reg->grid_height)
    {
        return NULL;
    }
    return reg->grid[nx] + ny;
}

//frees the allocated memory in the region
void reg_memfree(Region *reg)
{
    for (Room **room=reg->roomlist; room<reg->roomlist + MAX_ROOM_COUNT; room++)
    {
        free(*room);
    }
    for (int8_t **column = reg->grid; column < reg->grid+reg->grid_width; column++)
    {
        free(*column);
    }
    free(reg->grid);
}