/**
 * @file _define_video.h
 * @author Jun-ichi Nishikata
 * Base definition of video module.
 */

#ifndef WspVideo_WspVideoDefine_H_
#define WspVideo_WspVideoDefine_H_

#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif
#include <wsp/math/_define_mathutil.h>
#ifndef __WSP_IMAGEUTIL_DEFINE_IMAGEUTIL_H__
#include <wsp/image/_define_image.h>
#endif

#ifdef _WINDLL
    #define USE_LIBAV
#endif

namespace wsp{ 
/**
 * @brief Collection of functions and classes for WspVideo module
 */
namespace video{
    typedef enum {
        OPTICALFLOW_DISPLAY_XY,
        OPTICALFLOW_DISPLAY_X,
        OPTICALFLOW_DISPLAY_Y,
        OPTICALFLOW_DISPLAY_SCALAR_XY
    }OpticalflowDisplayType;
}}

#endif