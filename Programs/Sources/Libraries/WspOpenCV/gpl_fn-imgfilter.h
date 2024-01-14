/**
 * @file gpl_fn-imgfilter.h
 * @author Junichi Nishikata
 */


#ifndef WSP_IMAGE_GPL_FN_IMG_FILTER_H_
#define WSP_IMAGE_GPL_FN_IMG_FILTER_H_

#include "tpl_cl-image.h"

#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#endif

namespace wsp{ namespace img{
#ifdef USE_OPENCV
    WSP_DLL_EXPORT cv::Mat cvMakeGaborKernel(
        int kernel_size, 
        double sig, 
        double th, 
        double lm, 
        double ps
    );
#endif
}}

#endif
