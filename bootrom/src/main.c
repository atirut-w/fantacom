#include <z80io.h>
#include <display.h>

int main()
{
    if (display_init() != 0)
        return -1;
    
    print("Hello, World!");
    
    return 0;
}
