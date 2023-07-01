#pragma once
#include <winrt/windows.devices.enumeration.h>

using namespace winrt;
using namespace Windows::Devices::Enumeration;

class LightSensorDeviceWatcher
{
public:
    LightSensorDeviceWatcher() = default;
    ~LightSensorDeviceWatcher();
    LightSensorDeviceWatcher(const LightSensorDeviceWatcher&) = delete;
    LightSensorDeviceWatcher(LightSensorDeviceWatcher&&) = delete;

    auto Start() noexcept -> void;
    auto Stop() const noexcept -> void;
    auto IsStarted() const noexcept -> bool;

private:
    DeviceWatcher _deviceWatcher{ nullptr };
};

