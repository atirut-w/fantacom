#pragma once
#include <stdint.h>

typedef struct
{
    int num_banks;
    uint8_t bankmap[32];
    uint8_t reserved[32];
} MemInfo;
extern MemInfo meminfo;

void memory_init_meminfo();
