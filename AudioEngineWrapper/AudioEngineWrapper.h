#pragma once

using namespace System;

#include "../AudioEngine/AudioEngine.h"
#include "AudioDevices.h"

namespace AudioEngineWrapper
{
	public enum class AudioStrategyType
	{
		Duplex = 0,
		Loopback,
		FilePlayback,
		SinWave,
		DuplexFiltered,
		LoopbackFilter
	};

	public ref class AudioEngineService
	{
	public:
		AudioEngineService(size_t sampleRate, size_t maxBufferSize);

		void Start(DeviceData^ playback, DeviceData^ capture, AudioStrategyType type);

		void Stop();

		void ClearData();

		~AudioEngineService();
		!AudioEngineService();

		array<double>^ GetBuffer();

	internal:
		const std::vector<float>& GetLastBuffer();

	private:
		AudioEngine::FancyAudioEngine* engine;
	};
}
