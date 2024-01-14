/**
* @file _tpl_fn-fourier_transform.inl
* 
*/

#ifndef WSP_MATH_TEMPLATE_FUNCTION_FOURIER_TRANSFORM_INL__
#define WSP_MATH_TEMPLATE_FUNCTION_FOURIER_TRANSFORM_INL__

#include "_tpl_fn-fourier_transform.hpp"

#include "_tpl_fn-math.h"
#include "_define_mathutil.h"
#include <wsp/common/fn-debug.h>

#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <stdexcept>

#define WSP_USE_OPTIMIZED_FFT

namespace {
    template<typename _Type>
    void _ApplyDft1dImpl(
        _Type *dest_r,
        _Type *dest_i,
        const _Type *src_r,
        const _Type *src_i,
        int sample_size,
        bool inverse)
    {
        WSP_REQUIRES(sample_size >= 2, "sample_size is less than 2\n");

        _Type sum_re, sum_im, pi, arg, freqSample;
        int sample;
        int freq;

        // initializing a destination array
        memset(dest_r, 0, sizeof(_Type)* sample_size);
        memset(dest_i, 0, sizeof(_Type)* sample_size);

        pi = (inverse == false) ? M_PI : -M_PI;

        // main loop
        for (freq = 0; freq < sample_size; ++freq)
        {
            sum_re = 0.0;
            sum_im = 0.0;
            arg = ((double)freq / (double)sample_size) * (2 * pi);
            for (sample = 0; sample<sample_size; ++sample)
            {
                freqSample = sample * arg;

                sum_re += src_r[sample] * cos(freqSample) + src_i[sample] * sin(freqSample);
                sum_im += src_i[sample] * cos(freqSample) - src_r[sample] * sin(freqSample);

                // Explanation for equation ----------------------------------
                //  image = REAL(image) + IMAGINARY(image);
                //  We define that an image has imaginary number, and it's 0;
                //  First, we get correlation between one sin function and a x line of the image.
                //  It will be spectrum.
                //
                // Pseudo-code
                //  F(freq) = sum(REAL(image, point) * cos(freq, point) - i * REAL(image, point) * sin(freq, point))
                //          + i * sum( IMAGINARY(image, point) * sin(freq, point)) - i * IMAGINARY(image, point) * sin(freq, point) );
                //          = sum(REAL(image, point) * cos(freq, point)) - i * sum(REAL(image, point) * sin(freq, point))
                //          + i * sum( IMAGINARY(image, point) * sin(freq, point)) + sum(IMAGINARY(image, point) * sin(freq, point));
                //  sum_re = REAL(srcData, iter) * cos( samplingPoint ) 
                //        + IMAGINARY(srcData, iter) * sin( samplingPoint );
                //  sum_im = IMAGINARY(srcData, iter) * cos( samplingPoint )
                //        - REAL(srcData, iter) * sin( samplingPoint );
                //  We have defined imaginary number of this image is 0,
                //  so IMAGINARY(...) is 0;
                //  sum_re = REAL(srcData, iter) * cos( samplingPoint );
                //  sum_im = - REAL(srcData, iter) * sin( samplingPoint );
                //------------------------------------------------------------

            }

            if (inverse)
            {
                dest_r[freq] = sum_re;
                dest_i[freq] = sum_im;
            }
            else
            {
                dest_r[freq] = sum_re / (double)sample_size;
                dest_i[freq] = sum_im / (double)sample_size;
            }
        }
    }


