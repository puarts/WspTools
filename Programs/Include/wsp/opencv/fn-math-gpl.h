#pragma once

#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <wsp/math/_define_mathutil.h>
#include <wsp/math/_tpl_fn-math.h>
#include <wsp/common/common_types.h>

#include <opencv2/opencv.hpp>

namespace wsp{ namespace opencv{
    // Fourier Transform ---------------------------------------
    WSP_DLL_EXPORT wsp::State FFT2D_CvGpu(cv::Mat &io_mat, bool dft1d=false, bool inverse=false);
    WSP_DLL_EXPORT wsp::State GetPOC_CvGpu_Double(double *o_poc, double *o_pocAv, const double *in_ref, const double *in_tpl, int width, int height, double low_pass_ratio, bool dft1d=false);
}}
