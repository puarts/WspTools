//! WspFnGpuImgFilter.cu
// @author Junichi Nishikata


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cutil_math.h>

#include <wsp/gpu/WspFnGpuImgFilter.h>
#include <wsp/gpu/WspFnGpuMatrix.h>
#include <wsp/mathutil/fn-math-gpl.h>

#define USE_SIMPLE_FILTER 1




/*
Perform Gaussian filter on a 2D image using CUDA

Parameters:
d_src  - pointer to input image in device memory
d_dest - pointer to destination image in device memory
d_temp - pointer to temporary storage in device memory
width  - image width
height - image height
sigma  - sigma of Gaussian
order  - filter order (0, 1 or 2)
*/

// 8-bit RGBA version
wsp::State WspFnGpu::GaussianFilterRGBA(u32 *h_dest, u32 *h_src, int width, int height, float sigma, int order, int nthreads)
{
    if(width<=0 || height<=0){ return WspStd::Failure; }

    u32* d_src, *d_dest, *d_temp;
    u32 length = width*height;

    size_t size = length * sizeof(float);

    // Allocate vectors in device memory
    cutilSafeCall( cudaMalloc((void**)&d_src, size) );
    cutilSafeCall( cudaMalloc((void**)&d_dest, size) );
    cutilSafeCall( cudaMalloc((void**)&d_temp, size) );

    // Copy vectors from host memory to device memory
    cutilSafeCall( cudaMemcpy(d_src, h_src, size, cudaMemcpyHostToDevice) );

    WspFnGpu::GaussianFilterRGBA(d_src, d_dest, d_temp, width, height, sigma, order, nthreads);

    cutilSafeCall( cudaMemcpy(h_dest, d_dest, size, cudaMemcpyDeviceToHost) );

    cutilSafeCall(cudaFree(d_src));
    cutilSafeCall(cudaFree(d_dest));
    cutilSafeCall(cudaFree(d_temp));

    cutilDeviceReset();

    return WSP_STATE_SUCCESS;
}

void WspFnGpu::GaussianFilterRGBA(u32 *d_src, u32 *d_dest, u32 *d_temp, int width, int height, float sigma, int order, int nthreads)
{
    // compute filter coefficients
    const float
        nsigma = sigma < 0.1f ? 0.1f : sigma,
        alpha = 1.695f / nsigma,
        ema = (float)exp(-alpha),
        ema2 = (float)exp(-2*alpha),
        b1 = -2*ema,
        b2 = ema2;

    float a0 = 0, a1 = 0, a2 = 0, a3 = 0, coefp = 0, coefn = 0;
    switch (order) {
        case 0: {
            const float k = (1-ema)*(1-ema)/(1+2*alpha*ema-ema2);
            a0 = k;
            a1 = k*(alpha-1)*ema;
            a2 = k*(alpha+1)*ema;
            a3 = -k*ema2;
            break;
        }

        case 1: {
            const float k = (1-ema)*(1-ema)/ema;
            a0 = k*ema;
            a1 = a3 = 0;
            a2 = -a0;
            break;
        }

        case 2: {
            const float
                ea = (float)exp(-alpha),
                k = -(ema2-1)/(2*alpha*ema),
                kn = (-2*(-1+3*ea-3*ea*ea+ea*ea*ea)/(3*ea+1+3*ea*ea+ea*ea*ea));
            a0 = kn;
            a1 = -kn*(1+k*alpha)*ema;
            a2 = kn*(1-k*alpha)*ema;
            a3 = -kn*ema2;
            break;
        }

        default:
            WSP_COMMON_ERROR_LOG("GaussianFilter: invalid order parameter!\n");
            return;
    }
    coefp = (a0+a1)/(1+b1+b2);
    coefn = (a2+a3)/(1+b1+b2);

    // process columns
#if USE_SIMPLE_FILTER
    WspFnGpu::d_simpleRecursive_rgba<<< wsp::math::iDivUp(width, nthreads), nthreads >>>(d_src, d_temp, width, height, ema);
#else
    WspFnGpu::d_recursiveGaussian_rgba<<< wsp::math::iDivUp(width, nthreads), nthreads >>>(d_src, d_temp, width, height, a0, a1, a2, a3, b1, b2, coefp, coefn);
#endif
    cutilCheckMsg("Kernel execution failed");

    WspFnGpu::transpose(d_temp, d_dest, width, height);
    cutilCheckMsg("transpose: Kernel execution failed");

    // process rows
#if USE_SIMPLE_FILTER
    WspFnGpu::d_simpleRecursive_rgba<<< wsp::math::iDivUp(height, nthreads), nthreads >>>(d_dest, d_temp, height, width, ema);
#else
    WspFnGpu::d_recursiveGaussian_rgba<<< wsp::math::iDivUp(height, nthreads), nthreads >>>(d_dest, d_temp, height, width, a0, a1, a2, a3, b1, b2, coefp, coefn);
#endif
    cutilCheckMsg("Kernel execution failed");

    WspFnGpu::transpose(d_temp, d_dest, height, width);
}