    /*-------------------------------------------------------------------------
       This computes an in-place complex-to-complex FFT
       io_real and io_imag are the real and imaginary arrays of 2^m points.
       ifft = false gives forward transform
       ifft = true gives reverse transform

         Formula: forward
                      N-1
                      ---
                  1   \          - j k 2 pi n / N
          X(n) = ---   >   x(k) e                    = forward transform
                  N   /                                n=0..N-1
                      ---
                      k=0

          Formula: reverse
                      N-1
                      ---
                      \          j k 2 pi n / N
          X(n) =       >   x(k) e                    = forward transform
                      /                                n=0..N-1
                      ---
                      k=0
    */
    template<typename _Type>
    void _ApplyFft1dImpl(
        _Type *io_real,
        _Type *io_imag,
        s32 sample_size,
        s32 exponent_of_size,
        bool ifft
        )
    {
        WSP_REQUIRES(sample_size >= 2, "sample_size is less than 2\n");
        WSP_REQUIRES(wsp::math::IsPowerOfTwo(sample_size), "sample_size is not power of two");
        WSP_REQUIRES(wsp::math::GetExponentOfNearestPowerOfTwo(sample_size) == exponent_of_size, "exponent_of_size is not equal to exponent of size");

        s32 i, i1, j, k, i2, l, l1, l2;
        s32 len;
        _Type c1, c2, tx, ty, t1, t2, u1, u2, z;

        // Do the bit reversal
        i2 = sample_size >> 1;
        j = 0;
        len = sample_size - 1;
        for (i = 0; i < len; ++i)
        {
            if (i < j)
            {
                tx = io_real[i];
                ty = io_imag[i];
                io_real[i] = io_real[j];
                io_imag[i] = io_imag[j];
                io_real[j] = tx;
                io_imag[j] = ty;
            }

            k = i2;
            while (k <= j)
            {
                j -= k;
                k >>= 1;
            }

            j += k;
        }

        // Compute the FFT
        c1 = -1.0;
        c2 = 0.0;
        l2 = 1;
        for (l = 0; l < exponent_of_size; ++l)
        {
            l1 = l2;
            l2 <<= 1;
            u1 = 1.0;
            u2 = 0.0;
            for (j = 0; j<l1; ++j)
            {
                for (i = j; i<sample_size; i += l2)
                {
                    i1 = i + l1;
                    t1 = u1 * io_real[i1] - u2 * io_imag[i1];
                    t2 = u1 * io_imag[i1] + u2 * io_real[i1];
                    io_real[i1] = io_real[i] - t1;
                    io_imag[i1] = io_imag[i] - t2;
                    io_real[i] += t1;
                    io_imag[i] += t2;
                }

                z = u1 * c1 - u2 * c2;
                u2 = u1 * c2 + u2 * c1;
                u1 = z;
            }

            c2 = sqrt((1.0 - c1) / 2.0);
            if (ifft == false)
            {
                c2 = -c2;
            }

            c1 = sqrt((1.0 + c1) / 2.0);
        }

        // Scaling for forward transform
        if (ifft == false)
        {
            for (i = 0; i < sample_size; i++)
            {
                io_real[i] /= sample_size;
                io_imag[i] /= sample_size;
            }
        }
    }

