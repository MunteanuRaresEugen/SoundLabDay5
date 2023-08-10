#include "AudioDevices.h"

#include <algorithm>
#include <iterator>

AudioEngine::AudioDevices::AudioDevices()
	:m_context()
{
}

bool AudioEngine::AudioDevices::Init()
{
	ma_result result;
	ma_device_info* pPlaybackDeviceInfos;
	ma_uint32 playbackDeviceCount;
	ma_device_info* pCaptureDeviceInfos;
	ma_uint32 captureDeviceCount;
	ma_uint32 iDevice;

	if (ma_context_init(NULL, 0, NULL, &m_context) != MA_SUCCESS) {
		return false;
	}

	result = ma_context_get_devices(&m_context, &pPlaybackDeviceInfos, &playbackDeviceCount, &pCaptureDeviceInfos, &captureDeviceCount);
	if (result != MA_SUCCESS) {
		return false;
	}

	std::copy(&pPlaybackDeviceInfos[0], &pPlaybackDeviceInfos[playbackDeviceCount], std::back_inserter(m_playbackDevices));
	std::copy(&pCaptureDeviceInfos[0], &pCaptureDeviceInfos[captureDeviceCount], std::back_inserter(m_captureDevices));

	return true;
}

const std::vector<ma_device_info>& AudioEngine::AudioDevices::GetPlaybackDevices()
{
	Init();
	Uninit();

	return m_playbackDevices;

}

const std::vector<ma_device_info>& AudioEngine::AudioDevices::GetCaptureDevices()
{
	Init();
	Uninit();

	return m_captureDevices;
}

bool AudioEngine::AudioDevices::Uninit()
{
	ma_context_uninit(&m_context);
	return true;
}

AudioEngine::AudioDevices::~AudioDevices()
{

}
