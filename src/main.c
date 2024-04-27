#include "main.h"

int main(int argc, char **argv)
{
    Region reg;

    srand(time(NULL));
    rand();

    initial_map(&reg);

    initscr();

    init_debug_print(&reg);
    
    endwin();
    
    return 0;
}