    template<typename _Type>
    void _ApplyFft1dImpl_Opt(
        _Type *io_real, _Type *io_imag, s32 sample_size, s32 exponent_of_size, bool ifft)
    {
        WSP_REQUIRES(sample_size >= 2, "sample_size is less than 2\n");
        WSP_REQUIRES(wsp::math::IsPowerOfTwo(sample_size), "sample_size is not power of two");
        WSP_REQUIRES(wsp::math::GetExponentOfNearestPowerOfTwo(sample_size) == exponent_of_size, "exponent_of_size is not equal to exponent of size");

        s32 i2;
        _Type c1, c2, tx, ty, t1, t2, u1, u2, z;
        _Type *real_ptr, *imag_ptr, *end_ptr;
        _Type *real_ptr_2, *imag_ptr_2, *end_ptr_2, *real_ptr_i1, *imag_ptr_i1;
        _Type *real_ptr_j, *imag_ptr_j;
        s32 j, k;
        s32 l, l1, l2;

        // Do the bit reversal
        i2 = sample_size >> 1;
        {
            real_ptr = io_real;
            imag_ptr = io_imag;
            end_ptr = io_real + sample_size - 1;
            for (j = 0; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                real_ptr_j = io_real + j;
                if (real_ptr < real_ptr_j)
                {
                    imag_ptr_j = io_imag + j;
                    // swap complex[i] and complex[j]
                    tx = *real_ptr;
                    ty = *imag_ptr;
                    *real_ptr = *real_ptr_j;
                    *imag_ptr = *imag_ptr_j;
                    *real_ptr_j = tx;
                    *imag_ptr_j = ty;
                }

                for (k = i2; k <= j; k >>= 1){ j -= k; }
                j += k;
            }
        }

        // Compute FFT
        {
            c1 = -1.0;
            c2 = 0.0;
            l2 = 1;
            for (l = 0; l<exponent_of_size; ++l)
            {
                l1 = l2;
                l2 <<= 1;
                u1 = 1.0;
                u2 = 0.0;
                real_ptr = io_real;
                imag_ptr = io_imag;
                end_ptr = io_real + l1;
                end_ptr_2 = io_real + sample_size;
                for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
                {
                    real_ptr_2 = real_ptr;
                    imag_ptr_2 = imag_ptr;
                    for (; real_ptr_2<end_ptr_2; real_ptr_2 += l2, imag_ptr_2 += l2)
                    {
                        real_ptr_i1 = real_ptr_2 + l1;
                        imag_ptr_i1 = imag_ptr_2 + l1;
                        t1 = u1 * (*real_ptr_i1) - u2 * (*imag_ptr_i1);
                        t2 = u1 * (*imag_ptr_i1) + u2 * (*real_ptr_i1);
                        *real_ptr_i1 = *real_ptr_2 - t1;
                        *imag_ptr_i1 = *imag_ptr_2 - t2;
                        *real_ptr_2 += t1;
                        *imag_ptr_2 += t2;
                    }
                    z = u1 * c1 - u2 * c2;
                    u2 = u1 * c2 + u2 * c1;
                    u1 = z;
                }

                c2 = static_cast<_Type>(sqrt((1.0 - c1) / 2.0));
                if (ifft == false){ c2 = -c2; }
                c1 = static_cast<_Type>(sqrt((1.0 + c1) / 2.0));
            }
        }

        // Scaling sample_size for forward transoform
        if (ifft == false)
        {
            double mult = 1.0 / (double)sample_size;
            real_ptr = io_real;
            imag_ptr = io_imag;
            end_ptr = real_ptr + sample_size;
            for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr)
            {
                *real_ptr = (_Type)(*real_ptr * mult);
                *imag_ptr = (_Type)(*imag_ptr * mult);
            }
        }
    }

