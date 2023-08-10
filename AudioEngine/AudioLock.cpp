#include "AudioLock.h"
#include <mutex>

AudioLock::AudioLock() : m_mutex(new std::mutex())
{

}

AudioLock::~AudioLock()
{
	delete m_mutex;
}

void AudioLock::Lock()
{
	auto mutex = static_cast<std::mutex*>(m_mutex);
	mutex->lock();
}

void AudioLock::Unlock()
{
	auto mutex = static_cast<std::mutex*>(m_mutex);
	mutex->unlock();
}
