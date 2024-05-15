#include "items.h"


//Returns an item structure corresponding to the given item index
/// @param index Item Index
/// @param name Buffer where the item name will be stored. If it is NULL, the operation is ignored
Item getitem(ItemIndex index, char *name)
{
    Item item;
    switch(index)
    {
        case ITEM_HEAL1:
            if(name!=NULL){strcpy(name, "Pansement");}
            item.name = name;
            item.symb = L'🩹';
            item.stat = 0;
            break;

        case ITEM_SHIELD:
            if(name!=NULL){strcpy(name, "Bouclier");}
            item.name = name;
            item.symb = L'🛡';
            break;

        case ITEM_BASE_WEAPON: //base WEAPON
            if(name!=NULL){strcpy(name, "[Aucune]");}
            item.name = name;
            item.symb = L' ';
            item.type = WEAPON;
            item.stat = 0;
            break;

        case ITEM_WEAPON_STICKS:
            if(name!=NULL){strcpy(name, "Baguettes");}
            item.name = name;
            item.symb = L'🥢';
            item.type = WEAPON;
            item.stat = 0;
            break;

        case ITEM_WEAPON_BOXING:
            if(name!=NULL){strcpy(name, "Gant Boxe");}
            item.name = name;
            item.symb = L'🥊';
            item.type = WEAPON;
            item.stat = 0;
            break;
        
        case ITEM_WEAPON_KEY:
            if(name!=NULL){strcpy(name, "Cle");}
            item.name = name;
            item.symb = L'🔧';
            item.type = WEAPON;
            item.stat = 0;
            break;

        case ITEM_WEAPON_KNIFE:
            if(name!=NULL){strcpy(name, "Couteau");}
            item.name = name;
            item.symb = L'🔪';
            item.type = WEAPON;
            item.stat = 0;
            break;

        case ITEM_WEAPON_SWORD:
            if(name!=NULL){strcpy(name, "Dague");}
            item.name = name;
            item.symb = L'🗡';
            item.type = WEAPON;
            item.stat = 0;
            break;
    }
    return item;
}
