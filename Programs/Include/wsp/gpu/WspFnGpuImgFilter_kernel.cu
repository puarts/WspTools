//! WspFnGpuImgFilter_kernel.cu
// @author Junichi Nishikata

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cutil_math.h>

#include <wsp/gpu/WspFnGpuImgFilter.h>
#include "WspFnGpuInline.cu"

/*
    simple 1st order recursive filter
    - processes one image column per thread

    parameters:    
    id - pointer to input data (RGBA image packed into 32-bit integers)
    od - pointer to output data 
    w  - image width
    h  - image height
    a  - blur parameter
*/
__global__ void WspFnGpu::d_simpleRecursive_rgba(u32 *d_src, u32 *d_dst, int w, int h, float a)
{
    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
    if (x >= w){ return; }
    
    d_src += x;    // advance pointers to correct column
    d_dst += x;

    // forward pass
    float4 pix_prev = RgbaIntToFloat(*d_src);  // previous output
    for (int y = 0; y < h; y++) {
        float4 pix = RgbaIntToFloat(*d_src);
        float4 pix_dst = pix + a*(pix_prev - pix);   // simple lerp between current and previous value
        *d_dst = RgbaFloatToInt(pix_dst);
        d_src += w; d_dst += w;    // move to next row
        pix_prev = pix_dst;
    }

    // reset pointers to point to last element in column
    d_src -= w;
    d_dst -= w;

    // reverse pass
    // ensures response is symmetrical
    pix_prev = RgbaIntToFloat(*d_src);
    for (int y = h-1; y >= 0; y--) {
        float4 pix = RgbaIntToFloat(*d_src);
        float4 pix_new = pix + a*(pix_prev - pix);
        *d_dst = RgbaFloatToInt((RgbaIntToFloat(*d_dst) + pix_new)*0.5f);
        d_src -= w; d_dst -= w;  // move to previous row
        pix_prev = pix_new;
    }
}

/*
    recursive Gaussian filter

    parameters:    
    id - pointer to input data (RGBA image packed into 32-bit integers)
    od - pointer to output data 
    w  - image width
    h  - image height
    a0-a3, b1, b2, coefp, coefn - filter parameters
*/

__global__ void
WspFnGpu::d_recursiveGaussian_rgba(u32 *id, u32 *od, int w, int h, float a0, float a1, float a2, float a3, float b1, float b2, float coefp, float coefn)
{
    unsigned int x = blockIdx.x*blockDim.x + threadIdx.x;
    if (x >= w) return;
    
    id += x;    // advance pointers to correct column
    od += x;

    // forward pass
    float4 xp = make_float4(0.0f);  // previous input
    float4 yp = make_float4(0.0f);  // previous output
    float4 yb = make_float4(0.0f);  // previous output by 2
#if CLAMP_TO_EDGE
    xp = RgbaIntToFloat(*id); yb = coefp*xp; yp = yb;
#endif
    for (int y = 0; y < h; y++) {
        float4 xc = RgbaIntToFloat(*id);
        float4 yc = a0*xc + a1*xp - b1*yp - b2*yb;
        *od = RgbaFloatToInt(yc);
        id += w; od += w;    // move to next row
        xp = xc; yb = yp; yp = yc; 
    }

    // reset pointers to point to last element in column
    id -= w;
    od -= w;

    // reverse pass
    // ensures response is symmetrical
    float4 xn = make_float4(0.0f);
    float4 xa = make_float4(0.0f);
    float4 yn = make_float4(0.0f);
    float4 ya = make_float4(0.0f);
#if CLAMP_TO_EDGE
    xn = xa = RgbaIntToFloat(*id); yn = coefn*xn; ya = yn;
#endif
    for (int y = h-1; y >= 0; y--) {
        float4 xc = RgbaIntToFloat(*id);
        float4 yc = a2*xn + a3*xa - b1*yn - b2*ya;
        xa = xn; xn = xc; ya = yn; yn = yc;
        *od = RgbaFloatToInt(RgbaIntToFloat(*od) + yc);
        id -= w; od -= w;  // move to previous row
    }
}
