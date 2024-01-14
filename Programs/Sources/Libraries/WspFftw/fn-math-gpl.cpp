// @file fn-math.cpp
// @author Jun-ichi Nishikata

#define _USE_MATH_DEFINES
#include <math.h>

#include <wsp/math/_tpl_fn-math.h>
#include <wsp/math/_fn-math.h>

#include <fftw3.h>
#include "fn-math-gpl.h"
#include <wsp/common/fn-debug.h>
#include <stdexcept>

// ------------------------------------------------------------------
// Fourier Transform
// ------------------------------------------------------------------
namespace{
    void _FftwApplyFft1dImpl(
        double* io_real, double* io_imaginary, s32 sample_size, s32 sign)
    {
        fftw_plan fft_plan;
        fftw_complex* in_complex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* sample_size);
        WSP_ASSERT_NOT_NULL(in_complex);

        fftw_complex* out_complex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* sample_size);
        WSP_ASSERT_NOT_NULL(out_complex);

        // setup FFTW plans
        fft_plan = fftw_plan_dft_1d(
            sample_size, in_complex, out_complex, sign, FFTW_ESTIMATE);

        // copy source data to fftw_complex array
        {
            const double* real_ptr = io_real;
            const double* imag_ptr = io_imaginary;
            const double* end_ptr = real_ptr + sample_size;
            double* complex_ptr = reinterpret_cast<double*>(in_complex);
            for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                *complex_ptr = *real_ptr;
                ++complex_ptr;
                *complex_ptr = *imag_ptr;
                ++complex_ptr;
            }
        }

        // obtain the FFT of data
        fftw_execute(fft_plan);

        // copy processed data to source array
        {
            double* real_ptr = io_real;
            double* imag_ptr = io_imaginary;
            double* end_ptr = real_ptr + sample_size;
            const double* complex_ptr = reinterpret_cast<double*>(out_complex);
            for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                *real_ptr = *complex_ptr;
                ++complex_ptr;
                *imag_ptr = *complex_ptr;
                ++complex_ptr;
            }
        }

        if (sign == FFTW_FORWARD)
        {
            double mult = 1.0 / sample_size;
            double* real_ptr = io_real;
            double* imag_ptr = io_imaginary;
            double* end_ptr = real_ptr + sample_size;
            for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                *real_ptr *= mult;
                *imag_ptr *= mult;
            }
        }

        // free memories
        fftw_destroy_plan(fft_plan);

        fftw_free(in_complex);
        fftw_free(out_complex);
    }

    void _FftwApplyFft2dImpl(
        double* io_real, double* io_imaginary, s32 width, s32 height, s32 sign)
    {
        s32 sample_size = width * height;
        fftw_plan fft_plan;
        fftw_complex* in_complex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* sample_size);
        WSP_ASSERT_NOT_NULL(in_complex);

        fftw_complex* out_complex = (fftw_complex*)fftw_malloc(sizeof(fftw_complex)* sample_size);
        WSP_ASSERT_NOT_NULL(out_complex);

        // setup FFTW plans
        fft_plan = fftw_plan_dft_2d(
            height, width, in_complex, out_complex, sign, FFTW_ESTIMATE);

        // copy source data to fftw_complex array
        {
            const double* real_ptr = io_real;
            const double* imag_ptr = io_imaginary;
            const double* end_ptr = real_ptr + sample_size;
            double* complex_ptr = reinterpret_cast<double*>(in_complex);
            for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                *complex_ptr = *real_ptr;
                ++complex_ptr;
                *complex_ptr = *imag_ptr;
                ++complex_ptr;
            }
        }

        // obtain the FFT of data
        fftw_execute(fft_plan);

        // copy processed data to source array
        {
            double* real_ptr = io_real;
            double* imag_ptr = io_imaginary;
            double* end_ptr = real_ptr + sample_size;
            const double* complex_ptr = reinterpret_cast<double*>(out_complex);
            for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                *real_ptr = *complex_ptr;
                ++complex_ptr;
                *imag_ptr = *complex_ptr;
                ++complex_ptr;
            }
        }

        if (sign == FFTW_FORWARD)
        {
            double mult = 1.0 / sample_size;
            double* real_ptr = io_real;
            double* imag_ptr = io_imaginary;
            double* end_ptr = real_ptr + sample_size;
            for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                *real_ptr *= mult;
                *imag_ptr *= mult;
            }
        }

        // free memories
        fftw_destroy_plan(fft_plan);

        fftw_free(in_complex);
        fftw_free(out_complex);
    }
}

void wsp::fftw::FftwApplyFft1d(
    double* io_real, double* io_imaginary, s32 sample_size)
{
    WSP_REQUIRES_IF_THEN_THROW(
        sample_size >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)sample_size);

    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(sample_size),
        std::invalid_argument,
        "input size is not power of two, size = %d", (s32)sample_size);

    _FftwApplyFft1dImpl(io_real, io_imaginary, sample_size, FFTW_FORWARD);
}

WSP_DLL_EXPORT void wsp::fftw::FftwApplyInverseFft1d(
    double* io_real, double* io_imaginary, s32 sample_size)
{
    WSP_REQUIRES_IF_THEN_THROW(
        sample_size >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)sample_size);

    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(sample_size),
        std::invalid_argument,
        "input size is not power of two, size = %d", (s32)sample_size);

    _FftwApplyFft1dImpl(io_real, io_imaginary, sample_size, FFTW_BACKWARD);
}

void wsp::fftw::FftwApplyFft2d(
    double* io_real, double* io_imaginary, s32 width, s32 height)
{
    WSP_REQUIRES_IF_THEN_THROW(
        width >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)width);

    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(width),
        std::invalid_argument,
        "input size is not power of two, size = %d", (s32)width);

    WSP_REQUIRES_IF_THEN_THROW(
        height >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)height);

    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(height),
        std::invalid_argument,
        "input size is not power of two, size = %d", (s32)height);

    _FftwApplyFft2dImpl(io_real, io_imaginary, width, height, FFTW_FORWARD);
}


void wsp::fftw::FftwApplyInverseFft2d(
    double* io_real, double* io_imaginary, s32 width, s32 height)
{
    WSP_REQUIRES_IF_THEN_THROW(
        width >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)width);

    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(width),
        std::invalid_argument,
        "input size is not power of two, size = %d", (s32)width);

    WSP_REQUIRES_IF_THEN_THROW(
        height >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)height);

    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(height),
        std::invalid_argument,
        "input size is not power of two, size = %d", (s32)height);

    _FftwApplyFft2dImpl(io_real, io_imaginary, width, height, FFTW_BACKWARD);
}

