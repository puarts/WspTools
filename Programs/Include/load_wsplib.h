
#ifndef _LOAD_WSPLIB_H_
#define _LOAD_WSPLIB_H_

#ifdef _DEBUG
    #pragma comment(lib, "WspCommon-d.lib")
    #pragma comment(lib, "WspImage-d.lib")
    #pragma comment(lib, "WspMath-d.lib")
    #pragma comment(lib, "WspGraphics-d.lib")
    #pragma comment(lib, "WspDynamics-d.lib")
    //#pragma comment(lib, "WspAnim-d.lib")
    //#pragma comment(lib, "WspGpu-d.lib")
    //#pragma comment(lib, "WspWeb-d.lib")
    //#pragma comment(lib, "WspVideo-d.lib")
    #pragma comment(lib, "WspGui-d.lib")
    #pragma comment(lib, "WspKinect-d.lib")
    #pragma comment(lib, "WspSound-d.lib")
    #ifdef USE_OPENMAYA
    #pragma comment(lib, "WspMayad.lib")
    #endif
#else
    #pragma comment(lib, "WspCommon.lib")
    #pragma comment(lib, "WspImage.lib")
    #pragma comment(lib, "WspMath.lib")
    #pragma comment(lib, "WspGraphics.lib")
    #pragma comment(lib, "WspDynamics.lib")
    //#pragma comment(lib, "WspAnim.lib")
    //#pragma comment(lib, "WspGpu.lib")
    //#pragma comment(lib, "WspWeb.lib")
    //#pragma comment(lib, "WspVideo.lib")
    #pragma comment(lib, "WspGui.lib")
    #pragma comment(lib, "WspKinect.lib")
    #pragma comment(lib, "WspSoundCore.lib")
    #pragma comment(lib, "WspSoundUtil.lib")
    #ifdef USE_OPENMAYA
    #pragma comment(lib, "WspMaya.lib")
    #endif
#endif

#endif

