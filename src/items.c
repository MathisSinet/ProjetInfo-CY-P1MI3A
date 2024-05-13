#include "items.h"

Item getitem(ItemIndex index, char *name)
{
    Item item;
    switch(index)
    {
        case ITEM_BASE_WEAPON: //base WEAPON
            if(name!=NULL){strcpy(name, "[Aucune]");}
            item.name = name;
            item.symb = L' ';
            item.type = WEAPON;
            item.stat1 = 0.0;
            item.stat2 = 0.0;
            break;
        case ITEM_HEAL1:
            if(name!=NULL){strcpy(name, "Pansement");}
            item.name = name;
            item.symb = ITEM_HEAL1_SYMB;
            break;
    }
    return item;
}
