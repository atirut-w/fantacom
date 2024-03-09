#include <disk.h>
#include <z80io.h>

void disk_select(char disk)
{
    outc_port(0x0300, disk);
}

unsigned int disk_send_command(char cmd)
{
    outc_port(0x0301, cmd);
    return inw_port(0x0302);
}

void disk_set_data(int data)
{
    outw_port(0x0302, data);
}
