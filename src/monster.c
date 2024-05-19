#include "monster.h"


//Returns an item structure corresponding to the given item index
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
            monster.atk = 1;
            monster.baseatkdelay = 2;
            monster.atkdelay = 2;
            break;
        case MONSTER_ALIEN:
            monster.symb = L'👽';
            monster.hp = 30;
            monster.atk = 2;
            monster.baseatkdelay = 3;
            monster.atkdelay = 3;
            break;
        case MONSTER_SOUCOUPE:
            monster.symb = L'🛸';
            monster.hp = 60;
            monster.atk = 2;
            monster.baseatkdelay = 4;
            monster.atkdelay = 4;
            break;
    }
    return monster;
}
