//#pragma once
//
//#include "../AudioEngine/AudioDevices.h"
//using namespace System;
//
//namespace AudioEngineWrapper
//{
//	public ref class DeviceData
//	{
//	public:
//		property String^ Name;
//		property bool IsDefault;
//
//		bool Equals(Object^ other) override;
//
//	internal:
//		DeviceData(ma_device_info& info);
//
//		ma_device_info* info;
//	};
//
//	public ref class AudioDevices
//	{
//	public:
//		AudioDevices();
//		~AudioDevices();
//		!AudioDevices();
//
//		array<DeviceData^>^ GetPlaybackDevices();
//
//		array<DeviceData^>^ GetCaptureDevices();
//
//	private:
//		AudioEngine::AudioDevices* m_devices;
//
//		static DeviceData^ Convert(const ma_device_info& info);
//	};
//}
//
