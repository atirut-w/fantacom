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
            registers.data = disks[registers.disk_select].last_sector;
            break;
        }
        case COMMAND_GET_STATUS:
            registers.data = disks[registers.disk_select].get_status();
            break;
        case COMMAND_SEEK:
        {
            Disk &disk = disks[registers.disk_select];
            if (disk.get_status() == Disk::STATUS_BUSY)
            {
                registers.data = 0;
                break;
            }
            disk.set_status(Disk::STATUS_BUSY);
            disk.disk_stream->seekg(registers.data * 512);
            disk.current_sector = registers.data;
            
            if (registers.data != disk.current_sector + 1)
            {
                auto worker = std::thread([registers = registers, disk = &disk]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    disk->set_status(Disk::STATUS_IDLE);
                });
                worker.detach();
            }
            else
            {
                disk.set_status(Disk::STATUS_IDLE);
            }

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
