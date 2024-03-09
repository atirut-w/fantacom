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
    enum Status
    {
        STATUS_IDLE,
        STATUS_BUSY
    } status = STATUS_IDLE;
    std::mutex mutex;

    Disk(std::shared_ptr<std::istream> disk_stream)
    {
        this->disk_stream = disk_stream;
    }
};

class DiskCtrl : public IODevice
{
public:
    std::vector<Disk> disks;

    DiskCtrl(void *ctx);
    uint8_t in(uint16_t addr) override;
    void out(uint16_t addr, uint8_t val) override;

    struct
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
