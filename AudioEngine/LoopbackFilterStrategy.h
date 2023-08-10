#pragma once

#include "LoopbackStrategy.h"

namespace AudioEngine
{
	class LoopbackFilterStrategy : public LoopbackStrategy
	{
		virtual void OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) override;
	};
}

