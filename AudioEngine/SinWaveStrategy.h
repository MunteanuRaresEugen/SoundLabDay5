#pragma once
#include "IAudioStrategy.h"

namespace AudioEngine
{
	class SinWaveStrategy : public IAudioStrategy
	{
	public:

		// Inherited via IAudioStrategy
		virtual bool Init(const AudioParameters& params) override;
		virtual bool Uninit() override;
		virtual void OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) override;
		virtual const void* ChooseData(const void* input, const void* output) override;

	private:

		ma_device m_device;
		size_t m_index;
	};
}

