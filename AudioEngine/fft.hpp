/**
 * Copyright (c) 2013-2020 Dmitry Ivanov
 *
 * This file is a part of Simple-FFT project and is distributed under the terms
 * of MIT license: https://opensource.org/licenses/MIT
 */

#ifndef __SIMPLE_FFT__FFT_HPP__
#define __SIMPLE_FFT__FFT_HPP__

#include <complex>
#include <cstddef>
#include <math.h>
#include <vector>

using std::size_t;
typedef double real_type;
typedef std::complex<real_type> complex_type;

constexpr double M_PI = 3.1415926535897932;

namespace simple_fft 
{
	namespace error_handling
	{
		enum class EC_SimpleFFT
		{
			SUCCESS = 0,
			UNSUPPORTED_DIMENSIONALITY,
			WRONG_FFT_DIRECTION,
			ONE_OF_DIMS_ISNT_POWER_OF_TWO,
			NUM_OF_ELEMS_IS_ZERO,
			WRONG_CHECK_FFT_MODE,
			RELATIVE_ERROR_TOO_LARGE
		};

		inline void GetErrorDescription(const EC_SimpleFFT error_code, const char*& error_description)
		{
			switch (error_code)
			{
			case EC_SimpleFFT::SUCCESS:
				error_description = "Calculation was successful!";
				break;
			case EC_SimpleFFT::UNSUPPORTED_DIMENSIONALITY:
				error_description = "Unsupported dimensionality: currently only 1D, 2D and 3D arrays are supported";
				break;
			case EC_SimpleFFT::WRONG_FFT_DIRECTION:
				error_description = "Wrong direction for FFT was specified";
				break;
			case EC_SimpleFFT::ONE_OF_DIMS_ISNT_POWER_OF_TWO:
				error_description = "Unsupported dimensionality: one of dimensions is not a power of 2";
				break;
			case EC_SimpleFFT::NUM_OF_ELEMS_IS_ZERO:
				error_description = "Number of elements for FFT or IFFT is zero!";
				break;
			case EC_SimpleFFT::WRONG_CHECK_FFT_MODE:
				error_description = "Wrong check FFT mode was specified (should be either Parseval theorem or energy conservation check";
				break;
			case EC_SimpleFFT::RELATIVE_ERROR_TOO_LARGE:
				error_description = "Relative error returned by FFT test exceeds specified relative tolerance";
				break;
			default:
				error_description = "Unknown error";
				break;
			}
		}

	} // namespace error_handling

	namespace copy_array
	{
		template <class TComplexArray1D>
		void copyArray(const TComplexArray1D& data_from, TComplexArray1D& data_to, const size_t size)
		{
			for (int i = 0; i < size; ++i)
			{
				data_to[i] = data_from[i];
			}
		}

		template <class TComplexArray1D, class TRealArray1D>
		void copyArray(const TRealArray1D& data_from, TComplexArray1D& data_to, const size_t size)
		{
			// NOTE: user's complex type should have constructor like
			// "complex(real, imag)", where each of real and imag has
			// real type.

			for (int i = 0; i < size; ++i)
			{
				data_to[i] = complex_type(data_from[i], 0.0);
			}
		}

		template <class TComplexArray2D>
		void copyArray(const TComplexArray2D& data_from, TComplexArray2D& data_to, const size_t size1, const size_t size2)
		{
			for (int i = 0; i < size1; ++i)
			{
				for (int j = 0; j < size2; ++j)
				{
					data_to[i][j] = data_from[i][j];
				}
			}
		}

		template <class TComplexArray2D, class TRealArray2D>
		void copyArray(const TRealArray2D& data_from, TComplexArray2D& data_to, const size_t size1, const size_t size2)
		{
			// NOTE: user's complex type should have constructor like
			// "complex(real, imag)", where each of real and imag has
			// real type.

			for (int i = 0; i < size1; ++i)
			{
				for (int j = 0; j < size2; ++j)
				{
					data_to[i][j] = complex_type(data_from[i][j], 0.0);
				}
			}
		}