#define USE_OPTIMIZED_TransposeComplex
    template<typename _Type>
    void TransposeComplex(
        _Type* o_real, _Type* o_imag, 
        const _Type* in_real, const _Type* in_imag, 
        s32 width, s32 height)
    {
#ifdef USE_OPTIMIZED_TransposeComplex
        s32 len = width * height;
        s32 back_pix = len - 1;
        const _Type* src_r = in_real;
        const _Type* src_i = in_imag;
        const _Type* end_src = src_r + len - 1;
        _Type* dst_r = o_real;
        _Type* dst_i = o_imag;
        _Type* end_dst = dst_r + len;
        for (; dst_r != end_dst; ++dst_r, ++dst_i, src_r += width, src_i += width)
        {
            if (src_r > end_src)
            {
                src_r -= back_pix;
                src_i -= back_pix;
            }

            *dst_r = *src_r;
            *dst_i = *src_i;
        }
#else
        for (s32 x = 0; x < width; ++x)
        {
            for (s32 y = 0; y < height; ++y)
            {
                s32 dest_index = x * height + y;
                s32 src_index = y * width + x;
                o_real[dest_index] = in_real[src_index];
                o_imag[dest_index] = in_imag[src_index];
            }
        }
#endif
    }

    template<typename _Type>
    void _ApplyDft2dImpl(
        _Type *io_real, _Type *io_imag,
        s32 width, s32 height, bool inverse,
        void(*window_function)(_Type*, s32))
    {
        WSP_REQUIRES(width > 1, "width must be larger than 1");
        WSP_REQUIRES(height > 1, "height must be larger than 1");

        s32 len = width*height;
        s32 back_pix = len - 1;
        _Type *real_ptr, *imag_ptr, *dest_real_ptr, *dest_imag_ptr;
        _Type *end_ptr;
        _Type *tmp_r;
        _Type *tmp_i;

        // -----------------------------------------------------
        // horizontal FFT
        if (window_function != NULL)
        {
            real_ptr = io_real;
            imag_ptr = io_imag;
            end_ptr = io_real + len;
            for (; real_ptr != end_ptr; real_ptr += width, imag_ptr += width)
            {
                window_function(real_ptr, width);
                window_function(imag_ptr, width);
            }
        }

        tmp_r = (_Type*)malloc(len*sizeof(_Type));
        tmp_i = (_Type*)malloc(len*sizeof(_Type));

        {
            real_ptr = io_real;
            imag_ptr = io_imag;
            dest_real_ptr = tmp_r;
            dest_imag_ptr = tmp_i;
            end_ptr = real_ptr + len;
            for (; real_ptr != end_ptr; 
                real_ptr += width, imag_ptr += width, dest_real_ptr += width, dest_imag_ptr += width)
            {
                _ApplyDft1dImpl(dest_real_ptr, dest_imag_ptr, real_ptr, imag_ptr, width, inverse);
            }
        }

        // transpose
        TransposeComplex<_Type>(io_real, io_imag, tmp_r, tmp_i, width, height);

        // vertical FFT
        if (window_function != NULL)
        {
            real_ptr = io_real;
            imag_ptr = io_imag;
            end_ptr = real_ptr + len;
            for (; real_ptr != end_ptr; real_ptr += height, imag_ptr += height)
            {
                window_function(real_ptr, height);
                window_function(imag_ptr, height);
            }
        }

        {
            real_ptr = io_real;
            imag_ptr = io_imag;
            dest_real_ptr = tmp_r;
            dest_imag_ptr = tmp_i;
            end_ptr = real_ptr + len;
            for (; real_ptr != end_ptr; 
                real_ptr += height, imag_ptr += height, dest_real_ptr += height, dest_imag_ptr += height)
            {
                _ApplyDft1dImpl(dest_real_ptr, dest_imag_ptr, real_ptr, imag_ptr, height, inverse);
            }
        }

        // transpose
        TransposeComplex<_Type>(io_real, io_imag, tmp_r, tmp_i, height, width);

        free(tmp_r);
        free(tmp_i);
    }

    template<typename _Type>
    void _ApplyFft2dImpl(
        _Type *io_real, _Type *io_imag,
        s32 width, s32 height, s32 exponent_x, s32 exponent_y, bool ifft, 
        void (*window_function)(_Type*, s32))
    {
        WSP_REQUIRES(width > 1, "width must be larger than 1");
        WSP_REQUIRES(height > 1, "height must be larger than 1");
        WSP_REQUIRES(wsp::math::IsPowerOfTwo(width), "width is not power of two");
        WSP_REQUIRES(wsp::math::IsPowerOfTwo(height), "height is not power of two");
        WSP_REQUIRES(wsp::math::GetNearestPowerOfTwo(width) != exponent_x, "exponent_x is not equal to exponent of width");
        WSP_REQUIRES(wsp::math::GetNearestPowerOfTwo(height) != exponent_y, "exponent_y is not equal to exponent of height");

        s32 len = width*height;
        s32 back_pix = len - 1;
        _Type *real_ptr, *imag_ptr;
        _Type *end_ptr;
        _Type *tmp_r;
        _Type *tmp_i;

        // -----------------------------------------------------
        // horizontal FFT
        if (window_function != NULL)
        {
            real_ptr = io_real;
            imag_ptr = io_imag;
            end_ptr = io_real + len;
            for (; real_ptr != end_ptr; real_ptr += width, imag_ptr += width)
            {
                window_function(real_ptr, width);
                window_function(imag_ptr, width);
            }
        }

        {
            real_ptr = io_real;
            imag_ptr = io_imag;
            end_ptr = io_real + len;
            for (; real_ptr != end_ptr; real_ptr += width, imag_ptr += width)
            {
#ifdef WSP_USE_OPTIMIZED_FFT
                _ApplyFft1dImpl_Opt(real_ptr, imag_ptr, width, exponent_x, ifft);
#else
                _ApplyFft1dImpl(real_ptr, imag_ptr, width, exponent_x, ifft);
#endif
            }
        }

        tmp_r = (_Type*)malloc(len*sizeof(_Type));
        tmp_i = (_Type*)malloc(len*sizeof(_Type));

        // transpose
        TransposeComplex<_Type>(tmp_r, tmp_i, io_real, io_imag, width, height);

        // vertical FFT
        if (window_function != NULL)
        {
            real_ptr = tmp_r;
            imag_ptr = tmp_i;
            end_ptr = real_ptr + len;
            for (; real_ptr != end_ptr; real_ptr += height, imag_ptr += height)
            {
                window_function(real_ptr, height);
                window_function(imag_ptr, height);
            }
        }

        {
            real_ptr = tmp_r;
            imag_ptr = tmp_i;
            end_ptr = real_ptr + len;
            for (; real_ptr != end_ptr; real_ptr += height, imag_ptr += height)
            {
#ifdef WSP_USE_OPTIMIZED_FFT
                _ApplyFft1dImpl_Opt(real_ptr, imag_ptr, height, exponent_y, ifft);
#else
                _ApplyFft1dImpl(real_ptr, imag_ptr, height, exponent_y, ifft);
#endif
            }
        }

        // transpose
        TransposeComplex<_Type>(io_real, io_imag, tmp_r, tmp_i, height, width);

        free(tmp_r);
        free(tmp_i);
    }
}

