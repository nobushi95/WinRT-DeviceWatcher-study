#include "LightSensorDeviceWatcher.h"
#include <winrt/windows.devices.sensors.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>

using namespace winrt;
using namespace Windows::Devices::Sensors;
using namespace Windows::Foundation::Collections;

LightSensorDeviceWatcher::~LightSensorDeviceWatcher()
{
    Stop();
}

auto LightSensorDeviceWatcher::Start(std::function<void(const std::wstring&)> handler) noexcept -> void
{
    _deviceWatcher = DeviceInformation::CreateWatcher(LightSensor::GetDeviceSelector());

    // デバイスが追加されたときのイベントハンドラ
    auto deviceAddedHandler = [&handler](const DeviceWatcher&, const DeviceInformation&)
    {
        handler(L"Added.");
    };

    // デバイスが削除されたときのイベントハンドラ
    auto deviceRemovedHandler = [&handler](const DeviceWatcher&, const DeviceInformationUpdate&)
    {
        handler(L"Removed.");
    };

    // 列挙が完了したときのイベントハンドラ
    auto enumerationCompletedHandler = [&handler](const DeviceWatcher&, const winrt::Windows::Foundation::IInspectable&)
    {
        handler(L"Enumaration Completed.");
    };

    // デバイスの更新があったときのイベントハンドラ
    auto deviceUpdatedHandler = [&handler](const DeviceWatcher&, const DeviceInformationUpdate&)
    {
        handler(L"Update.");
    };

    // イベントハンドラを設定
    _deviceWatcher.Added(deviceAddedHandler);
    _deviceWatcher.Removed(deviceRemovedHandler);
    _deviceWatcher.EnumerationCompleted(enumerationCompletedHandler);
    _deviceWatcher.Updated(deviceUpdatedHandler);
}

auto LightSensorDeviceWatcher::Stop() const noexcept -> void
{
    if (IsStarted())
    {
        _deviceWatcher.Stop();
    }
}

auto LightSensorDeviceWatcher::IsStarted() const noexcept -> bool
{
    if (_deviceWatcher == nullptr)
        return false;
    auto status = _deviceWatcher.Status();
    return (status == DeviceWatcherStatus::Started) ||
        (status == DeviceWatcherStatus::EnumerationCompleted);
}
