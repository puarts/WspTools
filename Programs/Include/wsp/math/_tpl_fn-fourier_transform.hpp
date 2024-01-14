/**
 * @file _tpl_fn-fourier_transform.h
 * 
 */

#ifndef WSP_MATH_TEMPLATE_FUNCTION_FOURIER_TRANSFORM_HPP__
#define WSP_MATH_TEMPLATE_FUNCTION_FOURIER_TRANSFORM_HPP__

#include <wsp/common/_ctypes.h>

namespace wsp{ namespace math{
    template<typename T> 
    inline void ApplyHannWindow(T *io_data, s32 length);

    template<typename T> 
    inline void ApplyHammingWindow(T *io_data, s32 length);

    //! \ingroup WspMath
    //! \brief 1D Discreat Fourier Transform
    template<typename _Type>
    void ApplyDft1d(
        _Type *o_real, _Type *o_imaginary,
        const _Type *in_real, const _Type *in_imaginary, s32 sampleSize);

    template<typename _Type>
    void ApplyInverseDft1d(
        _Type *o_real, _Type *o_imaginary,
        const _Type *in_real, const _Type *in_imaginary, s32 sampleSize);

    template<typename _Type>
    void ApplyDft2d(
        _Type *io_real, _Type *io_imag,
        s32 width, s32 height,
        void(*window_function)(_Type*, s32) = NULL);

    template<typename _Type>
    void ApplyInverseDft2d(
        _Type *io_real, _Type *io_imag,
        s32 width, s32 height,
        void(*window_function)(_Type*, s32) = NULL);

    template<typename _Type>
    void ApplyFft1d(
        _Type *io_real, 
        _Type *io_imag, 
        s32 size, 
        s32 exponent_of_size = -1);

    template<typename _Type>
    void ApplyInverseFft1d(
        _Type *io_real, 
        _Type *io_imag, 
        s32 size, 
        s32 exponent_of_size = -1);

    //! \ingroup WspMath
    //! \brief 2D per line Fast Fourier Transform
    template<typename _Type>
    void ApplyFft2d(
        _Type *io_real, _Type *io_imag,
        s32 width, s32 height, s32 exponent_x = -1, s32 exponent_y = -1,
        void(*window_function)(_Type*, s32) = NULL);

    //! \ingroup WspMath
    //! \brief 2D per line Inverse Fast Fourier Transform
    template<typename _Type>
    void ApplyInverseFft2d(
        _Type *io_real, _Type *io_imag,
        s32 width, s32 height, s32 exponent_x = -1, s32 exponent_y = -1);

    template<typename _InType, typename _OutType>
    void GetPowerSpectrum(
        _OutType *o_powerspectrum, 
        const _InType *in_real, 
        const _InType *in_imag, 
        s32 size);

    //! @brief Calculate log value from input data.
    template<typename _Type>
    void ApplyLogTransformation(_Type *io_data, s32 length);
}}

#include "_tpl_fn-fourier_transform.inl"

#endif
