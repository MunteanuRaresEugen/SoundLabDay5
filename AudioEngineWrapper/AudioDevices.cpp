#include "pch.h"
#include "AudioDevices.h"
//#include "Utils.h"
//
//namespace AudioEngineWrapper
//{
//    AudioDevices::AudioDevices()
//    {
//        m_devices = new AudioEngine::AudioDevices();
//    }
//
//    AudioDevices::~AudioDevices()
//    {
//        this->!AudioDevices();
//    }
//
//    AudioDevices::!AudioDevices()
//    {
//        delete m_devices;
//        m_devices = nullptr;
//        this->~AudioDevices();
//    }
//
//    array<DeviceData^>^ AudioDevices::GetPlaybackDevices()
//    {
//        const auto& data = m_devices->GetPlaybackDevices();
//        return Utils::ConvertToManangedWithFunc<ma_device_info, DeviceData^>(data, Convert, data.size());
//    }
//
//    array<DeviceData^>^ AudioDevices::GetCaptureDevices()
//    {
//        const auto& data = m_devices->GetCaptureDevices();
//        return Utils::ConvertToManangedWithFunc<ma_device_info, DeviceData^>(data, Convert, data.size());
//    }
//
//    DeviceData^ AudioDevices::Convert(const ma_device_info& info)
//    {
//        auto result = gcnew DeviceData(const_cast<ma_device_info&>(info));
//        result->Name = gcnew String(info.name);
//        result->IsDefault = info.isDefault;
//        return result;
//    }
//
//    bool DeviceData::Equals(Object^ other)
//    {
//        if (auto data = dynamic_cast<DeviceData^>(other))
//        {
//            return this->IsDefault == data->IsDefault && this->Name == data->Name;
//        }
//
//        return false;
//    }
//
//    DeviceData::DeviceData(ma_device_info& info)
//    {
//        this->info = &info;
//    }
//}
