#include <z80io.h>
#include <stdio.h>
#include <string.h>

int init_display()
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

int memcheck()
{
    volatile char *test_ptr = (char *)0x4000;
    uint8_t bank = in_port(3) + 1;
    int total = 8;

    printf("Performing memory check...\n");
    do
    {
        out_port(4, bank++);
        *test_ptr = 0x55;
        if (*test_ptr == 0x55)
        {
            total += 4;
            printf("\r%4d KiB", total);
        }
    } while (bank != 0);
    printf(" OK\n");

    return total;
}

void init_interrupts()
{
__asm
    ei
    ret
__endasm;
}

int main()
{
    if (init_display() != 0)
        return -1;
    init_interrupts();
    
    printf("FantaCom Boot ROM (C) Atirut Wattanamongkol & contributors\n\n");
    memcheck();

    return 0;
}
