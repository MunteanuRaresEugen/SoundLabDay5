#pragma once

#include <vector>
#include "ImportExport.h"
#include "miniaudio.h"

namespace AudioEngine
{
	class AUDIOENGINE_API AudioDevices
	{
	public:
		AudioDevices();

		bool Init();

		const std::vector<ma_device_info>& GetPlaybackDevices();
		const std::vector<ma_device_info>& GetCaptureDevices();

		bool Uninit();

		~AudioDevices();
	private:
		ma_context m_context;
		std::vector<ma_device_info> m_playbackDevices;
		std::vector<ma_device_info> m_captureDevices;
	};
}

