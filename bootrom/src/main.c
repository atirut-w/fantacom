#include <z80io.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <ivt.h>
#include <disk.h>
#include <stdint.h>

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

typedef struct
{
    uint8_t num_banks;
    uint8_t bankmap[32];
} MemInfo;

MemInfo memcheck()
{
    MemInfo info;
    volatile char *test_ptr = (char *)0x4000;
    uint8_t bios_ram = inc_port(2);
    uint8_t vga_ram = inc_port(3);
    uint8_t valid_start = 0;
    uint8_t valid_end = 0;

    printf("Building bankmap...\n");
    for (uint8_t bank = 0; bank < bios_ram; bank++)
    {
        info.bankmap[(bank / 8)] &= ~(1 << (bank % 8)); // Anything that comes before is guaranteed to be invalid
    }
    info.bankmap[(bios_ram / 8)] |= (1 << (bios_ram % 8));
    info.bankmap[(vga_ram / 8)] |= (1 << (vga_ram % 8));
    info.num_banks = 2;
    
    uint8_t bank = vga_ram + 1;
    int total = 8; // 8KiB guaranteed (BIOS & VGA RAM)
    do
    {
        outc_port(4, bank++);
        *test_ptr = 0x55;
        if (*test_ptr == 0x55)
        {
            total += 4;
            printf("\r%4d KiB", total);
            info.num_banks++;
            info.bankmap[(bank / 8)] |= (1 << (bank % 8));
        }
        else
        {
            info.bankmap[(bank / 8)] &= ~(1 << (bank % 8));
        }
    } while (bank != 0);
    printf(" OK\n");

    return info;
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
