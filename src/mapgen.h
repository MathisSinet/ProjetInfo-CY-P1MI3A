/*__________MAP GENERATION HEADER__________*/

#ifndef MAPGEN_HEADER
#define MAPGEN_HEADER

#include "gamebasic.h"
#include "monster.h"


Room *allocate_room(Region* r);
void init_room(Room* room);

bool is_free_box(Region* reg, Co corner, uint16_t width, uint16_t height);
void reserve_box(Region* reg, Co corner);
void unreserve_box(Region* reg, Co corner);
int reserve_room(Region* reg, Room* room, Pole pole);

void wall_room(Region* reg, Room* room);

void extend_grid(Region* reg, Pole dir);


void initial_map(Region* reg, Player *pl);

void place_ew_side_doors(Region *reg, Room *room);
void place_ns_side_doors(Region *reg, Room *room);

bool newroom_valid_space(Region *reg, Room *source, Co corner, uint16_t width, uint16_t height);
void generate_room(Region *reg, Room* from, Pole dir);

void fill_room(Region *reg, Room *room, Pole pole);
void fill_item(Region *reg, Room *room, ItemInRoom *item);
void fill_monster(Region *reg, Room *room, MonsterInRoom *monster);

#endif