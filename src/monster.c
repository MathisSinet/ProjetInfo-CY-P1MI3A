#include "monster.h"


// Returns an item structure corresponding to the given item index
/// @param index Monster Index
/// @param name Buffer where the item name will be stored. If it is NULL, the operation is ignored
Monster getmonster(MonsterIndex index, char *name)
{
    Monster monster;
    switch(index)
    {
        case MONSTER_INVADER:
            monster.symb = L'👾';
            monster.hp = 15;
            monster.atk = 1.0;
            monster.xp_reward = 100;
            monster.agression_value = 30;
            monster.baseatkdelay = 0.7;
            monster.basemovedelay = 0.25;
            break;
        case MONSTER_ALIEN:
            monster.symb = L'👽';
            monster.hp = 30;
            monster.atk = 2.0;
            monster.xp_reward = 200;
            monster.agression_value = 70;
            monster.baseatkdelay = 0.95;
            monster.basemovedelay = 0.7;
            break;
        case MONSTER_SAUCER:
            monster.symb = L'🛸';
            monster.hp = 60;
            monster.atk = 3.0;
            monster.xp_reward = 300;
            monster.agression_value = 70;
            monster.baseatkdelay = 1.35;
            monster.basemovedelay = 1.1;
            break;
    }
    return monster;
}
