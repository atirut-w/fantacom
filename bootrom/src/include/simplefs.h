#pragma once
#include <stdint.h>

enum FSFlags
{
    FS_FLAGS_DIRTY = 1,
    FS_FLAGS_READONLY = 2
};

typedef struct
{
    char signature[4];
    uint8_t flags;
    uint8_t revision;
    uint16_t namelist_blocks;
    uint16_t block_size;
    uint16_t blocks_low;
    uint8_t blocks_high;
    uint16_t blocks_used_low;
    uint8_t blocks_used_high;
    char label[19];
} Superblock;

void simplefs_seek(Superblock superblock, int block);
void simplefs_read(Superblock superblock, uint8_t *buffer, int count);
