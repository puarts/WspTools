//! _define_mathutil.h
// @author Jun-ichi Nishikata

#ifndef WspMath_WspMathDefine_H_
#define WspMath_WspMathDefine_H_

#include <math.h>
#ifndef __WSP_COMMONCORE_DEFINE_COMMONCORE_H__
#include <wsp/common/_define_commoncore.h>
#endif

#ifdef _WINDLL
    #ifdef _DEBUG
        #pragma comment(lib, "WspCommonUtil-d.lib")
    #else
        #pragma comment(lib, "WspCommonUtil.lib")
    #endif
    #ifdef USE_FFTW
        #include <load_fftw.h>
    #endif
    #ifdef USE_OPENCV
        #include <load_opencv.h>
    #endif
    #ifdef _WIN32
        #pragma comment(lib, "ws2_32.lib")
    #endif
#endif

#endif