#define USE_OPTIMIZED_ApplyHannWindow
template<typename _Type>
inline void wsp::math::ApplyHannWindow(_Type *io_data, s32 length)
{
    double n, M;
    M = (double)floor(((double)length - 1) / 2.0);
#ifndef USE_OPTIMIZED_ApplyHannWindow
    for (s32 i = 0; i<length; ++i)
    {
        n = (double)i - M;
        io_data[i] = static_cast<_Type>(io_data[i] * (1 + cos(M_PI*n / M)) / 2.0);
    }
#else
    _Type* data_ptr = io_data;
    _Type* end_ptr = io_data + length;
    for (int i = 0; data_ptr != end_ptr; ++data_ptr, ++i)
    {
        n = (double)i - M;
        *data_ptr = static_cast<_Type>(*data_ptr * (1 + cos(M_PI*n / M)) / 2.0);
    }
#endif
}

#define USE_OPTIMIZED_ApplyHammingWindow
template<typename _Type>
inline void wsp::math::ApplyHammingWindow(_Type *io_data, s32 length)
{
    double x;
#ifndef USE_OPTIMIZED_ApplyHammingWindow
    for (s32 i = 0; i<length; ++i)
    {
        x = (double)i / (double)length;
        io_data[i] = static_cast<T>(io_data[i] * 0.54 - 0.46 * cos(2 * M_PI*x));
    }
#else
    _Type* data_ptr = io_data;
    _Type* end_ptr = io_data + length;
    for (int i = 0; data_ptr != end_ptr; ++data_ptr, ++i)
    {
        x = (double)i / (double)length;
        *data_ptr = static_cast<_Type>(*data_ptr * 0.54 - 0.46 * cos(2 * M_PI*x));
    }
#endif
}

template<typename _Type>
void wsp::math::ApplyDft1d(
    _Type *dest_r, _Type *dest_i,
    const _Type *src_r, const _Type *src_i, int sample_size)
{
    WSP_REQUIRES_IF_THEN_THROW(
        sample_size >= 2,
        std::invalid_argument,
        "input array size is less than 2, sample_size = %d", (s32)sample_size);

    _ApplyDft1dImpl<_Type>(dest_r, dest_i, src_r, src_i, sample_size, false);
}

