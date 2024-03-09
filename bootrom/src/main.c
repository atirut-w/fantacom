#include <z80io.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <ivt.h>

int init_display()
{
    // Find a memory bank for our VGA buffer
    // NOTE: We can continue on from the bank we used as RAM
    volatile char *test_ptr = (char *)0x3000;
    uint8_t bank = inc_port(2) + 1;
    do
    {
        outc_port(3, bank++);
        *test_ptr = 0x55;
        if (*test_ptr == 0x55)
            break;
    } while (bank != 0);

    if (inc_port(3) == 0)
        return -1; // Checked all banks, no luck

    memset((void *)test_ptr, 0, 80 * 25 * 2);
    outc_port(0x0100, 0x30); // Set VGA buffer to 0x3000
    return 0;
}

int memcheck()
{
    volatile char *test_ptr = (char *)0x4000;
    uint8_t bank = inc_port(3) + 1;
    int total = 8;

    printf("Performing memory check...\n");
    do
    {
        outc_port(4, bank++);
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

void bad_int() __interrupt
{
    printf("BAD INT");
}

void init_interrupts()
{
    for (int i = 0; i < 128; i++)
    {
        ivt[i] = bad_int;
    }
    keyboard_install_interrupt();

__asm
    push af
    GLOBAL _ivt
    ld a, _ivt >> 8
    ld i, a
    pop af

    im 2
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

    while (1)
    {
        int scancode = pop_key();
        if (scancode == 0 || (scancode & 0x8000) != 0)
            continue;
        
        printf("Scancode: 0x%04x\n", scancode);
    }

    return 0;
}
