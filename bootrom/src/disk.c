#include <disk.h>
#include <z80io.h>
#include <string.h>

enum DiskCommand
{
    COMMAND_GET_PRESENCE,
    COMMAND_GET_LAST_SECTOR,
    COMMAND_GET_STATUS,
    
    COMMAND_SEEK,
    COMMAND_READ,
    COMMAND_WRITE
};

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
    return disk_send_command(COMMAND_GET_PRESENCE) > 0;
}

int disk_get_status()
{
    return disk_send_command(COMMAND_GET_STATUS);
}

void disk_wait_idle()
{
    while (disk_get_status() == 1);
}

int disk_get_last_sector()
{
    return disk_send_command(COMMAND_GET_LAST_SECTOR);
}

void disk_seek(int sector)
{
    disk_set_data(sector);
    disk_send_command(COMMAND_SEEK);
}

void disk_read(void *data)
{
    disk_set_data(data);
    disk_send_command(COMMAND_READ);
}
