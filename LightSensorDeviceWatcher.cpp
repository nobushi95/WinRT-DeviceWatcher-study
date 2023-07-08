#include "LightSensorDeviceWatcher.h"
#include <winrt/windows.devices.sensors.h>
#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>

using namespace winrt;
using namespace Windows::Devices::Sensors;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;

LightSensorDeviceWatcher::~LightSensorDeviceWatcher()
{
    Stop();
}

auto LightSensorDeviceWatcher::Start(std::function<void(const std::wstring&)> handler) noexcept -> void
{
    _deviceWatcher = DeviceInformation::CreateWatcher(LightSensor::GetDeviceSelector());

    if (_deviceWatcher == nullptr)
        return;

    // デバイスが追加されたときのイベントハンドラ
    auto addedHandler = [&handler](const DeviceWatcher&, const DeviceInformation&)
    {
        handler(L"Added.");
    };

    // デバイスが削除されたときのイベントハンドラ
    auto removedHandler = [&handler](const DeviceWatcher&, const DeviceInformationUpdate&)
    {
        handler(L"Removed.");
    };

    // 列挙が完了したときのイベントハンドラ
    auto enumerationCompletedHandler = [&handler](const DeviceWatcher&, const IInspectable&)
    {
        handler(L"Enumaration Completed.");
    };

    // デバイスの更新があったときのイベントハンドラ
    auto updatedHandler = [&handler](const DeviceWatcher&, const DeviceInformationUpdate&)
    {
        handler(L"Update.");
    };

    auto stoppedHandler = [&handler](const DeviceWatcher&, const IInspectable&)
    {
        handler(L"Stopped");
    };

    // イベントハンドラを設定
    // すべてのイベントハンドラを購読しないとイベントが飛んでこない
    _tokenAdded = _deviceWatcher.Added(addedHandler);
    _tokenRemoved = _deviceWatcher.Removed(removedHandler);
    _tokenEnumerationCompleted = _deviceWatcher.EnumerationCompleted(enumerationCompletedHandler);
    _tokenUpdated = _deviceWatcher.Updated(updatedHandler);
    _tokenStopped = _deviceWatcher.Stopped(stoppedHandler);

    _deviceWatcher.Start();
}

auto LightSensorDeviceWatcher::Stop() const noexcept -> void
{
    if (_deviceWatcher == nullptr)
        return;

    if (_tokenAdded)
    {
        _deviceWatcher.Added(_tokenAdded);
    }
    if (_tokenRemoved)
    {
        _deviceWatcher.Removed(_tokenRemoved);
    }
    if (_tokenEnumerationCompleted)
    {
        _deviceWatcher.EnumerationCompleted(_tokenEnumerationCompleted);
    }
    if (_tokenUpdated)
    {
        _deviceWatcher.Updated(_tokenUpdated);
    }
    if (_tokenStopped)
    {
        _deviceWatcher.Stopped(_tokenStopped);
    }

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
