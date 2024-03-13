#include <simplefs.h>
#include <disk.h>
#include <string.h>

void simplefs_seek(Superblock superblock, int block)
{
    disk_seek(block * (superblock.block_size / 512));
    disk_wait_idle();
}

void simplefs_read(Superblock superblock, void *buffer, int count)
{
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < (superblock.block_size / 512); j++)
        {
            disk_read(buffer);
            buffer += 512;
            disk_wait_idle();
        }
    }
}
