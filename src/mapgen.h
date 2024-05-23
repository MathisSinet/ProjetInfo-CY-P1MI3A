/*
mapgen.h
This header provides map generation functions
*/

#ifndef MAPGEN_HEADER
#define MAPGEN_HEADER

#include "gamebasic.h"
#include "monster.h"

// Allocates memory for a room, stores it in the region and returns a pointer to it
Room *allocate_room(Region* r);
// Initializes a non-generated room with no doors 
void init_room(Room* room);

// Used by reserve_room
bool is_free_box(Region* reg, Co corner, uint16_t width, uint16_t height);
void reserve_box(Region* reg, Co corner);
void unreserve_box(Region* reg, Co corner);
// Reserves space for a room (1:success, 0:failure)
int reserve_room(Region* reg, Room* room, Pole pole);

// Creates the walls of the room in the grid
void wall_room(Region* reg, Room* room);
// Extends allocated memory for the grid
void extend_grid(Region* reg, Pole dir);

// Initializes a new map
void initial_map(Region* reg, Player *pl);


void place_ew_side_doors(Region *reg, Room *room);
void place_ns_side_doors(Region *reg, Room *room);

bool newroom_valid_space(Region *reg, Room *source, Co corner, uint16_t width, uint16_t height);
// Generates a room when a player enters it for the first time
void generate_room(Region *reg, Room* from, Pole dir);
void fill_room(Region *reg, Room *room, Pole pole);
void fill_item(Region *reg, Room *room, ItemInRoom *item);
void fill_monster(Region *reg, Room *room, MonsterInRoom *monster);

#endif