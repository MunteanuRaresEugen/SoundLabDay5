#include "FilePlaybackStrategy.h"
#include <filesystem>

bool AudioEngine::FilePlaybackStrategy::Init(const AudioParameters& params)
{
    ma_result result;
    ma_device_config deviceConfig;
    ma_decoder_config decoderConfig;

    m_device = std::make_unique<ma_device>();
    m_decoder = std::make_unique<ma_decoder>();

    if (std::filesystem::exists("./output.wav") == false)
    {
        return false;
    }

    decoderConfig = ma_decoder_config_init(params.format, params.channels, params.sampleRate);

    result = ma_decoder_init_file("./output.wav", &decoderConfig, m_decoder.get());

    if (result != MA_SUCCESS)
    {
        return false;
    }

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.pDeviceID = params.playbackDevice;
    deviceConfig.playback.format = m_decoder->outputFormat;
    deviceConfig.playback.channels = m_decoder->outputChannels;
    deviceConfig.sampleRate = m_decoder->outputSampleRate;
    deviceConfig.dataCallback = params.data_callback;
    deviceConfig.pUserData = m_decoder.get();

    if (ma_device_init(nullptr, &deviceConfig, m_device.get()) != MA_SUCCESS)
    {
        ma_decoder_uninit(m_decoder.get());
        return false;
    }

    if (ma_device_start(m_device.get()) != MA_SUCCESS)
    {
        ma_device_uninit(m_device.get());
        ma_decoder_uninit(m_decoder.get());
        return false;
    }

    return true;
}

bool AudioEngine::FilePlaybackStrategy::Uninit()
{
    ma_device_uninit(m_device.get());
    ma_decoder_uninit(m_decoder.get());
    return true;
}

void AudioEngine::FilePlaybackStrategy::OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder_read_pcm_frames(m_decoder.get(), pOutput, frameCount, nullptr);
}

const void* AudioEngine::FilePlaybackStrategy::ChooseData(const void* input, const void* output)
{
    return output;
}
