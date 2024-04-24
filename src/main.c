#include "main.h"

int main(int argc, char **argv)
{
    Region reg;

    srand(time(NULL));
    rand();

    initial_map(&reg);
    HelloWorld();
    
    return 0;
}