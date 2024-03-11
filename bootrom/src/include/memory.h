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
int memory_get_free_bank();
void memory_free_bank(int bank);
