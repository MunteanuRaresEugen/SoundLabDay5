#pragma once

#include "fft.hpp"
#include "ImportExport.h"

namespace AudioEngine
{
	class FFTProcessor
	{
	public:
		template<class T>
		static bool GetFFTMagnitude(const std::vector<T>& input, std::vector<double>& output, const char*& errorMessage)
		{
			std::vector<complex_type> result(input.size());

			if (!simple_fft::FFT(input, result, input.size(), errorMessage))
			{
				return false;
			}

			std::vector<double> magnitude(result.size() / 2);

			for (size_t i = 0; i < magnitude.size(); i++)
			{
				magnitude[i] = std::abs(result[i]) / magnitude.size();
			}

			output = magnitude;

			return true;
		}

		template<class T, class TBuffer>
		static bool GetFFTMagnitudeWithBuffer(const std::vector<T>& input, TBuffer& output, const char*& errorMessage)
		{
			std::vector<complex_type> result(input.size());

			if (!simple_fft::FFT(input, result, input.size(), errorMessage))
			{
				return false;
			}

			size_t size = input.size() / 2;

			for (size_t i = 0; i < size; i++)
			{
				output[i] = std::abs(result[i]) / size;
			}

			return true;
		}
	};
}