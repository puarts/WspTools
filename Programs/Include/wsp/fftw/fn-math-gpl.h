/**
 * @file fn-math.h
 * @author Junichi Nishikata
 */

#pragma once

#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <wsp/math/_define_mathutil.h>
#include <wsp/math/_tpl_fn-math.h>
#include <wsp/common/common_types.h>

namespace wsp{ namespace fftw{
    
    inline int iDivUp(int a, int b){ return (a % b != 0) ? (a / b + 1) : (a / b); }

    // Fourier Transform ---------------------------------------

    WSP_DLL_EXPORT void FftwApplyFft1d(
        double* io_real, double* io_imaginary, s32 sample_size);

    WSP_DLL_EXPORT void FftwApplyInverseFft1d(
        double* io_real, double* io_imaginary, s32 sample_size);

    WSP_DLL_EXPORT void FftwApplyFft2d(
        double* io_real, double* io_imaginary, s32 width, s32 height);

    WSP_DLL_EXPORT void FftwApplyInverseFft2d(
        double* io_real, double* io_imaginary, s32 width, s32 height);
}}
