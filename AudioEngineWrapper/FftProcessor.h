#pragma once

#include "SignalGeneratorWrapper.h"
#include "AudioEngineWrapper.h"

namespace AudioEngineWrapper
{
    public ref class FftProcessor
    {
    public:

        static array<double>^ GetMagnitude(SignalGenerator^ generator);

        static array<double>^ GetMagnitude(AudioEngineService^ engine);
    };
}

