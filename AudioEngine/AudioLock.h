#pragma once

struct AudioLock
{
public:
	AudioLock();
	~AudioLock();

	void Lock();
	void Unlock();

private:
	void* m_mutex;
};

