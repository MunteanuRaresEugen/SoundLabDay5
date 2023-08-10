#include "DuplexStrategy.h"
#include <cstring>

namespace AudioEngine
{ 
	bool DuplexStrategy::Init(const AudioParameters& params)
	{
		ma_result result;
		ma_device_config deviceConfig;
		m_device = std::make_unique<ma_device>();

		deviceConfig = ma_device_config_init(ma_device_type_duplex);
		deviceConfig.sampleRate = params.sampleRate;
		deviceConfig.capture.pDeviceID = params.captureDevice;
		deviceConfig.capture.format = params.format;
		deviceConfig.capture.channels = params.channels;
		deviceConfig.capture.shareMode = ma_share_mode_shared;
		deviceConfig.playback.pDeviceID = params.playbackDevice;
		deviceConfig.playback.format = params.format;
		deviceConfig.playback.channels = params.channels;
		deviceConfig.dataCallback = params.data_callback;

		result = ma_device_init(nullptr, &deviceConfig, m_device.get());

		if (result != MA_SUCCESS)
		{
			ma_device_uninit(m_device.get());
			return false;
		}

		ma_device_start(m_device.get());

		return true;
	}

	bool DuplexStrategy::Uninit()
	{
		ma_device_uninit(m_device.get());

		return true;
	}

	void DuplexStrategy::OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		auto size = ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels);
		memcpy(pOutput, pInput, frameCount * size);
	}

	const void* DuplexStrategy::ChooseData(const void* input, const  void* output)
	{
		return input;
	}
}
