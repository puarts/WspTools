//! WspFnGpuVector.h
// @author Junichi Nishikata


#ifndef __WspFnGpuVector_H__
#define __WspFnGpuVector_H__

#include <wsp/gpu/WspGpuDefine.h>

// Variables
namespace WspFnGpu{
    WSP_DLL_EXPORT wsp::State VectorAdd(float *o_vec, float *in_vecA, float *in_vecB, int length);
    __global__ void VectorAdd_d(const float* A, const float* B, float* C, int N);
}

#endif