#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>

template <typename Address>
struct Bus {
  using Read = std::function<uint8_t(Address)>;
  using Write = std::function<void(Address, uint8_t)>;

  struct Device {
    Address start;
    Read read;
    Write write;
  };

  std::vector<Device> devices;

  void add_device(Device device) {
    devices.push_back(device);
  }

  void remove_device(Device device) {
    devices.erase(std::remove(devices.begin(), devices.end(), device), devices.end());
  }

  uint8_t read(Address address) const {
    for (const auto &device : devices) {
      if (address >= device.start) {
        return device.read(address - device.start);
      }
    }
    return 0;
  }

  void write(Address address, uint8_t value) {
    for (const auto &device : devices) {
      if (address >= device.start) {
        device.write(address - device.start, value);
        return;
      }
    }
  }
};
