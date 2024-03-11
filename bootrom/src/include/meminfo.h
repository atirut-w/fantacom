#pragma once
#include <stdint.h>

typedef struct
{
    uint8_t num_banks;
    uint8_t bankmap[32];
    uint8_t reserved[32];
} MemInfo;
extern MemInfo meminfo;

void meminfo_init();
