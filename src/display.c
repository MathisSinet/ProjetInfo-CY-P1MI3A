#include "display.h"

void HelloWorld()
{
    printw("Hello world!\n");
    refresh();
    getch();
}

void init_debug_print(Region *reg)
{
    int w,h;
    getmaxyx(stdscr, h, w);
    printw("w=%d,h=%d", w, h);
    refresh();
    getch();
    clear();
    refresh();
    for (int y=-h/2; y<h/2; y++)
    {
        move(y+h/2,0);
        for (int x=-w/2; x<w/2; x++)
        {
            if (
                reg->zero.x + x < 0 || 
                reg->zero.x + x >= reg->grid_width || 
                reg->zero.y + y < 0 || 
                reg->zero.y + y >reg->grid_height)
            {
                printw(" ");
                continue;
            }
            if(x==0 && y==0)
            {
                printw("C");
                continue;
            }
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
    }

    refresh();
    getch();
}