template<typename _Type>
void wsp::math::ApplyInverseDft1d(
    _Type *dest_r, _Type *dest_i,
    const _Type *src_r, const _Type *src_i, int sample_size)
{
    WSP_REQUIRES_IF_THEN_THROW(
        sample_size >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)sample_size);

    _ApplyDft1dImpl<_Type>(dest_r, dest_i, src_r, src_i, sample_size, true);
}

template<typename _Type>
void wsp::math::ApplyFft1d(
    _Type *io_real,
    _Type *io_imag,
    s32 size,
    s32 exponent_of_size)
{
    WSP_REQUIRES_IF_THEN_THROW(
        size >= 2, 
        std::invalid_argument, 
        "input array size is less than 2, size = %d", (s32)size);
    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(size), 
        std::invalid_argument, 
        "input size is not power of two, size = %d", (s32)size);
    WSP_REQUIRES_IF_THEN_THROW(
        exponent_of_size == -1 ||
        exponent_of_size == wsp::math::GetExponentOfNearestPowerOfTwo(size),
        std::invalid_argument, 
        "exponent_of_size if not equals to actual exponent of size, (exponent_of_size = %d) == (%d)", (s32)exponent_of_size, wsp::math::GetExponentOfNearestPowerOfTwo(size));

    if (exponent_of_size == -1)
    {
        s32 size_power_of_two;
        wsp::math::GetNearestPowerOfTwo(&size_power_of_two, &exponent_of_size, size);
        if (size != size_power_of_two)
        {
            WSP_THROW(std::invalid_argument,
                "size is not power of 2, %d != %d\n", size, size_power_of_two);
        }
    }

    _ApplyFft1dImpl_Opt(
        io_real,
        io_imag,
        size,
        exponent_of_size,
        false);
}

template<typename _Type>
void wsp::math::ApplyInverseFft1d(
    _Type *io_real,
    _Type *io_imag,
    s32 size,
    s32 exponent_of_size
    )
{
    WSP_REQUIRES_IF_THEN_THROW(
        size >= 2,
        std::invalid_argument,
        "input array size is less than 2, size = %d", (s32)size);
    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(size),
        std::invalid_argument,
        "input size is not power of two, size = %d", (s32)size);
    WSP_REQUIRES_IF_THEN_THROW(
        exponent_of_size == -1 ||
        exponent_of_size == wsp::math::GetExponentOfNearestPowerOfTwo(size),
        std::invalid_argument,
        "exponent_of_size if not equals to actual exponent of size, (exponent_of_size = %d) == (%d)", (s32)exponent_of_size, wsp::math::GetExponentOfNearestPowerOfTwo(size));

    if (exponent_of_size <= 0)
    {
        s32 size_power_of_two;
        wsp::math::GetNearestPowerOfTwo(&size_power_of_two, &exponent_of_size, size);

        if (size != size_power_of_two)
        {
            WSP_THROW(std::invalid_argument,
                "size is not power of 2, %d != %d\n", size, size_power_of_two);
        }
    }

    _ApplyFft1dImpl_Opt(
        io_real,
        io_imag,
        size,
        exponent_of_size,
        true);
}

//
//void dft_2d(
//    short x[ROWS][COLS], y[ROWS][COLS], r[ROWS][COLS], i[ROWS][COLS])
//{
//    int   n, m, u, v, um, vn, mvn, M_2, N_2;
//    float c, s, p, w, x0, y0, twopie_d;
//
//    M_2 = M / 2;
//    N_2 = N / 2;
//    twopie_d = (2. * pie) / (M*N);
//
//    for (v = -M_2; v <= M_2 - 1; ++v)
//    {
//        for (u = -N_2; u <= N_2 - 1; ++u)
//        {
//            printf("\n      v=%3d u%3d--", v, u);
//
//            um = u*M;
//            vn = v*N;
//
//            x0 = 0;
//            y0 = 0;
//
//            for (m = -M_2; m <= M_2 - 1; ++m)
//            {
//                mvn = m*vn;
//
//                printf(" m%2d", m);
//                for (n = -N_2; n <= N_2 - 1; n++){
//                    /* you can probably separate the following
//                    to increase speed */
//                    /**p  = 2. * pie * (n*u*M + m*v*N) / (N*M);**/
//                    p = twopie_d * (n*um + mvn);
//                    c = cos(p);
//                    s = sin(p);
//                    /* the y array is all zero is remove it
//                    from the calculations
//                    */
//                    /*****
//                    x0 = x0 + c*x[m+M_2][n+N_2] + s*y[m+M_2][n+N_2];
//                    y0 = y0 + c*y[m+M_2][n+N_2] - s*x[m+M_2][n+N_2];
//                    *****/
//                    x0 = x0 + c*x[m + M_2][n + N_2];
//                    y0 = y0 - s*x[m + M_2][n + N_2];
//                }  /* ends loop over n */
//            }  /* ends loop over m */
//
//            r[v + M_2][u + N_2] = x0;
//            i[v + M_2][u + N_2] = y0;
//        }  /* ends loop over u */
//    }  /* ends loop over v */
//
//}  /* ends dft_2d */

