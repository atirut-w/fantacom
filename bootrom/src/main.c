#include <z80io.h>
#include <stdio.h>

int display_init()
{
    // Find a memory bank for our VGA buffer
    // NOTE: We can continue on from the bank we used as RAM
    volatile char *test_ptr = (char *)0x3000;
    uint8_t bank = in_port(2) + 1;
    do
    {
        out_port(3, bank++);
        *test_ptr = 0x55;
        if (*test_ptr == 0x55)
            break;
    } while (bank != 0);

    if (in_port(3) == 0)
        return -1; // Checked all banks, no luck

    memset((void *)test_ptr, 0, 80 * 25 * 2);
    out_port(0x0100, 0x30); // Set VGA buffer to 0x3000
    return 0;
}

int main()
{
    if (display_init() != 0)
        return -1;
    
    printf("Hello, World from C on the Z80!\r\n");
    printf("Here's another line.\r\n");
    
    return 0;
}
