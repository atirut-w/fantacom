#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <sys/types.h>

template <typename Address>
struct Bus {
  virtual ~Bus() = default;
  virtual uint8_t read(Address address) const = 0;
  virtual void write(Address address, uint8_t value) = 0;
};

template <typename Address>
struct DeviceBus : public Bus<Address> {
  struct Device {
    Address start;
    int size;
    std::function<uint8_t(Address)> read;
    std::function<void(Address, uint8_t)> write;
    bool handles(Address addr) const {
      return addr >= start && addr < start + size;
    }
  };

  std::vector<Device> devices;

  uint8_t read(Address address) const override {
    for (const auto &device : devices) {
      if (device.handles(address)) {
        return device.read(address - device.start);
      }
    }
    return 0;
  }

  void write(Address address, uint8_t value) override {
    for (const auto &device : devices) {
      if (device.handles(address)) {
        device.write(address - device.start, value);
        return;
      }
    }
  }
};
