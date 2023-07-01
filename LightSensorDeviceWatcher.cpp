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

auto LightSensorDeviceWatcher::Start() noexcept -> void
{
    _deviceWatcher = DeviceInformation::CreateWatcher(LightSensor::GetDeviceSelector());

    // デバイスが追加されたときのイベントハンドラ
    auto deviceAddedHandler = [](const DeviceWatcher&, const DeviceInformation&)
    {

    };

    // デバイスが削除されたときのイベントハンドラ
    auto deviceRemovedHandler = [](const DeviceWatcher&, const DeviceInformationUpdate&)
    {

    };

    // 列挙が完了したときのイベントハンドラ
    auto enumerationCompletedHandler = [](const DeviceWatcher&, const winrt::Windows::Foundation::IInspectable&)
    {
    };

    // デバイスの更新があったときのイベントハンドラ
    auto deviceUpdatedHandler = [](const DeviceWatcher&, const DeviceInformationUpdate&)
    {
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
