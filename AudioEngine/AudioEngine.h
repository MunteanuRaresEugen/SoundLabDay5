#pragma once

#include <vector>
#include "ImportExport.h"
#include "IAudioStrategy.h"
#include <memory>
#include <limits>
#include "AudioLock.h"

namespace AudioEngine
{
	enum class AUDIOENGINE_API AudioStrategyType
	{
		Duplex = 0,
		Loopback,
		FilePlayback,
		SinWave,
		DuplexFiltered,
		LoopbackFiltered
	};

	class AUDIOENGINE_API FancyAudioEngine
	{
	public:
		FancyAudioEngine(size_t sampleRate = 44100, size_t maxBufferSize = ~0);

		FancyAudioEngine(const FancyAudioEngine& engine) = delete;
		FancyAudioEngine(FancyAudioEngine&& engine) = delete;

		void Start(const ma_device_id* playbackDevice,
			const ma_device_id* captureDevice, AudioStrategyType type = AudioStrategyType::Duplex);

		void Stop();

		void ClearBuffer();

		const std::vector<float>& GetBufferData() noexcept;

	private:
		ma_device* m_device;
		std::vector<std::vector<float>> m_buffers;

		size_t m_maxBufferSize;
		size_t m_sampleRate;

		void callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
		friend void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

		std::unique_ptr<IAudioStrategy> m_strategy;

		AudioLock m_mutex;
	};
}

