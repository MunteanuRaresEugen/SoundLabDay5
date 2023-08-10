#pragma once

#include <vector>
#include "ImportExport.h"

namespace AudioEngine
{
	enum class AUDIOENGINE_API SignalType
	{
		Sin,
		Dirac,
		Square,
		Sum,
		SquareWithSums,
		SumFiltered
	};

	class AUDIOENGINE_API SignalGenerator
	{
	public:
		SignalGenerator(double time, size_t sampleRate, double freq);

		const std::vector<double>& CreateSignal(const SignalType type);

		const std::vector<double>& GetLastBuffer() const noexcept;

		size_t GetSampleRate() const noexcept;

	private:
		const std::vector<double>& CreateSin();
		const std::vector<double>& CreateDirac();
		const std::vector<double>& CreateSquare();
		const std::vector<double>& CreateSum();
		const std::vector<double>& CreateSquareWithSums();
		const std::vector<double>& CreateSumFiltered();

		const size_t m_size;
		const size_t m_sampleRate;
		const double m_freq;

		std::vector<double> m_buffer;
	};
}
