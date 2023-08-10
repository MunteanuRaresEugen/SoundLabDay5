#pragma once

#ifdef AUDIOENGINE_EXPORTS
#define AUDIOENGINE_API __declspec(dllexport)
#else
#define AUDIOENGINE_API __declspec(dllimport)
#endif // AUDIOENGINE_EXPORTS