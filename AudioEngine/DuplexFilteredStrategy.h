#pragma once
#include "DuplexStrategy.h"

namespace AudioEngine
{
	class DuplexFilteredStrategy :
		public DuplexStrategy
	{
	public:
		virtual void OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) override;
	};

}
