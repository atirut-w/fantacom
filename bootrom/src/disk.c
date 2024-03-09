#include <disk.h>
#include <z80io.h>
#include <string.h>

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

bool disk_check_presense()
{
    return disk_send_command(0) > 0;
}

int disk_get_status()
{
    return disk_send_command(2);
}

void disk_wait_idle()
{
    while (disk_get_status() == 1);
}

int disk_get_last_sector()
{
    return disk_send_command(1);
}

void disk_seek(int sector)
{
    disk_set_data(sector);
    disk_send_command(3);
}

void disk_read(int sector, void *data)
{
    disk_set_data(data);
    disk_send_command(4);
}
