//! WspFnGpuVector_kernel.cu
// @author Junichi Nishikata

#ifndef _WspFnGpuVector_kernel_CU_
#define _WspFnGpuVector_kernel_CU_

#include <wsp/gpu/WspFnGpuVector.h>

// Device code
__global__ void WspFnGpu::VectorAdd_d(const float* A, const float* B, float* C, int N)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < N){
        C[i] = A[i] + B[i];
    }
}

#endif