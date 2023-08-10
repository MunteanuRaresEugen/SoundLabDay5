#include "SinWaveStrategy.h"
#include <numbers>
#include <cmath>

bool AudioEngine::SinWaveStrategy::Init(const AudioParameters& params)
{
    ma_device_config deviceConfig;

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.pDeviceID = params.playbackDevice;
    deviceConfig.playback.format = params.format;
    deviceConfig.playback.channels = params.channels;
    deviceConfig.sampleRate = params.sampleRate;
    deviceConfig.dataCallback = params.data_callback;

    if (ma_device_init(nullptr, &deviceConfig, &m_device) != ma_result::MA_SUCCESS)
    {
        return false;
    }

    if (ma_device_start(&m_device) != ma_result::MA_SUCCESS)
    {
        return false;
    }

    return true;
}

bool AudioEngine::SinWaveStrategy::Uninit()
{
    ma_device_uninit(&m_device);

    return true;
}

void AudioEngine::SinWaveStrategy::OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    float* buffer = (float*)pOutput;

    for (size_t i = 0; i < frameCount; i++)
    {
        buffer[i] = std::sin(((double)m_index++ / pDevice->sampleRate) * 2 * std::numbers::pi * 440);
    }
}

const void* AudioEngine::SinWaveStrategy::ChooseData(const void* input, const void* output)
{
    return output;
}
