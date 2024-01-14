//! WspFnGpuMatrix.h
// @author Junichi Nishikata


#ifndef __WspFnGpuMatrix_H__
#define __WspFnGpuMatrix_H__

#include <wsp/gpu/WspGpuDefine.h>

// Variables
namespace WspFnGpu{
    WSP_DLL_EXPORT void transpose(u32 *d_src, u32 *d_dest, u32 width, int height);

    __global__ void d_transpose(u32 *odata, u32 *idata, int width, int height);
}

#endif