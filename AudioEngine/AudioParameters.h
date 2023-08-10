#pragma once
#include "miniaudio.h"
#include <string>

namespace AudioEngine
{
	struct AudioParameters
	{
		const ma_device_id* playbackDevice;
		const ma_device_id* captureDevice;
		const ma_format format;
		const ma_uint32 sampleRate;
		const ma_uint32 channels;
		const ma_device_data_proc data_callback;
	};
}
