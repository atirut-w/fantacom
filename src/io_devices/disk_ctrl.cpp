#include <io_devices/disk_ctrl.hpp>
#include <machine.hpp>
#include <iostream>

DiskCtrl::DiskCtrl(void *ctx) : IODevice(ctx)
{
    size = sizeof(registers);
}

uint8_t DiskCtrl::in(uint16_t addr)
{
    if (registers.command == COMMAND_GET_PRESENCE)
    {
        registers.data = registers.disk_select < disks.size();
    }
    else if (registers.disk_select >= disks.size())
    {
        registers.data = 0;
    }
    else
    {
        switch (registers.command)
        {
        case COMMAND_GET_LAST_SECTOR:
        {
            disks[registers.disk_select].mutex.lock();

            auto stream = disks[registers.disk_select].disk_stream;
            auto current = stream->tellg();
            stream->seekg(0, std::ios::end);
            auto result = (stream->tellg() / 512) - 1;
            stream->seekg(current);
            
            disks[registers.disk_select].mutex.unlock();
            registers.data = result;
            break;
        }
        default:
            registers.data = 0;
            break;
        }
    }

    if (addr < sizeof(registers))
    {
        return ((uint8_t *)&registers)[addr];
    }
    return 0;
}

void DiskCtrl::out(uint16_t addr, uint8_t val)
{
    if (addr < sizeof(registers))
    {
        ((uint8_t *)&registers)[addr] = val;
    }
}
