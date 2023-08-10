#pragma once

#include "IAudioStrategy.h"
#include <memory>

namespace AudioEngine
{
	class DuplexStrategy : public IAudioStrategy
	{
	public:
		// Inherited via IAudioStrategy
		virtual bool Init(const AudioParameters& params) override;
		virtual bool Uninit() override;
		virtual void OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) override;
		virtual const void* ChooseData(const void* input, const void* output) override;

	private:
		std::unique_ptr<ma_device> m_device;
	};
}

