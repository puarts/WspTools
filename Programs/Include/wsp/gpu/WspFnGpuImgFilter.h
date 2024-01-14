//! WspFnGpuImgFilter.h
// @author Junichi Nishikata


#ifndef __WspFnGpuImgFilter_H__
#define __WspFnGpuImgFilter_H__

#include <wsp/gpu/WspGpuDefine.h>



namespace WspFnGpu{
    //! Export Functions -----------------------------------------------------------
    WSP_DLL_EXPORT wsp::State GaussianFilterRGBA(u32 *h_dest, u32 *h_src, int width, int height, float sigma=10.0f, int order=0, int nthreads=64);

    //! Gpu Functions --------------------------------------------------------------
    void GaussianFilterRGBA(u32 *d_src, u32 *d_dest, u32 *d_temp, int width, int height, float sigma, int order, int nthreads);

    //! Kernel Functions -----------------------------------------------------------
    __global__ void d_simpleRecursive_rgba(u32 *id, u32 *od, int w, int h, float a);
    __global__ void d_recursiveGaussian_rgba(u32 *id, u32 *od, int w, int h, float a0, float a1, float a2, float a3, float b1, float b2, float coefp, float coefn);
}

#endif