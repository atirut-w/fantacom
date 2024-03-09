#include <io_devices/disk_ctrl.hpp>
#include <machine.hpp>
#include <iostream>
#include <thread>

DiskCtrl::DiskCtrl(void *ctx) : IODevice(ctx)
{
    size = sizeof(registers);
}

uint8_t DiskCtrl::in(uint16_t addr)
{
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
    if (addr != offsetof(Registers, command))
        return;

    if (registers.command == COMMAND_GET_PRESENCE)
    {
        registers.data = registers.disk_select < disks.size();
    }
    else if (registers.disk_select < disks.size())
    {
        switch (registers.command)
        {
        case COMMAND_GET_LAST_SECTOR:
        {
            if (disks[registers.disk_select].get_status() == Disk::STATUS_BUSY)
            {
                registers.data = 0;
                break;
            }
            disks[registers.disk_select].set_status(Disk::STATUS_BUSY);

            auto stream = disks[registers.disk_select].disk_stream;
            auto current = stream->tellg();
            stream->seekg(0, std::ios::end);
            auto result = (stream->tellg() / 512) - 1;
            stream->seekg(current);

            disks[registers.disk_select].set_status(Disk::STATUS_IDLE);
            registers.data = result;
            break;
        }
        case COMMAND_GET_STATUS:
            registers.data = disks[registers.disk_select].get_status();
            break;
        case COMMAND_SEEK:
        {
            if (disks[registers.disk_select].get_status() == Disk::STATUS_BUSY)
            {
                registers.data = 0;
                break;
            }
            disks[registers.disk_select].set_status(Disk::STATUS_BUSY);
            disks[registers.disk_select].disk_stream->seekg(registers.data * 512);
            disks[registers.disk_select].set_status(Disk::STATUS_IDLE);
            break;
        }
        case COMMAND_READ:
        {
            if (disks[registers.disk_select].get_status() == Disk::STATUS_BUSY)
            {
                break;
            }
            disks[registers.disk_select].set_status(Disk::STATUS_BUSY);

            auto worker = std::thread([registers = registers, disk = &disks[registers.disk_select], machine = (Machine *)ctx]() {
                char data[512];
                disk->disk_stream->read(data, 512);
                
                uint16_t dst = registers.data;
                for (int i = 0; i < 512; i++)
                {
                    machine->mutex.lock();
                    machine->mmu->write(dst++, data[i]);
                    machine->mutex.unlock();
                }
                disk->set_status(Disk::STATUS_IDLE);
            });
            worker.detach();
            break;
        }
        default:
            registers.data = 0;
            break;
        }
    }
}