template<typename _Type>
void wsp::math::ApplyDft2d(
    _Type *io_real, _Type *io_imaginary,
    s32 width, s32 height,
    void(*window_function)(_Type*, s32))
{
    WSP_REQUIRES_IF_THEN_THROW(
        width >= 2,
        std::invalid_argument,
        "input array width is less than 2, width = %d", (s32)width);

    _ApplyDft2dImpl<_Type>(
        io_real, io_imaginary, width, height, false, window_function);
}

template<typename _Type>
void wsp::math::ApplyInverseDft2d(
    _Type *io_real, _Type *io_imaginary,
    s32 width, s32 height,
    void(*window_function)(_Type*, s32))
{
    WSP_REQUIRES_IF_THEN_THROW(
        width >= 2,
        std::invalid_argument,
        "input array width is less than 2, width = %d", (s32)width);

    _ApplyDft2dImpl<_Type>(
        io_real, io_imaginary, width, height, true, window_function);
}

template<typename _Type>
void wsp::math::ApplyFft2d(
    _Type *io_real, _Type *io_imag,
    s32 width, s32 height, s32 exponent_x, s32 exponent_y,
    void(*window_function)(_Type*, s32))
{
    WSP_REQUIRES_IF_THEN_THROW(
        width >= 2,
        std::invalid_argument,
        "input array width is less than 2, width = %d", (s32)width);
    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(width),
        std::invalid_argument,
        "input width is not power of two, width = %d", (s32)width);
    WSP_REQUIRES_IF_THEN_THROW(
        exponent_x == -1 ||
        exponent_x == wsp::math::GetExponentOfNearestPowerOfTwo(width),
        std::invalid_argument,
        "exponent_x is not equals to actual exponent of width, (exponent_x = %d) == (%d)", 
        (s32)exponent_x, wsp::math::GetExponentOfNearestPowerOfTwo(width));
    WSP_REQUIRES_IF_THEN_THROW(
        height >= 2,
        std::invalid_argument,
        "input array width is less than 2, width = %d", (s32)height);
    WSP_REQUIRES_IF_THEN_THROW(
        wsp::math::IsPowerOfTwo(height),
        std::invalid_argument,
        "input width is not power of two, width = %d", (s32)height);
    WSP_REQUIRES_IF_THEN_THROW(
        exponent_y == -1 ||
        exponent_y == wsp::math::GetExponentOfNearestPowerOfTwo(height),
        std::invalid_argument,
        "exponent_y is not equals to actual exponent of height, (exponent_y = %d) == (%d)", 
        (s32)exponent_y, wsp::math::GetExponentOfNearestPowerOfTwo(height));

    if (exponent_x <= 0)
    {
        s32 size_power_of_two;
        wsp::math::GetNearestPowerOfTwo(&size_power_of_two, &exponent_x, width);
        if (width != size_power_of_two)
        {
            WSP_THROW(std::invalid_argument,
                "size is not power of 2, %d != %d\n", width, size_power_of_two);
        }
    }

    if (exponent_y <= 0)
    {
        s32 size_power_of_two;
        wsp::math::GetNearestPowerOfTwo(&size_power_of_two, &exponent_y, height);
        if (height != size_power_of_two)
        {
            WSP_THROW(std::invalid_argument,
                "size is not power of 2, %d != %d\n", height, size_power_of_two);
        }
    }

    _ApplyFft2dImpl(
        io_real, io_imag, 
        width, height, exponent_x, exponent_y, false,
        window_function);
}

