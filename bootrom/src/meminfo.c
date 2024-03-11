#include <meminfo.h>
#include <z80io.h>
#include <string.h>

MemInfo meminfo;

void meminfo_init()
{
    volatile char *test_ptr = (char *)0x3000;
    memset(&meminfo, 0, sizeof(MemInfo));

    uint8_t bios_ram = inc_port(2);
    meminfo.bankmap[(bios_ram / 8)] |= (1 << (bios_ram % 8));
    meminfo.reserved[(bios_ram / 8)] |= (1 << (bios_ram % 8));
    meminfo.num_banks = 1;
    
    uint8_t bank = bios_ram + 1;
    do
    {
        outc_port(3, bank);
        *test_ptr = 0x55;
        if (*test_ptr == 0x55)
        {
            meminfo.num_banks++;
            meminfo.bankmap[(bank / 8)] |= (1 << (bank % 8));
        }
        else
        {
            meminfo.bankmap[(bank / 8)] &= ~(1 << (bank % 8));
        }
        bank++;
    } while (bank != 0);
}
