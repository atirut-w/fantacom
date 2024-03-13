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

enum FileFlags
{
    FILE_FLAGS_OTHER_EXECUTE = 1 << 0,
    FILE_FLAGS_OTHER_WRITE = 1 << 1,
    FILE_FLAGS_OTHER_READ = 1 << 2,

    FILE_FLAGS_GROUP_EXECUTE = 1 << 3,
    FILE_FLAGS_GROUP_WRITE = 1 << 4,
    FILE_FLAGS_GROUP_READ = 1 << 5,

    FILE_FLAGS_OWNER_EXECUTE = 1 << 6,
    FILE_FLAGS_OWNER_WRITE = 1 << 7,
    FILE_FLAGS_OWNER_READ = 1 << 8,

    FILE_FLAGS_STICKY = 1 << 9,
    FILE_FLAGS_SET_GID = 1 << 10,
    FILE_FLAGS_SET_UID = 1 << 11,

    FILE_FLAGS_TYPE_MASK = 0xf000,
    FILE_FLAGS_TYPE_FIFO = 0x1000,
    FILE_FLAGS_TYPE_CHAR_DEV = 0x2000,
    FILE_FLAGS_TYPE_DIR = 0x4000,
    FILE_FLAGS_TYPE_BLOCK_DEV = 0x6000,
    FILE_FLAGS_TYPE_FILE = 0x8000,
    FILE_FLAGS_TYPE_SYMLINK = 0xa000,
    FILE_FLAGS_TYPE_SOCKET = 0xc000
};

typedef struct
{
    uint16_t flags;
    uint16_t datablock;
    uint16_t next;
    uint16_t prev;
    uint16_t parent;
    uint32_t size;
    uint64_t created;
    uint64_t modified;
    uint16_t uid;
    uint16_t gid;
    char fname[64];
} NamelistEntry;

void simplefs_seek(Superblock superblock, int block);
void simplefs_read(Superblock superblock, void *buffer, int count);
