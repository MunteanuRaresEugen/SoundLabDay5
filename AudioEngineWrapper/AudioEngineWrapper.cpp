#include "pch.h"

#include "AudioEngineWrapper.h"
#include <exception>
#include "Utils.h"

using namespace System::Diagnostics;

AudioEngineWrapper::AudioEngineService::AudioEngineService(size_t sampleRate, size_t maxBufferSize)
{
	try
	{
		engine = new AudioEngine::FancyAudioEngine(sampleRate, maxBufferSize);
	}
	catch (const std::exception& e)
	{
		Debug::Assert(false, gcnew System::String(e.what()));
	}
}

void AudioEngineWrapper::AudioEngineService::Start(DeviceData^ playback, DeviceData^ capture, AudioStrategyType type)
{
	try
	{
		engine->Start(&playback->info->id, &capture->info->id, (AudioEngine::AudioStrategyType)type);
	}
	catch (const std::exception& e)
	{
		throw gcnew System::Exception(gcnew System::String(e.what()));
	}
}

void AudioEngineWrapper::AudioEngineService::Stop()
{
	engine->Stop();
}

void AudioEngineWrapper::AudioEngineService::ClearData()
{
	engine->ClearBuffer();
}

AudioEngineWrapper::AudioEngineService::~AudioEngineService()
{
	this->!AudioEngineService();
}

AudioEngineWrapper::AudioEngineService::!AudioEngineService()
{
	delete engine;
	engine = nullptr;
}

array<double>^ AudioEngineWrapper::AudioEngineService::GetBuffer()
{
	const auto& buffer = engine->GetBufferData();
	return Utils::ConvertToMananged<float, double>(buffer, buffer.size());
}

const std::vector<float>& AudioEngineWrapper::AudioEngineService::GetLastBuffer()
{
	return engine->GetBufferData();
}