		template <class TComplexArray3D>
		void copyArray(const TComplexArray3D& data_from, TComplexArray3D& data_to, const size_t size1, const size_t size2, const size_t size3)
		{
			for (int i = 0; i < size1; ++i)
			{
				for (int j = 0; j < size2; ++j)
				{
					for (int k = 0; k < size3; ++k)
					{
						data_to[i][j][k] = data_from[i][j][k];
					}
				}
			}
		}

		template <class TComplexArray3D, class TRealArray3D>
		void copyArray(const TRealArray3D& data_from, TComplexArray3D& data_to, const size_t size1, const size_t size2, const size_t size3)
		{
			// NOTE: user's complex type should have constructor like
			// "complex(real, imag)", where each of real and imag has
			// real type.

			for (int i = 0; i < size1; ++i)
			{
				for (int j = 0; j < size2; ++j)
				{
					for (int k = 0; k < size3; ++k)
					{
						data_to[i][j][k] = complex_type(data_from[i][j][k], 0.0);
					}
				}
			}
		}

	}

	namespace impl
	{
		enum class FFT_direction
		{
			FFT_FORWARD = 0,
			FFT_BACKWARD
		};

		// checking whether the size of array dimension is power of 2
		// via "complement and compare" method
		inline bool isPowerOfTwo(const size_t num)
		{
			return num && (!(num & (num - 1)));
		}

		inline bool checkNumElements(const size_t num_elements, const char*& error_description)
		{
			if (!isPowerOfTwo(num_elements)) {
				simple_fft::error_handling::GetErrorDescription(simple_fft::error_handling::EC_SimpleFFT::ONE_OF_DIMS_ISNT_POWER_OF_TWO, error_description);
				return false;
			}

			return true;
		}

		template <class TComplexArray1D>
		inline void scaleValues(TComplexArray1D& data, const size_t num_elements)
		{
			real_type mult = 1.0 / num_elements;

			for (int i = 0; i < num_elements; ++i)
			{
				data[i] *= mult;
			}
		}

		// NOTE: explicit template specialization for the case of std::vector<complex_type>
		// because it is used in 2D and 3D FFT for both array classes with square and round
		// brackets of element access operator; I need to guarantee that sub-FFT 1D will
		// use square brackets for element access operator anyway. It is pretty ugly
		// to duplicate the code but I haven't found more elegant solution.
		template <>
		inline void scaleValues<std::vector<complex_type> >(std::vector<complex_type>& data,
			const size_t num_elements)
		{
			real_type mult = 1.0 / num_elements;

			for (int i = 0; i < num_elements; ++i)
			{
				data[i] *= mult;
			}
		}

		template <class TComplexArray1D>
		inline void bufferExchangeHelper(TComplexArray1D& data, const size_t index_from,
			const size_t index_to, complex_type& buf)
		{
			buf = data[index_from];
			data[index_from] = data[index_to];
			data[index_to] = buf;
		}

		// NOTE: explicit template specialization for the case of std::vector<complex_type>
		// because it is used in 2D and 3D FFT for both array classes with square and round
		// brackets of element access operator; I need to guarantee that sub-FFT 1D will
		// use square brackets for element access operator anyway. It is pretty ugly
		// to duplicate the code but I haven't found more elegant solution.
		template <>
		inline void bufferExchangeHelper<std::vector<complex_type> >(std::vector<complex_type>& data,
			const size_t index_from,
			const size_t index_to,
			complex_type& buf)
		{
			buf = data[index_from];
			data[index_from] = data[index_to];
			data[index_to] = buf;
		}

		template <class TComplexArray1D>
		inline void rearrangeData(TComplexArray1D& data, const size_t num_elements)
		{
			complex_type buf;

			size_t target_index = 0;
			size_t bit_mask;

			for (size_t i = 0; i < num_elements; ++i)
			{
				if (target_index > i)
				{
					bufferExchangeHelper(data, target_index, i, buf);
				}

				// Initialize the bit mask
				bit_mask = num_elements;

				// While bit is 1
				while (target_index & (bit_mask >>= 1)) // bit_mask = bit_mask >> 1
				{
					// Drop bit:
					// & is bitwise AND,
					// ~ is bitwise NOT
					target_index &= ~bit_mask; // target_index = target_index & (~bit_mask)
				}

				// | is bitwise OR
				target_index |= bit_mask; // target_index = target_index | bit_mask
			}
		}