template<typename _Type>
void wsp::math::ApplyInverseFft2d(
    _Type *io_real, _Type *io_imag,
    s32 width, s32 height, s32 exponent_x, s32 exponent_y)
{
    //WSP_REQUIRES_IF_THEN_THROW(
    //    width >= 2,
    //    std::invalid_argument,
    //    "input array width is less than 2, width = %d", (s32)width);
    //WSP_REQUIRES_IF_THEN_THROW(
    //    wsp::math::IsPowerOfTwo(width),
    //    std::invalid_argument,
    //    "input width is not power of two, width = %d", (s32)width);
    //WSP_REQUIRES_IF_THEN_THROW(
    //    exponent_x == -1 ||
    //    exponent_x == wsp::math::GetExponentOfNearestPowerOfTwo(width),
    //    std::invalid_argument,
    //    "exponent_x is not equals to actual exponent of width, (exponent_x = %d) == (%d)", (s32)exponent_x, wsp::math::GetExponentOfNearestPowerOfTwo(width));
    //WSP_REQUIRES_IF_THEN_THROW(
    //    height >= 2,
    //    std::invalid_argument,
    //    "input array width is less than 2, width = %d", (s32)height);
    //WSP_REQUIRES_IF_THEN_THROW(
    //    wsp::math::IsPowerOfTwo(height),
    //    std::invalid_argument,
    //    "input width is not power of two, width = %d", (s32)height);
    //WSP_REQUIRES_IF_THEN_THROW(
    //    exponent_y == -1 ||
    //    exponent_y == wsp::math::GetExponentOfNearestPowerOfTwo(height),
    //    std::invalid_argument,
    //    "exponent_y is not equals to actual exponent of height, (exponent_y = %d) == (%d)", (s32)exponent_y, wsp::math::GetExponentOfNearestPowerOfTwo(height));

    //if (exponent_x <= 0)
    //{
    //    s32 size_power_of_two;
    //    wsp::math::GetNearestPowerOfTwo(&size_power_of_two, &exponent_x, width);
    //    if (width != size_power_of_two)
    //    {
    //        throw std::invalid_argument("size is not power of 2");
    //    }
    //}

    //if (exponent_y <= 0)
    //{
    //    s32 size_power_of_two;
    //    wsp::math::GetNearestPowerOfTwo(&size_power_of_two, &exponent_y, height);
    //    if (height != size_power_of_two)
    //    {
    //        throw WSP_NEW std::invalid_argument("size is not power of 2");
    //    }
    //}

    _ApplyFft2dImpl<_Type>(
        io_real, io_imag, 
        width, height, exponent_x, exponent_y, true, NULL);
}

template<typename _Type, typename _OutType>
void wsp::math::GetPowerSpectrum(
    _OutType *o_dest, const _Type *in_real, const _Type *in_imag, s32 size)
{
    _OutType *o_ptr = o_dest;
    const _Type *real_ptr = in_real;
    const _Type *imag_ptr = in_imag;
    const _Type *end_ptr = real_ptr + size;
    for (; real_ptr != end_ptr; ++real_ptr, ++imag_ptr, ++o_ptr)
    {
        *o_ptr = static_cast<_OutType>((*real_ptr) * (*real_ptr) + (*imag_ptr) * (*imag_ptr));
    }
}

template<typename _Type>
void wsp::math::ApplyLogTransformation(_Type *io_data, s32 length)
{
    _Type *data_ptr = io_data;
    _Type *end_ptr = data_ptr + length;
    for (; data_ptr != end_ptr; ++data_ptr)
    {
        *data_ptr = log(*data_ptr);
    }
}

#endif
