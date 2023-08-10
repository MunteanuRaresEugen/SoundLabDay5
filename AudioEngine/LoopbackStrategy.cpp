#include "LoopbackStrategy.h"

namespace AudioEngine
{
    bool LoopbackStrategy::Init(const AudioParameters& params)
    {
        m_device = std::make_unique<ma_device>();
        m_encoder = std::make_unique<ma_encoder>();
        ma_result result;
        ma_encoder_config encoderConfig;
        ma_device_config deviceConfig;

        /* Loopback mode is currently only supported on WASAPI. */
        ma_backend backends[] = {
            ma_backend_wasapi
        };

        encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, params.format, params.channels, params.sampleRate);

        if (ma_encoder_init_file("./output.wav", &encoderConfig, m_encoder.get()) != MA_SUCCESS)
        {
            return false;
        }

        deviceConfig = ma_device_config_init(ma_device_type_loopback);
        deviceConfig.capture.pDeviceID = params.playbackDevice; // for loopback strategy the playback device will represent the "capture" device
        deviceConfig.capture.format = m_encoder->config.format;
        deviceConfig.capture.channels = m_encoder->config.channels;
        deviceConfig.sampleRate = m_encoder->config.sampleRate;
        deviceConfig.dataCallback = params.data_callback;
        deviceConfig.pUserData = m_encoder.get();

        result = ma_device_init_ex(backends, sizeof(backends) / sizeof(backends[0]), nullptr, &deviceConfig, m_device.get());
        if (result != MA_SUCCESS)
        {
            return false;
        }

        result = ma_device_start(m_device.get());

        if (result != MA_SUCCESS)
        {
            ma_device_uninit(m_device.get());
            return false;
        }

        return true;
    }

    bool LoopbackStrategy::Uninit()
    {
        ma_device_uninit(m_device.get());
        ma_encoder_uninit(m_encoder.get());

        return true;
    }

    void LoopbackStrategy::OnCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
    {
        ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData;
        ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, nullptr);
    }

    const const void* LoopbackStrategy::ChooseData(const void* input, const void* output)
    {
        return input;
    }
}
