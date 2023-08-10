#include "DuplexFilteredStrategy.h"
#include "DigitalFilters.h"

void AudioEngine::DuplexFilteredStrategy::OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{	
	const double cutoff = 500;
	LowPassFilter3 filter(1./pDevice->sampleRate, 2*std::numbers::pi * cutoff);
	float* input = (float*)pInput;
	float* output = (float*)pOutput;
	for (size_t buferIndex = 0; buferIndex < frameCount; buferIndex++)
	{
		input[buferIndex] = filter.update(input[buferIndex]);
	}

	DuplexStrategy::OnCallback(pDevice, output, input, frameCount);
}
