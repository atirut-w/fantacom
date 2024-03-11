#pragma once
#include <io_device.hpp>
#include <istream>
#include <vector>
#include <memory>
#include <mutex>

class Disk
{
public:
    std::shared_ptr<std::istream> disk_stream;
    int last_sector;
    int current_sector;

    enum Status
    {
        STATUS_IDLE,
        STATUS_BUSY
    } status = STATUS_IDLE;
    std::shared_ptr<std::mutex> mutex;

    Disk(std::shared_ptr<std::istream> disk_stream)
    {
        this->disk_stream = disk_stream;
        
        disk_stream->seekg(0, std::ios::end);
        last_sector = (disk_stream->tellg() / 512);
        disk_stream->seekg(0);
        current_sector = 0;

        mutex = std::make_shared<std::mutex>();
    }

    int get_status()
    {
        mutex->lock();
        auto result = status;
        mutex->unlock();
        return result;
    }

    void set_status(Status status)
    {
        mutex->lock();
        this->status = status;
        mutex->unlock();
    }
};

class DiskCtrl : public IODevice
{
public:
    std::vector<Disk> disks;

    DiskCtrl(void *ctx);
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;

    struct Registers
    {
        uint8_t disk_select;
        uint8_t command;
        uint16_t data;
    } registers;

    enum Command
    {
        COMMAND_GET_PRESENCE,
        COMMAND_GET_LAST_SECTOR,
        COMMAND_GET_STATUS,
        
        COMMAND_SEEK,
        COMMAND_READ,
        COMMAND_WRITE
    };
};
