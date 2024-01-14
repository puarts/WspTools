

#include <wsp/gpu/WspFnGpuMatrix.h>
#include <wsp/mathutil/fn-math-gpl.h>



/*
    Transpose a 2D array (see SDK transpose example)
*/
void WspFnGpu::transpose(u32 *d_src, u32 *d_dest, u32 width, int height)
{
    dim3 grid(wsp::math::iDivUp(width, BLOCK_DIM), wsp::math::iDivUp(height, BLOCK_DIM), 1);
    dim3 threads(BLOCK_DIM, BLOCK_DIM, 1);
    WspFnGpu::d_transpose<<< grid, threads >>>(d_dest, d_src, width, height);
    cutilCheckMsg("Kernel execution failed");
}