		template <class TComplexArray1D>
		inline void fftTransformHelper(TComplexArray1D& data, const size_t match,
			const size_t k, complex_type& product,
			const complex_type factor)
		{
			product = data[match] * factor;
			data[match] = data[k] - product;
			data[k] += product;
		}

		// NOTE: explicit template specialization for the case of std::vector<complex_type>
		// because it is used in 2D and 3D FFT for both array classes with square and round
		// brackets of element access operator; I need to guarantee that sub-FFT 1D will
		// use square brackets for element access operator anyway. It is pretty ugly
		// to duplicate the code but I haven't found more elegant solution.
		template <>
		inline void fftTransformHelper<std::vector<complex_type> >(std::vector<complex_type>& data,
			const size_t match,
			const size_t k,
			complex_type& product,
			const complex_type factor)
		{
			product = data[match] * factor;
			data[match] = data[k] - product;
			data[k] += product;
		}

		template <class TComplexArray1D>
		bool makeTransform(TComplexArray1D& data, const size_t num_elements,
			const FFT_direction fft_direction, const char*& error_description)
		{
			using namespace error_handling;
			using std::sin;

			double local_pi;
			switch (fft_direction)
			{
			case(FFT_direction::FFT_FORWARD):
				local_pi = -M_PI;
				break;
			case(FFT_direction::FFT_BACKWARD):
				local_pi = M_PI;
				break;
			default:
				GetErrorDescription(EC_SimpleFFT::WRONG_FFT_DIRECTION, error_description);
				return false;
			}

			// declare variables to cycle the bits of initial signal
			size_t next, match;
			real_type sine;
			real_type delta;
			complex_type mult, factor, product;

			// NOTE: user's complex type should have constructor like
			// "complex(real, imag)", where each of real and imag has
			// real type.

			// cycle for all bit positions of initial signal
			for (size_t i = 1; i < num_elements; i <<= 1)
			{
				next = i << 1;  // getting the next bit
				delta = local_pi / i;    // angle increasing
				sine = sin(0.5 * delta);    // supplementary sin
				// multiplier for trigonometric recurrence
				mult = complex_type(-2.0 * sine * sine, sin(delta));
				factor = 1.0;   // start transform factor

				for (size_t j = 0; j < i; ++j) // iterations through groups
					// with different transform factors
				{
					for (size_t k = j; k < num_elements; k += next) // iterations through
						// pairs within group
					{
						match = k + i;
						fftTransformHelper(data, match, k, product, factor);
					}
					factor = mult * factor + factor;
				}
			}

			return true;
		}

		// Generic template for complex FFT followed by its explicit specializations
		template <class TComplexArray, int NumDims>
		struct CFFT
		{

		};

		// 1D FFT:
		template <class TComplexArray1D>
		struct CFFT<TComplexArray1D, 1>
		{
			// NOTE: passing by pointer is needed to avoid using element access operator
			static bool FFT_inplace(TComplexArray1D& data, const size_t size,
				const FFT_direction fft_direction,
				const char*& error_description)
			{
				if (!checkNumElements(size, error_description)) {
					return false;
				}

				rearrangeData(data, size);

				if (!makeTransform(data, size, fft_direction, error_description)) {
					return false;
				}

				if (FFT_direction::FFT_BACKWARD == fft_direction) {
					scaleValues(data, size);
				}

				return true;
			}
		};

