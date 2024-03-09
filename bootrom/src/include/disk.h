#pragma once
#include <stdbool.h>

void disk_select(char disk);
unsigned int disk_send_command(char cmd);
void disk_set_data(int data);

bool disk_check_presence();
int disk_get_status();
void disk_wait_idle();
int disk_get_last_sector();
void disk_seek(int sector);
void disk_read(int sector, void *data);
