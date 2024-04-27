#include "display.h"

void HelloWorld()
{
    printw("Hello world!\n");
    refresh();
    getch();
}

void init_debug_print(Region *reg)
{
    for (int y=-25; y<25; y++)
    {
        for (int x=-25; x<25; x++)
        {
            switch (*get_from_grid(reg, x, y))
            {
            case VOID:
                printw(" ");
                break;
            case RESERVED:
                printw("R");
                break;
            case WALL:
                printw("W");
                break;
            case DOOR:
                printw("D");
                break;
            default:
                break;
            }
        }
        printw("\n");
    }

    refresh();
    getch();
}