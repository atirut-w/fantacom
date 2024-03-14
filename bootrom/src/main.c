#include <z80io.h>
#include <stdio.h>
#include <string.h>
#include <keyboard.h>
#include <ivt.h>
#include <disk.h>
#include <stdint.h>
#include <stdbool.h>
#include <memory.h>
#include <simplefs.h>

int init_display()
{
    int bank = memory_get_free_bank();
    if (bank == -1)
        return -1;

    outc_port(3, bank);
    memset((char *)0x3000, 0, 80 * 25 * 2);
    outc_port(0x0100, 0x30); // Set VGA buffer to 0x3000
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

int boot_simplefs()
{
    uint8_t *scratch = (uint8_t *)0x8000;
    Superblock superblock;
    memcpy(superblock, scratch, sizeof(Superblock));
    printf("Attempting to boot from %s\n", superblock.label);

    // We can only address up to sector 0xffff, so we have to check if the FS is too large
    int block_size_s = superblock.block_size / 512;
    if (superblock.blocks_low > (0x10000 / block_size_s) || superblock.blocks_high > 0)
    {
        printf("Filesystem too large\n");
        return -1;
    }
    printf("%d blocks used out of %d\n", superblock.blocks_used_low, superblock.blocks_low);

    NamelistEntry root;
    NamelistEntry current;

    simplefs_seek(superblock, 2);
    simplefs_read(superblock, scratch, 1);
    memcpy(&root, scratch, sizeof(NamelistEntry));

    int block = (root.datablock * 128) / superblock.block_size;
    int offset = (root.datablock * 128) % superblock.block_size;
    bool bootable = false;

    printf("Finding boot.bin...");
    do
    {
        if (offset == 0)
        {
            simplefs_seek(superblock, block + 2);
            simplefs_read(superblock, scratch, 1);
        }
        memcpy(&current, scratch + offset, sizeof(NamelistEntry));
        
        if (memcmp("boot.bin", current.fname, 8) == 0 && current.flags & FILE_FLAGS_TYPE_MASK == FILE_FLAGS_TYPE_FILE)
        {
            bootable = true;
            break;
        }

        block = (current.next * 128) / superblock.block_size;
        offset = (current.next * 128) % superblock.block_size;
    } while (current.next > 0);

    if (!bootable)
    {
        printf(" not found\n");
        return -1;
    }
    printf(" loading...");
    
    return 0;
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
    disk_seek(0);
    disk_wait_idle();

    {
        uint8_t bank = memory_get_free_bank();
        if (bank == -1)
        {
            printf("Could not allocate memory for boot sector\n");
            return -1;
        }
        outc_port(0x8, bank);
    }

    uint8_t *sector = (uint8_t *)0x8000; // Top 32 KiB of RAM
    disk_read(sector);
    disk_wait_idle();

    if (*(uint16_t *)0x81fe == 0xaa55)
    {
    __asm
        ld hl, 0x3000
        ld sp, hl
        jp 0x8000
    __endasm;
    }
    else if (memcmp(sector, "\x1bSFS", 4) == 0)
    {
        boot_simplefs();
        return 0;
    }
    else
    {
        printf("Disk 0 is not bootable\n");
        return -1;
    }

    return 0;
}
