#include <z80io.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <ivt.h>
#include <disk.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>

char *disk_scratch = (char *)0x8000;

int init_display()
{
    int bank = memory_get_free_bank();
    if (bank == -1)
        return -1;

    outb(3, bank);
    memset((char *)0x3000, 0, 80 * 25 * 2);
    outb(0x0100, 0x30); // Set VGA buffer to 0x3000
    return 0;
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
    // outb(0x0100, 0); // Assure the user we entered BIOS by flashing the ROM's guts
    memory_init_meminfo();
    if (init_display() != 0)
        return -1;
    init_interrupts();

    return 0; // TODO: Update the following codes for the new Internal Component Interconnect system.
    
    printf("FantaCom Boot ROM (C) Atirut Wattanamongkol & contributors\n\n");
    printf("Total memory: %d KiB\n", meminfo.num_banks * 4);

    disk_select(0);
    if (!disk_check_presense())
    {
        printf("Disk 0 not present\n");
        return -1;
    }
    disk_seek(0);
    disk_wait_idle();

    {
        uint8_t bank = memory_get_free_bank();
        if (bank == -1)
        {
            printf("Could not allocate memory for boot sector\n");
            return -1;
        }
        outb(0x8, bank);
    }

    disk_read(disk_scratch);
    disk_wait_idle();

    if (*(uint16_t *)0x81fe == 0xaa55)
    {
    __asm
        ld hl, 0x3000
        ld sp, hl
        jp 0x8000
    __endasm;
    }
    else
    {
        printf("Disk 0 is not bootable\n");
        return -1;
    }

    return 0;
}
