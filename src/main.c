#include "main.h"

int main(int argc, char **argv)
{
    Region reg;

    srand(time(NULL));
    rand();

    initscr();

    initial_map(&reg);
    HelloWorld();
    
    endwin();
    
    return 0;
}