		// 2D FFT
		template <class TComplexArray2D>
		struct CFFT<TComplexArray2D, 2>
		{
			static bool FFT_inplace(TComplexArray2D& data, const size_t size1, const size_t size2,
				const FFT_direction fft_direction, const char*& error_description)
			{
				int n_rows = static_cast<int>(size1);
				int n_cols = static_cast<int>(size2);

				// fft for columns
				std::vector<complex_type> subarray(n_rows); // each column has n_rows elements

				for (int j = 0; j < n_cols; ++j)
				{
					for (int i = 0; i < n_rows; ++i)
					{
						subarray[i] = data[i][j];
					}

					if (!CFFT<std::vector<complex_type>, 1>::FFT_inplace(subarray, size1,
						fft_direction,
						error_description))
					{
						return false;
					}

					for (int i = 0; i < n_rows; ++i)
					{
						data[i][j] = subarray[i];
					}
				}

				// fft for rows
				subarray.resize(n_cols); // each row has n_cols elements

				for (int i = 0; i < n_rows; ++i)
				{
					for (int j = 0; j < n_cols; ++j)
					{
						subarray[j] = data[i][j];
					}

					if (!CFFT<std::vector<complex_type>, 1>::FFT_inplace(subarray, size2,
						fft_direction,
						error_description))
					{
						return false;
					}

					for (int j = 0; j < n_cols; ++j)
					{
						data[i][j] = subarray[j];
					}
				}

				return true;
			}
		};

		// 3D FFT
		template <class TComplexArray3D>
		struct CFFT<TComplexArray3D, 3>
		{
			static bool FFT_inplace(TComplexArray3D& data, const size_t size1, const size_t size2,
				const size_t size3, const FFT_direction fft_direction,
				const char*& error_description)
			{
				int n_rows = static_cast<int>(size1);
				int n_cols = static_cast<int>(size2);
				int n_depth = static_cast<int>(size3);

				std::vector<complex_type> subarray(n_rows); // for fft for columns: each column has n_rows elements

				for (int k = 0; k < n_depth; ++k) // for all depth layers
				{
					// fft for columns
					for (int j = 0; j < n_cols; ++j)
					{
						for (int i = 0; i < n_rows; ++i)
						{
							subarray[i] = data[i][j][k];
						}

						if (!CFFT<std::vector<complex_type>, 1>::FFT_inplace(subarray, size1,
							fft_direction,
							error_description))
						{
							return false;
						}

						for (int i = 0; i < n_rows; ++i) {
							data[i][j][k] = subarray[i];
						}
					}
				}

				subarray.resize(n_cols); // for fft for rows: each row has n_cols elements

				for (int k = 0; k < n_depth; ++k) // for all depth layers
				{
					// fft for rows
					for (int i = 0; i < n_rows; ++i)
					{
						for (int j = 0; j < n_cols; ++j) {
							subarray[j] = data[i][j][k];
						}

						if (!CFFT<std::vector<complex_type>, 1>::FFT_inplace(subarray, size2,
							fft_direction,
							error_description))
						{
							return false;
						}

						for (int j = 0; j < n_cols; ++j)
						{
							data[i][j][k] = subarray[j];
						}
					}
				}

				// fft for depth
				subarray.resize(n_depth); // each depth strip contains n_depth elements

				for (int i = 0; i < n_rows; ++i) // for all rows layers
				{
					for (int j = 0; j < n_cols; ++j) // for all cols layers
					{
						for (int k = 0; k < n_depth; ++k)
						{
							subarray[k] = data[i][j][k];
						}

						if (!CFFT<std::vector<complex_type>, 1>::FFT_inplace(subarray, size3,
							fft_direction,
							error_description))
						{
							return false;
						}

						for (int k = 0; k < n_depth; ++k)
						{
							data[i][j][k] = subarray[k];
						}
					}
				}

				return true;
			}
		};

	} // namespace impl

