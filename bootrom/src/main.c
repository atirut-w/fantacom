#include <z80io.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <ivt.h>
#include <disk.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

int init_display()
{
    uint8_t bank = 0;
    do
    {
        bool present = meminfo.bankmap[(bank / 8)] & (1 << (bank % 8));
        bool reserved = meminfo.reserved[(bank / 8)] & (1 << (bank % 8));
        if (present && !reserved)
        {
            meminfo.reserved[(bank / 8)] |= (1 << (bank % 8));
            outc_port(3, bank);
            memset((void *)0x3000, 0, 80 * 25 * 2);
            outc_port(0x0100, 0x30); // Set VGA buffer to 0x3000
            return 0;
        }
        bank++;
    } while (bank != 0);

    return -1;
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
    outc_port(0x0100, 0); // Assure the user we entered BIOS by flashing the ROM's guts
    memory_init_meminfo();
    if (init_display() != 0)
        return -1;
    init_interrupts();
    
    printf("FantaCom Boot ROM (C) Atirut Wattanamongkol & contributors\n\n");
    printf("Total memory: %d KiB\n", meminfo.num_banks * 4);

    disk_select(0);
    if (!disk_check_presense())
    {
        printf("Disk 0 not present\n");
        return -1;
    }
    printf("Last sector: %u\n", disk_get_last_sector());
    disk_seek(0);
    disk_wait_idle();

    char sector[512];
    disk_read(0, sector);
    // Note: don't wait, just draw the column header while the controller DMAs

    printf("\\  ");
    for (int i = 0; i < 16; i++)
    {
        printf("x%01X ", i);
    }
    printf("0123456789ABCDEF\n \\ ");
    for (int i = 0; i < 16; i++)
    {
        printf("-- ");
    }
    printf("----------------\n");

    disk_wait_idle();
    for (int row = 0; row < 16; row++)
    {
        printf("%01Xx ", row);
        for (int col = 0; col < 16; col++)
        {
            printf("%02x ", sector[row * 16 + col] & 0xff);
        }
        for (int col = 0; col < 16; col++)
        {
            char ch = sector[row * 16 + col];
            printf("%c", ch < 32 || ch > 126 ? '.' : ch);
        }
        printf("\n");
    }

    return 0;
}
