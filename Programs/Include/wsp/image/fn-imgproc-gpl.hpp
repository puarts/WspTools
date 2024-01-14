/**
 * @file fn-imgproc-gpl.h
 * @author Junichi Nishikata
 */

#ifndef __WSP_IMAGE_FUNCTION_IMAGE_PROC_GPL_HPP__
#define __WSP_IMAGE_FUNCTION_IMAGE_PROC_GPL_HPP__

#define USE_FFTW

#ifndef M_PI
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <wsp/math/_define_mathutil.h>
#include <wsp/math/_tpl_fn-math.h>
#include <wsp/common/common_types.h>

#ifdef USE_OPENCV
    #include <opencv2/opencv.hpp"
#endif
#ifdef USE_FFTW
    #include <fftw3.h>
#endif

//#include "tpl_cl-matrix.h"

namespace wsp{ namespace img{
    



}}



#endif