	// in-place, complex, forward
	template <class TComplexArray1D>
	bool FFT(TComplexArray1D& data, const size_t size, const char*& error_description)
	{
		return impl::CFFT<TComplexArray1D, 1>::FFT_inplace(data, size, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	template <class TComplexArray2D>
	bool FFT(TComplexArray2D& data, const size_t size1, const size_t size2, const char*& error_description)
	{
		return impl::CFFT<TComplexArray2D, 2>::FFT_inplace(data, size1, size2, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	template <class TComplexArray3D>
	bool FFT(TComplexArray3D& data, const size_t size1, const size_t size2, const size_t size3, const char*& error_description)
	{
		return impl::CFFT<TComplexArray3D, 3>::FFT_inplace(data, size1, size2, size3, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	// in-place, complex, inverse
	template <class TComplexArray1D>
	bool IFFT(TComplexArray1D& data, const size_t size, const char*& error_description)
	{
		return impl::CFFT<TComplexArray1D, 1>::FFT_inplace(data, size, impl::FFT_direction::FFT_BACKWARD, error_description);
	}

	template <class TComplexArray2D>
	bool IFFT(TComplexArray2D& data, const size_t size1, const size_t size2, const char*& error_description)
	{
		return impl::CFFT<TComplexArray2D, 2>::FFT_inplace(data, size1, size2, impl::FFT_direction::FFT_BACKWARD, error_description);
	}

	template <class TComplexArray3D>
	bool IFFT(TComplexArray3D& data, const size_t size1, const size_t size2, const size_t size3, const char*& error_description)
	{
		return impl::CFFT<TComplexArray3D, 3>::FFT_inplace(data, size1, size2, size3, impl::FFT_direction::FFT_BACKWARD, error_description);
	}

	// not-in-place, complex, forward
	template <class TComplexArray1D>
	bool FFT(const TComplexArray1D& data_in, TComplexArray1D& data_out, const size_t size, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size);
		return impl::CFFT<TComplexArray1D, 1>::FFT_inplace(data_out, size, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	template <class TComplexArray2D>
	bool FFT(const TComplexArray2D& data_in, TComplexArray2D& data_out, const size_t size1, const size_t size2, const char*& error_description) 
	{
		copy_array::copyArray(data_in, data_out, size1, size2);
		return impl::CFFT<TComplexArray2D, 2>::FFT_inplace(data_out, size1, size2, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	template <class TComplexArray3D>
	bool FFT(const TComplexArray3D& data_in, TComplexArray3D& data_out, const size_t size1, const size_t size2, const size_t size3, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size1, size2, size3);
		return impl::CFFT<TComplexArray3D, 3>::FFT_inplace(data_out, size1, size2, size3, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	// not-in-place, complex, inverse
	template <class TComplexArray1D>
	bool IFFT(const TComplexArray1D& data_in, TComplexArray1D& data_out, const size_t size, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size);
		return impl::CFFT<TComplexArray1D, 1>::FFT_inplace(data_out, size, impl::FFT_direction::FFT_BACKWARD, error_description);
	}

	template <class TComplexArray2D>
	bool IFFT(const TComplexArray2D& data_in, TComplexArray2D& data_out, const size_t size1, const size_t size2, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size1, size2);
		return impl::CFFT<TComplexArray2D, 2>::FFT_inplace(data_out, size1, size2, impl::FFT_direction::FFT_BACKWARD, error_description);
	}

	template <class TComplexArray3D>
	bool IFFT(const TComplexArray3D& data_in, TComplexArray3D& data_out, const size_t size1, const size_t size2, const size_t size3, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size1, size2, size3);
		return impl::CFFT<TComplexArray3D, 3>::FFT_inplace(data_out, size1, size2, size3, impl::FFT_direction::FFT_BACKWARD, error_description);
	}

	// not-in-place, real, forward
	template <class TRealArray1D, class TComplexArray1D>
	bool FFT(const TRealArray1D& data_in, TComplexArray1D& data_out, const size_t size, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size);
		return impl::CFFT<TComplexArray1D, 1>::FFT_inplace(data_out, size, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	template <class TRealArray2D, class TComplexArray2D>
	bool FFT(const TRealArray2D& data_in, TComplexArray2D& data_out, const size_t size1, const size_t size2, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size1, size2);
		return impl::CFFT<TComplexArray2D, 2>::FFT_inplace(data_out, size1, size2, impl::FFT_direction::FFT_FORWARD, error_description);
	}

	template <class TRealArray3D, class TComplexArray3D>
	bool FFT(const TRealArray3D& data_in, TComplexArray3D& data_out, const size_t size1, const size_t size2, const size_t size3, const char*& error_description)
	{
		copy_array::copyArray(data_in, data_out, size1, size2, size3);
		return impl::CFFT<TComplexArray3D, 3>::FFT_inplace(data_out, size1, size2, size3, impl::FFT_direction::FFT_FORWARD, error_description);
	}
}

#endif // __SIMPLE_FFT__FFT_HPP__
