/*
settings.h
Header to define general constants used by the program
*/

#ifndef SETTINGS_HEADER
#define SETTINGS_HEADER

/*__________DISPLAY SETTINGS__________*/

#define MIN_TERMINAL_WIDTH 75
#define MIN_TERMINAL_HEIGHT 34

/*__________ROOM GENERATION PARAMETERS__________*/

#define MAX_ROOM_COUNT 80

#define MAX_ROOM_WIDTH 31
#define MAX_ROOM_HEIGHT 17

#define INIT_ROOM_WIDTH 23
#define INIT_ROOM_HEIGHT 11

#define MIN_ROOM_WIDTH 5
#define MIN_ROOM_HEIGHT 5

#define SIDE_DOORS_PROBABILITY 500

/*__________MEMORY PARAMETERS__________*/

#define MAX_PLAYER_NAME_COUNT 27
#define MAX_INVENTORY_SIZE 5
#define ITEM_NAME_SIZE 15

/*__________BALANCE CHANGES__________*/

#define DEFAULT_DEATH_TIMER 600.0

#define PLAYER_BASE_HP 10
#define PLAYER_BASE_ATKDELAY 0.6

/*__________QUEST SETTINGS__________*/

#define GOAL_MONSTER_KILLS 20

/*__________RANDOM NUMBER GENERATOR SETTINGS__________*/

#define RAND_A 157
#define RAND_C 221

#endif