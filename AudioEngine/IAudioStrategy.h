#pragma once

#include "AudioParameters.h"

namespace AudioEngine
{
	class IAudioStrategy
	{
	public:
		virtual bool Init(const AudioParameters& params) = 0;
		virtual bool Uninit() = 0;
		virtual void OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) = 0;
		virtual const void* ChooseData(const void* input, const void* output) = 0;
	};
}

