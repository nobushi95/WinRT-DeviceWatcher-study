#pragma once
#include <winrt/windows.devices.enumeration.h>
#include <string>
#include <functional>

using namespace winrt;
using namespace Windows::Devices::Enumeration;

class LightSensorDeviceWatcher
{
public:
    LightSensorDeviceWatcher() = default;
    ~LightSensorDeviceWatcher();
    LightSensorDeviceWatcher(const LightSensorDeviceWatcher&) = delete;
    LightSensorDeviceWatcher(LightSensorDeviceWatcher&&) = delete;

    auto Start(std::function<void(const std::wstring&)> handler) noexcept -> void;
    auto Stop() const noexcept -> void;
    auto IsStarted() const noexcept -> bool;

private:
    DeviceWatcher _deviceWatcher{ nullptr };
    event_token _tokenAdded, _tokenRemoved, _tokenEnumerationCompleted, _tokenUpdated, _tokenStopped;
};

